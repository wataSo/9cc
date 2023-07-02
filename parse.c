#include "9cc.h"

static Token *token;

static Node *expr();
static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *primary();

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs  = lhs;
	node->rhs  = rhs;
	return node;
}

static Node *new_node_num(int val) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
	return node;
}

//
//
static bool consume(char *op) {
	if (token->kind   != TK_RESERVED || 
		strlen(op)    != token->len  ||
		memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}


// if next token is expected symbol, we forward a token
// and return true. Otherwise return fals
static void expect(char *op) {
	if (token->kind != TK_RESERVED || 
		strlen(op)    != token->len  ||
		memcmp(token->str, op, token->len))
		error_at(token->str, "expected \"%s\"", op);
	token = token->next;
}

// if next token is number, we forward a token and return the number.
// Otherwise report error.
static int expect_number() {
	if (token->kind != TK_NUM)
		error_at(token->str, "expected a number");
	int val = token->val;
	token = token->next;
	return val;
}

static bool at_eof() {
	return token->kind == TK_EOF;
}

// expr = equality
static Node *expr() {
	return equality();
}

// equality = relational ("==" relational | "!=" relational)*
static Node *equality() {
	Node *node = relational();

	for (;;) {
		if (consume("=="))
			node = new_node(ND_EQ, node, relational());
		else if (consume("!="))
			node = new_node(ND_NE, node, relational());
		else
			return node;
	}
}

// relation = add ("<" add | "<=" add | ">" add | ">=" add)
static Node *relational() {
	Node *node = add();

	for (;;) {
		if (consume("<"))
			node = new_node(ND_LT, node, add());
		else if (consume("<="))
			node = new_node(ND_LE, node, add());
		else if (consume(">"))
			node = new_node(ND_LT, add(), node);
		else if (consume(">="))
			node = new_node(ND_LE, add(), node);
		else
			return node;
	}

}

// add = mul ("+" mul | "-" mul)*
static Node *add() {
	Node *node = mul();

	for (;;) {
		if (consume("+"))
			node = new_node(ND_ADD, node, mul());
		else if (consume("-"))
			node = new_node(ND_SUB, node, mul());
		else
			return node;
	}
}

//mul = unary ("*" unary | "/" unary)*
static Node *mul() {
	Node *node = unary();
	for (;;) {
		if (consume("*"))
			node = new_node(ND_MUL, node, unary());
		else if (consume("/"))
			node = new_node(ND_DIV, node, unary());
		else
			return node;
	}
}

// unary   = ("+" | "-")? unary | primary
static Node *unary() {
	if (consume("+"))
		return unary();
	if (consume("-"))
		return new_node(ND_SUB, new_node_num(0), unary());
	return primary();
}

//primary = "(" expr ")" | num
static Node *primary() {
	// if next token is "(" then, "(" expr ")".
	if (consume("(")) {
		Node *node = expr();
		expect(")");
		return node;
	}

	return new_node_num(expect_number());
}

Node *parse(Token *tok) {
    token = tok;
    Node *node = expr();
    // error handling
    return node;
}