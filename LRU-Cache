#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#define VALUE_SIZE 50
#define MAX_SIZE_HASHMAP 1000
#define MAX_DATA_SIZE 50

int totalSizeQueue = 0;
int cacheCapacity = 0;


typedef struct Node
{
    int key ; 
    char data[VALUE_SIZE];

    struct Node* previousNode;
    struct Node* next;

    struct Node* chainingNext;
}Node;


Node** hashmap;

Node* mostRecentlyUsed = NULL;
Node* leastRecentlyUsed = NULL;

void createCache(int capacity);
char* getKey( int key );
void putKey( int key ,char *data);

void menuPrint(void);
bool isCommandCorrect( char *inputCommand);

void putFunction(int key , char* inputData);
void getFunction(int key);
void exitFunction(void);
bool capacityValid(int inputCapacity); 
bool inputValidation( char input[] ,  int inputSize);
bool isEmptyorSpaces(const char input[]) ;
bool isNumber(const char input[]);
void clearInputBuffer();

int hashFunction(int key);
void moveToFront(Node *nodeToMove);
void insertAtFront(Node *nodeToInsert);
Node* removeFromLRU();
void removeFromHashmap(int key);

int main()
{
    
    char capacityInput[50];
    int capacity;

    printf("\nEnter cache capacity : ");
    while (1)
    {
        fgets( capacityInput , sizeof(capacityInput) , stdin);

        if (!strchr(capacityInput, '\n'))
        {
            clearInputBuffer();   
        }

        capacityInput[strcspn(capacityInput, "\n")] = '\0';

        if( !inputValidation(capacityInput , sizeof(capacityInput)))
        {
            continue;
        }

        capacity = atoi(capacityInput);
            
        if( !capacityValid(capacity))
        {
            printf("Invalid capacity! Enter integer between 1 and 1000.\n");
            continue;
        }

        createCache(capacity);
        break;
    }


    menuPrint();
    char command[200];
    while(1)
    {
        fgets( command , sizeof(command) , stdin);

        if (!strchr(command, '\n')) 
        {
            int indexChar;
            while ((indexChar = getchar()) != '\n' && indexChar!= EOF);
        }

        command[ strcspn( command , "\n")] = '\0';
    
        if( !isCommandCorrect(command))
        {
            printf("\nEnter a valid command : \n");
        }
    }
}

void createCache(int capacity)
{
    cacheCapacity = capacity;
    hashmap = malloc( MAX_SIZE_HASHMAP * sizeof( Node* ));
    if( hashmap == NULL)
    {
        printf("\n Memory Allocation Failed");
        return;
    }

    for( int index=0 ; index<MAX_SIZE_HASHMAP ; index++)
    {
        hashmap[index] = NULL;
    }

    mostRecentlyUsed = NULL;
    leastRecentlyUsed = NULL;
    printf("Cache created successfully !");
}

int hashFunction(int key)
{
    return key % MAX_SIZE_HASHMAP;
}

void insertAtFront(Node *nodeToInsert)
{
    if( mostRecentlyUsed == NULL )
    {
        nodeToInsert->previousNode = NULL;
        nodeToInsert->next = NULL;
        
        mostRecentlyUsed = nodeToInsert;
        leastRecentlyUsed = nodeToInsert;
    }
    else
    {
        nodeToInsert->previousNode = NULL;
        nodeToInsert->next = mostRecentlyUsed ; 

        mostRecentlyUsed->previousNode = nodeToInsert;
        mostRecentlyUsed = nodeToInsert;
    }
}

Node* removeFromLRU()
{
    if( leastRecentlyUsed == NULL)
    {
        printf("\n cache empty !");
        return NULL;
    }

    Node* nodeToDelete = leastRecentlyUsed; 

    if (mostRecentlyUsed == leastRecentlyUsed)
    {
        mostRecentlyUsed = leastRecentlyUsed = NULL;
    }

    else
    {
        leastRecentlyUsed = leastRecentlyUsed->previousNode;
        leastRecentlyUsed->next = NULL;
    }

    totalSizeQueue--;

    return nodeToDelete;
}

void removeFromHashmap(int key)
{
    int index = hashFunction(key);

    Node* currentNode = hashmap[index];
    Node* previousNode = NULL;

    while( currentNode != NULL )
    {
        if( currentNode->key == key )
        {
            Node* toDelete = currentNode;

            if( previousNode == NULL )
            {
                hashmap[index] = currentNode->chainingNext;
            }
            else
            {
                previousNode->chainingNext = currentNode->chainingNext;
            }
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->chainingNext;
    }
}

void moveToFront(Node *nodeToMove)
{
    if (nodeToMove == mostRecentlyUsed) 
    {
        return;
    }

    if (nodeToMove->previousNode) 
    {
        nodeToMove->previousNode->next = nodeToMove->next;
    }
    if (nodeToMove->next) 
    {
        nodeToMove->next->previousNode = nodeToMove->previousNode;
    }

    if (nodeToMove == leastRecentlyUsed)
    {
        leastRecentlyUsed = leastRecentlyUsed->previousNode;
        if (leastRecentlyUsed) 
        {
            leastRecentlyUsed->next = NULL;
        }
    }

    nodeToMove->next = mostRecentlyUsed;
    nodeToMove->previousNode = NULL;

    mostRecentlyUsed->previousNode = nodeToMove;
    mostRecentlyUsed = nodeToMove;
}

void putKey( int key ,char *data)
{
    int indexInMap = hashFunction(key);

    Node* currentNode = hashmap[indexInMap];
    while( currentNode != NULL )
    {
       if (currentNode->key == key)
        {
            strcpy(currentNode->data, data);
            moveToFront(currentNode);
            return;
        }
       currentNode = currentNode->chainingNext;
    }

    Node* newNode = malloc ( sizeof(Node ));
    if( newNode == NULL)
    {
        printf("\n Memory allocation failed");
        return;
    }
    newNode->key = key;
    strcpy(newNode->data , data);
    newNode->previousNode = NULL;
    newNode->next = NULL;
    newNode->chainingNext = NULL;

    newNode->chainingNext = hashmap[indexInMap];
    hashmap[indexInMap] = newNode;

    insertAtFront(newNode);

    totalSizeQueue++;

    if (totalSizeQueue > cacheCapacity)
    {
        Node* nodeToDelete = removeFromLRU();
        if (nodeToDelete != NULL)
        {
            removeFromHashmap(nodeToDelete->key);
            free(nodeToDelete);
            nodeToDelete = NULL;
        }
    }
}

char* getKey( int key )
{
    int index = hashFunction(key);

    Node *currentNode = hashmap[index];

    if( currentNode == NULL)
    {
        return "NULL";
    }

    while( currentNode != NULL )
    {
        if( currentNode->key == key )
        {
            moveToFront(currentNode);
            return currentNode->data;
        }
    
        currentNode = currentNode->chainingNext;
    }
    
    return "NULL";
}

void menuPrint(void)
{
    printf("\n__________________________________________________");
    printf("\nCommands allowed: ");
    printf("\nput <key> <data>");
    printf("\nget <key>");
    printf("\nexit");
    printf("\n__________________________________________________\n");
}

bool isCommandCorrect( char *inputCommand)
{
    char command[50];
    char argument[100];
    command[0]='\0';
    argument[0]='\0'; 

    while (isspace(*inputCommand)) 
    {   
        inputCommand++;
    }

    int count = sscanf(inputCommand, "%49s %99[^\n]" , command , argument);

    if (count <= 0) 
    {
        return false;
    }

    if (strcmp(command, "put") == 0)
    {
        int key;
        char value[MAX_DATA_SIZE];
        char extra;

        int count = sscanf(argument, "%d %49s %c", &key, value, &extra);

        if (count == 2)
        {
            if (key < 0)
            {
                printf("Key must be non-negative.\n");
                return true;
            }
            putFunction(key, value);
            return true;
        }
        else if (count == 1)
        {
            printf("Missing value.\nUsage: put <key> <value>\n");
        }
        else if (count == 3)
        {
            printf("Value cannot contain spaces.\n");
        }
        return true;
    }

    else if (strcmp(command, "get") == 0)
    {
        int key;
        char extra;

        int count = sscanf(argument, "%d %c", &key, &extra);

        if (count == 1)
        {
            getFunction(key);
            return true;
        }
        else if (count == 0)
        {
            printf("Missing key.\nUsage: get <key>\n");
        }
        else
        {
            printf("Invalid arguments.\nUsage: get <key>\n");
        }
        return true;
    }

    else if (strcmp(command, "exit") == 0) 
    {
        if (argument[0] == '\0')
        {
            exitFunction();
            return true;
        }
        return false;
    }

    return false;
}

void putFunction(int key , char* inputData)
{
    if( key < 0)
    {
        printf("Key not valid");
        return;
    }

    putKey(key , inputData);
}

void getFunction( int key)
{
    if( key < 0)
    {
        printf("Key not valid");
        return;
    }

    char* value = getKey(key);
    printf("%s\n", value);
}

void exitFunction(void)
{
    Node *currentNode = mostRecentlyUsed;
    while (currentNode != NULL)
    {
        Node *nodeToDelete = currentNode;
        currentNode = currentNode->next;
        free(nodeToDelete);
    }

    free(hashmap);
    hashmap = NULL;
    mostRecentlyUsed = NULL;
    leastRecentlyUsed = NULL;

    printf("\nexiting !!\n");
    exit(0);
}

bool capacityValid(int inputCapacity)
{
    if (inputCapacity >= 1 && inputCapacity <= 1000)
    {
        return true;
    }

    return false;
}

bool inputValidation( char input[] ,  int inputSize)
{
    if (isEmptyorSpaces(input))
    {
        printf("Input cannot be empty!\n");
        printf("Enter again : ");
        return false;
    }

    if (!isNumber(input))
    {
        printf("Invalid input! Enter a valid number \n");
        printf("Enter again : ");
        return false;
    }

    return true;
    
}


bool isNumber(const char input[]) 
{
    int index=0 ;   
    while( input[index] && isspace((unsigned char) input[index]))
    {
        index++;
    }
    if( input[index] && !isdigit((unsigned char) input[index]))
    {
        return false;
    }
    while( input[index] && isdigit((unsigned char) input[index]))
    {
        index++;
    }
    while( input[index] && isspace((unsigned char) input[index]))
    {
        index++;
    }
    return input[index] == '\0'; 
}

bool isEmptyorSpaces(const char input[]) 
{
    if (input[0] == '\0') 
    {
        return true;
    }
     for(int index=0 ; input[index] != '\0' ; index++)
    {
        if(!isspace(input[index]))
        {  
            return false; 
        }
    }
    return true;
}

void clearInputBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}
