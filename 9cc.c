#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

// kind of token
typedef enum {
	TK_RESERVED, // kigou
	TK_NUM,      // integer token
	TK_EOF,      // end of input token
} TokenKind;

typedef struct Token Token;

// token type
struct Token {
	TokenKind kind; // token type
	Token *next;    // next input token
	int val;        // If kind is TK_NUM, then that num
	char *str;      // Token string
};

// Input program
char *user_input;

// token that we focus on now
Token *token;

// function for error
// args of that func is same as printf
void error(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

// Reports an error loation and exit.
void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, "");
	fprintf(stderr, "^ ");
	fprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//
//
bool consume(char op) {
	if (token->kind != TK_RESERVED || token->str[0] != op)
		return false;
	token = token->next;
	return true;
}


// if next token is expected symbol, we forward a token
// and return true. Otherwise return fals
void expect(char op) {
	if (token->kind != TK_RESERVED || token->str[0] != op)
		error_at(token->str, "expected '%c'", op);
	token = token->next;
}

// if next token is number, we forward a token and return the number.
// Otherwise report error.
int expect_number() {
	if (token->kind != TK_NUM)
		error_at(token->str, "expected a number");
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

//we generate new token and link to cur.
Token *new_token(TokenKind kind, Token *cur, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	return tok;
}

//
Token *tokenize() {
	char *p = user_input;
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		// skip blank string
		if (isspace(*p)) {
			p++;
			continue;
		}

		if (*p == '+' || *p == '-') {
			cur = new_token(TK_RESERVED, cur, p++);
			continue;
		}

		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p);
			cur->val = strtol(p, &p, 10);
			continue;
		}

		error_at(p, "expected a number");
	}

	new_token(TK_EOF, cur, p);
	return head.next;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "don't correct num of arg\n");
		return 1;
	}

	// tokenize
	user_input = argv[1];
	token = tokenize();

	// first half of ascenbry
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	// String in first of formula must be number.
	printf("	mov rax, %ld\n", expect_number());

	// cunsume token that `+ <num>` or `- <num>`, and output ascenbry
	while (!at_eof()) {
		if (consume('+')) {
			printf("	add rax, %ld\n", expect_number());
			continue;
		}
		expect('-');
		printf("	sub rax, %ld\n", expect_number());
	}

	printf("	ret\n");
	return 0;
}
