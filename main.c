#include "9cc.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "don't correct num of arg\n");
		return 1;
	}

	Token *token = tokenize(argv[1]);
	Node *node = parse(token);

	// first half of ascenbry
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// we generate code while wolking down AST
	gen(node);

	// Top of Stack have return value of input,
	// therefor we load the value to RAX and return.
	printf("	pop rax\n");
	printf("	ret\n");
	return 0;
}
