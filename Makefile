CC=gcc
CD=cd
RM=rm
SRC=src
INCL=datastructures/datastructures.c lexer.c parse/parse.c parse/parse_type.c woah_error.c
MAIN=main.c
TARGET=woah
FLAGS=-lm -Wall

all: $(TARGET)

$(TARGET):
	$(CD) $(SRC); $(CC) $(MAIN) $(INCL) $(FLAGS) -o ../$(TARGET)

expand:
	$(CD) $(SRC); $(CC) $(MAIN) $(INCL) -E

clean:
	$(RM) $(TARGET)
