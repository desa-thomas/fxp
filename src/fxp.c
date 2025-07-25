#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fxp.h"


/*----EXPRESSION TREE DATA STRUCTURE----*/
tNODE* create_tNode(char* val)
{
    tNODE* node = malloc(sizeof(tNODE));
    node->lChild = NULL; 
    node->rChild = NULL; 
    node->val = val; 

    return node; 
}

void freeTree(tNODE *root)
{
    if(root->lChild)
        freeTree(root->lChild); 
    if(root->rChild)
        freeTree(root->rChild);
    
    free(root);
}

/*----SIMPLE STACK DATA STRUCTURE---*/
sNODE* create_sNode(char* val)
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

void push(Stack* stack,char* val)
{

    sNODE* node = create_sNode(val);

    //check if NOT empty
    if(stack->top)
    {
        node->next = stack->top; 

    }
    //update top and len
    stack->top = node; 
    stack->len++; 
}

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

char* infix_to_postfix(char* expr)
{
    //1. scan from left to right
    //2. If operand, put in postfix expr
    //3. else
        //If precedence of current scanned operator is higher
        //than the precedence of operator at top of stack (or empty)
        //(or top of stack is '(' ) push operator onto stack

        //else pop all operators from stack that have precedence higher
        //or equal to that of current operator
    
    //4. if '(' push onto stack
    //5. if ')', pop stack and output it until '(' is encountered, then discard both
    //6. repeat 2-5 until infix expr is scanned
    //Once scanning is over, pop stack and add operators to expr untill it is not empty
    //if any brackets are left parse error

    char currStr [50]; 
    int currstrlen = 0; 
    int (*check_char)(int) = NULL; // A function pointer!!! will point to function
    char ch;
    int i= 0; 

    while(i < (int) strlen(expr))
    {
        ch = expr[i]; 

        strcpy(currStr, ""); 

        //if digit or letter
        //finishing parsing word or entire number, and store it in 'currStr'
        if(isdigit(ch) || isalpha(ch))
        {
            if(isdigit(ch))
                check_char = isdigit;
            else
                check_char = isalpha;

            char2str(currStr, ch);       //make currstr = c
            while(expr[i+1] != STREND && check_char(expr[i+1])) //while next char is a digit or letter respectively
            {
                i++;
                currstrlen = strlen(currStr);
                appendChar(currStr, expr[i], currstrlen); // append character
            }

            printf("%s\n", currStr); 
        }

        //is operator +, -, /, *, ^ 
        else if(isOperator(ch))
        {

        }
        // ( or ) or [ or ]
        else if (isBracket(ch))
        {

        }
        //Disregard white space
        else if(ch == ' ')
        {
            i++; 
            continue;
        }

        //err
        else 
        {
            char msg [22];
            sprintf(msg, "Invalid Character: %c", ch); 
            perror(msg); 
        }

        i++; 
    }

    return "bruh"; 
}

Bool isStrOpr(char *expr)
{
    Bool isStrOpr = False; 
    const char* funcs [] = {"cos", "sin", "tan",
                     "sec", "csc","cot",
                      "log", "exp", "sqrt"}; 

    for(int i = 0; i < len(funcs); i ++ )
    {
        if(strcmp(expr, funcs[i]) == 0)
        {
            isStrOpr = True; 
            break; 
        }
    }

    return isStrOpr; 
}