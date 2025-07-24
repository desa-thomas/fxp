#include <stdio.h>
#include <stdlib.h>
#include "fxp.h"





/* SIMPLE STACK DATA STRUCTURE*/
sNODE* create_node(char* val)
{ 
    sNODE* node = malloc(sizeof(sNODE));
    node->next = NULL; 
    node->val = val;
    return node; 
 }

Stack* create_stack()
{
    Stack *stack = malloc(sizeof(Stack));
    stack->top = NULL; 
    stack->len = 0; 

    return stack;
}

void freeStack(Stack *stack)
{
    sNODE* node = stack->top; 
    sNODE* pnode = NULL; 

    while(node)
    {
        pnode = node; 
        node = node->next; 

        free(pnode);
    }

    free(stack); 
}

//debug
void printstack(Stack* stack)
{
    sNODE* node = stack->top;
    int i = 0;
    while(node)
    {
        printf("%d -> %s\n", i, node->val); 
        node = node->next;
        i++; 
    }
}

void push(Stack* stack, char* val)
{

    sNODE* node = create_node(val);

    //check if NOT empty
    if(stack->top)
    {
        node->next = stack->top; 

    }
    //update top and len
    stack->top = node; 
    stack->len++; 
}

//return '\0' on empty stack
char* pop(Stack* stack)
{
    char* val; 
    sNODE* node; 

    if(stack->top)
    {
        node = stack->top;
        stack->top = stack->top->next;
        stack->len--; 
        val = node->val;

        //free node
        free(node); 
    }

    else
    {
        perror("CANNOT POP FROM EMPTY STACK");
        val = NULL;  
    }

    return val; 
}

char* peek(Stack* stack)
{
    char* val; 
    if (stack->top)
    {
        val = stack->top->val; 
    }
    else 
    {
        perror("CANNOT PEEK AN EMPTY STACK");
        val = NULL; 
    }

    return val; 
}