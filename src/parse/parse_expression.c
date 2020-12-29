#include "parse_expression.h"

bool compound_boolean_expression(Array tokens_array, struct BoolExpr* expr, int start, int end)
{
   /* Determines whether or not the expression is a compound expression.
    *
    * Parameters
    * ----------
    * - same as parse_boolean_expression() and also:
    * 
    * - struct BoolExpr expr: the return value for parse_boolean_expression().
    * 
    * Modifies
    * --------
    * - struct BoolExpr* expr if and only if it is compound.
    * 
    * Returns: whether or not it is compound.
    * -------
    */
    struct Token** tokens = (struct Token **)tokens_array->buffer;
    int operators[10]     = { T_OR, T_AND, T_XOR, T_NOT, T_DBL_EQUALS,
                              T_INEQ, T_LT, T_GT, T_LEQ, T_GEQ };

    // Round brackets and square brackets
    int bracket_depths[2] = { 0, 0 };

    // If it's a compound expression (e.g. "var1 and var2 or not var3")
    for (int op = 0; op < 9; op++) {
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
                expr->type  = BET_NESTED;
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

struct FunctionCall* parse_function_call(Array tokens_array, struct BoolExpr* expr, int start, int end)
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
    * A pointer to a nested struct FunctionCall containing the expression.
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

struct BoolExpr* parse_boolean_expression(Array tokens_array, int start, int end)
{
   /* Parses a boolean expression into a struct BoolExpr*
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the boolean expression.
    * 
    * - int start: the first token in the expression.
    * 
    * - int end: the last token in the expression.
    * 
    * Modifies: Nothing.
    * --------
    * 
    * Returns
    * -------
    * A pointer to a nested struct BoolExpr containing the expression.
    */
    struct Token** tokens = (struct Token **)tokens_array->buffer;
    struct BoolExpr* expr = malloc(sizeof(struct BoolExpr));

    malloc_error(expr, PARSE_BOOLEAN_EXPRESSION_ALLOC_BOOLEXPR);

    if (start == end && (tokens[start]->token_type == T_NAME
    || tokens[start]->token_type == T_TRUE || tokens[start]->token_type == T_FALSE)) {
        expr->type  = BET_ATOM;
        expr->value = tokens[start]->token_type;

        return expr;
    } else if (compound_boolean_expression(tokens_array, expr, start, end)) {
        return expr;
    } else if ()

    else {
        error_message("Unrecognised syntax in boolean expression.\n");
        error_println(tokens[start]->line_no, tokens[start]->col_no);
        exit(-SYNTAX_ERROR);
    }
}

