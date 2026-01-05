/**
 * parser.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdlib.h>

#include "../lexer/token.h"
#include "parser.h"

// Retorna o token atual em um parser_
token_t *parser_peek(parser_t *parser) {
	if (!parser)
		return NULL;
	if (!parser->tokens || !parser->tokens->tokens)
		return NULL;

	return &parser->tokens->tokens[parser->pos];
}

// Retorna o token atual e incrementa o ponteiro em um parser_
token_t *parser_advance(parser_t *parser) {
	if (!parser)
		return NULL;
	if (!parser->tokens || !parser->tokens->tokens)
		return NULL;

	return &parser->tokens->tokens[parser->pos++];
}

// Espera um tipo de token de um parser_t, se não encontrado, dá erro
// Se encontrado, avança
// Retorna 0 em caso de sucesso e != 0 em caso de erro
int parser_expect(parser_t *parser, token_type_t type) {
	if (!parser)
		return 1;

	token_t *token = parser_peek(parser);
	if (!token)
		return 2;

	if (token->type != type) {
		token_error(*token, "Syntax error: Expected %s, but found %s\n",
				   token_get_string(type), token_get_string(token->type));
		return 3;
	}

	parser_advance(parser);
	return 0;
}

// Cria um novo parser_
parser_t *parser_create(token_array_t *tokens) {
	if (!tokens)
		return NULL;

	parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
	if (!parser)
		return NULL;

	parser->pos = 0;
	parser->tokens = tokens;

	return parser;
}

// Parsea um parser
ast_node_t *parser_parse(parser_t *parser) {
	if (!parser)
		return NULL;



	return NULL;
}

// Destroi um parser_
void parser_destroy(parser_t *parser) {
	if (!parser)
		return;

	free(parser);
}
