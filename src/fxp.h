/*
fxp: lightweight C function parser and utility
*/

/*
CONSTANTS
*/
#define STREND '\0'
/*
MACROS
*/

#define isOperator(expr) expr == '+' || expr == '-' || expr == '*' || expr == '/' || expr == '^'
#define isBracket(expr) expr == '(' || expr == ')' || expr == '[' || expr == ']'
#define len(arr) (int)(sizeof(arr)/sizeof(arr[0]))

#define appendChar(str, ch, len) str[len] = ch; str[len+1] = STREND
#define char2str(str, ch) str[0] = ch; str[1] = STREND

/* 
Structs 
*/
//type of node value
typedef enum{
    NODE_TYPE, 
    STRING_TYPE
}node_datatype;

/*
NODE is used with the Stack struct for stacks with string contents:
infix_to_postfix
*/
typedef struct{
    void* next;
    void* val;
}NODE;

typedef struct
{
    NODE* top;
    int len;
    node_datatype type; 
}Stack;

/* expression tree data structure*/
typedef struct
{
    void* lChild;
    void* rChild; 
    char* val; 
}tNODE;  

typedef enum
{False, True}Bool; 

/* structure for single variate functions*/
typedef struct
{
    /* independent and dependent variables */
    char indep;
    char dep;

}fox; // for f(x) or f o x 

/* 

STRING STACK METHODS

*/
Stack* create_stack(node_datatype type);
NODE* create_node(void* val, node_datatype datatype);

void free_node(NODE*); 
void freeStack(Stack* stack);   //free all pointers
void push(Stack* stack, char* val); 

/*
returns val stored in top node 
NOTE: call free(val) when done using popped value
*/
void* pop(Stack* stack); 
void* peek(Stack* stack);
//debugging:
void printstack(Stack* stack); 

/* convert infix string expr to postfix expression using stack */
int infix_to_postfix(char* expr, char* postfix, int buffersize); 
#define CONCAT_OVERFLOW(s1, s2, buffer) (strlen(s1) + strlen(s2) +1 > (size_t)buffer)

/*
Checks if a string is a function identifier i.e., log, cos, sin, sqrt ...
as oposed to an operator represented by a char, like '+', '-', '*', '-' ...

The reason for the name is because "log", etc, and be thought of a "string operators"
and '-', '+', etc as "character operators" in the context of the infix to postfix conversion algorithm
*/
Bool isStrOpr(char* expr); 
/* 
Compare char operators oper1 and opr2 by precedence
-1 lower precedence
0  equal precedence
1  higher precedence
*/
int cmpopr(char opr1, char opr2); 

/* 
|
EXPRESSION TREE METHODS 

*/

/* Create a tree node*/
tNODE* create_tNode(char* val); 
/* Free all memory allocated by the expression tree */
void freeTree(tNODE* root);     
/* Create expression tree from postfix string */
tNODE* create_expression_tree(char* postfix);