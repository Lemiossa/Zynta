/**
 * token.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum token_type {
	TOK_FLOAT,   // 3.14
	TOK_INTEGER, // 123
	TOK_STRING,  // "Hello World"

	TOK_IDENTIFIER, // main

	TOK_SEMICOLON, // ;
	TOK_COMMA,	 // ,
	TOK_DOT,		 // .

	TOK_EQUAL, // =

	TOK_PLUS,		// +
	TOK_MINUS,	// -
	TOK_ASTERISK, // *
	TOK_SLASH,	// /
	TOK_PERCENT,	// %

	TOK_BITWISE_OR,  // |
	TOK_BITWISE_XOR, // ^
	TOK_BITWISE_AND, // &

	TOK_BITWISE_SHIFT_LEFT,  // <<
	TOK_BITWISE_SHIFT_RIGHT, // >>

	TOK_EQUAL_EQUAL, // ==
	TOK_NOT_EQUAL,   // !=

	TOK_LESS_THAN,		// <
	TOK_GREATER_THAN,		// >
	TOK_LESS_OR_EQUAL,	// <=
	TOK_GREATER_OR_EQUAL, // >=

	TOK_AND, // && ou and
	TOK_OR,  // || ou or

	TOK_LPAREN,	// (
	TOK_RPAREN,	// )
	TOK_LBRACE,	// {
	TOK_RBRACE,	// }
	TOK_LBRACKET, // [
	TOK_RBRACKET, // ]

	TOK_KEYWORD_TRUE,	 // True
	TOK_KEYWORD_FALSE, // False

	TOK_KEYWORD_NIL, // Nil

	TOK_EOF // \0
} token_type_t;

typedef struct token {
	const char *start;
	size_t length;
	token_type_t type;

	size_t line;
	size_t column;
	const char *realContent;
} token_t;

typedef struct token_array {
	token_t *tokens;
	size_t count;
	size_t cap;
} token_array_t;

const char *token_get_string(token_type_t type);
token_array_t *token_array_create(size_t initialCap);
bool token_array_push(token_array_t *array, token_t token);
int token_array_print(token_array_t *array);
void token_array_destroy(token_array_t *array);
void token_error(token_t token, const char *format, ...);
