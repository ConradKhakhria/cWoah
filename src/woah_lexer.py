import ply.lex as lex

tokens = (
  "COMMENT",
  "NEWLINE",
  "LBRACKET_RD",
  "RBRACKET_RD",
  "LBRACKET_SQ",
  "RBRACKET_SQ",
  "LBRACKET_CL",
  "RBRACKET_CL",
  "COMMA",
  "FULL_STOP",
  "COLON",
  "SEMICOLON",
  "AMPERSAND",
  "AT_SYM",
  "EQUALS2",
  "EQUALS1",
  "LESSTHAN",
  "GREATERTHAN",
  "FWD_SLASH",
  "BACKSLASH",
  "ADD_ASSIGN",
  "SUB_ASSIGN",
  "MUL_ASSIGN",
  "DIV_ASSIGN",
  "PLUSPLUS",
  "MINUSMINUS",
  "PLUS",
  "MINUS",
  "ASTERISK",
  "DBL_QUOTE",
  "SGL_QUOTE",
  # Operators etc
  "NOT_EQUAL",
  "LEQ",
  "GEQ",
  "SHL",
  "SHR",
  "ARROW",
  "SWAP",
  # Numbers
  "B2NUM",
  "B16NUM",
  "B10NUM",
  "FLOAT",
  # Keywords
  "FN",
  "STRUCT",
  "USE",
  "MACRO",
  "GLOBALS",
  "WHILE",
  "FOR",
  "IF",
  "ELIF",
  "ELSE",
  "IN",
  "SELF",
  "RETURN",
  "STACK",
  "HEAP",
  "AND",
  "OR",
  "NOT",
  # Otherwise
  "NAME"
)

def t_NEWLINE(t):
  r'\n'
  t.lexer.lineno += 1
  return t

def t_COMMENT(t):
  r'\/\/.*?(?=\n)'
  t.lexer.skip(1)

def t_error(t):
  print("Illegal character '%s'" % t.value[0])
  t.lexer.skip(1)

def find_column(input, token):
     line_start = input.rfind('\n', 0, token.lexpos) + 1
     return (token.lexpos - line_start) + 1

t_ignore      = ' \t'
t_LBRACKET_RD = r'\('
t_RBRACKET_RD = r'\)'
t_LBRACKET_SQ = r'\['
t_RBRACKET_SQ = r'\]'
t_LBRACKET_CL = r'{'
t_RBRACKET_CL = r'}'
t_COMMA       = r','
t_FULL_STOP   = r'\.'
t_COLON       = r':'
t_SEMICOLON   = r';'
t_AMPERSAND   = r'\&'
t_AT_SYM      = r'\@'
t_EQUALS1     = r'\='
t_LESSTHAN    = r'\<'
t_GREATERTHAN = r'\>'
t_FWD_SLASH   = r'\/'
t_BACKSLASH   = r'\\'
t_ADD_ASSIGN  = r'\+\='
t_SUB_ASSIGN  = r'\-\='
t_MUL_ASSIGN  = r'\*\='
t_DIV_ASSIGN  = r'\/\='
t_PLUSPLUS    = r'\+\+'
t_MINUSMINUS  = r'\-\-'
t_PLUS        = r'\+'
t_MINUS       = r'\-'
t_ASTERISK    = r'\*'
t_DBL_QUOTE   = r'"'
t_SGL_QUOTE   = r'\''
t_NOT_EQUAL   = r'\!\='
t_LEQ         = r'\<\='
t_GEQ         = r'\>\='
t_SHL         = r'\<\<'
t_SHR         = r'\>\>'
t_ARROW       = r'\-\>'
t_SWAP        = r'\<\-\>'
t_B2NUM       = r'0b[01][01_]*'
t_B16NUM      = r'0x[0-9a-eA-E][0-9a-eA-E_]*'
t_B10NUM      = r'[0-9][0-9]*'
t_FLOAT       = r'[0-9_]*.[0-9_]+'
t_FN          = r'fn'
t_STRUCT      = r'struct'
t_USE         = r'use'
t_MACRO       = r'macro'
t_GLOBALS     = r'globals'
t_WHILE       = r'while'
t_FOR         = r'for'
t_IF          = r'if'
t_ELIF        = r'elif'
t_ELSE        = r'else'
t_IN          = r'in'
t_SELF        = r'self'
t_RETURN      = r'return'
t_STACK       = r'stack'
t_HEAP        = r'heap'
t_AND         = r'and'
t_OR          = r'or'
t_NOT         = r'not'
t_NAME        = r'[a-zA-Z_][a-zA-Z0-9_]*'

def get_tokens(file_contents: str) -> list:
  lexer = lex.lex()
  lexer.input(file_contents)

  while True:
     tok = lexer.token()
     if not tok: 
         break      # No more input
     print(tok)
