all: main

main: main.o sensor.o common.o output.o user.o
	gcc -D _POSIX_C_SOURCE=200809L -pthread -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -o main sensor.o common.o user.o output.o main.o  -lm

user.o: user.c user.h common.h
	gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c user.c

output.o: output.c output.h common.h
	gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c output.c

sensor.o: sensor.c sensor.h common.h
	gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c sensor.c

common.o: common.c common.h
	gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c common.c

main.o: main.c sensor.h user.h common.h output.h
	gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c main.c

# Build hardware test utility
test: test_hardware.c
	gcc -o test_hardware test_hardware.c -Wall
 
clean:
	rm -f *.o
	rm -f main
	rm -f test_hardware
	