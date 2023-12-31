#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define MAX_LINES_OF_PROGRAM 100
#define END_OF_CODE NULL

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              tokenize.c                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// kind of token
typedef enum {
	TK_RESERVED, // kigou
	TK_IDENT,    // shikibetusi
	TK_NUM,      // integer token
	TK_EOF,      // end of input token
} TokenKind;

// Token type
typedef struct Token Token;
struct Token {
	TokenKind kind; // token type
	Token *next;    // next input token
	int val;        // If kind is TK_NUM, then that num
	char *str;      // Token string
	int len;        // length of Token
};

void error_at(char *loc, char *fmt, ...);

Token *tokenize(char *p);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              parse.c                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// kind of abstract syntax tree
typedef enum {
	ND_ADD,    // +
	ND_SUB,    // -
	ND_MUL,    // *
	ND_DIV,    // /
	ND_ASSIGN, // =
	ND_LVAR,   // local variable
	ND_EQ,     // ==
	ND_NE,     // !=
	ND_LT,     // <
	ND_LE,     // <=
	ND_NUM,    // integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
	NodeKind kind;  // node type
	Node *lhs;      // left hand side
	Node *rhs; 		// right hand side
	int val;   		// use when kind is ND_NUM
	int offset;     // use when kind is ND_LVAR
};

void parse(Token *tok, Node **code);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              codegen.c                                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void codegen(Node **node);