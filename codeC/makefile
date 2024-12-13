# MAKEFILE temporaire, nous permetant de compiler tout le code (ChatGPT)
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = exec

# Règles
all: $(TARGET)

$(TARGET): teste.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
