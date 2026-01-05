/**
 * lexer.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stddef.h>

#include "token.h"

typedef  struct lexer {
	const char *contents;
	size_t contents_length;
	token_array_t *tokens;

	size_t pos;
	size_t line;
	size_t column;
} lexer_t;

lexer_t *lexer_create(const char *contents, size_t contents_length);
token_array_t *lexer_lex(lexer_t *lexer);
void lexer_destroy(lexer_t *lexer);
