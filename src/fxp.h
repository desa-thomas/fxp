/*
fxp: lightweight C function parser and utility
*/

/* Structs */

/* Stack and tree nodes*/
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

/* structure for single variate functions*/
typedef struct
{
    /* independent and dependent variables */
    char indep;
    char dep;

}fox; // for f(x) or f o x 


void test(); 

/* Stack methods*/
Stack* create_stack();
sNODE* create_node(char* val);
//free all pointers
void freeStack(Stack* stack);
void push(Stack*, char* val); 
char* pop(Stack* stack); 
char* peek(Stack* stack);
//debugging:
void printstack(Stack* stack); 


