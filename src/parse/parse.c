#include "parse.h"

int traverse_block(Array prog, int start, int end, int open, int close)
{
   /* Gets the index of the next token that's not inside a bracketed block
    *
    * Parameters
    * ----------
    * - Array prog: the Array of tokens that the program consists of.
    * 
    * - int start:  The index of the first element inside a bracketed
    *               block. For example in the block:
    * 
    *               for i: int in 0 : array.len() {
    *                   io.println("{}", array[i]);    
    *               }
    * 
    *               the first token inside the block would be T_NAME 'io'.
    * 
    * - int end:    The index of the end of the array slice. NOTE: this means
    *               that traverse_block() will not travel to or beyond this
    *               index and will return -1 if it is reached.
    * 
    * - int open:   The TokenType of the opening delimiter. In the above example,
    *               int open = T_OPEN_CURLY_BRKT and int close = T_CLOSE_CURLY_BRKT.
    * 
    * - int close:  ""          ""          ""          ""
    * 
    * Returns: the index of the closing token delimiting the block
    * ------- 
    */
    struct Token** tokens = (struct Token **)prog->buffer;
    int bracket_depth     = 1;
    int index = start;

    while (start < end && bracket_depth > 0) {
        if (tokens[index]->token_type == open) {
            bracket_depth += 1;
        } else if (tokens[index]->token_type == close) {
            bracket_depth -= 1;
        }

        index += 1;
    }

    if (bracket_depth > 0) { // i.e. unbalanced brackets.
        return -1;
    } else {
        return index - 1; // As it goes one beyond the last close.
    }
}

int collect_blocks(Array tokens, Array* blocks)
{
   /* This will collect all of the blocks defined in the source program.
    *
    * Parameters
    * ----------
    * - Array tokens:    the Array containing the program tokens.
    * 
    * - Array* blocks:   the list of Array types to modify
    * 
    * Modifies
    * -------
    * A list containing:
    * - Array functions: all functions defined in the program.
    * - Array structs:   all structs.
    * - Array types:     all typedefs.
    * - Array module:    in case the source file is defined as (part of) a module,
    *                    this contains all of the above to be exported.
    * 
    * Returns
    * -------
    * 0 if successful, an error code if any (non-fatal) errors occur
    */
    int index = 0;

    while (index < tokens->index) {
        
    }

    return 0;
}
