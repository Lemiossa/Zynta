/**
 * keywords.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include "token.h"

typedef struct keyword {
	const char *name;
	token_type_t token;
} keyword_t;

static keyword_t keywords[] = {
	{"and", TOK_AND},			  {"or", TOK_OR},
	{"True", TOK_KEYWORD_TRUE}, {"False", TOK_KEYWORD_FALSE},
	{"Nil", TOK_KEYWORD_NIL},	  {NULL, TOK_EOF}};
