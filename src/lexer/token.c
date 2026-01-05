/**
 * token.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "token.h"

// Retorna uma string de um TOK
const char *token_get_string(token_type_t type) {
	switch (type) {
	case TOK_FLOAT:
		return "FLOAT";
	case TOK_INTEGER:
		return "INTEGER";
	case TOK_STRING:
		return "STRING";
	case TOK_IDENTIFIER:
		return "IDENTIFIER";
	case TOK_SEMICOLON:
		return ";";
	case TOK_COMMA:
		return ",";
	case TOK_DOT:
		return ".";
	case TOK_EQUAL:
		return "=";
	case TOK_PLUS:
		return "+";
	case TOK_MINUS:
		return "-";
	case TOK_ASTERISK:
		return "*";
	case TOK_SLASH:
		return "/";
	case TOK_PERCENT:
		return "%";
	case TOK_BITWISE_OR:
		return "|";
	case TOK_BITWISE_XOR:
		return "^";
	case TOK_BITWISE_AND:
		return "&";
	case TOK_BITWISE_SHIFT_LEFT:
		return "<<";
	case TOK_BITWISE_SHIFT_RIGHT:
		return ">>";
	case TOK_EQUAL_EQUAL:
		return "==";
	case TOK_NOT_EQUAL:
		return "!=";
	case TOK_LESS_THAN:
		return "<";
	case TOK_GREATER_THAN:
		return ">";
	case TOK_LESS_OR_EQUAL:
		return "<=";
	case TOK_GREATER_OR_EQUAL:
		return ">=";
	case TOK_AND:
		return "&&";
	case TOK_OR:
		return "||";
	case TOK_LPAREN:
		return "(";
	case TOK_RPAREN:
		return ")";
	case TOK_LBRACE:
		return "{";
	case TOK_RBRACE:
		return "}";
	case TOK_LBRACKET:
		return "[";
	case TOK_RBRACKET:
		return "]";
	case TOK_EOF:
		return "EOF";
	default:
		return "UNKNOWN";
	}
}

// Cria um novo token_array
token_array_t *token_array_create(size_t initialCap) {
	token_array_t *array = (token_array_t *)malloc(sizeof(token_array_t));
	if (!array)
		return NULL;

	array->count = 0;
	array->cap = initialCap;
	array->tokens = (token_t *)malloc(sizeof(token_t) * initialCap);

	return array;
}

// Adiciona um token a lista do token_array
bool token_array_push(token_array_t *array, token_t token) {
	if (!array)
		return false;

	if (array->count >= array->cap) {
		size_t newCap = array->cap * 2;

		token_t *newTokens =
			(token_t *)realloc(array->tokens, sizeof(token_t) * newCap);
		if (!newTokens)
			return false;

		array->cap = newCap;
		array->tokens = newTokens;
	}

	array->tokens[array->count++] = token;

	return true;
}

// Imprime um token_array
int token_array_print(token_array_t *array) {
	if (!array)
		return 1;
	if (!array->tokens)
		return 2;

	for (size_t i = 0; i < array->count; i++) {
		token_t token = array->tokens[i];
		printf("token_t %zu: type='%s', lexeme='%.*s', pos=(%zu,%zu)\n", i,
			   token_get_string(token.type), (int)token.length, token.start,
			   token.line, token.column);
	}

	return true;
}

// Destroi um token_array
void token_array_destroy(token_array_t *array) {
	if (!array)
		return;

	free(array->tokens);
	free(array);
}

// Aponta pra um erro e mostra ele usando Token
void token_error(token_t token, const char *format, ...) {
	printf("\033[31mERROR[%zu:%zu]: ", token.line, token.column);

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");

	point_to_word(token.realContent, token.line - 1, token.column - 1,
				token.length);
	printf("\033[0m");
}
