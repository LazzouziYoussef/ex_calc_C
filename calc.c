#include "calc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  const char *src;
  int pos;
} Lexer;

typedef struct {
  Lexer lexer;
  Token current;
} Parser;

static Token lexer_next(Lexer *lexer) {
  while (isspace((unsigned char)lexer->src[lexer->pos]))
    lexer->pos++;

  char c = lexer->src[lexer->pos];

  if (c == '\0')
    return (Token){TOKEN_END, 0};

  if (c >= '0' && c <= '9') {
    char *end;
    double value = strtod(lexer->src + lexer->pos, &end);
    lexer->pos += (int)(end - (lexer->src + lexer->pos));
    return (Token){TOKEN_NUMBER, value};
  }

  lexer->pos++;
  switch (c) {
  case '+':
    return (Token){TOKEN_PLUS, 0};
  case '-':
    return (Token){TOKEN_MINUS, 0};
  case '*':
    return (Token){TOKEN_MULTIPLY, 0};
  case '/':
    return (Token){TOKEN_DIVIDE, 0};
  case '(':
    return (Token){TOKEN_LPAREN, 0};
  case ')':
    return (Token){TOKEN_RPAREN, 0};
  default:
    return (Token){TOKEN_END, 0};
  }
}

static Parser parser_init(const char *src) {
  Parser p;
  p.lexer.src = src;
  p.lexer.pos = 0;
  p.current = lexer_next(&p.lexer);
  return p;
}

static Token parser_peek(Parser *p) { return p->current; }

static Token parser_consume(Parser *p) {
  Token t = p->current;
  p->current = lexer_next(&p->lexer);
  return t;
}

static char token_to_op(TokenType type) {
  switch (type) {
  case TOKEN_PLUS:
    return '+';
  case TOKEN_MINUS:
    return '-';
  case TOKEN_MULTIPLY:
    return '*';
  case TOKEN_DIVIDE:
    return '/';
  default:
    return '?';
  }
}

static ASTNode *parse_expression(Parser *p);

static ASTNode *parse_primary(Parser *p) {
  Token t = parser_peek(p);
  if (t.type == TOKEN_NUMBER) {
    parser_consume(p);
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->value = t.value;
    node->left = node->right = NULL;
    return node;
  } else if (t.type == TOKEN_LPAREN) {
    parser_consume(p);
    ASTNode *node = parse_expression(p);
    if (parser_peek(p).type != TOKEN_RPAREN) {
      calc_free(node);
      return NULL;
    }
    parser_consume(p);
    return node;
  }
  return NULL;
}

static ASTNode *parse_term(Parser *p) {
  ASTNode *left = parse_primary(p);
  if (!left)
    return NULL;

  while (parser_peek(p).type == TOKEN_MULTIPLY ||
         parser_peek(p).type == TOKEN_DIVIDE) {
    Token op = parser_consume(p);
    ASTNode *right = parse_primary(p);
    if (!right) {
      calc_free(left);
      return NULL;
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_OP;
    node->op = token_to_op(op.type);
    node->left = left;
    node->right = right;
    left = node;
  }

  return left;
}

static ASTNode *parse_expression(Parser *p) {
  ASTNode *left = parse_term(p);
  if (!left)
    return NULL;

  while (parser_peek(p).type == TOKEN_PLUS ||
         parser_peek(p).type == TOKEN_MINUS) {
    Token op = parser_consume(p);
    ASTNode *right = parse_term(p);
    if (!right) {
      calc_free(left);
      return NULL;
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_OP;
    node->op = token_to_op(op.type);
    node->left = left;
    node->right = right;
    left = node;
  }

  return left;
}

ASTNode *calc_parse(const char *expr) {
  Parser p = parser_init(expr);
  return parse_expression(&p);
}

void calc_free(ASTNode *node) {
  if (!node)
    return;
  calc_free(node->left);
  calc_free(node->right);
  free(node);
}
double calc_eval(ASTNode *node) {
  if (!node)
    return 0;
  if (node->type == NODE_NUMBER)
    return node->value;

  double left = calc_eval(node->left);
  double right = calc_eval(node->right);
  switch (node->op) {
  case '+':
    return left + right;
  case '-':
    return left - right;
  case '*':
    return left * right;
  case '/':
    return left / right;
  default:
    return 0;
  }
}
void calc_print_tree(ASTNode *node, const char *prefix, int is_right) {
  if (!node)
    return;

  printf("%s", prefix);
  printf("%s", is_right ? "├── " : "└── ");

  if (node->type == NODE_NUMBER)
    printf("%g\n", node->value);
  else
    printf("%c\n", node->op);

  if (node->left || node->right) {
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix,
             is_right ? "│   " : "    ");
    calc_print_tree(node->left, new_prefix, 1);
    calc_print_tree(node->right, new_prefix, 0);
  }
}