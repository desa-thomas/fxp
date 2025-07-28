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
typedef struct{
    void* next;
    char* val;
}sNODE;

typedef struct
{
    sNODE* top;
    int len;
}Stack;

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
Stack methods
*/

Stack* create_stack();
sNODE* create_sNode(char* val);
void free_node(sNODE*); 
void freeStack(Stack* stack);   //free all pointers
void push(Stack* stack, char* val); 

/*
returns val stored in top node 
NOTE: call free(val) when done using popped value
*/
char* pop(Stack* stack); 
char* peek(Stack* stack);
//debugging:
void printstack(Stack* stack); 

/* convert infix string expr to postfix expression using stack */
int infix_to_postfix(char* expr, char* postfix, int buffersize); 
#define CONCAT_OVERFLOW(s1, s2, buffer) (strlen(s1) + strlen(s2) > (size_t)buffer)

/*Is a string operator i.e., log, cos, sin, sqrt ...*/
Bool isStrOpr(char* expr); 
/* 
Compare oper1 to opr2:
-1 lower precedence
0  equal precedence
1  higher precedence
*/
int cmpopr(char opr1, char opr2);


/* 
expression tree methods 
*/
tNODE* create_tNode(char* val); 
void freeTree(tNODE* root);     //free all pointers
