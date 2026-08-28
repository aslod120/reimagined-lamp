CCFLAGS = -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -lws2_32
CSFLAGS = -I include/ -lwinmm -lws2_32

all: main

main:
	gcc ./client/*.c -o client.exe $(CCFLAGS)
	gcc ./client/*.c -o client2.exe $(CCFLAGS)
	gcc ./server/*.c -o server.exe $(CSFLAGS)