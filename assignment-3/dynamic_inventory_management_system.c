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


void getProductDetails(const int numberOfProduct , ProductDetails *Product); 
void getValidID( const int numberOfProduct , char *product_id , ProductDetails *Product  , const int size);
bool isProductIdUnique( const int numberOfProduct , const char *product_id , ProductDetails *Product);
void getValidProductName( const int numberOfProduct , char *product_name , const int size);
void getValidProductPrice(const int numberOfProduct , char *product_price , const int size); //<-and  currenlty on this
void getValidProductQuantity(const int numberOfProduct , char *product_quantity , const int size);

// void addNewProduct(void);
void viewAllProduct(const int numberOfProduct , const ProductDetails *Product);
void updateProductQuantity(const int numberOfProduct , ProductDetails *Product ); //<- currenlty on this
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
    getProductDetails(  numberOfProduct , Product);

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
                    viewAllProduct(numberOfProduct , Product);
                    continue;
            case 3 : 
                    updateProductQuantity(numberOfProduct , Product);
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
    char numOfProduct[6];
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
    int size=0;
    for(int index=0 ; index<numberOfProduct ; index++)
    {
        printf("\nEnter details for product %d " , index+1);
        printf("\nProduct ID : ");
        fgets(Product[index].productID , sizeof(Product[index].productID) , stdin);
        size= sizeof(Product[index].productID);
        getValidID( index , Product[index].productID , Product , size );

        printf("Product Name : ");
        fgets(Product[index].productName , sizeof(Product[index].productName) , stdin);
        size= sizeof(Product[index].productName);
        getValidProductName( numberOfProduct , Product[index].productName ,  size );
        
        printf("Product Price : ");
        fgets(Product[index].productPrice , sizeof(Product[index].productPrice) , stdin);
        size= sizeof(Product[index].productPrice);
        getValidProductPrice( numberOfProduct , Product[index].productPrice ,  size );

        printf("Product Quantity : ");
        fgets(Product[index].productQuantity , sizeof(Product[index].productQuantity) , stdin);
        size= sizeof(Product[index].productQuantity);
        getValidProductQuantity( numberOfProduct , Product[index].productQuantity ,  size );

        
    }
}

void getValidID( const int numberOfProduct , char *product_id , ProductDetails *Product , const int size)
{
    while(1)
    {
        if( strchr( product_id, '\n') == NULL)
        {
            clearInputBuffer();
        }

        product_id[ strcspn( product_id , "\n")] = '\0';

        if( isEmptyorSpaces (product_id) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter Product ID again : ");
            fgets( product_id , size , stdin);
            continue;
        }

        if( !isNumber ( product_id))
        {
            printf("Input should be numeric value only!\n");
            printf("Enter Product ID again : ");
            fgets( product_id , size , stdin);
            continue;
        }
        
        int productID = -1;
        int parsedItem = sscanf( product_id , "%d" , &productID);
        if( parsedItem != 1)
        {
            printf("Please enter a valid number! \n");
            printf("Enter Product ID again : ");
            fgets( product_id ,size , stdin);
            continue;
        }

        if( productID <0 || productID > 10000)
        {
            printf(" Input should be between 1 and 10000 only! \n");
            printf("Enter Product ID again : ");
            fgets( product_id , size , stdin);
            continue;
        }
        
        if( !isProductIdUnique( numberOfProduct ,  product_id , Product))
        {
            printf("Product Id should be unique  ");
            printf("\nEnter Product ID again : ");
            fgets( product_id , size, stdin);
            continue;
        }
        break;
    }
}

bool isProductIdUnique( const int numberOfProduct , const char *product_id ,  ProductDetails *Product)
{
    for(int index=0 ; index<numberOfProduct ; index++)
    {
        if( strcmp(Product[index].productID , product_id) == 0 )
        {
            return false;
        }
    }
    return true;
}

void getValidProductName( const int numberOfProduct , char *product_name , const int size)
{
    while(1)
    {
        if( strchr( product_name, '\n') == NULL)
        {
            clearInputBuffer();
        }

        product_name[ strcspn( product_name , "\n")] = '\0';

        if( isEmptyorSpaces (product_name) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter Product Name again : ");
            fgets( product_name , size , stdin);
            continue;
        }

        bool validName = true ; 
        for( int index=0 ; product_name[index] != '\0' ; index++ )
        {
            if ( !isalpha((unsigned char)product_name[index]) && !isspace((unsigned char)product_name[index]) )
            {
                validName = false;
                break;
            }
        }

        if( !validName)
        {
            printf("Product Name should be characters and spaces only ");
            printf("\nEnter Product Name again : ");
            fgets( product_name , size , stdin);
            continue;
        }

        break;
    }
}


void getValidProductPrice(const int numberOfProduct , char *product_price , const int size)
{
    while(1)
    {
        if( strchr( product_price, '\n') == NULL)
        {
            clearInputBuffer();
        }

        product_price[strcspn(product_price , "\n")] = '\0';

        if( isEmptyorSpaces (product_price) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter Product Price again : ");
            fgets( product_price , size , stdin);
            continue;
        }

        //=================================== currenlty working on validation of the floating point vals


        // if( !isNumber ( product_price))
        // {
        //     printf("Input should be numeric value only!\n");
        //     printf("Enter Product Price again : ");
        //     fgets( product_price , size , stdin);
        //     continue;
        // }

        float productPrice = -1;
        
        int parsedItem = sscanf( product_price , "%f" , &productPrice);
        if( parsedItem != 1)
        {
            printf("Please enter a valid price! \n");
            printf("Enter Product price again : ");
            fgets( product_price ,size , stdin);
            continue;
        }

        if( productPrice <0 || productPrice > 100000)
        {
            printf(" Price should be between 1 and 100000 only! \n");
            printf("Enter Product Price again : ");
            fgets( product_price , size , stdin);
            continue;
        }

        break;
    }
}

void getValidProductQuantity(const int numberOfProduct , char *product_quantity , const int size)
{
    while(1)
    {
        if( strchr( product_quantity, '\n') == NULL)
        {
            clearInputBuffer();
        }

        product_quantity[strcspn(product_quantity , "\n")] = '\0';

        if( isEmptyorSpaces (product_quantity) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter Product Quantity again : ");
            fgets( product_quantity , size , stdin);
            continue;
        }

        if( !isNumber ( product_quantity))
        {
            printf("Input should be numeric value only!\n");
            printf("Enter Product Quantity again : ");
            fgets( product_quantity , size , stdin);
            continue;
        }

        int productQuantity = -1;
        
        int parsedItem = sscanf( product_quantity , "%d" , &productQuantity);
        if( parsedItem != 1)
        {
            printf("Please enter a valid price! \n");
            printf("Enter Product Quantity again : ");
            fgets( product_quantity ,size , stdin);
            continue;
        }

        if( productQuantity <0 || productQuantity > 1000000)
        {
            printf(" Price should be between 1 and 1000000 only! \n");
            printf("Enter Product Quantity again : ");
            fgets( product_quantity , size , stdin);
            continue;
        }
        break;
    }
}

void viewAllProduct(const int numberOfProduct , const ProductDetails *Product)
{
   printf("\n========= PRODUCT LIST ========= ");
   for(int index=0 ; index<numberOfProduct ; index++)
    {
        printf("\nProduct ID :%s | Name : %s | Price : %s | Quantity : %s " , 
            Product[index].productID , Product[index].productName , Product[index].productPrice 
            , Product[index].productQuantity 
        );
    } 
}

void updateProductQuantity(const int numberOfProduct , ProductDetails *Product )
{
    char inputID[10];
    printf("\nEnter Product ID to update quantity: ");
    fgets( inputID , sizeof(inputID) , stdin);
    while(1)
    {
        if( strchr( inputID, '\n') == NULL)
        {
            clearInputBuffer();
        }

        inputID[ strcspn( inputID , "\n")] = '\0';

        if( isEmptyorSpaces (inputID) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter Product ID again : ");
            fgets( inputID , sizeof(inputID) , stdin);
            continue;
        }

        if( !isNumber ( inputID))
        {
            printf("Input should be numeric value only!\n");
            printf("Enter Product ID again : ");
            fgets( inputID , sizeof(inputID) , stdin);
            continue;
        }

        break;
    }
    
    for( int index=0 ; index<numberOfProduct ; index++)
    {
        if(strcmp( Product[index].productID , inputID ) ==0 )
        {
            printf("\nProduct ID :%s | Name : %s | Price : %s | Quantity : %s " , 
                Product[index].productID , Product[index].productName , Product[index].productPrice 
                , Product[index].productQuantity 
            );
        }
        else
        {
            printf("\nNo such product id exist ");
            break;
        }
    }
}
