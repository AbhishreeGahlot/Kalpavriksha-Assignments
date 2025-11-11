#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#define NUMBER_OF_BLOCKS 1024
#define BLOCK_SIZE 512
#define NAME_SIZE 50
#define SIZE_FOR_BLOCK_POINTERS 10

char VIRTUAL_DISK[NUMBER_OF_BLOCKS][BLOCK_SIZE];
bool blockUsed[NUMBER_OF_BLOCKS]={false};

int totalBlocks = 1024;
int usedBlocks = 0;

typedef struct FileNode 
{
    char name[NAME_SIZE];
    bool isDirectory ; 
    struct FileNode *parent; 
    struct FileNode *child; 

    struct FileNode *prev; 
    struct FileNode *next;

    int blockPointers[SIZE_FOR_BLOCK_POINTERS]; 
    int contentSize ;
}FileNode;

typedef struct FreeBlock  
{
    int index; 
    struct FreeBlock *prev;
    struct FreeBlock *next;
}FreeBlock;

FreeBlock *freeBlockHead = NULL;
FreeBlock *freeBlockTail = NULL;

bool isCommandCorrect ( FileNode **current_working_directory,  char *inputCommand);
bool isValidName( const char *name);
void processEscapesCharacter(char *inputString);

void directoryInitialiser( FileNode **directory_ptr , char name_of_directory[]);
FileNode* fileInitialiser(FileNode *current_working_directory , char name_of_file[]);
void initializeFreeBlockList(void);

FileNode* makeDirectory ( FileNode *current_working_directory , char argument[100]);
FileNode* removeDirectory(FileNode *current_working_directory , char argument[100]);

void listAll(FileNode *current_working_directory);
FileNode* changeDirectoryToParent ( FileNode *current_working_directory );
FileNode* changeDirectory ( FileNode *current_working_directory , char directory_name[]);
void printWorkingDirectory(FileNode *current_working_directory);

void displayDiskUsage();
void freeAllNodes(FileNode *node);
void exitFunction(FileNode *current_working_directory);

void writeToFile(FileNode *current_working_directory, char *fileName, char *fileContent);
void read(FileNode *current_working_directory , char fileName[]);
void delete(FileNode *current_working_directory ,char fileName[]);

int main()
{
    printf("\n$ ./vfs ");
    printf("\nCompact VFS - ready. Type 'exit' to quit. ");

    FileNode *root = NULL;
    initializeFreeBlockList();
    directoryInitialiser( &root , "/");
    FileNode *current_working_directory = root;

    char command[100];
    while(1)
    {
        printf("\n%s >",current_working_directory->name);
        fgets( command , sizeof(command) , stdin);
        if (!strchr(command, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        command[ strcspn( command , "\n")] = '\0';
    
        if( !isCommandCorrect( &current_working_directory , command))
        {
            printf("Enter a valid command\n");
        }

    }
    return 0;
}

bool isValidName( const char *name)
{
    if (strlen(name) > NAME_SIZE - 1)  
    {
        printf("Error: Name too long (max %d characters allowed)\n", NAME_SIZE - 1);
        return false;
    }

    for( int index =0 ; name[index] != '\0' ; index++)
    {
        char indexCharacter =  name[index];
        if( ! ( isalnum( indexCharacter ) ||  indexCharacter== '.' || indexCharacter== '/' ||
                indexCharacter== '@' || indexCharacter== ' ' || indexCharacter== '_' || indexCharacter=='-' ) )
        {
            return false;  
        }
    }

    return true;
}

bool isCommandCorrect ( FileNode **current_working_directory,  char *inputCommand)
{
    char command[50];
    char argument[100];
    command[0]='\0';
    argument[0]='\0'; 

    while (isspace(*inputCommand)) 
    {   
        inputCommand++;
    }

    sscanf(inputCommand, "%49s %99[^\n]" , command , argument);

    //1. mkdir
    if( strcmp( command , "mkdir") == 0 )
    {
        if( strlen(argument)==0)
        {
            printf("Usage : mdkir <directory-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            FileNode* newDir = makeDirectory( *current_working_directory , argument);
            if(newDir != NULL)
            {
                printf("Directory '%s' created successfully.\n" , newDir->name);
            }
            return true;
        }
    }
    //2. rmdir
    if( strcmp(command , "rmdir") == 0 )
    {
        if( strlen(argument)==0)
        {
            printf("Usage : rmdir <directory-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            *current_working_directory = removeDirectory( *current_working_directory , argument);
            return true;
        }
    }
    //3. cd
    if( strcmp(command , "cd") == 0 )
    {
        if (strcmp(argument, "..") == 0 || strlen(argument) == 0)
        {
            *current_working_directory = changeDirectoryToParent(*current_working_directory);
        }
        else    
        {
            if (!isValidName(argument))
            {
                printf(" Invalid Characters in arguments \n");
            }
            *current_working_directory = changeDirectory( *current_working_directory , argument);
        }
        return true;
    }

    //4. ls
    if( strcmp(command , "ls") == 0 && strlen(argument)==0 )
    {
        listAll(*current_working_directory);
        return true;
    }
    //5. pwd
    if( strcmp(command , "pwd") == 0 && strlen(argument)==0 )
    {
        printWorkingDirectory(*current_working_directory);
        return true;
    }
    //6. df
    if( strcmp(command , "df") == 0  && strlen(argument)==0)
    {
        displayDiskUsage();
        return true;
    }
    //7. exit
    if( strcmp(command , "exit") == 0 && strlen(argument)==0)
    {
        exitFunction(*current_working_directory);
        return true;
    }

    //8. create
    if (strcmp(command, "create") == 0)
    {
        char extra_argument[100] = {0};
        if (sscanf(inputCommand, "%49s %49s %99s", command, argument, extra_argument) == 3)
        {
            printf("Usage : create <file-name>\n");
            return true;
        }

        if (strlen(argument) == 0)
        {
            printf("Usage : create <file-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf("Invalid Characters in arguments\n");
        }
        else
        {
            FileNode *newFile = fileInitialiser(*current_working_directory, argument);
            if (newFile != NULL)
                printf("File '%s' created successfully.\n", newFile->name);
        }
        return true;
    }

    //9. write 
    if (strcmp(command, "write") == 0)
    {
        char fileName[NAME_SIZE] = {0};
        char fileContent[BLOCK_SIZE] = {0};

        sscanf(inputCommand, "write %49s", fileName);

        const char *start = strchr(inputCommand, '"');
        if (start)
        {
            start++;
            const char *end = strchr(start, '"');
            if (end)
            {
                size_t length = end - start;
                strncpy(fileContent, start, length);
                fileContent[length] = '\0';
            }
        }

        if (strlen(fileName) == 0 || strlen(fileContent) == 0)
        {
            printf("Usage : write <file-name> \"<content>\"\n");
            return true;
        }

        if (!isValidName(fileName))
        {
            printf(" Invalid Characters in fileName \n");
            return true;
        }

        processEscapesCharacter(fileContent);
        writeToFile(*current_working_directory, fileName, fileContent);
        return true;
    }
    //10. read
    if( strcmp(command , "read") == 0 )
    {
        if( strlen(argument)==0)
        {
            printf("Usage : read <file-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            read( *current_working_directory  , argument);
            return true;
        }
    }
    //11. delete
    if( strcmp(command , "delete") == 0 )
    {
       if( strlen(argument)==0)
        {
            printf("Usage : delete <file-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            delete( *current_working_directory , argument);
            return true;
        }
    }

    return false;
}


void directoryInitialiser( FileNode **directory_ptr , char name_of_directory[])
{
    FileNode *directory = malloc( sizeof( FileNode ) );
    if( directory == NULL )
    {
        printf("\nMemory Allocation Failed");
        exit(1);
    }
    strcpy(directory->name  , name_of_directory);
    directory->isDirectory = true;
    directory->parent = NULL; 
    directory->child = NULL; 
    directory->prev = directory;  
    directory->next = directory; 
    directory->contentSize = 0;

    for (int index = 0; index < SIZE_FOR_BLOCK_POINTERS; index++)
    {
        directory->blockPointers[index] = -1;
    }

    *directory_ptr= directory;
}

FileNode* fileInitialiser(FileNode *current_working_directory , char name_of_file[])
{
    FileNode* child = current_working_directory->child;
    
    if( child != NULL)
    {
        FileNode* head = child;
        do
        {
            if( strcmp(child->name, name_of_file ) ==0)
            {
                printf("File or Directory with this name , already exist! \n");
                return NULL;
            }
            child = child->next;
        } while (child != head);  
    }

    FileNode *file = malloc( sizeof(FileNode));
    if( file == NULL)
    {
        printf("\nMemory Allocation Failed");
        return NULL;
    }

    
    strncpy(file->name ,  name_of_file, NAME_SIZE - 1);
    file->name[NAME_SIZE - 1] = '\0';
    file->isDirectory = false;
    file->parent = current_working_directory; 
    file->child = NULL;  
    file->contentSize = 0;
    file->next = NULL;
    file->prev = NULL;
    
    for( int i=0 ; i<SIZE_FOR_BLOCK_POINTERS ; i++)
    {
        file->blockPointers[i]= -1;
    }

    if( current_working_directory->child == NULL)
    {
        current_working_directory->child = file;
        file->prev = file;
        file->next = file;
    }
    else
    {
        FileNode* head = current_working_directory->child;
        FileNode* tail = head->prev;

        tail->next = file;
        file->prev = tail;
        file->next = head;
        head->prev = file;
    }

    return file;
}

FileNode* makeDirectory ( FileNode *current_working_directory , char argument[100]  )
{
    FileNode *childNode = current_working_directory->child ;
    if( childNode != NULL)
    {
        do
        {
            if( strcmp(childNode->name , argument)==0 )
            {
                printf("Directory with this name already exist !\n");
                return NULL;
            }
            childNode = childNode->next;
        } while ( childNode != current_working_directory->child) ;
    } 

    FileNode *newDirectory = malloc( sizeof( FileNode ));
    if( newDirectory == NULL )
    {
        printf("\nMemory  allocation failed ");
        return NULL;
    }

    strncpy(newDirectory->name, argument, NAME_SIZE - 1);
    newDirectory->name[NAME_SIZE - 1] = '\0';
    newDirectory->isDirectory = true;
    newDirectory->parent = current_working_directory;
    newDirectory->child = NULL;

    if( current_working_directory->child == NULL) 
    {
        newDirectory->prev = newDirectory;
        newDirectory->next = newDirectory;
        current_working_directory->child = newDirectory;
    }
    else
    {
        FileNode *head = current_working_directory->child;
        FileNode *tail = head->prev;

        tail->next = newDirectory;
        newDirectory->prev = tail;
        newDirectory->next = head;
        head->prev = newDirectory;
    }
return newDirectory;
}


void listAll(FileNode *current_working_directory)
{
    FileNode *childNode = current_working_directory->child ;
    if( childNode == NULL)
    {
        printf("Directory is empty!\n");
        return;
    }
    else
    {
        do
        {
            if(childNode->isDirectory)
            {
                printf(" %s/\n",childNode->name);
            }
            else
            {
                printf(" %s\n",childNode->name);
            }
            childNode = childNode->next;
        } while ( childNode != current_working_directory->child) ;
    } 
}

FileNode* changeDirectoryToParent ( FileNode *current_working_directory )
{
    if( strcmp(current_working_directory->name , "/")==0)
    {
        printf("Already at root directory\n");
        return current_working_directory;
    }
    else
    {
        current_working_directory = current_working_directory->parent;
        printf("Moved to '%s'\n", current_working_directory->name);
        printf("\n");
        return current_working_directory;
    }
}

FileNode* changeDirectory ( FileNode *current_working_directory , char directory_name[])
{
    FileNode* directoryToSearch = current_working_directory->child;
    if (directoryToSearch == NULL)
    {
        printf("No subdirectories under '%s'\n", current_working_directory->name);
        return current_working_directory;
    }

    FileNode* head = directoryToSearch;
    do
    {
        if( directoryToSearch->isDirectory && strcmp(directoryToSearch->name, directory_name)==0 )
        {
            current_working_directory = directoryToSearch;
            printf("\n Moved to /%s ", current_working_directory->name);
            printf("\n");
            return current_working_directory;
        }
            
        directoryToSearch = directoryToSearch->next;
    } while ( directoryToSearch != head) ;

    printf("Directory '%s' not found under '%s'\n", directory_name, current_working_directory->name);
    return current_working_directory;
}

void printWorkingDirectory(FileNode *current_working_directory)
{
    if (!current_working_directory)
    {
        return;
    }

    if (current_working_directory->parent)
    {
        printWorkingDirectory(current_working_directory->parent);
        if (strcmp(current_working_directory->name, "/") != 0)
        {
            printf("%s/", current_working_directory->name);
        }
    }

    else
    {
        printf("/");
    }
}

FileNode* removeDirectory(FileNode *current_working_directory , char argument[100])
{
    if( current_working_directory->child == NULL) 
    {
        printf("No subdirectories under %s directory\n ",current_working_directory->name);
        return current_working_directory;
    }

    FileNode* current = current_working_directory->child;
    FileNode* head = current;
    FileNode* directoryToRemove = NULL;

    do
    {
        if( current->isDirectory && strcmp(current->name, argument)==0 )
        {
            directoryToRemove = current;
            break;
        }
        current = current->next;
    } while ( current != head) ;

    if( directoryToRemove == NULL ) 
    {
        printf("Directory '%s' not found under '%s'\n",argument, current_working_directory->name);
        return current_working_directory;
    }   
    
    if( directoryToRemove->child != NULL) 
    {
        printf("The %s directory is not empty , can't be deleted\n",directoryToRemove->name);
        return current_working_directory;
    }
 
    if( directoryToRemove->next == directoryToRemove && directoryToRemove->prev == directoryToRemove)
    {
        current_working_directory->child = NULL;
    }
    else 
    {
        directoryToRemove->prev->next = directoryToRemove->next;
        directoryToRemove->next->prev = directoryToRemove->prev;

        if( current_working_directory->child == directoryToRemove) 
        {
            current_working_directory->child = directoryToRemove->next;
        }
    }

    free(directoryToRemove);
    printf("Directory '%s' removed successfully.\n", argument);
    directoryToRemove = NULL;
    return current_working_directory;
}

void displayDiskUsage()
{
    int freeBlocks = totalBlocks - usedBlocks;

    printf("Total Blocks : %d\n", totalBlocks);
    printf("Used Blocks  : %d\n", usedBlocks);
    printf("Free Blocks  : %d\n", freeBlocks);
    float usage = (totalBlocks > 0) ? ((float)usedBlocks / totalBlocks * 100) : 0.0f;
    printf("Disk Usage   : %.2f %% \n",usage);
}   

void writeToFile(FileNode *current_working_directory, char *fileName, char *fileContent) 
{
    if( current_working_directory->child == NULL) 
    {
        printf("No files under %s directory\n ",current_working_directory->name);
        return;
    }

    FileNode* fileToWrite = NULL;
    FileNode* head = current_working_directory->child;
    FileNode* child = head;
    
    do
    {
        if ( strcmp( child->name , fileName) == 0)
        {
            fileToWrite = child;
            break;
        }
        child = child->next;
    } while ( child != head);
    
    if( fileToWrite == NULL ) 
    {
        printf("File'%s' not found under '%s'\n",fileName, current_working_directory->name);
        return;
    }   

    if (fileToWrite->isDirectory) 
    {
        printf("Cannot write to a directory.\n");
        return;
    }

    int contentSize = strlen(fileContent);
    int requiredBlocks = (contentSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (requiredBlocks > SIZE_FOR_BLOCK_POINTERS) 
    {
        printf("File too large. Maximum size is %d bytes.\n", SIZE_FOR_BLOCK_POINTERS * BLOCK_SIZE);
        return;
    }

    int freeBlocks = 0;
    FreeBlock *tempCount = freeBlockHead;
    while (tempCount) 
    {
        freeBlocks++;
        tempCount = tempCount->next;
    }

    if (freeBlocks < requiredBlocks) 
    {
        printf("Not enough disk space to write file.\n");
        return;
    }
    
    int allocated = 0;
    FreeBlock *temp = freeBlockHead;
    while (temp && allocated < requiredBlocks) 
    {
        int blockIndex = temp->index;
        blockUsed[blockIndex] = true;
        usedBlocks++;
        fileToWrite->blockPointers[allocated++] = blockIndex;

        freeBlockHead = temp->next;
        if (freeBlockHead) 
        {
            freeBlockHead->prev = NULL;
        }
        else 
        {
            freeBlockTail = NULL;
        }

        FreeBlock *toFree = temp;
        temp = temp->next;
        free(toFree);
    }

    int written = 0;
    for (int index = 0; index < allocated; index++) 
    {
        int blockIndex = fileToWrite->blockPointers[index];
        int remaining = contentSize - written;
        int toWrite = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
        
        memcpy(VIRTUAL_DISK[blockIndex], fileContent + written, toWrite);
        written += toWrite;
    }

    fileToWrite->contentSize = contentSize;

    printf("Content written to file '%s'. (%d bytes)\n", fileName, contentSize);
}

void read(FileNode *current_working_directory , char fileName[])
{
    if( current_working_directory->child == NULL )
    {
        printf("No file in this directory\n");
        return;
    }

    FileNode* fileToRead = NULL;
    FileNode* head = current_working_directory->child;
    FileNode* child = head;

    do
    {
        if( !(child->isDirectory) && strcmp( child->name , fileName) ==0 )
        {
            fileToRead = child;
            break;
        }
        child = child->next;
    } while (child != head );
    

    if( fileToRead == NULL)
    {
        printf("No %s file exist in the directory \n",fileName);
        return;
    }

    if (fileToRead->contentSize == 0)
    {
        printf("File '%s' is empty.\n", fileName);
        return;
    }

    printf("File content (%d bytes):\n", fileToRead->contentSize);

    int remaining = fileToRead->contentSize;
    for (int index = 0; index < SIZE_FOR_BLOCK_POINTERS && remaining > 0; index++) 
    {
        int blockIndex = fileToRead->blockPointers[index];
        if (blockIndex == -1) break;

        int toRead = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;
        fwrite(VIRTUAL_DISK[blockIndex], 1, toRead, stdout);
        remaining -= toRead;
    }

    printf("\n"); 
}

void delete(FileNode *current_working_directory ,char fileName[])
{
    if( current_working_directory->child == NULL )
    {
        printf("No file in this directory\n");
        return;
    }

    FileNode* fileToDelete = NULL;
    FileNode* head = current_working_directory->child;
    FileNode* child = head;

    do
    {
        if( !(child->isDirectory) && strcmp( child->name , fileName) ==0 )
        {
            fileToDelete = child;
            break;
        }
        child = child->next;
    } while (child != head);
    

    if( fileToDelete == NULL)
    {
        printf("No %s file exist in the directory \n",fileName);
        return;
    }

    for (int index = 0; index < SIZE_FOR_BLOCK_POINTERS; index++) 
    {
        int blockIndex = fileToDelete->blockPointers[index];
        if (blockIndex != -1) 
        {
            blockUsed[blockIndex] = false;
            memset(VIRTUAL_DISK[blockIndex], 0, BLOCK_SIZE);

            FreeBlock *newBlock = malloc(sizeof(FreeBlock));
            newBlock->index = blockIndex;
            newBlock->next = NULL;
            newBlock->prev = freeBlockTail;
            if (freeBlockTail)
                freeBlockTail->next = newBlock;
            else
                freeBlockHead = newBlock;
            freeBlockTail = newBlock;

            fileToDelete->blockPointers[index] = -1;

            if (usedBlocks > 0)
            {
                usedBlocks--;
            }
        }
    }


    if (fileToDelete->next == fileToDelete && fileToDelete->prev == fileToDelete)
    {
        current_working_directory->child = NULL;
    }
    else
    {
        fileToDelete->prev->next = fileToDelete->next;
        fileToDelete->next->prev = fileToDelete->prev;

        if (current_working_directory->child == fileToDelete)
        {
            current_working_directory->child = fileToDelete->next;
        }
    }

    free(fileToDelete);
    fileToDelete = NULL;

    printf("File '%s' deleted successfully.\n", fileName);
}

void freeAllNodes(FileNode *node)
{
    if (node == NULL)
        return;

    FileNode *child = node->child;
    if (child)
    {
        FileNode *start = child;
        do
        {
            FileNode *next = child->next;
            freeAllNodes(child);
            child = (next == start) ? NULL : next;
        } while (child);
    }

    if (!node->isDirectory)
    {
        for (int index = 0; index < SIZE_FOR_BLOCK_POINTERS; index++)
        {
            int idx = node->blockPointers[index];
            if (idx != -1)
            {
                blockUsed[idx] = false;
                if (usedBlocks > 0)
                {
                    usedBlocks--;
                }
            }
        }
    }

    free(node);
}


void exitFunction(FileNode *current_working_directory)
{
    while (current_working_directory->parent != NULL)
    {
        current_working_directory = current_working_directory->parent;
    }

    freeAllNodes(current_working_directory);

    FreeBlock *temp;
    while (freeBlockHead)
    {
        temp = freeBlockHead;
        freeBlockHead = freeBlockHead->next;
        free(temp);
    }

    printf("Memory released. Exiting program...\n");
    exit(0);
}


void processEscapesCharacter(char *inputString)
{
    if (inputString == NULL) 
    {
        return;
    }

    char *source = inputString;
    char *destination = inputString;

    while (*source)
    {
        if (*source == '\\' && *(source + 1) == 'n')
        {
            *destination++ = '\n';
            source += 2; 
        }
        else
        {
            *destination++ = *source++;
        }
    }

    *destination = '\0';
}

void initializeFreeBlockList() {
    for (int index = 0; index < NUMBER_OF_BLOCKS; index++) 
    {
        FreeBlock *newBlock = malloc(sizeof(FreeBlock));
        newBlock->index = index;
        newBlock->prev = freeBlockTail;
        newBlock->next = NULL;
        if (freeBlockTail)
            freeBlockTail->next = newBlock;
        else
            freeBlockHead = newBlock;
        freeBlockTail = newBlock;
        blockUsed[index] = false;
    }

    totalBlocks = NUMBER_OF_BLOCKS;
    usedBlocks = 0;
}
