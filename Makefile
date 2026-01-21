all: main
	cp main $(HOME)/cmpt433/public/myApps/main

main: main.o sensor.o common.o output.o user.o
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -pthread -o main sensor.o common.o user.o output.o main.o  -lm

user.o: user.c user.h common.h
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c user.c

output.o: output.c output.h common.h
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c output.c

sensor.o: sensor.c sensor.h common.h
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c sensor.c

common.o: common.c common.h
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c common.c

main.o: main.c sensor.h user.h common.h output.h
	arm-linux-gnueabihf-gcc -D _POSIX_C_SOURCE=200809L -Wall -O2 -std=c17 -Wall -Werror -Wvla -Wextra -c main.c
 
clean:
	rm -f *.o
	rm -f main
	
