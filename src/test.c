#include "fxp.h"
#include <stdio.h>
#include <string.h>

void stack_testing();
void test_infix_to_postfix();
void test_tree();
void test_list_exprs(char *exprs[], int len);
void test_evaluating();

char *exprs[] = {"(++ 3)", "56 (( 89 -", "(3 + ) + 4", "sin sin sin x"};
char* more_exprs[] = {"x^2 + 3x + 5", "sin(x/2)", "5e^x", "e^(10x)", "sqrt((x^2+4)/x^3)", "((e^(2x) + e^x + 3)/(4x^2 + 5) )^2"};

int main() {
  // test_list_exprs(exprs, (int)sizeof(exprs) / sizeof(exprs[0]));
  // test_list_exprs(more_exprs, (int)(sizeof(more_exprs)/sizeof(more_exprs[0]))); 
  
  double res =  linear_interpolation_x(1, 5, 5, 5, 3);
  printf("res: %.2f\n", res); 
}

/*
TESTING STACK DATA STRUCTURE IN 'fxp.h'
*/
void stack_testing() {
  printf("This is major Tom to GroundControl\n");

  Stack *stack = create_stack(STRING_TYPE);
  char *arr[5] = {"k", "4", "0", "+", "-"};
  char *peekval;

  for (int i = 0; i < (int)(sizeof(arr) / sizeof(arr[0])); i++) {
    push(stack, arr[i]);
  }

  printstack(stack);
  printf("--------Pushed--------\n");
  peekval = peek(stack);
  printf("peek: %s\n", peekval ? peekval : "NULL");

  while (stack->top) {
    char *val = pop(stack);
    printf("pop: %s\n", val);
  }
  char *val = pop(stack);
  printf("pop: %s\n", val ? val : "NULL");
  peekval = peek(stack);
  printf("peek: %s\n", peekval ? peekval : "NULL");

  freeStack(stack);
}

/*
TEST INFIX TO POSTFIX FUNCTION
*/
void test_infix_to_postfix() {
  int buffersize = 200;
  printf("infix expression: ");
  char expr[buffersize];
  char postfix[buffersize];

  scanf("%[^\n]", expr);

  // while 'quit' is not entered: convert infix to postfix
  while (strcmp(expr, "quit") != 0) {

    int err = infix_to_postfix(expr, postfix, buffersize);

    if (!err)
      printf("postfix: %s\n", postfix);

    printf("errcode: %d\n", err);

    printf("infix expression: ");
    scanf(" %[^\n]", expr);
  }
}

void test_tree() {
  char *expr = "5e^x";
  char postfix[200];
  int err = infix_to_postfix(expr, postfix, 200);
  if (!err) {
    printf("expr: %s\n", expr);
    printf("postfix: %s\n", postfix);
    NODE *root = create_expression_tree(postfix);

    printTree(root);

    double result = evaluate_tree(root, 5);
    printf("f(5) = %f\n", result);

    freeTree(root);
  }
}

void test_list_exprs(char *exprs[], int len) {
  const double test_vals[] = {1, 7, 5.6, 100, -88};

  for (int i = 0; i < len; i++) {
    char postfix[400];
    printf("-----------------------------------------------------------------------------------------------\n"); 
    printf("expr: %s\n", exprs[i]);
    int err = infix_to_postfix(exprs[i], postfix, 400);

    if (!err) {
      printf("postfix: %s\n", postfix);
      NODE *root = create_expression_tree(postfix);
      printTree(root);

      double result;
      for (int j = 0; j < (int)(sizeof(test_vals) / sizeof(test_vals[0])); j++) {
        result = evaluate_tree(root, test_vals[j]);
        printf("f(%.2f) = %.2f\n", test_vals[j], result); 
      }

      freeTree(root); 
    }
    else{
      printf("Failed to parse infix expression\n"); 
    }

  }
}

