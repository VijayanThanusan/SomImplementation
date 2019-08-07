CC = gcc
SRC = main.c initialise.c network.c apprentissage.c
OBJ = main.o initialise.o network.o apprentissage.o

SOM:	$(OBJ) som.h
	$(CC) -o SOM $(OBJ) -lm
%.o:	%.c som.h
	$(CC) -c $<

clean:
	rm *.o *~
