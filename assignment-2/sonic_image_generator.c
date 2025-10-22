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


    bool isSizeValid(const int matrixSize);
    bool isEmptyorSpaces(const char *matrixInput);
    void clearInputBuffer(void);
    bool isNumber(const char *matrixInput);

    int generateRandomNumber();
    void generateImageMatrix(const int matrixSize, int (*matrix)[matrixSize]);
    void printMatrix(const int matrixSize, int (*matrix)[matrixSize]);
    void rotatedImageMatrix(const int matrixSize, int (*matrix)[matrixSize]);

    void swap( int *firstNumber , int *secondNumber );

    void smoothingFilter(const int matrixSize , int (*matrix)[matrixSize]);


    int main()
    {
        char matrixInput[10];
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
        int (*matrix)[matrixSize] = imageMatrix;
        
        state = (unsigned int)time(NULL);

        generateImageMatrix(matrixSize, matrix);
        printf(" Original Randomly Generated Matrix : \n");
        printMatrix(matrixSize , matrix);
        printf("\n");

        printf("Matrix after 90 degree Clockwise Rotation : \n");
        rotatedImageMatrix( matrixSize , matrix );
        printMatrix(matrixSize , matrix);
        printf("\n");    

        printf("Matrix after Applying 3x3 Smoothing Filter : \n");
        smoothingFilter(matrixSize , matrix);
        printMatrix(matrixSize , matrix);
        printf("\n");

        return 0;
    }

    bool isSizeValid(const  int matrixSize)
    {
        if(matrixSize >=2 && matrixSize <=10)
        {
            return true;
        }
        
        return false;
    }

    bool isEmptyorSpaces(const char *matrixInput)
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

    bool isNumber(const char *matrixInput)
    {
        int i = 0;

        while (matrixInput[i] && isspace((unsigned char)matrixInput[i])) 
        {
            i++;
        }
        
        if (!isdigit((unsigned char)matrixInput[i]))
        {
            return false;
        } 

        while (matrixInput[i] && isdigit((unsigned char)matrixInput[i]))
        {
            i++;
        } 

        while (matrixInput[i] && isspace((unsigned char)matrixInput[i])) 
        {
            i++;
        }

        return matrixInput[i] == '\0';
    }

    int generateRandomNumber()
    {
        state = ( state * mutiplier + increment ) % 256 ;
        return state;
    }


    void generateImageMatrix(int matrixSize, int (*matrix)[matrixSize]) {
        for (int row = 0; row < matrixSize; row++) {
            for (int column = 0; column  < matrixSize; column++) {
                *(*(matrix+row) +column) = generateRandomNumber();
            }
        }
    }

    void printMatrix(int matrixSize, int (*matrix)[matrixSize])
    {
        for(int row=0 ; row<matrixSize ; row++)
        {
            for(int column=0 ; column<matrixSize ; column++)
            {
                printf("%5d " , *(*(matrix+row) +column) );
            }
            printf("\n");
        }
    }

    void rotatedImageMatrix(int matrixSize, int (*matrix)[matrixSize])
    {
        for( int row=0 ; row<matrixSize  ; row++)
        {
            for( int column = row + 1 ; column < matrixSize ; column++)
            {
                swap( (*( matrix + row ) + column ) , (*( matrix + column ) + row ) );
            }
        }  

        for( int row=0 ; row<matrixSize ; row++)
        {
            int *startElement =  *(matrix + row );
            int *endElement = *( matrix + row ) + ( matrixSize - 1);

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
        int temporaryValue = *secondNumber ; 
        *secondNumber = *firstNumber ;
        *firstNumber = temporaryValue;
    }

    void smoothingFilter(int matrixSize , int (*matrix)[matrixSize])
    {
        int previousRow[matrixSize];
        int currentRow[matrixSize];
        int newRow[matrixSize];

        for( int row=0 ; row < matrixSize  ; row++)
        {
            for (int col = 0; col < matrixSize; col++) 
            {
                *(currentRow + col) = *(*(matrix + row) + col);
            }   

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

                    for ( int sourceColumn = -1 ; sourceColumn<=1 ; sourceColumn++)
                    {
                        int newColumn = column + sourceColumn;

                        if( newColumn<0 || newColumn>= matrixSize )
                        {
                            continue;
                        }

                        if (sourceRow == -1)
                        {
                            totalSum += *(previousRow+ newColumn); //upper
                        }    
                        else if (sourceRow == 0)
                        {
                            totalSum += *(currentRow +newColumn); //current 
                        } 
                        else
                        {
                            totalSum += *(*(matrix + newRow) +newColumn); //below 
                        }

                        elementCount++;
                    }
                }
                int newValue = totalSum / elementCount;

                *(newRow + column) = newValue;
            }

            for (int col = 0; col < matrixSize; col++)
            {
                *(*(matrix + row) + col) = *(newRow + col);
            }
            
            for (int col = 0; col < matrixSize; col++) 
            {
                *(previousRow + col) = *(currentRow + col);
            }   
        }
    }
