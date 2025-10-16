#include<stdio.h>
#include<stdbool.h>
#include <ctype.h>
#include<string.h>
#include <time.h>
#include<stdlib.h>


// Linear congruential algo constants
#define mutiplier 1103515245u
#define increment 12345u
unsigned int state;


bool isSizeValid(int matrixSize);
bool isEmptyorSpaces(char *matrixInput);
void clearInputBuffer(void);
bool isNumber(char *matrixInput);

int generateRandomNumber();
void generateImageMatrix(int matrixSize, int (*p)[matrixSize]);
void printMatrix(int matrixSize, int (*p)[matrixSize]);
void rotatedImageMatrix(int matrixSize, int (*p)[matrixSize]);

void swap( int *a , int *b );

void smoothingFilter(int matrixSize , int (*p)[matrixSize]);


int main()
{
    char matrixInput[5];
    int matrixSize;
    while(1)
    {
        printf("\nEnter the matrix size value 2-10 :");
        fgets( matrixInput , sizeof(matrixInput) , stdin );
        
        if (strchr(matrixInput, '\n') == NULL)
        {
            clearInputBuffer();
        }

        matrixInput[strcspn(matrixInput, "\n")] = '\0';

        if(isEmptyorSpaces(matrixInput))
        {
            printf("Input cannot be empty or spaces only!\n");
            continue;
        }

        if (!isNumber(matrixInput))
        {
            printf("Input must contain only digits!\n");
            continue;
        }

        matrixSize = -1; 
   
        int parsedItems = sscanf(matrixInput, "%d" , &matrixSize);
        if( parsedItems != 1 )
        {
            printf("Please enter a valid number\n");
            continue;
        }

        if(!isSizeValid(matrixSize))
        {
            printf("Enter valid value that is between 2-10\n");
            continue;
        }

        break;
    }
   
    printf(" Matrix size is %d x %d \n" , matrixSize , matrixSize);

    int imageMatrix[matrixSize][matrixSize];
    int (*p)[matrixSize] = imageMatrix;
    
    state = (unsigned int)time(NULL);

    generateImageMatrix(matrixSize, p);
    printf(" Original Randomly Generated Matrix : \n");
    printMatrix(matrixSize , p);
    printf("\n");

    printf("Matrix after 90 degree Clockwise Rotation : \n");
    rotatedImageMatrix( matrixSize , p );
    printMatrix(matrixSize , p);
    printf("\n");    

    printf("Matrix after Applying 3x3 Smoothing Filter : \n");
    smoothingFilter(matrixSize , p);
    printMatrix(matrixSize , p);
    printf("\n");

    return 0;
}

bool isSizeValid(int matrixSize)
{
    if(matrixSize >=2 && matrixSize <=10)
    {
        return true;
    }
    
    return false;
}

bool isEmptyorSpaces(char *matrixInput)
{
    for( int index=0 ; matrixInput[index] != '\0' ; index++)
    {
        if( !isspace(matrixInput[index]))  
        {
            return false ; 
        }
    }
    return true; 
}

void clearInputBuffer(void)
{
    int inputChar;
    while (( inputChar = getchar()) != '\n' && inputChar !=EOF);
}

bool isNumber ( char *matrixInput)
{
    for( int charIndex=0 ; matrixInput[charIndex] !='\0' ; charIndex++)
    {
        if(!isdigit((unsigned char) matrixInput[charIndex]))
        {
            return false;   
        }
    }
    return true;
}

int generateRandomNumber()
{
    state = ( state * mutiplier + increment ) % 256 ;
    return state;
}


void generateImageMatrix(int matrixSize, int (*p)[matrixSize]) {
    for (int row = 0; row < matrixSize; row++) {
        for (int column = 0; column  < matrixSize; column++) {
            *(*(p+row) +column) = generateRandomNumber();
        }
    }
}

void printMatrix(int matrixSize, int (*p)[matrixSize])
{
     for(int row=0 ; row<matrixSize ; row++)
    {
        for(int column=0 ; column<matrixSize ; column++)
        {
            printf("%3d " , *(*(p+row) +column) );
        }
        printf("\n");
    }
}

void rotatedImageMatrix(int matrixSize, int (*p)[matrixSize])
{
    for( int row=0 ; row<matrixSize  ; row++)
    {
        for( int column = row + 1 ; column < matrixSize ; column++)
        {
            swap( (*( p + row ) + column ) , (*( p + column ) + row ) );
        }
    }  

    for( int row=0 ; row<matrixSize ; row++)
    {
        int *startElement =  *(p + row );
        int *endElement = *( p + row ) + ( matrixSize - 1);

        while( startElement < endElement)
        {
            swap( startElement , endElement);
            startElement++;
            endElement--;
        }
    }
}

void swap( int *firstNumber , int *secondNumber )
{
    int temp = *secondNumber ; 
    *secondNumber = *firstNumber ;
    *firstNumber = temp;
}

void smoothingFilter(int matrixSize , int (*p)[matrixSize])
{
     for( int row=0 ; row < matrixSize  ; row++)
    {
        for( int column = 0 ; column < matrixSize ; column++)
        {
            int elementCount = 0;
            int totalSum = 0;

            for ( int sourceRow = -1 ; sourceRow<=1 ; sourceRow++)
            {
                int newRow = row + sourceRow ;

                if( newRow < 0 || newRow >= matrixSize )
                {
                    continue;
                }

                int *rowIndex = *(p + newRow);

                for ( int sourceColumn = -1 ; sourceColumn<=1 ; sourceColumn++)
                {
                    int newColumn = column + sourceColumn;

                    if( newColumn < 0 || newColumn >= matrixSize )
                    {
                        continue;
                    }

                    totalSum += *(rowIndex + newColumn );
                    elementCount++;
                }
            }
            int newValue = totalSum / elementCount;

            *(*(p + row) + column) = newValue;
        }
    }
}
