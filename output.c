#include "output.h"

bool thresholdHigh(double value, double tpoint) {
    if( value > tpoint) {
        return false;
    }   
    else {
            return true;
        }
}

bool thresholdLow(double value, double tpoint) {
    if(value < tpoint) {
        return false;
    }   
    else  {
        return true;
    }
}

void *displayOutput(void *arg)
{
    struct thread_data *data = arg;
    bool end_thread = false;
    bool initialization = true;
    
    bool alarm_state = false;
    bool obstructed = false;

    while(1) {
        pthread_mutex_lock(&data->mutexControl);
        {
            end_thread = data->end_all_threads;
        }
        pthread_mutex_unlock(&data->mutexControl);

        pthread_mutex_lock(&data->mutexAlarm);
        {
            alarm_state = data->general_alarm;
            obstructed = data->obstructed_alarm;
        }
        pthread_mutex_unlock(&data->mutexAlarm);

        if(end_thread == false) {
        if(initialization == true) {
            initialization = false;
            sleepForMs(1000);
            }
            
            else {
            if((obstructed == false) || (alarm_state == true)) {
                pthread_mutex_lock(&data->mutexTemp);
                pthread_mutex_lock(&data->mutexIR);
                {
                    printf("\n");
                    printf("Temperature: %.2f, IR Voltage: %.2f \n", data->temp_value, data->IR_value);
                }
                pthread_mutex_unlock(&data->mutexIR);
                pthread_mutex_unlock(&data->mutexTemp);

                pthread_mutex_lock(&data->mutexAir);
                {
                    printf("CO Voltage: %.2f, CO2 Voltage: %.2f, Smoke Voltage: %.2f \n", data->CO_value, data->CO2_value, data->smoke_value);
                    printf("\n");
                }
                pthread_mutex_unlock(&data->mutexAir);
            }
            else 
            if((obstructed == true) && (alarm_state == false)) {
                printf("WARNING: Board likely obstructed!\n");
                printf("\n");
            }

            sleepForMs(2500);
        }
        }
        else 
        if(end_thread == true) {
            return NULL;
        }
        
        initialization = false;
        sleepForMs(2500);
    }

}

void *calculateStatus(void *arg)
{
    struct thread_data *data = arg;
    bool end_thread = false;
    bool initialization = true;

    while(1) {
        pthread_mutex_lock(&data->mutexControl);
        {
            end_thread = data->end_all_threads;
        }
        pthread_mutex_unlock(&data->mutexControl);

        if(end_thread == false) {
        if(initialization == false) {
            // Interpret IR readings
            int ir_count = 0;
            int ir_sum = 0;

            pthread_mutex_lock(&data->mutexIR);
            {
                for(int i = 0; i < BUFFER_SIZE; i++) {
                    if(thresholdHigh(data->IR_buffer[i], IRPOINT) == true) {
                        ir_count++;
                    }
                    ir_sum = ir_sum + data->IR_buffer[i];
                    }   
                data->IR_value = ir_sum / BUFFER_SIZE;
            }
            pthread_mutex_unlock(&data->mutexIR);
            
            pthread_mutex_lock(&data->mutexAlarm);
            {
                if(ir_count == BUFFER_SIZE) {
                    data->alarm_IR = true;
                }
                else {
                    data->alarm_IR = false;
                }
            }
            pthread_mutex_unlock(&data->mutexIR);
            pthread_mutex_unlock(&data->mutexAlarm);

            // Read temperature separately to avoid nested locks
            double temp_val;
            pthread_mutex_lock(&data->mutexTemp);
            {
                temp_val = data->temp_value;
            }
            pthread_mutex_unlock(&data->mutexTemp);

            // Read air sensor values separately
            double co_val, co2_val, smoke_val;
            pthread_mutex_lock(&data->mutexAir);
            {
                co_val = data->CO_value;
                co2_val = data->CO2_value;
                smoke_val = data->smoke_value;
            }
            pthread_mutex_unlock(&data->mutexAir);

            // Now update alarm states with single lock
            pthread_mutex_lock(&data->mutexAlarm);
            {
                // Temperature
                if(thresholdLow(temp_val, TEMPPOINT) == true) {
                    data->alarm_temp = true;
                }
                else {
                    data->alarm_temp = false;
                }

                // Air quality sensors
                if(thresholdHigh(co_val, COPOINT) == true) {
                    data->alarm_CO = true;
                }
                else {
                    data->alarm_CO = false;
                }

                if(thresholdHigh(co2_val, CO2POINT) == true) {
                    data->alarm_CO2 = true;
                }
                else {
                    data->alarm_CO2 = false;
                }

                if(thresholdLow(smoke_val, SMOKEPOINT) == true) {
                    data->alarm_smoke = true;
                }
                else {
                    data->alarm_smoke = false;
                }
            }
            pthread_mutex_unlock(&data->mutexAlarm);

            sleepForMs(500);
        }
        else {
            initialization = false;
        }
        }
        else {
            return NULL;
        }
    }
}

void *calcAlarm(void *arg)
{   
    struct thread_data *data = arg;
    bool end_thread = false;
    bool initialization = true;

    bool temp_trigger, ir_trigger, smoke_trigger, CO_trigger, CO2_trigger;
    int alarm_count = 0;

    bool alarm_state = false;
    bool warning_state = false;
    bool obstructed = false;

    while(1) {
        pthread_mutex_lock(&data->mutexControl);
        {
            end_thread = data->end_all_threads;
        }
        pthread_mutex_unlock(&data->mutexControl);

        system("i2cset -y 1 0x70 0x21 0x00");
        system("i2cset -y 1 0x70 0x81 0x00");
        system("i2cset -y 1 0x70 0x06 0x00");
        system("i2cset -y 1 0x70 0x08 0x00");
        system("i2cset -y 1 0x70 0x04 0x00");
        system("i2cset -y 1 0x70 0x0A 0x00");
        system("i2cset -y 1 0x70 0x02 0x00");
        system("i2cset -y 1 0x70 0x0C 0x00");
        system("i2cset -y 1 0x70 0x00 0x00");
        system("i2cset -y 1 0x70 0x0E 0x00");

        if(end_thread == false) {
        if(initialization == true) {
            initialization = false;
            sleepForMs(1000);
        }
        else {
            if(((alarm_state == true) || (warning_state == true))) {

                if(alarm_count >= BUFFER_SIZE) {
                    alarm_count = BUFFER_SIZE;
                    alarm_count--;
                }
                else {
                    alarm_count--;
                }

                if(alarm_count == 0) {
                    warning_state = false;
                    alarm_state = false;
                }
            }
            else {
                alarm_count = 0;
            }

            pthread_mutex_lock(&data->mutexAlarm);
            {
                temp_trigger = data->alarm_temp;
                ir_trigger = data->alarm_IR;
                smoke_trigger = data->alarm_smoke;
                CO2_trigger = data->alarm_CO2;
                CO_trigger = data->alarm_CO;
            }
            pthread_mutex_unlock(&data->mutexAlarm);

            if(CO_trigger == true) {
                alarm_count++;
                //printf("CO TRIGGER\n");
                alarm_state = true;
            }
            if(temp_trigger == true) {
                alarm_count++;
                //printf("temp TRIGGER\n");
            }
            if(CO2_trigger == true) {
                //printf("CO2 TRIGGER\n");
                alarm_count++;
            }
            if(smoke_trigger == true) {
                //printf("SMOKE TRIGGER\n");
                alarm_count++;
            }

            // Obstructed case
            if((alarm_count < 2) && (ir_trigger == true) && (alarm_state == false)) {
                obstructed = true;
            }
            else {
                obstructed = false;
            }

            // Fire case
            if((alarm_count >= 2) && (obstructed == false)) {
                alarm_state = true;
            }
            else
            // Warning case
            if((alarm_count == 1) && (alarm_state == false)) {
                warning_state = true;
            }
            else {
                warning_state = false;
            }

            pthread_mutex_lock(&data->mutexAlarm);
            {
                data->general_alarm = alarm_state;
                data->obstructed_alarm = obstructed;
            }
            pthread_mutex_unlock(&data->mutexAlarm);

            if((((warning_state == true) || (obstructed == true)) && (alarm_state == false))) {
                // Creates ! mark
                system("i2cset -y 1 0x70 0x06 0x0C");
                system("i2cset -y 1 0x70 0x08 0x0C");
                system("i2cset -y 1 0x70 0x04 0x0C");
                system("i2cset -y 1 0x70 0x0A 0x0C");
                system("i2cset -y 1 0x70 0x02 0x0C");
                system("i2cset -y 1 0x70 0x0C 0x00");
                system("i2cset -y 1 0x70 0x00 0x00");
                system("i2cset -y 1 0x70 0x0E 0x0C");
                sleepForMs(250);
            }

            if((warning_state == true) && (alarm_state == false) && (obstructed == false)) {
                    if(CO2_trigger == true) {
                        printf("\n");
                        printf("WARNING: CO2 levels above threshold!\n");
                    }
                    else
                    if(smoke_trigger == true) {
                        printf("\n");
                        printf("WARNING: Smoke / Fammable Gas levels above threshold!\n");
                    }
                    else
                    if(temp_trigger == true) {
                        printf("\n");
                        printf("WARNING: Temperature above threshold!\n");
                    }
                    }
                else
                if(alarm_state == true) {
                    if(CO_trigger == true) {
                        printf("\n");
                        printf("ALARM: CO LEVEL HIGHER THAN THRESHOLD, LEAVE AREA IMMEIDIATELY!");
                        printf("\n");
                    }
                    else {
                        printf("\n");
                        printf("ALARM: TWO OR MORE SENSORS ABOVE THRESHOLD: ");
                        if(CO2_trigger == true) {
                                printf("CO2 SENSOR, ");
                            }
                        if(smoke_trigger == true) {
                                printf("SMOKE/GAS SENSOR, ");
                            }
                        if(temp_trigger == true) {
                                printf("TEMPERATURE SENSOR, ");
                            }        
                        printf("\n");
                    }

                    // Turn all on
                    system("i2cset -y 1 0x70 0x06 0xFF");
                    system("i2cset -y 1 0x70 0x08 0xFF");
                    system("i2cset -y 1 0x70 0x04 0xFF");
                    system("i2cset -y 1 0x70 0x0A 0xFF");
                    system("i2cset -y 1 0x70 0x02 0xFF");
                    system("i2cset -y 1 0x70 0x0C 0xFF");
                    system("i2cset -y 1 0x70 0x00 0xFF");
                    system("i2cset -y 1 0x70 0x0E 0xFF");
                    sleepForMs(250);
                }
  
            sleepForMs(250);
        }
        }
        else {
            return NULL;
        }
    }
}
