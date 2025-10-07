//1.  Calculator Problem Statement
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include <ctype.h>
#include<string.h>

#define MAX 100
char infix[MAX];
char eqn[MAX];
char postfix[MAX];
// char stack[MAX];

bool valid_token(char infix[] , char eqn[]);
bool error_check(char eqn[]);
int precedence(char op);
void infix_to_postfix(char eqn[] , char postfix[]);
int evaluate_postfix(char eqn[]);
// void push(int x);
// int pop();
// int top=-1;

int main()
{
    
    printf("Enter the mathematical expression : "); 
    fgets(infix, sizeof(infix), stdin);
    // remove newline
    size_t len = strlen(infix);
    if(len > 0 && infix[len-1] == '\n') {
        infix[len-1] = '\0';}

    printf("\n The mathematical expression , you entered is: %s", infix);

    if(!valid_token(infix,eqn))  // Step 1: Tokenize and validate
    {
        printf("\n invALID EXPRESSION ");
        return 1;
    };

    if(!error_check(eqn))   // Step 2: error checking
    {
        printf("\n Expression failed error validation");
    }

    infix_to_postfix(eqn , postfix);  // Step 3: Convert to postfix
    printf("\n Postfix Expression is :  %s",postfix);

    
    int result = evaluate_postfix(postfix);  // Step 4: Evaluate postfix
    printf("\nResult = %d\n", result);

    return 0;
}

// function 1 : to check whether the expression is valid or invalid and also tokenizing 
bool valid_token(char infix[] , char eqn[])
{
    int i=0;  // for infix
    int j=0;  // for reqn
    
    while(infix[i] != '\0')
    {
        //1. skip spaces 
        if(infix[i] == ' ')
        {
            i++;
            continue;
        }
        //2. digit check
        if ( infix[i] >= '0' && infix[i] <= '9') // inserting consecutive number 
        {
            while(infix[i] >= '0' && infix[i] <= '9')
            {
                eqn[j++] = infix[i++];
            }
            eqn[j++]=' ';
        }
        //3. checking for operator 
        else if( infix[i]=='+' || infix[i]=='-' || infix[i]=='*' || infix[i]=='/')
        {
            eqn[j++]=infix[i++];
            eqn[j++]=' ';
        }
        //4. else invalid characters
        else
        {
            printf("\nInvalid characters in the expression");
            return false;
        }
    }

    eqn[j] = '\0';
    printf("\nAfter tokenisation : %s" , eqn);

return true;
}

// function 2 : to handle all the error handling edge cases 
bool error_check(char eqn[])
{
    // bool nextNumber = true; // initially eqn starts with operands , flag
    int i = 0;
    int lastType = -1 ; // 0 : number , 1 : operands 

    int length = strlen(eqn);

    // for (int k = 0; eqn[k] != '\0'; k++) {
    //     if (isdigit(eqn[k])) {
    //         nextNumber = true;
    //         break;
    //     }
    // }

    // if (!nextNumber) {
    //     printf("Expression is empty or contains no numbers\n");
    //     return false;
    // }

    while( eqn[i] != '\0')
    {
        if( eqn[i]== ' ')
        {
            i++;
            continue;
        }
        if (isdigit(eqn[i])) { 
            if (lastType == 0) { 
                printf("\n Error: two consecutive numbers in a row " );
                return false;
            }
            while (isdigit(eqn[i]))
            { i++;
            } 
            lastType = 0; 
        }
        else if( eqn[i]=='+' || eqn[i]=='-' || eqn[i]=='*' || eqn[i]=='/' )
        {
            if(lastType == 1)
            {
                printf(" \nError ! , two consecutive operators.\n ");
                return false; // error
            }   
            lastType = 1;
            i++;
        }
        else {
            printf("Error: Invalid character ");
            return false;
        }
    }

    int lastIndex = strlen(eqn) - 1;
    while (lastIndex >= 0 && eqn[lastIndex] == ' ') lastIndex--;
    if (eqn[lastIndex] == '+' || eqn[lastIndex] == '-' || eqn[lastIndex] == '*' || eqn[lastIndex] == '/') {
        printf("\n Invalid end of the equation , expression ends with an operator");
        return false;
    }
return true;
}


// function 3 : precedence checking 
int precedence(char op)
{
    if(op=='+'|| op=='-') return 1;
    if(op=='*' || op=='/') return 2;
    return 0;
}

//function 4 : infix to postfix 
void infix_to_postfix( char eqn[] , char postfix[] )
{
    char stack[MAX];
    int top = -1;
    int i=0 , j=0 ;

    while( eqn[i] != '\0')
    {
        if(eqn[i]==' ')
        {
            i++;
            continue;
        }

        if( isdigit(eqn[i])) // if digit then append to postfix
        {
            while(isdigit(eqn[i]))
            {
                postfix[j++] = eqn[i++];
            }
            postfix[j++] =' ';
        }

        else if( eqn[i]=='+' || eqn[i]=='-' || eqn[i]=='*' || eqn[i]=='/' ) // if operator then manage stack
        {
            while( top>=0 && precedence(stack[top]) >= precedence(eqn[i]))
            {
                postfix[j++] = stack[top--];
                postfix[j++] = ' ';
            }
            stack[++top] = eqn[i];
            i++;
        }
        else
        {
            printf("Invalid character in in infix_to_postfix. : %c\n", eqn[i]);
            exit(1);
        }
    }

    // pop remaining operators
    while (top >= 0) {
        postfix[j++] = stack[top--];
        postfix[j++] = ' ';
    }

    postfix[j] = '\0';
}

//function 5 : evaluate postfix
int evaluate_postfix(char eqn[]) 
{       int stack[MAX];
        int top = -1;
        int i = 0;

    while (eqn[i] != '\0') {
        if (eqn[i] == ' ') {
            i++;
            continue;
        }
        if (isdigit(eqn[i])) {  // If number → push to stack
            int num = 0;
            while (isdigit(eqn[i])) {
                num = num * 10 + (eqn[i] - '0');
                i++;
            }
            // push(num);
            stack[++top] = num;
        }
        else if (eqn[i] == '+' || eqn[i] == '-' || eqn[i] == '*' || eqn[i] == '/') {  // If operator → pop two and compute
            if (top < 1) {
                printf("\nError: Invalid postfix expression.\n");
                exit(1);
            }
            int val2 = stack[top--];
            int val1 = stack[top--];
            int result = 0;

            switch (eqn[i]) {
                case '+': 
                    result = val1 + val2; 
                    break;
                case '-':
                    result = val1 - val2; 
                    break;
                case '*': 
                    result = val1 * val2; 
                    break;
                case '/': 
                    if (val2 == 0) {
                        printf("\nError: Division by zero.\n");
                        exit(1);
                    }
                    result = val1 / val2;
                    break;
                    }
                stack[++top] = result;
                i++;
        }
        else {
            printf("Error: Invalid character '%c' in postfix.\n", eqn[i]);
            exit(1);
        }
    }

     if (top != 0) {
        printf("\nError: Invalid postfix evaluation.\n");
        exit(1);
    } 
    return stack[top];
}

// //function 6: 
// void push(int x) {
//     stack[++top] = x;
// }

// //function 7:
// int pop() {
//     return stack[top--];
// }
