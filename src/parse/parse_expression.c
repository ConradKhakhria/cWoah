#include "parse_expression.h"

bool compound_expression(Array tokens_array, int* operators, int operator_count,
                         struct ParseExpr* expr, int start, int end)
{
   /* Main logic for compound_boolean_expression() and compound_math_expression() */
    struct Token** tokens = (struct Token **)tokens_array->buffer;

    // To ignore sub-expressions in round and square brackets
    int bracket_depths[2] = { 0, 0 };

    for (int op = 0; op < operator_count - 1; op++) {
        for (int i = start; i <= end; i++) {
            if (tokens[i]->token_type == T_OPEN_BRKT) {
                bracket_depths[RND] += 1;
            } else if (tokens[i]->token_type == T_CLOSE_BRKT) {
                bracket_depths[RND] -= 1;
            } else if (tokens[i]->token_type == T_OPEN_SQ_BRKT) {
                bracket_depths[SQU] += 1;
            } else if (tokens[i]->token_type == T_CLOSE_SQ_BRKT) {
                bracket_depths[SQU] -= 1;
            } else if (tokens[i]->token_type == operators[op]) {
                expr->type  = (operators[0] == T_OR) ? PET_COMPOUND_BOOLEAN : PET_COMPOUND_ARITHMETIC;
                expr->value = operators[op];
                expr->expression.derivs = malloc(sizeof(struct BoolExpr *) * 2);

                malloc_error(expr->expression.derivs, PARSE_BOOLEAN_EXPRESSION_NESTED_LIST);

                if (op < 4) {
                    expr->expression.derivs[0] = parse_boolean_expression(
                        tokens_array, start, i - 1
                    );

                    expr->expression.derivs[1] = parse_boolean_expression(
                        tokens_array, i + 1, end
                    );
                } else {
                    expr->expression.derivs[0] = parse_math_expression(
                        tokens_array, start, i - 1
                    );

                    expr->expression.derivs[1] = parse_math_expression(
                        tokens_array, i + 1, end
                    );
                }

                return true;
            }
        }
    }

    return false;
}

bool compound_boolean_expression(Array tokens_array, struct ParseExpr* expr, int start, int end)
{
   /* Determines whether or not the expression is a compound boolean expression.
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the boolean expression.
    * 
    * - struct ParseExpr expr: the expression to be modified if it is a compound expression.
    * 
    * - int start: the index of the first token in the expression.
    * 
    * - int end: the index of the last token in the expression.
    * 
    * Modifies
    * --------
    * - struct BoolExpr* expr if and only if it is compound.
    * 
    * Returns: whether or not it is compound.
    * -------
    */

    int operators[10] = {
        T_OR,
        T_AND,
        T_XOR,
        T_NOT,
        T_DBL_EQUALS,
        T_LT,
        T_GT,
        T_LEQ,
        T_GEQ
    };

    return compound_expression(tokens_array, operators, 10, expr, start, end);
}

bool compound_math_expression(Array tokens_array, struct ParseExpr* expr, int start, int end)
{
   /* Determines whether or not the expression is a compound maths expression.
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the math expression.
    * 
    * - struct ParseExpr expr: the expression to be modified if it is a compound maths expression.
    * 
    * - int start: the index of the first token in the expression.
    * 
    * - int end: the index of the last token in the expression.
    * 
    * Modifies
    * --------
    * - struct ParseExpr* expr if and only if it is a compound maths expression.
    * 
    * Returns: whether or not it is a compound expression.
    * -------
    */

    int operators[5] = {
        T_PLUS,
        T_MINUS,
        T_ASTERISK,
        T_FWD_SLASH,
        T_PERCENT
    };

    return compound_expression(tokens_array, operators, 5, expr, start, end);
}

bool parse_function_call(Array tokens_array, struct ParseExpr* expr, int start, int end)
{
   /* Parses a function call into a struct FunctionCall*
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the boolean expression.
    * 
    * - int start: the first token in the expression.
    * 
    * - int end: the last token in the expression.
    * 
    * Modifies
    * --------
    * struct BoolExpr* expr if and only if it's a function call.
    * 
    * Returns
    * -------
    * Whether it is in fact a function call.
    * 
    */
    struct Token** tokens     = (struct Token **)tokens_array->buffer;
    struct FunctionCall* call = malloc(sizeof(struct FunctionCall));

    malloc_error(call, PARSE_FUNCTION_CALL_ALLOC_STRUCT);

    if (tokens[start]->token_type != T_NAME) {
        free(call);

        return false;
    } else if (tokens[start + 1]->token_type == T_OPEN_BRKT
    && tokens[end]->token_type == T_CLOSE_BRKT) {
        call->function_name = tokens[start];
        call->parent_name   = NULL;

        start += 2;
    } else if (tokens[start + 2] == T_NAME
    && tokens[start + 3]->token_type == T_OPEN_BRKT
    && tokens[start + 1]->token_type == T_FULL_STOP) {
        call->function_name = tokens[start + 2];
        call->parent_name   = tokens[start];

        start += 4;
    } else {
        free(call);

        return false;
    }

    // Each iteration lands on a new argument
    while (true) {
        
    }


}

bool parse_macro_use(Array tokens_array, struct ParseExpr* expr, int start, int end)
{
   /* Checks if the expression is a usage of a built-in macro
    * 
    * Parameters
    * ----------
    * ""        ""          ""
    * 
    * Modifies
    * --------
    * ""        ""          ""
    * 
    * Returns
    * -------
    * Whether or not the expression is a macro use
    */

    struct Token** tokens = (struct Token **)tokens_array->buffer;

    expr->type = PET_MACRO_USE;

    switch (tokens[start]->token_type) {
        case T_CAST:
            if (tokens[start + 1]->token_type != T_OPEN_SQ_BRKT
            ||  tokens[end]->token_type != T_CLOSE_SQ_BRKT) {
                error_message("cast! macro not followed by expression in square brackets.\n");
                error_println(tokens[start]->line_no, tokens[start]->col_no);
                exit(-SYNTAX_ERROR);
            } else {
                expr->expression.macro_use = (struct MacroUse) {
                    .macro_name = T_CAST,
                    .derivs     = parse_general_expression(
                        tokens_array, start + 2, end - 1
                    )
                };
            }

            return true;

        case T_HEAP:
        case T_STACK:
            if (tokens[start + 1]->token_type != T_OPEN_SQ_BRKT
            ||  tokens[end]->token_type != T_CLOSE_SQ_BRKT) {
                error_message("macro use not followed by square brackets.\n");
                error_println(tokens[start]->line_no, tokens[start]->col_no);
                exit(-SYNTAX_ERROR);
            } else if (start + 2 == end) {
                expr->expression.macro_use = (struct MacroUse) {
                    .macro_name = tokens[start]->token_type,
                    .derivs     = NULL
                };
            } else {
                expr->expression.macro_use = (struct MacroUse) {
                    .macro_name = T_HEAP,
                    .derivs     = parse_general_expression(
                        tokens_array, start + 2, end - 1
                    )
                };
            }

            return true;
    
        default:
            return false;
    }
}
