CC 		= g++
OBJS 	= Main.o Jeu.o Affichable.o ./Vaisseaux/Vaisseau.o ./Vaisseaux/Chercheur.o ./Vaisseaux/Tourelle.o ./Tirs/Tir.o
CFLAGS  = -c -Wall -ansi -pedantic
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LDFLAGS	= 
PGM		= space_war

all: $(PGM)

$(PGM):	$(OBJS)
	$(CC) -o $(PGM) $(LDFLAGS) $(OBJS) $(SFMLFLAGS)
.c.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f $(PGM)
	rm -f $(OBJS)
	rm -f *~
	rm -f *.gch
	rm -f ./Vaisseaux/*.gch
	rm -f ./Tirs/*.gch
