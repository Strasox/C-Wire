OBJ_DIR := codeC/fichier_o/

all : prog

$(OBJ_DIR)/main.o : main.c main.h 
	gcc -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/fonctions.o : fonctions.c fonctions.h
	gcc -c fonctions.c -o $(OBJ_DIR)/fonctions.o

$(OBJ_DIR)/structure.o : structure.c structure.h
	gcc -c structure.c -o $(OBJ_DIR)/structure.o
	
prog : $(OBJ_DIR)/main.o $(OBJ_DIR)/fonctions.o 
	gcc  $(OBJ_DIR)/structure.o $(OBJ_DIR)/main.o  $(OBJ_DIR)/fonctions.o -o prog -lm