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


bool isCommandCorrect ( FileNode **cwd,  char *inputCommand);
bool isValidName( const char *name);

void directoryInitialiser( FileNode **dir , char name_of_directory[]);
FileNode* fileInitialiser(FileNode *cwd , char name_of_file[]);

FileNode* makeDirectory ( FileNode *cwd , char argument[100]);
FileNode* removeDirectory(FileNode *cwd , char argument[100]);

void listAll(FileNode *cwd);
FileNode* changeDirectoryToParent ( FileNode *cwd );
FileNode* changeDirectory ( FileNode *cwd , char directory_name[]);
void printWorkingDirectory(FileNode *cwd);

void displayDiskUsage();
void freeAllNodes(FileNode *node);
void exitFunction(FileNode *cwd);

void writeToFile(FileNode *cwd, char fileName[] , char fileContent[]);
void read(FileNode *cwd , char fileName[]);
void delete(FileNode *cwd ,char fileName[]);

int main()
{
    printf("\n$ ./vfs ");
    printf("\nCompact VFS - ready. Type 'exit' to quit. ");

    FileNode *root = NULL;
    directoryInitialiser( &root , "/");
    FileNode *cwd = root;

    char command[100];
    while(1)
    {
        printf("\n%s >",cwd->name);
        fgets( command , sizeof(command) , stdin);
        if (!strchr(command, '\n')) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        command[ strcspn( command , "\n")] = '\0';
    
        if( !isCommandCorrect( &cwd , command))
        {
            printf("Enter a valid command\n");
        }

    }
    return 0;
}

bool isValidName( const char *name)
{
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

bool isCommandCorrect ( FileNode **cwd,  char *inputCommand)
{
    char command[50];
    char argument[100];
    command[0]='\0';
    argument[0]='\0'; 

    sscanf(inputCommand, "%49s %99[0-9a-zA-Z._@/-]" , command , argument  );

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
            FileNode* newDir = makeDirectory( *cwd , argument);
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
            *cwd = removeDirectory( *cwd , argument);
            return true;
        }
    }
    //3. cd
    if( strcmp(command , "cd") == 0 )
    {
        if (strcmp(argument, "..") == 0 || strlen(argument) == 0)
        {
            *cwd = changeDirectoryToParent(*cwd);
        }
        else    
        {
            if (!isValidName(argument))
            {
                printf(" Invalid Characters in arguments \n");
            }
            *cwd = changeDirectory( *cwd , argument);
        }
        return true;
    }

    //4. ls
    if( strcmp(command , "ls") == 0 && strlen(argument)==0 )
    {
        listAll(*cwd);
        return true;
    }
    //5. pwd
    if( strcmp(command , "pwd") == 0 && strlen(argument)==0 )
    {
        printWorkingDirectory(*cwd);
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
        exitFunction(*cwd);
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
            FileNode *newFile = fileInitialiser(*cwd, argument);
            if (newFile != NULL)
                printf("File '%s' created successfully.\n", newFile->name);
        }
        return true;
    }

    //9. write 
    if( strcmp(command , "write") == 0 )
    {
        char fileName[NAME_SIZE] = {0};
        char fileContent [BLOCK_SIZE]={0};

        if (sscanf(inputCommand, "write %49s \"%511[^\"]\"", fileName, fileContent) != 2)
        {
            printf("Usage : write <file-name> \"<content>\"\n");
            return true;
        }
        else if (!isValidName(fileName))
        {
            printf(" Invalid Characters in fileName \n");
            return true;
        }
           
        writeToFile(*cwd, fileName, fileContent);
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
            read( *cwd  , argument);
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
            delete( *cwd , argument);
            return true;
        }
    }

    return false;
}


void directoryInitialiser( FileNode **dir , char name_of_directory[])
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

    for (int i = 0; i < SIZE_FOR_BLOCK_POINTERS; i++)
    {
        directory->blockPointers[i] = -1;
    }

    *dir = directory;
}

FileNode* fileInitialiser(FileNode *cwd , char name_of_file[])
{
    FileNode* child = cwd->child;
    
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

    strcpy( file->name , name_of_file);
    file->isDirectory = false;
    file->parent = cwd; 
    file->child = NULL;  
    file->contentSize = 0;
    file->next = NULL;
    file->prev = NULL;
    
    for( int i=0 ; i<SIZE_FOR_BLOCK_POINTERS ; i++)
    {
        file->blockPointers[i]= -1;
    }

    if( cwd->child == NULL)
    {
        cwd->child = file;
        file->prev = file;
        file->next = file;
    }
    else
    {
        FileNode* head = cwd->child;
        FileNode* tail = head->prev;

        tail->next = file;
        file->prev = tail;
        file->next = head;
        head->prev = file;
    }

    return file;
}

FileNode* makeDirectory ( FileNode *cwd , char argument[100]  )
{
    FileNode *childNode = cwd->child ;
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
        } while ( childNode != cwd->child) ;
    } 

    FileNode *newDirectory = malloc( sizeof( FileNode ));
    if( newDirectory == NULL )
    {
        printf("\nMemory  allocation failed ");
        return NULL;
    }

    strcpy(newDirectory->name , argument);
    newDirectory->isDirectory = true;
    newDirectory->parent = cwd;
    newDirectory->child = NULL;

    if( cwd->child == NULL) 
    {
        newDirectory->prev = newDirectory;
        newDirectory->next = newDirectory;
        cwd->child = newDirectory;
    }
    else
    {
        FileNode *head = cwd->child;
        FileNode *tail = head->prev;

        tail->next = newDirectory;
        newDirectory->prev = tail;
        newDirectory->next = head;
        head->prev = newDirectory;
    }
return newDirectory;
}


void listAll(FileNode *cwd)
{
    FileNode *childNode = cwd->child ;
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
        } while ( childNode != cwd->child) ;
    } 
}

FileNode* changeDirectoryToParent ( FileNode *cwd )
{
    if( strcmp(cwd->name , "/")==0)
    {
        printf("Already at root directory\n");
        return cwd;
    }
    else
    {
        cwd = cwd->parent;
        printf("Moved to /%s", cwd->name);
        printf("\n");
        return cwd;
    }
}

FileNode* changeDirectory ( FileNode *cwd , char directory_name[])
{
    FileNode* directoryToSearch = cwd->child;
    if (directoryToSearch == NULL)
    {
        printf("No subdirectories under '%s'\n", cwd->name);
        return cwd;
    }

    FileNode* head = directoryToSearch;
    do
    {
        if( directoryToSearch->isDirectory && strcmp(directoryToSearch->name, directory_name)==0 )
        {
            cwd = directoryToSearch;
            printf("\n Moved to /%s ", cwd->name);
            printf("\n");
            return cwd;
        }
            
        directoryToSearch = directoryToSearch->next;
    } while ( directoryToSearch != head) ;

    printf("Directory '%s' not found under '%s'\n", directory_name, cwd->name);
    return cwd;
}

void printWorkingDirectory(FileNode *cwd)
{
    char path[100][NAME_SIZE];
    int count = 0;

    FileNode* currentDirectory = cwd;
    
    while( currentDirectory != NULL)
    {   
       strcpy(path[count], currentDirectory->name);
       count++;
       currentDirectory = currentDirectory->parent;
    }

    printf("\n");
    for (int index = count - 1; index >= 0; index--)
    {
        if (strcmp(path[index], "/") == 0)
            printf("/");
        else
            printf("%s/", path[index]);
    }
    printf("\n");
}

FileNode* removeDirectory(FileNode *cwd , char argument[100])
{
    if( cwd->child == NULL) 
    {
        printf("No subdirectories under %s directory\n ",cwd->name);
        return cwd;
    }

    FileNode* current = cwd->child;
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
        printf("Directory '%s' not found under '%s'\n",argument, cwd->name);
        return cwd;
    }   
    
    if( directoryToRemove->child != NULL) 
    {
        printf("The %s directory is not empty , can't be deleted\n",directoryToRemove->name);
        return cwd;
    }
 
    if( directoryToRemove->next == directoryToRemove && directoryToRemove->prev == directoryToRemove)
    {
        cwd->child = NULL;
    }
    else 
    {
        directoryToRemove->prev->next = directoryToRemove->next;
        directoryToRemove->next->prev = directoryToRemove->prev;

        if( cwd->child == directoryToRemove) 
        {
            cwd->child = directoryToRemove->next;
        }
    }

    free(directoryToRemove);
    printf("Directory '%s' removed successfully.\n", argument);
    directoryToRemove = NULL;
    return cwd;
}

void displayDiskUsage()
{
    int totalBlocks = NUMBER_OF_BLOCKS;
    int usedBlocks = 0;
    int freeBlocks = 0;

    for (int index = 0; index < NUMBER_OF_BLOCKS; index++)
    {
        if (blockUsed[index])
        {
            usedBlocks++;
        }
        else
        {
            freeBlocks++;
        }
    }

    printf("Total Blocks : %d\n", totalBlocks);
    printf("Used Blocks  : %d\n", usedBlocks);
    printf("Free Blocks  : %d\n", freeBlocks);
    printf("Disk Usage   : %.2f %% \n", (float)(usedBlocks) / totalBlocks * 100);
}   

void writeToFile(FileNode *cwd, char fileName[] , char fileContent[])
{
    if( cwd->child == NULL) 
    {
        printf("No files under %s directory\n ",cwd->name);
        return;
    }

    FileNode* fileToWrite = NULL;
    FileNode* child = cwd->child;
    FileNode* head = child;
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
        printf("File'%s' not found under '%s'\n",fileName, cwd->name);
        return;
    }   

    int contentSize = strlen(fileContent);
    int requiredBlocks = (contentSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if( requiredBlocks > SIZE_FOR_BLOCK_POINTERS)
    {
        printf("Error: File too large. Maximum %d blocks per file.\n", SIZE_FOR_BLOCK_POINTERS);
        return;
    }

    int freeSpaceCount = 0;
    for( int index=0 ; index<NUMBER_OF_BLOCKS ; index++)
    {
        if( !blockUsed[index])
        {
            freeSpaceCount++;
        }
    }

    if( freeSpaceCount < requiredBlocks)
    {
        printf("Error: Disk full! Not enough free space to write file.\n");
        return;
    }
    
    
    for (int index = 0; index< SIZE_FOR_BLOCK_POINTERS; index++)
    {
        if (fileToWrite->blockPointers[index] != -1)
        {
            int oldBlock = fileToWrite->blockPointers[index];
            blockUsed[oldBlock] = false;
            memset(VIRTUAL_DISK[oldBlock], 0, BLOCK_SIZE);
            fileToWrite->blockPointers[index] = -1;
        }
    }

    int written = 0;
    for (int index = 0, block = 0; index < NUMBER_OF_BLOCKS && block < requiredBlocks; index++)
    {
        if (!blockUsed[index])
        {
            blockUsed[index] = true;
            fileToWrite->blockPointers[block] = index;

            int remaining = contentSize - written;
            int toWrite = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;

            memcpy(VIRTUAL_DISK[index], fileContent + written, toWrite);

            written += toWrite;
            block++;
        }
    }

    fileToWrite->contentSize = contentSize;

    printf("Data written successfully (size=%d bytes).\n", contentSize);
}

void read(FileNode *cwd , char fileName[])
{
    if( cwd->child == NULL )
    {
        printf("No file in this directory\n");
        return;
    }

    FileNode* fileToRead = NULL;
    FileNode* child = cwd->child;
    FileNode* head = child;

    do
    {
        if( !(child->isDirectory) && strcmp( child->name , fileName) ==0 )
        {
            fileToRead = child;
            break;
        }
        child = child->next;
    } while (head != child );
    

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

    char contentOfFile[fileToRead->contentSize + 1];
    int bytesRead = 0;

    for (int index = 0; index < SIZE_FOR_BLOCK_POINTERS && bytesRead < fileToRead->contentSize; index++)
    {
        int blockIndex = fileToRead->blockPointers[index];
        if (blockIndex == -1)
        {
            break;
        }

        int remaining = fileToRead->contentSize - bytesRead;
        int toRead = (remaining > BLOCK_SIZE) ? BLOCK_SIZE : remaining;

        memcpy(contentOfFile + bytesRead, VIRTUAL_DISK[blockIndex], toRead);
        bytesRead += toRead;

    }

    contentOfFile[bytesRead] = '\0'; 
    printf("%s\n", contentOfFile);
}

void delete(FileNode *cwd ,char fileName[])
{
    if( cwd->child == NULL )
    {
        printf("No file in this directory\n");
        return;
    }

    FileNode* fileToDelete = NULL;
    FileNode* child = cwd->child;
    FileNode* head = child;

    do
    {
        if( !(child->isDirectory) && strcmp( child->name , fileName) ==0 )
        {
            fileToDelete = child;
            break;
        }
        child = child->next;
    } while (head != child );
    

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
            fileToDelete->blockPointers[index] = -1;
        }
    }

    if (fileToDelete->next == fileToDelete && fileToDelete->prev == fileToDelete)
    {
        cwd->child = NULL;
    }
    else
    {
        fileToDelete->prev->next = fileToDelete->next;
        fileToDelete->next->prev = fileToDelete->prev;

        if (cwd->child == fileToDelete)
        {
            cwd->child = fileToDelete->next;
        }
    }

    free(fileToDelete);
    fileToDelete = NULL;

    printf("File '%s' deleted successfully.\n", fileName);
}

void freeAllNodes(FileNode *node)
{
    if (node == NULL)
    {
        return;
    }

    if (node->child != NULL)
    {
        FileNode *child = node->child;
        FileNode *head = child;

        do
        {
            FileNode *next = child->next;
            freeAllNodes(child);
            child = next;
        } while (child != head);
    }

    free(node);
}

void exitFunction(FileNode *cwd)
{
    while (cwd->parent != NULL)
    {
        cwd = cwd->parent;
    }

    freeAllNodes(cwd);

    for (int index = 0; index < NUMBER_OF_BLOCKS; index++)
    {
        memset(VIRTUAL_DISK[index], 0, BLOCK_SIZE);
        blockUsed[index] = false;
    }

    printf("Memory released. Exiting program...\n");
    exit(0);
}
