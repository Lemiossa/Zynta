/**
 * zynta.c
 * Criado por Matheus Leme Da Silva
 */
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "lexer/token.h"

#include "parser/parser.h"

#include "zynta.h"

int run(char *content, size_t contentSize);
int programReturn = 0;

int runFile(int argc, char **argv) {
	(void)argv;
	if (argc < 2) {
		printf("Inválid arguments\n");
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (!f) {
		printf("Internal error: Failed to open %s: %s\n", argv[1],
			   strerror(errno));
		return 1;
	}

	if (fseek(f, 0, SEEK_END) < 0) {
		printf("Internal error: Failed to seek %s: %s\n", argv[1],
			   strerror(errno));
		return 1;
	}

	size_t filesize = ftell(f);
	rewind(f);

	char *filecontent = (char *)malloc(filesize);
	if (!filecontent) {
		printf("Internal error: Failed to alloc memory for file content\n");
		return 1;
	}

	if (fread(filecontent, 1, filesize, f) != filesize) {
		printf("Internal error: Failed to read %s: %s\n", argv[0],
			   strerror(errno));
		return 1;
	}

	run(filecontent, filesize);

	free(filecontent);
	fclose(f);

	return 0;
}

int run(char *content, size_t contentSize) {
	lexer_t *lexer = lexer_create(content, contentSize);
	token_array_t *tokens = lexer_lex(lexer);
#ifdef DEBUG
	TokenArrayPrint(tokens);
#endif
	parser_t *parser = parser_create(tokens);

	ast_node_t *ast = parser_parse(parser);
	(void)ast;

	parser_destroy(parser);
	lexer_destroy(lexer);

	return 0;
}
