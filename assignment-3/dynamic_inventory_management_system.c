#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct ProductDetails {
    int productID;
    char productName[60];
    float productPrice;
    int productQuantity;
} ProductDetails;

void menuPrint(void);
void clearInputBuffer(void);
bool isEmptyorSpaces(const char numOfProduct[]);
bool isNumber(const char numOfProduct[]);
bool isFloatNumber(const char numOfProduct[]);
bool isSizeValid(const int numberOfProduct);
int userInput(void);

void getProductDetails(int numberOfProduct, ProductDetails *Product);
void getValidID(const int numberOfProduct, char *product_id, ProductDetails *Product, const int size);
bool isProductIdUnique(const int numberOfProduct, const int product_id, ProductDetails *Product);
void getValidProductName( const int numberOfProduct , char *product_name , const int size);
void getValidProductPrice(const int numberOfProduct, char *product_price, const int size,  ProductDetails *Product, const int index);
void getValidProductQuantity(int numberOfProduct, char *qty_str, int size, ProductDetails *Product, int index);
void viewAllProduct(int numberOfProduct, const ProductDetails *Product);
int getInputIdToSearch(void);

void updateProductQuantity(int numberOfProduct, ProductDetails *Product);
void searchProductById(int numberOfProduct, const ProductDetails *Product);
void searchProductByName(int numberOfProduct, const ProductDetails *Product);
void searchProductByPriceRange(int numberOfProduct, const ProductDetails *Product);
void addNewProduct(int *numberOfProduct, ProductDetails **productInventory);
void deleteProduct(int *numberOfProduct, ProductDetails **productInventory);

int main() 
{
    printf("\n Welcome! ");
    int numberOfProduct = userInput();

    ProductDetails *Product = calloc(numberOfProduct, sizeof(ProductDetails));
    if (!Product) 
    {
        printf("Memory allocation failed!\n");
        return -1;
    }

    getProductDetails(numberOfProduct, Product);
    menuPrint();

    int choice;
    while (1) 
    {
        char inputChoice[10];
        printf("\nEnter your choice: ");
        if (fgets(inputChoice, sizeof(inputChoice), stdin) == NULL)
        {
            printf("Input error!\n");
            continue;
        }

        inputChoice[strcspn(inputChoice, "\n")] = '\0';

        if (isEmptyorSpaces(inputChoice))
        {
            printf("Input cannot be empty!\n");
            continue;
        }

        if (!isNumber(inputChoice))
        {
            printf("Invalid input! Enter a valid number \n");
            continue;
        }

        choice = atoi(inputChoice);

        switch (choice) 
        {
            case 1:
                addNewProduct(&numberOfProduct, &Product);
                menuPrint();
                break;
            case 2:
                viewAllProduct(numberOfProduct, Product);
                menuPrint();
                break;
            case 3:
                updateProductQuantity(numberOfProduct, Product);
                menuPrint();
                break;
            case 4:
                searchProductById(numberOfProduct, Product);
                menuPrint();
                break;
            case 5:
                searchProductByName(numberOfProduct, Product);
                menuPrint();
                break;
            case 6:
                searchProductByPriceRange(numberOfProduct, Product);
                menuPrint();
                break;
            case 7:
                deleteProduct(&numberOfProduct, &Product);
                menuPrint();
                break;
            case 8:
                printf("\nExiting program...");
                free(Product);
                Product = NULL;
                // printf("\n\n\n checking for dangling ptr %p" , Product);
                printf(" \n Memory released successfully . Exiting  Program...");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void menuPrint(void) 
{
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

void clearInputBuffer(void) 
{
    int userInput;
    while( (userInput = getchar()) != '\n'  && userInput!=EOF);
}

bool isEmptyorSpaces(const char numOfProduct[]) 
{
    if (numOfProduct[0] == '\0') 
    {
        return true;
    }
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

bool isFloatNumber(const char *product_price) 
{
    int index=0 ;
    bool hasDecimalPoint = false;   
    while( product_price[index] && isspace((unsigned char) product_price[index]))
    {
        index++;
    }
    if( product_price[index] !='.' && !isdigit((unsigned char) product_price[index]))
    {
        return false;
    }
    while( product_price[index])
    {
        if(isdigit((unsigned char) product_price[index]))
        {
            index++;
        }
        else if (  product_price[index] =='.')
        {
            if( hasDecimalPoint)
            {
                return false;
            }
            hasDecimalPoint = true;
            index++;
        }
        else
        {
            break;
        }  
    }
    while( product_price[index] && isspace((unsigned char) product_price[index]))
    {
        index++;
    }
    return product_price[index] == '\0'; 
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
    while(1)
    {
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

void getProductDetails(int numberOfProduct, ProductDetails *Product) 
{
    char productID[10];
    char productPrice[10];
    char productQuantity[7];
    for (int index = 0; index < numberOfProduct; index++) {
        printf("\nEnter details for product %d\n", index + 1);

        printf("Product ID: ");
        fgets(productID, sizeof(productID), stdin);
        getValidID(index, productID, Product, sizeof(productID));
        Product[index].productID = atoi(productID);

        printf("Product Name: ");
        fgets(Product[index].productName, sizeof(Product[index].productName), stdin);
        getValidProductName(numberOfProduct, Product[index].productName, sizeof(Product[index].productName));

        printf("Product Price: ");
        fgets(productPrice, sizeof(productPrice), stdin);
        getValidProductPrice(numberOfProduct, productPrice, sizeof(productPrice), Product, index);

        printf("Product Quantity: ");
        fgets(productQuantity, sizeof(productQuantity), stdin);
        getValidProductQuantity(numberOfProduct, productQuantity, sizeof(productQuantity), Product, index);
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

        if( !isProductIdUnique( numberOfProduct ,  productID  , Product))
        {
            printf("Product Id should be unique  ");
            printf("\nEnter Product ID again : ");
            fgets( product_id , size, stdin);
            continue;
        }

        break;
    }
}

bool isProductIdUnique(const int numberOfProduct, const int product_id, ProductDetails *Product) 
{
    for (int index = 0; index < numberOfProduct; index++)
    {
        if (Product[index].productID == product_id) 
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

void getValidProductPrice(const int numberOfProduct, char *product_price, const int size, ProductDetails *Product, const int index){
    float price;

    while (1) 
    {
        if( strchr( product_price, '\n') == NULL)
        {
            clearInputBuffer();
        }
        
        product_price[strcspn(product_price, "\n")] = '\0';
        if( isEmptyorSpaces (product_price) )
        {
            printf("Input cannot be empty or spaces only!\n");
            printf("Enter  Price again : ");
            fgets( product_price , size , stdin);
            continue;
        }

        if( !isFloatNumber ( product_price))
        {
            printf("Input should be numeric value only!\n");
            printf("Enter  Price again : ");
            fgets( product_price , size , stdin);
            continue;
        }

        float productPrice = -1;
        int parsedItem = sscanf( product_price , "%f" , &productPrice);
        if( parsedItem != 1)
        {
            printf("Please enter a valid price! \n");
            printf("Enter  price again : ");
            fgets( product_price ,size , stdin);
            continue;
        }

        if( productPrice <0 || productPrice > 100000)
        {
            printf(" Price should be between 1 and 100000 only! \n");
            clearInputBuffer();
            printf("Enter  Price again : ");
            clearInputBuffer();
            fgets( product_price , size , stdin);
            continue;
        }
        Product[index].productPrice = productPrice;

        break;
    }
}

void getValidProductQuantity(int numberOfProduct, char *product_quantity, int size, ProductDetails *Product, int index) 
{
    int productQuantity;
    while (1) 
    {
        if( strchr( product_quantity, '\n') == NULL)
        {
            clearInputBuffer();
        }

        product_quantity[strcspn(product_quantity, "\n")] = '\0';

        if (isEmptyorSpaces(product_quantity) || !isNumber(product_quantity) ||
            sscanf(product_quantity, "%d", &productQuantity) != 1 || 
            productQuantity < 0 || productQuantity > 1000000) 
        {
            printf("Enter valid quantity (0-1000000): ");
            fgets(product_quantity, size, stdin);
            continue;
        }
        
        Product[index].productQuantity = productQuantity;
        break;
    }
}

void viewAllProduct(int numberOfProduct, const ProductDetails *Product) 
{   
    if( numberOfProduct == 0)
    {
        printf("Inventory is empty!\n");
    }
    else
    {
        printf("\n========= PRODUCT LIST =========\n");
        for(int index=0 ; index<numberOfProduct ; index++)
        {
            printf("Product ID:%-6d | Name:%-15s | Price:%-8.2f | Qty:%-6d\n",
                Product[index].productID, Product[index].productName,
                Product[index].productPrice, Product[index].productQuantity
            );
        }
    }
}

int getInputIdToSearch(void) 
{
    char inputID[10];
    int id;
    while (1) 
    {
        fgets(inputID, sizeof(inputID), stdin);
        inputID[strcspn(inputID, "\n")] = '\0';
        if (isEmptyorSpaces(inputID) || !isNumber(inputID) || sscanf(inputID, "%d", &id) != 1) 
        {
            printf("Enter valid numeric ID: ");
            continue;
        }
        return id;
    }
}

void updateProductQuantity(int numberOfProduct, ProductDetails *products)
{
    printf("\nEnter Product ID to update quantity: ");
    int productID = getInputIdToSearch();

    bool found = false;

    for (int index = 0; index < numberOfProduct; index++) 
    {
        if (products[index].productID == productID) 
        {
            found = true;
            printf("Product ID:%-6d | Name:%-15s | Price:%-8.2f | Qty:%-6d\n",
                products[index].productID,
                products[index].productName,
                products[index].productPrice,
                products[index].productQuantity
            );

            printf("\nEnter new Quantity: ");
            char product_quantity[10];
            fgets(product_quantity, sizeof(product_quantity), stdin);

            getValidProductQuantity(numberOfProduct, product_quantity, sizeof(product_quantity), products, index);
            printf("Quantity updated successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("\nNo such product ID exists.");
    }
}

void searchProductById(const int numberOfProduct, const ProductDetails *Product) 
{
    printf("\nEnter Product ID to search: ");
    int product_id = getInputIdToSearch();
    for (int index = 0; index < numberOfProduct; index++) 
    {
        if (Product[index].productID == product_id) {
            printf("ID:%d | Name:%s | Price:%.2f | Qty:%d\n",
                   Product[index].productID, Product[index].productName, 
                   Product[index].productPrice, Product[index].productQuantity
            );
            return;
        }
    }
    printf("No such product ID exists.\n");
}

void searchProductByName(int numberOfProduct, const ProductDetails *Product) 
{
    char productName[60];
    printf("\nEnter name to search (partial allowed) : ");
    fgets( productName , sizeof(productName) , stdin);
    getValidProductName(numberOfProduct, productName, sizeof(productName));

    bool found=false;
    for( int index=0 ; index<numberOfProduct ; index++)
    {
        if(strstr( Product[index].productName ,productName  ) !=NULL )
        {
            found = true;
            printf("Product ID:%-6d | Name:%-15s | Price:%-8.2f | Qty:%-6d\n",
                   Product[index].productID, Product[index].productName, 
                   Product[index].productPrice, Product[index].productQuantity
            );
        }
    }
    
    if (!found) 
    {
        printf("No products found with this name.\n");
    }
}

void searchProductByPriceRange(int numberOfProduct, const ProductDetails *Product) 
{
    char minimumPrice[20];
    char maximumPrice[20];
    float minPrice = 0.0f;
    float maxPrice = 0.0f;

    while (1) 
    {
        printf("\nEnter minimum price: ");
        fgets(minimumPrice, sizeof(minimumPrice), stdin);
        minimumPrice[strcspn(minimumPrice, "\n")] = '\0';

        if (isEmptyorSpaces(minimumPrice)) 
        {
            printf("Input cannot be empty or spaces only!\n");
            continue;
        }

        if (!isFloatNumber(minimumPrice)) 
        {
            printf("Please enter a valid numeric value!\n");
            continue;
        }

        minPrice = atof(minimumPrice);
        if (minPrice < 0 || minPrice > 100000) 
        {
            printf("Price should be between 0 and 100000!\n");
            continue;
        }
        break;
    }

    while (1) 
    {
        printf("Enter maximum price: ");
        fgets(maximumPrice, sizeof(maximumPrice), stdin);
        maximumPrice[strcspn(maximumPrice, "\n")] = '\0';

        if (isEmptyorSpaces(maximumPrice)) 
        {
            printf("Input cannot be empty or spaces only!\n");
            continue;
        }

        if (!isFloatNumber(maximumPrice)) 
        {
            printf("Please enter a valid numeric value!\n");
            continue;
        }

        maxPrice = atof(maximumPrice);
        if (maxPrice < 0 || maxPrice > 100000 || maxPrice < minPrice) 
        {
            printf("Maximum price must be >= minimum price\n");
            continue;
        }
        break;
    }

    bool found = false;
    for (int i = 0; i < numberOfProduct; i++) 
    {
        if (Product[i].productPrice >= minPrice && Product[i].productPrice <= maxPrice) 
        {
            printf("ID:%d | Name:%s | Price:%.2f | Qty:%d\n",
                   Product[i].productID,
                   Product[i].productName,
                   Product[i].productPrice,
                   Product[i].productQuantity
            );
            found = true;
        }
    }

    if (!found) 
    {
        printf("No products in this price range.\n");
    }
}

void addNewProduct(int* numberOfProduct , ProductDetails **productInventory )
{
    int oldCount = *numberOfProduct;
    int newProductCount =  oldCount + 1;

    ProductDetails *newProduct = realloc( *productInventory , newProductCount * sizeof(ProductDetails));
    if( newProduct == NULL)
    {
        printf("\n Memory Allocation for new product failed");
        return ;
    }

    *productInventory = newProduct;
    printf("\nEnter details for new product:\n");

    char product_ID[10];
    char product_Price[10];
    char product_Quantity[7];

    printf("Product ID: ");
    fgets(product_ID, sizeof(product_ID), stdin);
    getValidID(oldCount, product_ID, *productInventory, sizeof(product_ID));
    (*productInventory)[newProductCount - 1].productID = atoi(product_ID);


    printf("Product Name: ");
    fgets((*productInventory)[newProductCount - 1].productName, sizeof((*productInventory)[newProductCount - 1].productName), stdin);
    getValidProductName(oldCount, (*productInventory)[newProductCount - 1].productName, sizeof((*productInventory)[newProductCount - 1].productName));

    printf("Product Price: ");
    fgets(product_Price, sizeof(product_Price), stdin);
    getValidProductPrice(oldCount, product_Price, sizeof(product_Price), *productInventory, newProductCount - 1);

    printf("Product Quantity: ");
    fgets(product_Quantity, sizeof(product_Quantity), stdin);
    getValidProductQuantity(oldCount, product_Quantity, sizeof(product_Quantity), *productInventory, newProductCount - 1);

    *numberOfProduct = newProductCount;
    printf("Product added successfully!\n");
}

void deleteProduct( int *numberOfProduct , ProductDetails **productInventory )
{
    printf("\nEnter Product ID to delete: ");
    int inputId = getInputIdToSearch();
    int found = -1;

    for (int index = 0; index < *numberOfProduct; index++)
    {
        if ((*productInventory)[index].productID == inputId)
        {
            found = index; 
            break;
        }
    }

    if(found == -1)
    {
        printf("\n No Product Exist with this id");
        return;
    }
         
    for (int index = found; index < *numberOfProduct - 1; index++)
    {
        (*productInventory)[index] = (*productInventory)[index + 1];
    }

    (*numberOfProduct)--;

    ProductDetails *newProduct = realloc(*productInventory, (*numberOfProduct) * sizeof(ProductDetails));
    if( newProduct == NULL  && *numberOfProduct > 0 )
    {
        printf("\n Memory Reallocation failed , keeping old array");
        return ;
    }

    if (*numberOfProduct == 0) 
    {
        free(*productInventory);
        *productInventory = NULL;
        printf("All products deleted.\n");
        return;
    }

    *productInventory = newProduct;
    printf("Product deleted successfully!\n");
}
