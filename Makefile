CC = g++
CFLAGS = -std=c++14
LIBS = -lpdcurses -lsfml-system -lsfml-network
NCWINDOWS = Window.o Content.o ContentWindow.o LogWindow.o MenuWindow.o InputForm.o
SOBJ = game-server.o $(NCWINDOWS) Server.o ServerPacket.o
GOOBJ = game-coordinator.o GameCoordinator.o $(NCWINDOWS) Server.o ServerPacket.o
COBJ = client.o Server.o ServerPacket.o
SRCPATH = src
CURSESPATH = C:/Users/Hubert/Dropbox/C++/Biblioteki/pd
SFMLPATH = C:/SFML

all: clean server gc client

clean:
	del *.o

%.o: $(SRCPATH)/%.cpp
	$(CC) $(CFLAGS) -c $< -I$(CURSESPATH) -I$(SFMLPATH)/include

server: $(SOBJ)
	$(CC) $(SOBJ) -L$(SFMLPATH)/lib -L$(CURSESPATH) $(LIBS) -o $@

gc: $(GOOBJ)
	$(CC) $(GOOBJ) -L$(SFMLPATH)/lib -L$(CURSESPATH) $(LIBS) -o $@

client: $(COBJ)
	$(CC) $(COBJ) -L$(SFMLPATH)/lib -L$(CURSESPATH) $(LIBS) -o $@
