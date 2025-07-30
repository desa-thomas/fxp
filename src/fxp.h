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

#define imax(a, b) (int) a > (int) b ? a : b


typedef enum
{False, True}Bool; 

//type of data that a node contains in `val`
typedef enum{
    NODE_TYPE,  //Node contains another node
    STRING_TYPE //Node contains a string
}node_datatype;

/* 
Structs 
*/
typedef struct{
    
    void* next;
    void* val;

    void* lChild; 
    void* rChild;
    int level; 
}NODE; //multipurpose node

typedef struct
{
    NODE* top;
    int len;
    node_datatype type; 
}Stack;

/* structure for single variate functions*/
typedef struct
{
    /* independent and dependent variables */
    char indep;
    char dep;

}fox; // for f(x) or f o x 

/* 
STACK METHODS
*/
Stack* create_stack(node_datatype type);
NODE* create_node(void* val, node_datatype datatype);

void free_node(NODE*); 
void freeStack(Stack* stack);   //free all pointers
void push(Stack* stack, void* val); 

/*
returns val stored in top node 
NOTE: call free(val) when done using popped value
*/
void* pop(Stack* stack); 
void* peek(Stack* stack);

//debugging:
void printstack(Stack* stack); 

/* 
convert infix string expr to postfix expression using stack 
Parameters:
    char* expr     - string containing infix expression
    char* postfix  - string to write postfix expression to
    int buffersize - size of writeable buffer (i.e., how many bytes is allocated to 'postfix' variable)
*/
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

/* Free all memory allocated by the expression tree */
void freeTree(NODE* root);     
/* Create expression tree from postfix string */
NODE* create_expression_tree(char* postfix);

/* debugging */
void rprinttree(NODE* node, char* padding, char* pointer); 
void printTree(NODE* root); 