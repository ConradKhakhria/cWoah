CC=gcc
CD=cd
RM=rm
SRC=src
INCL= datastructures/datastructures.c lexer.c parse/parse.c \
			parse/parse_type.c parse/parse_block_collect.c 				\
			woah_error.c misc.c
			
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
