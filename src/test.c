#include <stdio.h>
#include <string.h>
#include "fxp.h"

void stack_testing();
void test_infix_to_postfix();
void test_tree(); 

int main()
{    
    test_tree(); 
}   

/*
TESTING STACK DATA STRUCTURE IN 'fxp.h'
*/
void stack_testing()
{
    printf("This is major Tom to GroundControl\n"); 

    Stack* stack = create_stack(STRING_TYPE); 
    char* arr [5] = {"k", "4", "0", "+", "-"}; 
    char* peekval; 

    for (int i = 0; i < (int) (sizeof(arr)/sizeof(arr[0])); i++)
    {
        push(stack, arr[i]);
    }

    printstack(stack); 
    printf("--------Pushed--------\n");
    peekval = peek(stack); 
    printf("peek: %s\n", peekval ? peekval: "NULL"); 

    while(stack->top)
    {
        char* val = pop(stack);
        printf("pop: %s\n", val); 
    }
    char*val = pop(stack); 
    printf("pop: %s\n", val ? val : "NULL"); 
    peekval = peek(stack); 
    printf("peek: %s\n", peekval ? peekval : "NULL"); 

    freeStack(stack); 
}

/*
TEST INFIX TO POSTFIX FUNCTION
*/
void test_infix_to_postfix()
{
    int buffersize = 200; 
    printf("infix expression: ");
    char expr [buffersize]; 
    char postfix[buffersize]; 

    scanf("%[^\n]", expr); 

    //while 'quit' is not entered: convert infix to postfix
    while(strcmp(expr, "quit") != 0 )
    {

        int err = infix_to_postfix(expr, postfix, buffersize);

        if(!err)
            printf("postfix: %s\n", postfix); 
        
        printf("errcode: %d\n", err); 

        printf("infix expression: ");
        scanf(" %[^\n]", expr); 

    }
}

void test_tree()
{
    char* expr = "sqrt(5x++)";
    char postfix [200]; 
    int err = infix_to_postfix(expr, postfix, 200); 
    if(!err)
    {
        printf("expr: %s\n", expr);
        printf("postfix: %s\n", postfix); 
        NODE* root = create_expression_tree(postfix); 

        printTree(root); 

        double result = evaluate_tree(root, 5); 
        printf("f(5) = %f\n", result);

        freeTree(root); 
    }

}

void test_evaluating()
{
  const char* exprs [] = {"x^2 + 3x +5", " sin(x/2), 5e^x, e^(10x),sqrt()"}; 

}
