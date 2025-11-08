#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

#define NUMBER_OF_BLOCKS 1024
#define BLOCK_SIZE 512
char VIRTUAL_DISK[NUMBER_OF_BLOCKS][BLOCK_SIZE];
#define NAME_SIZE 50
#define SIZE_FOR_BLOCK_POINTERS 10

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
void fileInitialiser(FileNode **inputFile , char name_of_file[]);

FileNode* makeDirectory ( FileNode *cwd , char argument[100]);

void removeDirectory();

void listAll(FileNode *cwd);
FileNode* changeDirectoryToParent ( FileNode *cwd );
FileNode* changeDirectory ( FileNode *cwd , char directory_name[]);
void printWorkingDirectory(FileNode *cwd);

void df();
void exitFunction();

void create();
void write();
void read();
void delete();

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

    sscanf(inputCommand, "%49s %99[ 0-9a-zA-Z._@/- ]" , command , argument  );

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
            // rmdir(argument);
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
        // df();
        return true;
    }
    //7. exit
    if( strcmp(command , "exit") == 0 && strlen(argument)==0)
    {
        // exitFunction();
        exit(1);
        return true;
    }

    //8. create
    if( strcmp(command , "create") == 0 )
    {
        if( strlen(argument)==0)
        {
            printf("Usage : create <file-name>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            // create(argument);
            return true;
        }
    }
    //9. write : needs more arguments for content ( need buffer)
    if( strcmp(command , "write") == 0 )
    {
        if( strlen(argument)==0)
        {
            printf("Usage : write <file-name><content>\n");
        }
        else if (!isValidName(argument))
        {
            printf(" Invalid Characters in arguments \n");
        }
        else
        {
            // write(argument);
            return true;
        }
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
            // read(argument);
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
            // delete(argument);
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

void fileInitialiser(FileNode **inputFile , char name_of_file[])
{
    FileNode *file = malloc( sizeof(FileNode));
    if( file == NULL)
    {
        printf("\nMemory Allocation Failed");
        exit(1);
    }
    strcpy( file->name , name_of_file);
    file->isDirectory = false;
    file->parent = NULL; 
    file->child = NULL; 
    file->next = NULL; 
    file->prev = NULL; 
    file->contentSize = 0;
    
    for( int i=0 ; i<SIZE_FOR_BLOCK_POINTERS ; i++)
    {
        file->blockPointers[i]= -1;
    }

    *inputFile = file;
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
            printf(" %s/\n",childNode->name);
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
