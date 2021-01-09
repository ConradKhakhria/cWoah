#pragma once

// All types of token (for use in struct Token)
enum TokenType {
    // Single characters
    T_NEWLINE,
    T_OPEN_BRKT,        // (
    T_CLOSE_BRKT,       // )
    T_OPEN_SQ_BRKT,     // [
    T_CLOSE_SQ_BRKT,    // ]
    T_OPEN_CURLY_BRKT,  // {
    T_CLOSE_CURLY_BRKT, // }
    T_COMMA,            // ,
    T_FULL_STOP,        // .
    T_COLON,            // :
    T_SEMICOLON,        // ;
    T_AMPERSAND,        // &
    T_AT_SYM,           // @
    T_SGL_EQUALS,       // =
    T_LT,               // <
    T_GT,               // >
    T_FWD_SLASH,        // /
    T_BACKSLASH,        // (the reverse of the above)
    T_PLUS,             // +
    T_MINUS,            // -
    T_ASTERISK,         // *
    T_PERCENT,          // %
    T_DBL_QUOT_STRING,  // ".*?"
    T_SGL_QUOT_STRING,  // '.*?'

    // Operators
    T_DBL_EQUALS,       // ==
    T_INEQ,             // !=
    T_LEQ,              // <=
    T_GEQ,              // >=
    T_ADD_ASSIGN,       // +=
    T_SUB_ASSIGN,       // -=
    T_MUL_ASSIGN,       // *=
    T_DIV_ASSIGN,       // /=
    T_INC,              // ++
    T_DEC,              // --
    T_ARROW,            // ->
    T_SWAP,             // <->

    // Numbers
    T_B2NUM,            // -?0b[01]+
    T_B10NUM,           // -?[0-9]+
    T_B16NUM,           // -?(0x|0X)[0-9a-fA-F]+
    T_FLOAT,            // -?[0-9]*\.[0-9]+

    // Names
    T_NAME,             // [a-zA-Z_][a-zA-Z0-9_]*

    // Type names
    T_bool,
    T_i8,
    T_i16,
    T_i32,
    T_i64,
    T_i128,
    T_int,
    T_u8,
    T_u16,
    T_u32,
    T_u64,
    T_u128,
    T_uint,
    T_f8,
    T_f16,
    T_f32,
    T_f64,
    T_f128,
    T_float,
    T_char,

    // Keywords (all of the following are just the name but in upper case)
    T_AND,
    T_CAST,
    T_ELIF,
    T_ELSE,
    T_FALSE,
    T_FN,
    T_FOR,
    T_GLOBALS,
    T_HEAP,
    T_IF,
    T_IN,
    T_MACRO,
    T_MODULE,
    T_NONE,
    T_NOT,
    T_NULL,
    T_OR,
    T_RETURN,
    T_SELF,
    T_STACK,
    T_STRUCT,
    T_TRUE,
    T_TYPE,
    T_USE,
    T_WHILE,
    T_XOR
};


enum MallocErrorCodes {
    MAIN_PROGRAM_SOURCE_BUFFER,
    MAKE_ARRAY_INIT_STRUCT,
    MAKE_ARRAY_INIT_BUFFER,
    ARRAY_ADD_BUFFER_REALLOC,
    LEXER_ALLOC_TOKEN,
    PARAMETRIC_TYPE_DERIVS,
    TYPE_STRUCT,
    COLLECT_BLOCK_FUNCTION_STRUCT,
    COLLECT_BLOCK_FUNCTION_ARG_NAMES,
    COLLECT_BLOCK_FUNCTION_ARG_TYPES,
    COLLECT_BLOCK_FUNCTION_NAMES_REALLOC,
    COLLECT_BLOCK_FUNCTION_TYPES_REALLOC,
    COLLECT_BLOCK_STRUCT_STRUCT,
    COLLECT_BLOCK_STRUCT_FIELD_NAMES,
    COLLECT_BLOCK_STRUCT_FIELD_TYPES,
    COLLECT_BLOCK_STRUCT_NAMES_REALLOC,
    COLLECT_BLOCK_STRUCT_TYPES_REALLOC,
    COLLECT_BLOCK_TYPEDEF_STRUCT,
    COLLECT_BLOCK_IMPORT_STRUCT,
    COLLECT_BLOCK_IMPORT_LIST_ALLOC,
    COLLECT_BLOCK_EXPORT_RET_EXPORT,
    COLLECT_BLOCK_GLOBALS_STRUCT,
    COLLECT_BLOCK_GLOBALS_NAMES,
    COLLECT_BLOCK_GLOBALS_TYPES,
    PARSE_BOOLEAN_EXPRESSION_ALLOC_BOOLEXPR,
    PARSE_BOOLEAN_EXPRESSION_NESTED_LIST,
    PARSE_FUNCTION_CALL_ARGUMENT_EXPRS,
    PARSE_GENERAL_EXPRESSION_ALLOC_EXPR,
    PARSE_STATEMENT_EXPR_STRUCT,
    PARSE_STATEMENT_STMT_STRUCT
};

/* Parse structure enums */

enum TypeForm {
    TF_LIST,        // T[]
    TF_POINTER,     // &T
    TF_PARAMETRIC,  // T<x, y, ...>
    TF_STRUCT,      // struct foo
    TF_ATOMIC,      // T
};

enum ModuleImportType {
    MT_QUALIFIED,
    MT_SPECIFIC,
    MT_ALL
};

enum ParseExprType {
    PET_ATOMIC,
    PET_COMPOUND_BOOLEAN,
    PET_COMPOUND_ARITHMETIC,
    PET_FUNCTION_CALL,
    PET_LIST_INDEX,
    PET_MACRO_USE,
    PET_ATTR_RESOLUTION,
};

enum WStatementType {
    WPE_DECL,
    WPE_ASSIGN,
    WPE_IF,
    WPE_FOR,
    WPE_WHILE
};
