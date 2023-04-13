CC=gcc
#CPPFLAGS= -Wall  -Werror  -O3 -g 
CPPFLAGS= -Wall  -Werror  -g  -O3
LDFLAGS = -lm

SRC=process-manager.o
TARGET=allocate


debug ?= 1 
ifeq ($(debug),1) 
	CPPFLAGS= -Wall -Werror -g
endif


$(TARGET): $(SRC)
	$(CC) $(SRC) $(CPPFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o