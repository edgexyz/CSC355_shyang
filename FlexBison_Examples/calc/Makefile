# Makefile for building the simple calculator using flex and bison

# Compiler and tools
CC = gcc
FLEX = flex
BISON = bison

# Targets and flags
LEX_FILE = lexer.l
YACC_FILE = parser.y
EXEC = calculator
LEX_OUT = lex.yy.c
YACC_OUT = parser.tab.c
YACC_HEADER = parser.tab.h

# Flex and Bison output files
OBJS = $(YACC_OUT) $(LEX_OUT)

# Build the calculator executable
all: $(EXEC)

$(EXEC): $(OBJS)
		$(CC) -o $(EXEC) $(OBJS) -lfl

# Generate parser.tab.c and parser.tab.h using bison
$(YACC_OUT) $(YACC_HEADER): $(YACC_FILE)
		$(BISON) -d $(YACC_FILE)

# Generate lex.yy.c using flex
$(LEX_OUT): $(LEX_FILE)
		$(FLEX) $(LEX_FILE)

# Clean up generated files
clean:
		rm -f $(EXEC) $(LEX_OUT) $(YACC_OUT) $(YACC_HEADER)

