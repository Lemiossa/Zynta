/**
 * lexer.c
 * Criado por Matheus Leme Da Silva
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keywords.h"
#include "lexer.h"
#include "token.h"

// Retorna true se esta no fim
bool lexer_at_end(lexer_t *lexer) {
	if (!lexer || !lexer->contents || !lexer->tokens)
		return false; // prevenir um pouco
	if (lexer->pos >= lexer->contents_length)
		return true;

	return false;
}

// Retorna o caractere atual
char lexer_peek(lexer_t *lexer) {
	if (lexer_at_end(lexer))
		return 0;
	return lexer->contents[lexer->pos];
}

// Retorna o caractere atual e avança o ponteiro
char lexer_advance(lexer_t *lexer) {
	if (lexer_at_end(lexer))
		return 0;
	lexer->pos++;
	lexer->column++;
	return lexer_peek(lexer);
}

// Retorna o proximo caractere sem avançar
char lexer_next(lexer_t *lexer) {
	if (lexer_at_end(lexer))
		return 0;
	if (lexer->pos + 1 >= lexer->contents_length)
		return 0;
	return lexer->contents[lexer->pos + 1];
}

// Pula espaços, \t, \r e \n em um Lexer
void lexer_skip_whitespaces(lexer_t *lexer) {
	if (!lexer)
		return;

	char c = lexer_peek(lexer);

	while (!lexer_at_end(lexer) &&
		   (c == '\n' || c == '\t' || c == '\r' || c == ' ')) {
		if (c == '\n') {
			lexer->line++;
			lexer->column = 1;
		} else {
			lexer->column++;
		}
		lexer->pos++;
		c = lexer_peek(lexer);
	}
}

// Adiciona um token no array de Tokens do lexer
bool lexer_add_token(lexer_t *lexer, token_type_t type, const char *start,
				   size_t length) {
	if (!lexer)
		return false;
	token_t t;
	t.start = start;
	t.length = length;
	t.type = type;
	t.line = lexer->line;
	t.column = lexer->column;
	t.realContent = lexer->contents;

	if (!token_array_push(lexer->tokens, t)) {
		printf(
			"lexer_t internal error: Failed to add token to tokenlist, sorry\n");
		return false;
	}
	return true;
}

// Retorna o ponteiro atual para o content real de um Lexer
const char *lexer_peek_real(lexer_t *lexer) {
	if (!lexer)
		return NULL;
	if (lexer_at_end(lexer))
		return NULL;
	return &lexer->contents[lexer->pos];
}

// Cria um lexer_t novo
lexer_t *lexer_create(const char *contents, size_t contents_length) {
	if (!contents || contents_length == 0)
		return NULL;
	lexer_t *lexer = (lexer_t *)malloc(sizeof(lexer_t));
	if (!lexer)
		return NULL;

	lexer->contents = contents;
	lexer->contents_length = contents_length;
	lexer->tokens = token_array_create(64);
	if (!lexer->tokens) {
		printf("lexer_t internal error: Failed to create token array\n");
		free(lexer);
		return NULL;
	}

	lexer->pos = 0;
	lexer->line = 1;
	lexer->column = 1;

	return lexer;
}

// Analiza um Lexer
// Por enquanto, só inteiros, strings e identificadores
token_array_t *lexer_lex(lexer_t *lexer) {
	if (!lexer) {
		printf("lexer_t internal error: lexer_t is null\n");
		return NULL;
	}

	while (!lexer_at_end(lexer)) {
		lexer_skip_whitespaces(lexer);
		char c = lexer_peek(lexer);

		// Comentários
		if (c == '#') {
			while (!lexer_at_end(lexer) && c != '\n')
				c = lexer_advance(lexer);
			if (c == '\n') {
				lexer->line++;
				lexer->column = 1;
			}
			continue;
		}

		// Strings
		if (c == '"' || c == '\'') {
			char pair = c;
			lexer_advance(lexer);
			const char *start = lexer_peek_real(lexer);

			const char *end = NULL;
			while (!lexer_at_end(lexer)) {
				c = lexer_advance(lexer);
				end = lexer_peek_real(lexer);
				if (c == pair)
					break;
				if (c == '\n') {
					lexer->line++;
					lexer->column = 1;
					break;
				}
			}

			size_t length = (size_t)(end - start);
			if (!lexer_add_token(lexer, TOK_STRING, start, length))
				return NULL;

			if (c != pair) {
				token_error(lexer->tokens->tokens[lexer->tokens->count - 1],
						   "Lexical error: Unclosed string\n");
				return NULL;
			}
			lexer_advance(lexer);
			continue;
		}

		// Números
		if (isdigit(c)) {
			const char *start = lexer_peek_real(lexer);
			char *end;

			strtoll(start, &end, 0);

			// double
			if (*end == '.') {
				strtod(start, &end);
				size_t length = (size_t)(end - start);
				lexer->pos += length;
				lexer->column += length;

				if (!lexer_add_token(lexer, TOK_FLOAT, start, length))
					return NULL;
			} else {
				size_t length = (size_t)(end - start);
				lexer->pos += length;
				lexer->column += length;

				if (!lexer_add_token(lexer, TOK_INTEGER, start, length))
					return NULL;
			}
			continue;
		}

		// Identificadores
		if (c == '_' || isalpha(c)) {
			const char *start = lexer_peek_real(lexer);
			while (!lexer_at_end(lexer) && isalnum(c))
				c = lexer_advance(lexer);

			const char *end = lexer_peek_real(lexer);
			size_t length = (size_t)(end - start);
			if (!lexer_add_token(lexer, TOK_IDENTIFIER, start, length))
				return NULL;
			continue;
		}

		const char *start = lexer_peek_real(lexer);
		switch (c) {
		case ';': {
			lexer_add_token(lexer, TOK_SEMICOLON, start, 1);
		} break;
		case ',': {
			lexer_add_token(lexer, TOK_COMMA, start, 1);
		} break;
		case '.': {
			lexer_add_token(lexer, TOK_DOT, start, 1);
		} break;
		case '=': {
			if (lexer_next(lexer) == '=') {
				lexer_add_token(lexer, TOK_EQUAL_EQUAL, start, 2);
				lexer_advance(lexer);
			} else {
				lexer_add_token(lexer, TOK_EQUAL, start, 1);
			}
		} break;
		case '<': {
			if (lexer_next(lexer) == '=') {
				lexer_add_token(lexer, TOK_LESS_OR_EQUAL, start, 2);
				lexer_advance(lexer);
			} else if (lexer_next(lexer) == '<') {
				lexer_add_token(lexer, TOK_BITWISE_SHIFT_LEFT, start, 2);
				lexer_advance(lexer);
			} else {
				lexer_add_token(lexer, TOK_LESS_THAN, start, 1);
			}
		} break;
		case '>': {
			if (lexer_next(lexer) == '=') {
				lexer_add_token(lexer, TOK_GREATER_OR_EQUAL, start, 2);
				lexer_advance(lexer);
			} else if (lexer_next(lexer) == '>') {
				lexer_add_token(lexer, TOK_BITWISE_SHIFT_RIGHT, start, 2);
				lexer_advance(lexer);
			} else {
				lexer_add_token(lexer, TOK_GREATER_THAN, start, 1);
			}
		} break;
		case '+': {
			lexer_add_token(lexer, TOK_PLUS, start, 1);
		} break;
		case '-': {
			lexer_add_token(lexer, TOK_MINUS, start, 1);
		} break;
		case '*': {
			lexer_add_token(lexer, TOK_ASTERISK, start, 1);
		} break;
		case '/': {
			lexer_add_token(lexer, TOK_SLASH, start, 1);
		} break;
		case '%': {
			lexer_add_token(lexer, TOK_PERCENT, start, 1);
		} break;
		case '|': {
			if (lexer_next(lexer) == '|') {
				lexer_add_token(lexer, TOK_OR, start, 2);
				lexer_advance(lexer);
			} else {
				lexer_add_token(lexer, TOK_BITWISE_OR, start, 1);
			}
		} break;
		case '^': {
			lexer_add_token(lexer, TOK_BITWISE_XOR, start, 1);
		} break;
		case '&': {
			if (lexer_next(lexer) == '&') {
				lexer_add_token(lexer, TOK_AND, start, 2);
				lexer_advance(lexer);
			} else {
				lexer_add_token(lexer, TOK_BITWISE_AND, start, 1);
			}
		} break;
		case '(': {
			lexer_add_token(lexer, TOK_LPAREN, start, 1);
		} break;
		case ')': {
			lexer_add_token(lexer, TOK_RPAREN, start, 1);
		} break;
		case '[': {
			lexer_add_token(lexer, TOK_LBRACKET, start, 1);
		} break;
		case ']': {
			lexer_add_token(lexer, TOK_RBRACKET, start, 1);
		} break;
		case '{': {
			lexer_add_token(lexer, TOK_LBRACE, start, 1);
		} break;
		case '}': {
			lexer_add_token(lexer, TOK_RBRACE, start, 1);
		} break;
		}

		lexer_advance(lexer);
	}

	if (!lexer_add_token(lexer, TOK_EOF, NULL, 0)) {
		return NULL;
	}

	for (size_t i = 0; i < lexer->tokens->count; i++) {
		token_t *token = &lexer->tokens->tokens[i];
		if (token->type == TOK_IDENTIFIER) {
			for (size_t j = 0; keywords[j].name; j++) {
				if (token->length != (size_t)strlen(keywords[j].name))
					continue;
				if (strncmp(token->start, keywords[j].name, token->length) ==
					0) {
					token->type = keywords[j].token;
				}
			}
		}
	}

	return lexer->tokens;
}

// Destroi um Lexer
void lexer_destroy(lexer_t *lexer) {
	if (!lexer)
		return;

	if (lexer->tokens)
		token_array_destroy(lexer->tokens);
	free(lexer);
}
