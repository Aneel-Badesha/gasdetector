#include "sensor.h"

// 0 = Temperature
// 1 = IR
// 2 = CO2
// 3 = Smoke / Air quality
// 5 = CO

int getVoltage1Reading(char* value)
{
    // Open file
    FILE *f = fopen(value, "r");
    if (!f)
    {
        printf("ERROR: unable to open voltage input file.\n");
        exit(-1);
    }

    // Get reading
    int a2dReading = 0;
    // int itemsRead = fgets();
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

double calVoltageTemp(int voltage) // compute the int voltage reading to double (from 0)
{
    double result = (A2D_VOLTAGE_REF_V) * ((double)voltage / (double)A2D_MAX_READING);
    double temperature = (double)(1000 * result - 500) / (double)10;
    return temperature;
}

double calVoltage(int voltage)  {

    double value = (A2D_VOLTAGE_REF_V) * ((double)voltage / (double)A2D_MAX_READING);
    return value;
}

void *readTemperature(void *arg)
{
    struct thread_data *data = arg;
    bool end_thread = false;
    bool initialization = true;
    int index = 0;

    int raw_data;
    double cal_values[BUFFER_SIZE];

    pthread_mutex_lock(&data->mutexTemp);
    {
        data->temp_value = 0;
    }
    pthread_mutex_unlock(&data->mutexTemp);

    while(1) {
    
        pthread_mutex_lock(&data->mutexControl); 
        {
            end_thread = data->end_all_threads; 
        }
        pthread_mutex_unlock(&data->mutexControl);

        if(end_thread == false) {
            index = index % BUFFER_SIZE;

            if(initialization == true) {
                for(int i = 0; i < BUFFER_SIZE; i++) {
                    // First initialization
                    raw_data = getVoltage1Reading(A2D_FILE_VOLTAGE0);
                    cal_values[i] = calVoltageTemp(raw_data);
                    // Fill up buffer quickly
                    sleepForMs(10);
                }
                
                initialization = false;
            } 
            else {
                // Normal case
                raw_data = getVoltage1Reading(A2D_FILE_VOLTAGE0);
                cal_values[index] = calVoltageTemp(raw_data);
                index++; 
            }
            
            double value = 0;
            for(int i = 0; i < BUFFER_SIZE; i++) {
                // Calculate average
                value = value + cal_values[i];
            }

            pthread_mutex_lock(&data->mutexTemp);
            {
                data->temp_value = value / BUFFER_SIZE;
            }
            pthread_mutex_unlock(&data->mutexTemp);
     
            sleepForMs(250);
            
        } 
        else {
            return NULL;
        }
    }
}

void *readIR(void *arg)
{
    
    struct thread_data *data = arg;
    bool end_thread = false;
    bool initialization = true;

    int index = 0;
    int raw_data;

    pthread_mutex_lock(&data->mutexIR);
    {
        data->IR_value = 0;
    }
    pthread_mutex_unlock(&data->mutexIR);

    while(1) {
        pthread_mutex_lock(&data->mutexControl); 
        {
            end_thread = data->end_all_threads; 
        }
        pthread_mutex_unlock(&data->mutexControl);

        if(end_thread == false) {
            index = index % BUFFER_SIZE;

            if(initialization == true) {
                for(int i = 0; i < BUFFER_SIZE; i++) {
                    // First initialization
                    raw_data = getVoltage1Reading(A2D_FILE_VOLTAGE1);
                    
                    pthread_mutex_lock(&data->mutexIR);
                    {
                        data->IR_buffer[i] = calVoltage(raw_data);
                    }
                    pthread_mutex_unlock(&data->mutexIR);    

                    sleepForMs(10);
                }
                
                initialization = false;
            } 
            else {
                // Normal case
                pthread_mutex_lock(&data->mutexIR);
                {
                    raw_data = getVoltage1Reading(A2D_FILE_VOLTAGE1);
                    data->IR_buffer[index] = calVoltage(raw_data);
                    index++; 
                }
                pthread_mutex_unlock(&data->mutexIR);
            }

            sleepForMs(250);
        } 
        else {
            return NULL;
        }
    }
}

void *readAirSensors(void *arg)
{
    struct thread_data *data = arg;
    bool end_thread = false;
    int raw_data_CO, raw_data_CO2, raw_data_smoke;

    while(1) {
        pthread_mutex_lock(&data->mutexControl);
        {
            end_thread = data->end_all_threads;
        }
        pthread_mutex_unlock(&data->mutexControl);

        if(end_thread == false) {
            raw_data_CO = getVoltage1Reading(A2D_FILE_VOLTAGE5);
            raw_data_CO2 = getVoltage1Reading(A2D_FILE_VOLTAGE2);
            raw_data_smoke = getVoltage1Reading(A2D_FILE_VOLTAGE3);

            pthread_mutex_lock(&data->mutexAir);
            {
                data->CO_value = calVoltage(raw_data_CO);
                data->CO2_value = calVoltage(raw_data_CO2);
                data->smoke_value = calVoltage(raw_data_smoke);
                
            }
            pthread_mutex_unlock(&data->mutexAir);

            sleepForMs(250);
        }
        else {
            return NULL;
        }
    }
}

