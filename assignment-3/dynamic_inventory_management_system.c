#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

typedef struct ProductDetails 
{
    char productID[10];
    char productName[60];
    char productPrice[10];
    char productQuantity[7];
} ProductDetails;

int userInput(void);
void clearInputBuffer(void);
bool isEmptyorSpaces(const char numOfProduct[]);
bool isNumber(const char numOfProduct[]);
bool isSizeValid( const int numberOfProduct);

void getProductDetails(const int numberOfProduct , ProductDetails *Product); // in working
// void getValidInput( const int numberOfProduct , ProductDetails *Product);
// void addNewProduct(void);
// void viewAllProduct(void);
// void searchProductById(void);
// void searchProductByName(void);
// void searchProductByPriceRange(void);
// void deleteProduct(void);


int main()
{
    int numberOfProduct = userInput();
    printf(" you entered : %d" , numberOfProduct);

    ProductDetails *Product = malloc( numberOfProduct * sizeof(ProductDetails));
    if ( Product == NULL)
    {
        printf("Memory allocation failed\n");
        return -1;
    }

    //  function to take input of users 


    printf("\n ========= INVENTORY MENU ========= ");
    printf("\n 1. Add New Product \n 2. View All Products \n 3. Update Quantity \n 4. Search Product by ID");
    printf(" \n 5. Search Product by Name \n 6. Search Product by Price Range \n 7. Delete Product \n 8. Exit");

    int choice;
    while(1)
    {
        printf("\n Enter your choice: ");
        scanf("%d" , &choice);
        switch(choice)
        {
            case 1 : 
                    printf("\n add new product function goes here ");
                    continue;
            case 2 : 
                    printf("\n view all product function goes here ");
                    continue;
            case 3 : 
                    printf("\n update product quantity function goes here ");
                    continue;
            case 4 : 
                    printf("\n search by product id function goes here ");
                    continue;
            case 5 : 
                    printf("\n search by product name  function goes here ");
                    continue;
            case 6 : 
                    printf("\n search  product by price range function goes here ");
                    continue;
            case 7 : 
                    printf("\n delete product function goes here ");
                    continue;
            case 8 :
                    printf("\n exit product function goes here ");
                    break;
            default :
                    printf("\n Invalid choice");
                    continue;
        }
        break;
    }

    
    free(Product);
    Product = NULL;
    printf("\n\n\n checking for dangling ptr %p" , Product);
    printf(" \n Memory released successfully . Exiting  Program...");

    return 0;
}

void clearInputBuffer(void)
{
    int userInput;
    while( (userInput = getchar()) != '\n'  && userInput!=EOF);
}

bool isEmptyorSpaces(const char numOfProduct[])
{
    for(int index=0 ; numOfProduct[index] != '\0' ; index++)
    {
        if(!isspace(numOfProduct[index]))
        {  
            return false; 
        }
    }
    return true;
}

bool isNumber(const char numOfProduct[])
{
    int index=0 ;   

    while( numOfProduct[index] && isspace((unsigned char) numOfProduct[index]))
    {
        index++;
    }

    if( numOfProduct[index] && !isdigit((unsigned char) numOfProduct[index]))
    {
        return false;
    }

    while( numOfProduct[index] && isdigit((unsigned char) numOfProduct[index]))
    {
        index++;
    }

    while( numOfProduct[index] && isspace((unsigned char) numOfProduct[index]))
    {
        index++;
    }

    return numOfProduct[index] == '\0'; 
}

bool isSizeValid( const int numberOfProduct)
{
    if( numberOfProduct >= 1 && numberOfProduct <= 100)
    {
        return true;
    }
    return false;
}

int userInput(void)
{
    char numOfProduct[5];
    while(1){
        printf("\n enter initial number of products: ");
        fgets( numOfProduct , sizeof(numOfProduct) , stdin ); 
        if( strchr( numOfProduct , '\n') == NULL)
        {
            clearInputBuffer();
        }

        numOfProduct[ strcspn( numOfProduct , "\n")] = '\0';

        if( isEmptyorSpaces (numOfProduct) )
        {
            printf("Input cannot be empty or spaces only!\n");
            continue;
        }

        if( !isNumber ( numOfProduct))
        {
            printf("Input should be numeric value only!\n");
            continue;
        }
        
        int numberOfProduct = -1;
        int parsedItem = sscanf( numOfProduct , "%d" , &numberOfProduct);
        if( parsedItem != 1)
        {
            printf("Please enter a valid number! \n");
        }

        if( !isSizeValid(numberOfProduct))
        {
            printf(" Input should be between 1 and 100 only! \n");
            continue;
        }

        return numberOfProduct;
    }
}


void getProductDetails(const int numberOfProduct , ProductDetails *Product)
{
    for(int index=0 ; index<numberOfProduct ; index++)
    {
        printf("\n Enter details for product %d " , index+1);
        printf("\n Product ID : ");
        fgets(Product[index].productID , sizeof(Product[index]) , stdin);
        
        
    }
}

