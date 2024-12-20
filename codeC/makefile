OBJ_DIR := fichier_o/

all: exec

# Crée le répertoire des fichiers objets si nécessaire
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile les fichiers objets (les dépendances incluent la création du répertoire)
$(OBJ_DIR)/main.o: main.c main.h | $(OBJ_DIR)
	gcc -c main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/fonctions.o: fonctions.c fonctions.h | $(OBJ_DIR)
	gcc -c fonctions.c -o $(OBJ_DIR)/fonctions.o

$(OBJ_DIR)/structure.o: structure.c structure.h | $(OBJ_DIR)
	gcc -c structure.c -o $(OBJ_DIR)/structure.o

# Crée l'exécutable final
exec: $(OBJ_DIR)/main.o $(OBJ_DIR)/fonctions.o $(OBJ_DIR)/structure.o
	gcc $(OBJ_DIR)/main.o $(OBJ_DIR)/fonctions.o $(OBJ_DIR)/structure.o -o exec -lm

# Nettoie les fichiers générés
clean:
	rm -rf $(OBJ_DIR) exec