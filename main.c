#include "calc.h"
#include <stdio.h>
#include <string.h>

int main(void) {

  char line[1024];
  char print_tr_choice;
  printf("Expression Tree Calculator\n");

  printf("Enter expression: ");

  if (!fgets(line, sizeof(line), stdin)) {
    fprintf(stderr, "Failed to read input.\n");
    return 1;
  }
  line[strcspn(line, "\n")] = '\0';
  ASTNode *root = calc_parse(line);
  if (!root) {
    fprintf(stderr, "Parse error.\n");
    return 1;
  }

  double result = calc_eval(root);
  printf("Result: %g\n", result);
  printf("Print expression tree? (y/n): ");
  if (scanf(" %c", &print_tr_choice) == 1 &&
      (print_tr_choice == 'y' || print_tr_choice == 'Y')) {

    calc_print_tree(root, "", 0);
  }
  calc_free(root);
  return 0;
}