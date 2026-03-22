#ifndef CALC_H
#define CALC_H

typedef enum { NODE_NUMBER, NODE_OP } NodeType;

typedef struct ASTNode {
  NodeType type;
  double value;
  char op;
  struct ASTNode *left;
  struct ASTNode *right;
} ASTNode;

typedef enum {
  TOKEN_NUMBER,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULTIPLY,
  TOKEN_DIVIDE,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_END
} TokenType;

typedef struct {
  TokenType type;
  double value;
} Token;

ASTNode *calc_parse(const char *expr);
double calc_eval(ASTNode *node);
void calc_free(ASTNode *node);
void calc_print_tree(ASTNode *node, const char *prefix, int is_right);

#endif // CALC_H