/**
 * parser.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stddef.h>

#include "../lexer/token.h"
#include "ast_node.h"

typedef struct parser {
	size_t pos;
	token_array_t *tokens;
} parser_t;

parser_t *parser_create(token_array_t *tokens);
ast_node_t *parser_parse(parser_t *parser);
void parser_destroy(parser_t *parser);
