//1.  Calculator Problem Statement
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdbool.h>
#include <ctype.h>
#include<string.h>

#define MAX 100
char infix_expression[MAX];
char equation[MAX];
char postfix_expression[MAX];

bool valid_token(char infix_expression[] , char equation[]);
bool error_check(char equation[]);
int precedence(char op);
void infix_expression_to_postfix_expression(char equation[] , char postfix_expression[]);
int evaluate_postfix_expression(char equation[]);

int main()
{
    
    printf("Enter the mathematical expression : "); 
    fgets(infix_expression, sizeof(infix_expression), stdin);
    // remove newline
    size_t len = strlen(infix_expression);
    if(len > 0 && infix_expression[len-1] == '\n') {
        infix_expression[len-1] = '\0';}

    printf("\n The mathematical expression , you entered is: %s", infix_expression);

    if(!valid_token(infix_expression,equation))  // Step 1: Tokenize and validate
    {
        printf("\n invALID EXPRESSION ");
        return 1;
    };

    if(!error_check(equation))   // Step 2: error checking
    {
        printf("\n Expression failed error validation");
    }

    infix_expression_to_postfix_expression(equation , postfix_expression);  // Step 3: Convert to postfix_expression
    printf("\n postfix_expression Expression is :  %s",postfix_expression);

    
    int result = evaluate_postfix_expression(postfix_expression);  // Step 4: Evaluate postfix_expression
    printf("\nResult = %d\n", result);

    return 0;
}

// function 1 : to check whether the expression is valid or invalid and also tokenizing 
bool valid_token(char infix_expression[] , char equation[])
{
    int i=0;  // for infix_expression
    int j=0;  // for requation
    
    while(infix_expression[i] != '\0')
    {
        //1. skip spaces 
        if(infix_expression[i] == ' ')
        {
            i++;
            continue;
        }
        //2. digit check
        if ( infix_expression[i] >= '0' && infix_expression[i] <= '9') // inserting consecutive number 
        {
            while(infix_expression[i] >= '0' && infix_expression[i] <= '9')
            {
                equation[j++] = infix_expression[i++];
            }
            equation[j++]=' ';
        }
        //3. checking for operator 
        else if( infix_expression[i]=='+' || infix_expression[i]=='-' || infix_expression[i]=='*' || infix_expression[i]=='/')
        {
            equation[j++]=infix_expression[i++];
            equation[j++]=' ';
        }
        //4. else invalid characters
        else
        {
            printf("\nInvalid characters in the expression");
            return false;
        }
    }

    equation[j] = '\0';
    printf("\nAfter tokenisation : %s" , equation);

return true;
}

// function 2 : to handle all the error handling edge cases 
bool error_check(char equation[])
{
    // bool nextNumber = true; // initially equation starts with operands , flag
    int i = 0;
    int lastType = -1 ; // 0 : number , 1 : operands 

    int length = strlen(equation);

    // for (int k = 0; equation[k] != '\0'; k++) {
    //     if (isdigit(equation[k])) {
    //         nextNumber = true;
    //         break;
    //     }
    // }

    // if (!nextNumber) {
    //     printf("Expression is empty or contains no numbers\n");
    //     return false;
    // }

    while( equation[i] != '\0')
    {
        if( equation[i]== ' ')
        {
            i++;
            continue;
        }
        if (isdigit(equation[i])) { 
            if (lastType == 0) { 
                printf("\n Error: two consecutive numbers in a row " );
                return false;
            }
            while (isdigit(equation[i]))
            { i++;
            } 
            lastType = 0; 
        }
        else if( equation[i]=='+' || equation[i]=='-' || equation[i]=='*' || equation[i]=='/' )
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

    int lastIndex = strlen(equation) - 1;
    while (lastIndex >= 0 && equation[lastIndex] == ' ') lastIndex--;
    if (equation[lastIndex] == '+' || equation[lastIndex] == '-' || equation[lastIndex] == '*' || equation[lastIndex] == '/') {
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

//function 4 : infix_expression to postfix_expression 
void infix_expression_to_postfix_expression( char equation[] , char postfix_expression[] )
{
    char stack[MAX];
    int top = -1;
    int i=0 , j=0 ;

    while( equation[i] != '\0')
    {
        if(equation[i]==' ')
        {
            i++;
            continue;
        }

        if( isdigit(equation[i])) // if digit then append to postfix_expression
        {
            while(isdigit(equation[i]))
            {
                postfix_expression[j++] = equation[i++];
            }
            postfix_expression[j++] =' ';
        }

        else if( equation[i]=='+' || equation[i]=='-' || equation[i]=='*' || equation[i]=='/' ) // if operator then manage stack
        {
            while( top>=0 && precedence(stack[top]) >= precedence(equation[i]))
            {
                postfix_expression[j++] = stack[top--];
                postfix_expression[j++] = ' ';
            }
            stack[++top] = equation[i];
            i++;
        }
        else
        {
            printf("Invalid character in in infix_expression_to_postfix_expression. : %c\n", equation[i]);
            exit(1);
        }
    }

    // pop remaining operators
    while (top >= 0) {
        postfix_expression[j++] = stack[top--];
        postfix_expression[j++] = ' ';
    }

    postfix_expression[j] = '\0';
}

//function 5 : evaluate postfix_expression
int evaluate_postfix_expression(char equation[]) 
{       int stack[MAX];
        int top = -1;
        int i = 0;

    while (equation[i] != '\0') {
        if (equation[i] == ' ') {
            i++;
            continue;
        }
        if (isdigit(equation[i])) {  // If number → push to stack
            int num = 0;
            while (isdigit(equation[i])) {
                num = num * 10 + (equation[i] - '0');
                i++;
            }
            // push(num);
            stack[++top] = num;
        }
        else if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/') {  // If operator → pop two and compute
            if (top < 1) {
                printf("\nError: Invalid postfix_expression expression.\n");
                exit(1);
            }
            int value_2 = stack[top--];
            int value_1 = stack[top--];
            int result = 0;

            switch (equation[i]) {
                case '+': 
                    result = value_1 + value_2; 
                    break;
                case '-':
                    result = value_1 - value_2; 
                    break;
                case '*': 
                    result = value_1 * value_2; 
                    break;
                case '/': 
                    if (value_2 == 0) {
                        printf("\nError: Division by zero.\n");
                        exit(1);
                    }
                    result = value_1 / value_2;
                    break;
                    }
                stack[++top] = result;
                i++;
        }
        else {
            printf("Error: Invalid character '%c' in postfix_expression.\n", equation[i]);
            exit(1);
        }
    }

     if (top != 0) {
        printf("\nError: Invalid postfix_expression evaluation.\n");
        exit(1);
    } 
    return stack[top];
}

