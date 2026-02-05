/**
* parser.h
* Interface of the parser
*/

#pragma once

#include "token.h"
#include "ast.h"

typedef struct Parser {

} Parser;

Parser* create_parser(Token** tokens, int token_count);
void free_parser(Parser* parser);
ASTNode* parse_parser(Parser* parser);
