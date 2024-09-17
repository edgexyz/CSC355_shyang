GXX = g++
LEXER = flex
LEXER_FILE = lexer.l
LEXER_C_FILE = lex.yy.c
EXECUTABLE = lexer
CFLAGS = -Wall -Wno-unused-function -Wno-unneeded-internal-declaration -Wdeprecated
LDFLAGS = -lfl

all: $(EXECUTABLE)

$(LEXER_C_FILE): $(LEXER_FILE)
	$(LEXER) $(LEXER_FILE)

$(EXECUTABLE): $(LEXER_C_FILE)
	$(GXX) $(CFLAGS) -o $(EXECUTABLE) $(LEXER_C_FILE) $(LDFLAGS)

clean:
	rm -f $(LEXER_C_FILE) $(EXECUTABLE)

.PHONY: all clean run
