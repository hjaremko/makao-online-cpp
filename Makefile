CC = g++
CFLAGS = -std=c++14 -O2 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wmisleading-indentation -Wnull-dereference
LIBS = -lsfml-system -lsfml-network
# NCWINDOWS = Window.o Content.o ContentWindow.o LogWindow.o MenuWindow.o InputForm.o
# SOBJ = game-server.o $(NCWINDOWS) Server.o ServerPacket.o Game.o Deck.o
# GOOBJ = game-coordinator.o GameCoordinator.o $(NCWINDOWS) Server.o ServerPacket.o
# COBJ = game-client.o $(NCWINDOWS) Server.o ServerPacket.o
SOBJ = game-server.o Server.o Game.o Deck.o Card.o Player.o
COBJ = game-client.o Server.o Game.o Deck.o Card.o Player.o
GOOBJ = game-coordinator.o GameCoordinator.o Server.o
SRCPATH = src
# CURSESPATH = C:/Users/Hubert/Dropbox/C++/Biblioteki/pd
SFMLPATH = C:/SFML

all: server gc client clean

clean:
	del *.o

%.o: $(SRCPATH)/%.cpp
# $(CC) $(CFLAGS) -c $< -I$(CURSESPATH) -I$(SFMLPATH)/include -I$(SRCPATH)
	$(CC) $(CFLAGS) -c $< -I$(SFMLPATH)/include -I$(SRCPATH)

server: $(SOBJ)
	$(CC) $(SOBJ) -L$(SFMLPATH)/lib $(LIBS) -o $@

gc: $(GOOBJ)
	$(CC) $(GOOBJ) -L$(SFMLPATH)/lib $(LIBS) -o $@

client: $(COBJ)
# $(CC) $(COBJ) -L$(SFMLPATH)/lib -L$(CURSESPATH) $(LIBS) -o $@
	$(CC) $(COBJ) -L$(SFMLPATH)/lib $(LIBS) -o $@
