/**
 * ast_node.h
 * Criado por Matheus Leme Da Silva
 */
#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum ast_node_type {
	NODE_LITERAL_INTEGER,
	NODE_LITERAL_FLOATING,
	NODE_LITERAL_STRING,
	NODE_LITERAL_BOOLEAN,
	NODE_LITERAL_NIL,
} ast_node_type_t;

typedef union ast_node_data {
	// NODE_LITERAL_INTEGER
	long long integer;

	// NODE_LITERAL_FLOATING
	double floating;

	// NODE_LITERAL_STRING,
	struct {
		const char *start;
		size_t length;
	} string;

	// NODE_LITERAL_BOOLEAN
	bool boolean;
} ast_node_data_t;

typedef struct ast_node_t {
	ast_node_type_t type;
	ast_node_data_t data;
} ast_node_t;
