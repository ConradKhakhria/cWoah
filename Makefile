CC=gcc
CD=cd
RM=rm
SRC=src/
INCL=datastructures.c lexer.c
MAIN=main.c
TARGET=woah

all: $(TARGET)

$(TARGET):
	$(CD) $(SRC); $(CC) $(MAIN) $(INCL) -o ../$(TARGET)

clean:
	$(RM) $(TARGET)
