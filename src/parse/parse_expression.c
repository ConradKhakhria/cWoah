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

                expr->expression.derivs[0] = parse_general_expression(
                    tokens_array, NULL, start, i - 1
                );

                expr->expression.derivs[1] = parse_general_expression(
                    tokens_array, NULL, i + 1, end
                );

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
        T_INEQ,
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

bool parse_function_call(Array tokens_array, struct ParseExpr* expr,
                         struct ParseExpr* parent, int start, int end)
{
   /* Checks if the expression is a function call
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the boolean expression.
    * 
    * - struct ParseExpr* expr: the parse expression to be (potentially) modified.
    * 
    * - struct ParseExpr* parent: the parent to this function (NULL if there is none)
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

    struct Token** tokens = (struct Token **)tokens_array->buffer;
    int argument_start    = start + 2;
    int argument_end      = argument_start;

    if (tokens[start]->token_type != T_NAME || tokens[start + 1]->token_type != T_OPEN_BRKT
    ||  tokens[end]->token_type   != T_CLOSE_BRKT) {
        return false;
    } else {
        expr->type = PET_FUNCTION_CALL;
        expr->expression.call = (struct FunctionCall) {
            .function_name  = tokens[start],
            .parent_expr    = parent,
        };
    }

    malloc_error(expr->expression.call.argument_exprs, PARSE_FUNCTION_CALL_ARGUMENT_EXPRS);

    Array arguments = make_array();

    // Lands on a new argument each time
    while (true) {
        // Finds the last token of the argument
        while (true) {
            if (argument_end >= end) {
                error_message("Unclosed arguments in function call or weird syntax.\n");
                error_println(tokens[argument_start]->line_no, tokens[argument_start]->col_no);
                exit(-SYNTAX_ERROR);
            } else if (tokens[argument_end]->token_type == T_OPEN_BRKT) {
                argument_end = traverse_block(
                    tokens_array, argument_end, end + 1, T_OPEN_BRKT, T_CLOSE_BRKT
                );

                if (argument_end == -1) {
                    error_message("Unbalanced brackets in function argument.\n");
                    error_println(tokens[argument_start]->line_no, tokens[argument_start]->col_no);
                    exit(-SYNTAX_ERROR);
                }
            } else if (tokens[argument_end]->token_type == T_OPEN_SQ_BRKT) {
                argument_end = traverse_block(
                    tokens_array, argument_end, end + 1, T_OPEN_SQ_BRKT, T_CLOSE_SQ_BRKT
                );

                if (argument_end == -1) {
                    error_message("Unbalanced square brackets in function argument.\n");
                    error_println(tokens[argument_start]->line_no, tokens[argument_start]->col_no);
                    exit(-SYNTAX_ERROR);
                }
            } else if (tokens[argument_end]->token_type == T_CLOSE_BRKT
                   ||  tokens[argument_end]->token_type == T_COMMA) {
                break;
            }

            argument_end += 1;
        }

        array_add(arguments, (void *)parse_general_expression(
            tokens_array, NULL, argument_start, argument_end - 1
        ));

        if (tokens[argument_end]->token_type == T_COMMA) {
            argument_end  += 1;
            argument_start = argument_end;
        } else {
            break;
        }
    }

    expr->expression.call.argument_count = arguments->buffer;
    expr->expression.call.argument_count = arguments->index;

    free(arguments);

    return true;
}

bool parse_list_index(Array tokens_array, struct ParseExpr* expr, int start, int end)
{
   /* Checks if the expression is a list being indexed.
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
    * Whether or not the expression is a list indexing.
    */

    struct Token** tokens = (struct Token **)tokens_array->buffer;

    if (tokens[start]->token_type != T_NAME || tokens[start + 1]->token_type != T_OPEN_SQ_BRKT
    ||  tokens[end]->token_type   != T_CLOSE_SQ_BRKT) {
        return false;
    } else {
        expr->type = PET_LIST_INDEX;
        expr->expression.list_index = (struct ListIndex) {
            .list_name = tokens[start],
            .index     = parse_general_expression(
                tokens_array, NULL, start + 2, end - 1
            )
        };

        return true;
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
                        tokens_array, NULL, start + 2, end - 1
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
                        tokens_array, NULL, start + 2, end - 1
                    )
                };
            }

            return true;

        default:
            return false;
    }
}

struct ParseExpr* parse_general_expression(Array tokens_array, struct ParseExpr* prev, int start, int end)
{
   /* Type-agnostic recursive descent parser for expressions of any form.
    *
    * Parameters
    * ----------
    * - Array tokens_array: the tokens containing the expression.
    * 
    * - struct ParseExpr* prev: the expression evaluated to the left of this one
    * 
    * - int start: the index of the first token in the expression.
    * 
    * - int end: the index of the last token in the expression.
    * 
    * Returns
    * -------
    * the struct ParseExpr* for the expression
    */

    struct Token** tokens  = (struct Token **)tokens_array->buffer;
    struct ParseExpr* expr = malloc(sizeof(struct ParseExpr));
    int index = start;

    malloc_error(expr, PARSE_GENERAL_EXPRESSION_ALLOC_EXPR);

    if (tokens[start]->token_type == T_OPEN_BRKT) {
        if (traverse_block(tokens_array, start, end, T_OPEN_BRKT, T_CLOSE_BRKT) == end) {
            free(expr);
            return parse_general_expression(tokens_array, prev, start + 1, end - 1);
        }
    } else if (tokens[start]->token_type == T_OPEN_SQ_BRKT) {
        if (traverse_block(tokens_array, start, end, T_OPEN_SQ_BRKT, T_CLOSE_SQ_BRKT) == end) {
            expr->type = PET_LIST_INDEX;
            expr->expression.list_index = (struct ListIndex) {
                //TODO: implement direct list indexing of a complex expression
                // e.g. function_name()[0] 
            };

            return expr;
        }
    }

    if (start == end) {
        switch (tokens[start]->token_type) {
            case T_NAME:
            case T_B2NUM:
            case T_B10NUM:
            case T_B16NUM:
            case T_FLOAT:
                expr->type = PET_ATOMIC;
                expr->expression.atom = tokens[start];

                return expr;
        
            default:
                error_message("Unexpected isolated token in expression.\n");
                error_println(tokens[start]->line_no, tokens[start]->col_no);
                exit(-SYNTAX_ERROR);
        }
    } else if (compound_boolean_expression(tokens_array, expr, start, end)) {
        return expr;
    } else if (compound_math_expression(tokens_array, expr, start, end)) {
        return expr;
    }

    // Look for attribute resolution - if one is found, the function returns so in
    // the rest of the code we know there's no attribute resolution.
    while (index >= end) {
        if (tokens[index]->token_type == T_ARROW || tokens[index]->token_type == T_FULL_STOP) {
            expr->type = PET_ATTR_RESOLUTION;

            expr->expression.at_res.is_pointer  = tokens[index]->token_type == T_ARROW;

            expr->expression.at_res.parent_attr = parse_general_expression(
                tokens_array, prev, start, index - 1
            );

            expr->expression.at_res.child_attr  = parse_general_expression(
                tokens_array, expr->expression.at_res.parent_attr, index + 1, end
            );

            return expr;
        } else if (tokens[index]->token_type == T_OPEN_BRKT
          || tokens[index]->token_type == T_OPEN_SQ_BRKT) {
            index = traverse_block(
                tokens_array, index, end, tokens[index]->token_type,
                (tokens[index]->token_type == T_OPEN_BRKT) ? T_CLOSE_BRKT : T_CLOSE_SQ_BRKT
            );

            if (index == -1) {
                error_message("Unmatched brackets in expression.\n");
                error_println(tokens[start]->line_no, tokens[start]->col_no);
                exit(-SYNTAX_ERROR);
            }
        }

        index += 1;
    }

    // Potentially find function call
    if (parse_function_call(tokens_array, expr, prev, start, end)) {
        return expr;
    } else if (parse_macro_use(tokens_array, expr, start, end)) {
        return expr;
    } else if (parse_list_index(tokens_array, expr, start, end)) {
        return expr;
    } else {
        error_message("Totally unrecognised syntax in expression between '");
        fprint_slice(
            stderr, program_source_buffer, tokens[start]->start_i, tokens[start]->end_i
        );
        fprintf(stderr, "' and '");
        fprint_slice(
            stderr, program_source_buffer, tokens[end]->start_i, tokens[end]->end_i
        );
        fprintf(stderr, "'.\n");

        error_println(tokens[start]->line_no, tokens[start]->col_no);
        exit(-SYNTAX_ERROR);
    }
}
