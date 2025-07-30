#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fxp.h"


/*----EXPRESSION TREE DATA STRUCTURE----*/

void freeTree(NODE *root)
{
    if(root->lChild != NULL)
        freeTree(root->lChild); 
    if(root->rChild != NULL)
        freeTree(root->rChild);
    
    free(root->val); 
    free(root);
}

NODE* create_expression_tree(char *postfix)
{

    Stack* stack = create_stack(NODE_TYPE); 
    char* token = strtok(postfix, " ");

    //iterate over postfix expression
    while(token != NULL)
    {

        //create node for current element
        NODE* node = create_node(token, STRING_TYPE); 

        //if element is an operator: pop() = right_child, pop() = left_child. push current node
        Bool isFunc = isStrOpr(token); 
        if(isFunc || isOperator(token[0]))
        {
            if(peek(stack) != NULL)
            {
                node->rChild = pop(stack); 

                //non 'function operators' take two arguments 
                if(peek(stack) != NULL && !isFunc)
                    node->lChild = pop(stack); 

                push(stack, node); 
            }
            
        }
        //if element is an operand push node to stack
        else
            push(stack, node); 

        
        token = strtok(NULL, " ");
        
    }

    NODE* root = pop(stack); 
    freeStack(stack); 

    return root; 
}

/* 
DEBUGGING:
Preorder traversal of tree and print nodes + value in readable format
*/
void printTree(NODE *root)
{
    printf("r> %s\n", (char*)root->val); 

    rprinttree(root->lChild, "|  ", "|-l> ");
    rprinttree(root->rChild, "|  ", "|-r> ");
}

void rprinttree(NODE* node, char* padding, char* pointer)
{
    if(node!= NULL)
    {
        printf("%s%s%s\n", padding, pointer, (char*)node->val); 
        char new_padding [1024];
        snprintf(new_padding, sizeof(new_padding), "%s|  ", padding); 

        rprinttree(node->lChild, new_padding, "|-l> ");
        rprinttree(node->rChild, new_padding, "|-r> "); 
    }
    
}

/*
---- NODE ----
*/

NODE* create_node(void* val, node_datatype type)
{ 
    NODE* node = malloc(sizeof(NODE));
    if(node == NULL) return NULL; 

    if(type == STRING_TYPE)
    {
        /* copy string into malloced memory locaiton*/
        char* new_val = malloc(strlen(val) + 1); 
        if(new_val == NULL) {free(node); return NULL; }
        strcpy(new_val, val); 
        node->val = new_val; 
    }
    else
        node->val = val;
        
    node->next = NULL; 
    node->lChild = NULL; 
    node->rChild = NULL; 
    
    return node; 
 }

void free_node(NODE * node)
{
    free(node->val);
    free(node); 
}

/*
----STACK DATA STRUCTURE ---
*/

Stack* create_stack(node_datatype type)
{
    Stack *stack = malloc(sizeof(Stack));
    stack->top = NULL; 
    stack->len = 0; 
    stack->type = type; 

    return stack;
}

void freeStack(Stack *stack)
{
    NODE* node = stack->top; 
    NODE* pnode = NULL; 

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
    if (stack->type != STRING_TYPE)
        return; 

    NODE* node = stack->top;
    int i = 0;
    while(node)
    {
        printf("%d -> %s\n", i, (char*)node->val); 
        node = node->next;
        i++; 
    }
}

void push(Stack* stack,void* val)
{
    NODE* node = create_node(val, stack->type);

    //check if NOT empty
    if(stack->top)
    {
        node->next = stack->top; 

    }
    //update top and len
    stack->top = node; 
    stack->len++; 
}


void* pop(Stack* stack)
{

    void* val; 
    NODE* node; 

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

void* peek(Stack* stack)
{
    void* val; 
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

/* ---- INFIX TO POSTFIX FUNCTIONS ----*/
int infix_to_postfix(char* expr, char* postfix, int buffersize)
{
    /*
    make mutable copy of expr 
    This was done to make it easy to detect implicit multiplication e.g., 5(x+9)
    by adding '5' to the postfix expression, replacing it with '*' and skipping the iteration
    */
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

    /* variable setup*/
    int errcode = 0;                //0 : success, -999 : error
    strcpy(postfix,"");   //make sure passed output string empty
    Stack* stack = create_stack(STRING_TYPE); 

    char currStr [50];              //holds parsed number or funciton e.g., "900" or "sin" 
    int currstrlen = 0; 
    int (*check_char)(int) = NULL;  //A function pointer!!! will point to isalpha or isdigit
    Bool isFunc;                    //True if the current parsed string is a function operator (sin, cos...etc)

    char ch;
    int i= 0; 
    int len = (int) strlen(local_expr);  
    while(i < len && !errcode)
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
                        printf("ERROR: operator invalid (overflow over 50 char): %s\n", currStr); 
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
                //Note, we always push if encoutering a function (i.e., sin) so ch is 100% a char operator
                //pop all values of stack with higher precedence
                while((stack->top != NULL) && ( ( (isOperator(peekval[0])) && (cmpopr(peekval[0], ch) >=0) ) || (isStrOpr(peekval)) ) ) 
                {
                    char* opr = pop(stack); 
                    //add popped operator to postfix expression

                    if (!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        {
                            strncat(postfix, opr, strlen(opr)); 
                            strncat(postfix, " ", 2); 
                        }
                    else
                        {
                            errcode = -999;
                            free(opr);
                            break;
                        }
                        
                    free(opr); //free popped value

                    peekval = peek(stack); 
                }

                //once done popping all operators with higher precedence, push current operator
                char tmp[2];
                tmp[0] = ch; 
                tmp[1] = STREND; 
                push(stack, tmp); 
            }
        }

        //If it is an operand 
        else if(isdigit(currStr[0]) || isalpha(currStr[0]))
        {
            //make sure it is a valid function operator 
            if (isalpha(currStr[0]) && isalpha(currStr[1]) && !isStrOpr(currStr))
            {
                printf("ERROR: \"%s\" is not a valid operator\n", currStr); 
                errcode = -999; 
            }

            //add operand to postfix expression
            else if(!CONCAT_OVERFLOW(postfix, currStr, buffersize))
                {
                strncat(postfix, currStr, strlen(currStr));
                strncat(postfix, " ", 2); 

                //detect implicit multiplication i.e., 5(x) => 5*(x), 4sin(x) => 4*sin(x)
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
            //push open bracket
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
                while(stack->top != NULL && ((char*)peek(stack))[0] != '(')
                {                    
                    char* opr = pop(stack); 
                    
                    if(!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        {
                            strncat(postfix, opr, strlen(opr)); 
                            strncat(postfix, " ", 2); 
                        }
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
                
                //discard bracket
                char* val = pop(stack); 
                free(val); 

                //detect function operator before bracket ex: sin ( expr ) 
                if(isStrOpr(peek(stack)))
                {
                    char * opr = pop(stack); 
                    if(!CONCAT_OVERFLOW(postfix, opr, buffersize))
                        {
                            strncat(postfix, opr, strlen(opr));
                            strncat(postfix, " ", 2); 
                        }
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

        //err invalid character
        else 
        {
            printf("ERROR: Invalid character: %c\n", ch); 
            errcode = -999;
            break; 
        }
        
        i++; 
    }

    while (!errcode && stack->top != NULL)
    {
        char* opr = pop(stack);

        if(opr[0] == '(')
            {
                printf("ERROR: MISMATCHED BRACKETS\n"); 
                errcode= -999; 
                free(opr); 
                break;
            }
        if(!CONCAT_OVERFLOW(postfix, opr, buffersize))
        {
            strncat(postfix, opr, strlen(opr)); 
            strncat(postfix, " ", 2); 
        }
        free(opr); 
    }

    freeStack(stack); 
    free(local_expr); 

    if(errcode == -999)
        postfix = NULL; 

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