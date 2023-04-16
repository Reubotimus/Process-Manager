CC=gcc
#CPPFLAGS= -Wall  -Werror  -O3 -g 
CPPFLAGS= -Wall  -Werror  -g
LDFLAGS = -lm

SRC=process-manager.o input.o linked-list.o process-scheduler.o process.o queue.o memory-allocator.o
TARGET=allocate


debug ?= 1 
ifeq ($(debug),1) 
	CPPFLAGS= -Wall -Werror -g
endif


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CPPFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o