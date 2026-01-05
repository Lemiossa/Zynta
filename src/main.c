/**
 * main.c
 * Criado por Matheus Leme Da Silva
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "zynta.h"

#ifndef VERSION
#define VERSION "Undefined"
#endif

// Mostra a versão
int show_version(int argc, char **argv) {
	(void)argc;
	(void)argv;
	printf("Zynta v%s\n", VERSION);
	return 0;
}

typedef struct operand {
	const char *name;
	bool need;
} operand_t;

typedef struct command {
	char letter;
	const char *long_name;
	const char *description;
	int (*function)(int argc, char **args);
	int operand_count;
	operand_t *operands;
} command_t;

operand_t zynta_run_file_operands[] = {
	{"FILE", true},
};

command_t commands[] = {
	{'v', "version", "Show version of this program", show_version, 0, NULL},
	{'r', "run-file", "Run a zynta file", runFile, 1, zynta_run_file_operands},

	{0, NULL, NULL, NULL, 0, NULL} // Termina aqui
};

// Procura um comando por nome
command_t *find_commands_by_long_name(const char *long_name) {
	for (int i = 0; commands[i].long_name && commands[i].function; i++) {
		if (strcmp(long_name, commands[i].long_name) == 0) {
			return &commands[i];
		}
	}
	return NULL;
}

// Procura um comando por letra
command_t *find_command_by_letter(char letter) {
	for (int i = 0; commands[i].letter && commands[i].function; i++) {
		if (letter == commands[i].letter) {
			return &commands[i];
		}
	}
	return NULL;
}

// Exibe a ajuda baseado na lista de comados
void show_help(char *name) {
	printf("Usage: %s [commands]\n", name);
	printf("\033[32mShort commands starts with '-'\033[0m\n");
	printf("\033[34mLong commands starts with '--'\033[0m\n");
	printf("Commands: \n");

	for (int i = 0; commands[i].function; i++) {
		printf("    \033[32m-%c\033[0m | \033[34m--%s\033[0m ",
			   commands[i].letter, commands[i].long_name);
		if (commands[i].operands) {
			for (int j = 0; j < commands[i].operand_count; j++) {
				operand_t operand = commands[i].operands[j];
				if (!operand.name)
					break;

				if (operand.need) {
					printf("<%s> ", operand.name);
				} else {
					printf("[%s] ", operand.name);
				}
			}
		}
		printf("- \033[0m%s\n", commands[i].description);
	}

	printf(
		"    \033[32m-h\033[0m | \033[34m--help\033[0m - Show this message\n");
}

// Parsea comandos
int parse_commands(int argc, char **argv) {
	int last_command_return = 0;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			char *arg = argv[i];
			if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
				show_help(argv[0]);
				return 0;
			}
			if (arg[0] == '-') {
				if (arg[1] == '-') {
					command_t *command = find_commands_by_long_name(&arg[2]);
					if (!command) {
						printf("Unknown command: %s\n", arg);
						return 1;
					}
					last_command_return = command->function(argc - i, &argv[i]);
				} else {
					arg++;
					while (*arg) {
						command_t *command = find_command_by_letter(*arg);
						if (!command) {
							printf("Unknown command: %c\n", *arg);
							return 1;
						}
						last_command_return =
							command->function(argc - i, &argv[i]);
						arg++;
					}
				}
			}
		}
	}
	return last_command_return;
}

// Func principal da  linguagem
int main(int argc, char **argv) { return parse_commands(argc, argv); }
