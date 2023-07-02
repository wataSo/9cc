#include "9cc.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "don't correct num of arg\n");
		return 1;
	}

	Token *token = tokenize(argv[1]);
	Node *code[MAX_LINES_OF_PROGRAM];
	parse(token, code);

	// we generate code while wolking down AST
	codegen(code);

	return 0;
}
