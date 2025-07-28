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
    if(node == NULL) return NULL; 
    char* new_val = malloc(strlen(val) + 1); 
    if(new_val == NULL) {free(node); return NULL; }
    strcpy(new_val, val); 

    node->next = NULL; 
    node->val = new_val;
    return node; 
 }

void free_node(sNODE * node)
{
    free(node->val);
    free(node); 
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

        free_node(pnode);
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
        val = NULL; 
    }

    return val; 
}

int infix_to_postfix(char* expr, char* postfix, int buffersize)
{
    //make mutable copy of expr
    char* local_expr;
    local_expr = malloc(sizeof(expr) + 1); 
    if(local_expr == NULL)
    {
        printf("failed to allocated `local_expr`"); 
        return -999; 
    }
    if((int)strlen(expr)+1 > buffersize)
    {
        printf("buffersize must be >= %d (strlen(expr)+1)", (int)(strlen(expr)+1)); 
        return -999; 
    }
    strncpy(local_expr, expr, strlen(expr)); 


    int errcode = 1; //1 : success, -999 : error
    strcpy(postfix,""); //make sure passed output string empty
    Stack* stack = create_stack(); 

    char currStr [50]; 
    int currstrlen = 0; 
    char errmsg [100];
    int (*check_char)(int) = NULL; // A function pointer!!! will point to isalpha or isdigit
    Bool isFunc; 

    char ch;
    int i= 0; 
    int len = (int) strlen(local_expr);  
    while(i < len && errcode)
    {
        ch = local_expr[i]; 
        isFunc = False; 

        strcpy(currStr, ""); 

        // Parse number or function
        if(isdigit(ch) || isalpha(ch))
        {
            if(isdigit(ch))
                check_char = isdigit;
            else
                check_char = isalpha;

            char2str(currStr, ch);       //make currstr = c
            while(local_expr[i+1] != STREND && check_char(local_expr[i+1])) //while next char is a digit or letter respectively
            {
                i++;
                currstrlen = strlen(currStr);
                //handle str overflow
                if (currstrlen == 50)
                    {
                        sprintf(errmsg, "str too long (greater than 50 chars): %s\n", currStr); 
                        printf("ERROR: %s", errmsg); 
                        errcode = -999; 
                        break;
                    }
                appendChar(currStr, local_expr[i], currstrlen); // append character
            }

            if (isalpha(currStr[0])) 
                isFunc = isStrOpr(currStr); 
        }

        //is operator +, -, /, *, ^ or sin cos ..etc
        if(isOperator(ch) || isFunc)
        {
            char* peekval= peek(stack); 

            if (
            //If precedence is higher than top of stack
            ( (peekval != NULL) && (isFunc) ) ||
            //If precedence is higher than top of stack
            ((peekval != NULL) && (isOperator(peekval[0])) && (cmpopr(ch, peekval[0]) == 1) ) ||

            //If top of stack is (
            ( (peekval != NULL) && (peekval[0] == '(') ) ||
            //If stack is empty
            (peekval == NULL) )
            {
                //push operator to stack
                if (isFunc)
                    push(stack, currStr); 
                else
                {
                    char tmp [2]; 
                    tmp[0] = ch; 
                    tmp[1] = STREND; 
                    push(stack, tmp); 
                }
            }

            //else pop all operators from stack that have higher or equal precedence 
            else
            {
                //Note, we always push if encoutering a function (i.e., sin)
                //pop all values of stack with higher precedence
                while((stack->top != NULL) && (isOperator(peekval[0])) && (cmpopr(peekval[0], ch) >=0) )
                {
                    char* opr = pop(stack); 
                    //add popped operator to postfix expression

                    if (!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        strncat(postfix, opr, strlen(opr)); 
                    else
                        {
                            errcode = -999;
                            free(opr);
                            break;
                        }
                        
                    free(opr); //free popped value

                    peekval = peek(stack); 
                }
            }
        }

        //If it is an operand 
        else if(isdigit(currStr[0]) || isalpha(currStr[0]))
        {
            if(!CONCAT_OVERFLOW(postfix, currStr, buffersize))
                {
                strncat(postfix, currStr, strlen(currStr));

                //case: 5(x) => 5*(x)
                if(isdigit(currStr[0]) && (local_expr[i+1] == '(' || isalpha(local_expr[i+1])))
                    {
                        local_expr[i] = '*'; 
                        continue;
                    }
                }
            else
                errcode = -999;
        }

        //If it is a bracket
        else if (isBracket(ch))
        {
            if (ch == '(' || ch == '[')
                {
                    char tmp[2];
                    tmp[0] = ch; 
                    tmp[1] = STREND; 
                    push(stack, tmp); 
                }

            else 
            {
                //pop and output until '(' is encountered
                while(stack->top != NULL && peek(stack)[0] != '(')
                {                    
                    char* opr = pop(stack); 
                    
                    if(!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        strncat(postfix, opr, strlen(opr)); 
                    else
                    {
                        errcode = -999; 
                        free(opr); 
                        break; 
                    }
                    free(opr); 
                }

                char* peekval = peek(stack); 
                //Bracket error 
                if (peekval == NULL || (peekval[0] != '(') )
                    {
                        printf("ERROR: MISMATCHED BRACKETS\n"); 
                        errcode = -999;
                        break;
                    }
                
                char* val = pop(stack); //discard bracket
                free(val); 

                //ex: sin ( expr )
                if(isStrOpr(peek(stack)))
                {
                    char * opr = pop(stack); 
                    if(!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        strncat(postfix, opr, strlen(opr)); 
                    else
                        {
                            errcode = -999;
                            free(opr); 
                            break; 
                        }
                    free(opr); 
                }
            }
        }

        //Disregard white space
        else if(isspace(ch))
        {
            i++; 
            continue;
        }

        //err
        else 
        {
            sprintf(errmsg, "Invalid Character: %c", ch); 
            printf("ERROR: %s", errmsg); 
        }
 
        i++; 
    }

    while (errcode && stack->top != NULL)
    {
        char* opr = pop(stack);

        if(opr[0] == '(')
            {
                printf("ERROR: MISMATCHED BRACKETS\n"); 
                errcode= -999; 
                free(opr); 
                break;
            }
        
        strncat(postfix, opr, strlen(opr)); 
        free(opr); 
    }

    freeStack(stack); 

    return errcode; 
}

Bool isStrOpr(char *expr)
{
    if (expr == NULL)
        return False; 

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

//compare two char operators 
int cmpopr(char opr1, char opr2)
{
    char oprs [] = {opr1, opr2}; 
    int precedence[2], cmp; 
    
    for (int i = 0; i < 2; i ++)
    {
        switch (oprs[i])
        {
            case '^':
                precedence[i] = 2;
                break; 

            case '*': 
            case '/':
                precedence[i] = 1;
                break; 

            case '-':
            case '+':
                precedence[i] = 0; 
                break; 

            default:
                printf("opr %c is invalid", oprs[i]);
                return -999; 
                break;
        }
    
    }
    if(precedence[0] < precedence[1] )
        cmp = -1; 
    else if (precedence[0] == precedence[1])
        cmp = 0;
    else
        cmp = 1; 

    return cmp; 
}