CCFLAGS = -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32 #-mwindows
CSFLAGS = -I include/ -lwinmm -lws2_32

all: main run

main:
	gcc ./client/*.c -o client.exe $(CCFLAGS)
#gcc ./client/*.c -o client2.exe $(CCFLAGS)
#gcc ./server/*.c -o server.exe $(CSFLAGS)

run:
	./client.exe