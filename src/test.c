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
    char* expr = "5.0sin(x+4.5)^2 + 4x^2 + c";
    char postfix [200]; 
    int err = infix_to_postfix(expr, postfix, 200); 
    if(!err)
    {
        printf("expr: %s\n", expr); 
        NODE* root = create_expression_tree(postfix); 

        printTree(root); 
        freeTree(root); 
    }

}