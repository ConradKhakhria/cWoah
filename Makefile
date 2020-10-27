CC=gcc
CD=cd
RM=rm
SRC=src
INCL=datastructures/datastructures.c lexer.c parse/parse.c
MAIN=main.c
TARGET=woah

all: $(TARGET)

$(TARGET):
	$(CD) $(SRC); $(CC) $(MAIN) $(INCL) -o ../$(TARGET)

expand:
	$(CD) $(SRC); $(CC) $(MAIN) $(INCL) -E

clean:
	$(RM) $(TARGET)
