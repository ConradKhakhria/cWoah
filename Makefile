CC    = gcc
CD    = cd
RM    = rm
SRC   = src
INCL  = datastructures/datastructures.c \
		parse/parse_block_collect.c	    \
		parse/parse_expression.c		\
		parse/parse_type.c				\
		parse/parse.c 					\
		init.c							\
        lexer.c                         \
        misc.c                          \
        woah_error.c
			
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
