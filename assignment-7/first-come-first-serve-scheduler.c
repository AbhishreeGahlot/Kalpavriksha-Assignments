/*
After entering the process Details and kill command 
press : cntrl + z + enter 
to see the o/p
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_MAP_SIZE 1009
#define MAX_NAME_LENGTH 64
#define MAX_PROCESS 1000
#define MAX_EVENTS 1000

typedef enum 
{
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

typedef struct PCB 
{
    char name[MAX_NAME_LENGTH];
    int processID;

    int cpu_burst;
    int cpu_remaining;

    int io_start;
    int io_duration;
    int io_remaining;

    int executed_time;
    int completion_time;
    int io_time_total;

    int killed;
    int killed_time;

    State state;
    struct PCB *next;
} PCB;

typedef struct Queue 
{
    PCB *front;
    PCB *rear;
} Queue;

typedef struct HashMapNode 
{
    int key;
    PCB *process;
    struct HashMapNode *next;
} HashMapNode;

typedef struct KillEvent 
{
    int processID;
    int time;
} KillEvent;

HashMapNode *hashMap[HASH_MAP_SIZE];
Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;
PCB *processList[MAX_PROCESS];
KillEvent killList[MAX_EVENTS];

int processCount = 0;
int killCount = 0;
int totalProcesses = 0;


int hashFunction(int key);
void hashPut(int key, PCB *process);
PCB *hashGet(int key);

void initQueue(Queue *queue);
void enqueue(Queue *queue, PCB *process);
PCB *dequeue(Queue *queue);
PCB *removeFromQueue(Queue *queue, int processId);

PCB *createPCB(const char *name, int processId, int burst, int io_start, int io_duration);

void cleanString(char *inputString);
void inputParsing(char *inputLine);

void decrementIOQueue();
void checkIOCompletion();
void terminatePCB(PCB *process, int time);

void applyKillEvents(int time, PCB **running, int *terminated);

void simulate();
int compareProcessID(const void *a_string, const void *b_string);
void printResults();


int main() {
    initQueue(&readyQueue);
    initQueue(&waitingQueue);
    initQueue(&terminatedQueue);

    printf("Commands:");    
    printf("\n  <process_name> <pid> <burst_time> <io_start_time> <io_duration>");
    printf("\n  KILL <PID> <kill_time>");
    printf("\n");
    char line[256];
    while (fgets(line, sizeof(line), stdin)) 
    {
        inputParsing(line);
    }

    if (totalProcesses == 0) 
    {
        printf("No processes entered.\n");
        return 0;
    }

    simulate();
    printResults();
    return 0;
}


void cleanString(char *inputString) 
{
    int i = 0 ;
    int j = 0;
    while (isspace((unsigned char)inputString[i])) 
    {
        i++;
    }
    while (inputString[i]) 
    {
        inputString[j++] = inputString[i++];
    }
    
    inputString[j] = '\0';
    while (j > 0 && isspace((unsigned char)inputString[j - 1])) 
    {
        inputString[--j] = '\0';
    }
}

void inputParsing(char *line) 
{
    cleanString(line);
    if (strlen(line) == 0)
    {
        return;
    }

    char first[16];
    sscanf(line, "%15s", first);

    if (strcasecmp(first, "KILL") == 0) 
    {
        int processId, t;
        if (sscanf(line + strlen(first), "%d %d", &processId, &t) == 2) 
        {
            killList[killCount++] = (KillEvent){processId, t};
        }
        return;
    }

    char name[MAX_NAME_LENGTH];
    char io1[16] = "-", io2[16] = "-";
    int processId, burst;

    int n = sscanf(line, "%63s %d %d %15s %15s",
                   name, &processId, &burst, io1, io2
            );

    if (n < 3) 
    {
        return;
    }
    if (hashGet(processId)) 
    {
        return;
    }

    int io_start = (strcmp(io1, "-") ? atoi(io1) : -1);
    int io_duration   = (strcmp(io2, "-") ? atoi(io2) : 0);

    PCB *process = createPCB(name, processId, burst, io_start, io_duration);
    if (!process) 
    {
        return;
    }

    processList[processCount++] = process;
    totalProcesses++;

    hashPut(processId, process);
    enqueue(&readyQueue, process);
}

int hashFunction(int key) 
{
    int h = key % HASH_MAP_SIZE;
    return h < 0 ? h + HASH_MAP_SIZE : h;
}

void hashPut(int key, PCB *process) 
{
    int idx = hashFunction(key);
    HashMapNode *node = malloc(sizeof(HashMapNode));
    node->key = key;
    node->process = process;
    node->next = hashMap[idx];
    hashMap[idx] = node;
}

PCB *hashGet(int key) 
{
    int idx = hashFunction(key);
    for (HashMapNode *node = hashMap[idx]; node; node = node->next)
        if (node->key == key)
        {
            return node->process;
        }
    return NULL;
}


void initQueue(Queue *queue)
{
    queue->front = queue->rear = NULL;
}

void enqueue(Queue *queue, PCB *process) 
{
    process->next = NULL;
    if (!queue->rear)
    {
        queue->front = queue->rear = process;
    }
    else 
    {
        queue->rear->next = process;
        queue->rear = process;
    }
}

PCB *dequeue(Queue *queue) 
{
    if (!queue->front) 
    {
        return NULL;
    }
    PCB *process = queue->front;
    queue->front = process->next;
    if (!queue->front) 
    {
        queue->rear = NULL;
    }
    process->next = NULL;
    return process;
}

PCB *removeFromQueue(Queue *queue, int processId) 
{
    PCB *current = queue->front;
    PCB *previous = NULL;

    while (current) 
    {
        if (current->processID == processId) 
        {
            if (previous) 
            {
                previous->next = current->next;
            }
            else 
            {
                queue->front = current->next;
            }

            if (current == queue->rear)
            {
                queue->rear = previous;
            }
            current->next = NULL;
            return current;
        }
        previous = current;
        current = current->next;
    }

    return NULL;
}


PCB *createPCB(const char *name, int processId, int burst, int io_start, int io_duration) 
{
    PCB *process = malloc(sizeof(PCB));

    strncpy(process->name, name, MAX_NAME_LENGTH);

    process->name[MAX_NAME_LENGTH - 1] = '\0';

    process->processID = processId;
    process->cpu_burst = burst;
    process->cpu_remaining = burst;

    process->io_start = io_start;
    process->io_duration = io_duration;

    process->io_remaining = 0;
    process->io_time_total = io_duration;

    process->executed_time = 0;
    process->completion_time = -1;

    process->killed = 0;
    process->killed_time = -1;

    process->state = READY;
    process->next = NULL;

    return process;
}


void decrementIOQueue() 
{
    PCB *current = waitingQueue.front;
    while (current) 
    {
        if (current->io_remaining > 0)
        {
            current->io_remaining--;
        }

        current = current->next;
    }
}

void checkIOCompletion() 
{
    PCB *current = waitingQueue.front;
    PCB *prev = NULL;

    while (current) 
    {
        PCB *next = current->next;

        if (current->io_remaining == 0) 
        {
            if (prev) 
            {
                prev->next = current->next;
            }
            else 
            {
                waitingQueue.front = current->next;
            }

            if (current == waitingQueue.rear)
            {
                waitingQueue.rear = prev;
            }

            current->state = READY;

            enqueue(&readyQueue, current);
        } 
        else 
        {
            prev = current;
        }

        current = next;
    }
}


void terminatePCB(PCB *process, int time) 
{
    process->state = TERMINATED;
    process->completion_time = time;
    enqueue(&terminatedQueue, process);
}


void applyKillEvents(int time, PCB **running, int *terminated) 
{
    for (int index = 0; index < killCount; index++) 
    {
        if (killList[index].time != time)
        {
            continue;
        }

        int processId = killList[index].processID;
        PCB *process = hashGet(processId);

        if (!process || process->state == TERMINATED)
        {
            continue;
        }

        if (*running && (*running)->processID == processId) 
        {
            process->killed = 1;
            process->killed_time = time;

            terminatePCB(process, time);

            *running = NULL;
            (*terminated)++;

            continue;
        }

        PCB *processToRemove = removeFromQueue(&readyQueue, processId);
        if (!processToRemove) 
        {
            processToRemove = removeFromQueue(&waitingQueue, processId);
        }

        if (processToRemove) 
        {
            processToRemove->killed = 1;
            processToRemove->killed_time = time;

            terminatePCB(processToRemove, time);

            (*terminated)++;
        }
    }
}


void simulate() {
    int time = 0;
    int done = 0;

    PCB *running = NULL;

    while (done < totalProcesses) 
    {
        applyKillEvents(time, &running, &done);

        if (!running) 
        {
            running = dequeue(&readyQueue);
            if (running)
            {
                running->state = RUNNING;
            }
        }

        if (running) 
        {
            running->executed_time++;
            running->cpu_remaining--;
        }

        decrementIOQueue();

        if (running) 
        {
            if (running->cpu_remaining > 0 &&
                running->io_start >= 0 &&
                running->executed_time == running->io_start &&
                running->io_duration > 0) 
            {

                running->io_remaining = running->io_duration;
                running->state = WAITING;
                enqueue(&waitingQueue, running);
                running = NULL;
            }
            else if (running->cpu_remaining == 0) 
            {
                terminatePCB(running, time + 1);
                running = NULL;
                done++;
            }
        }

        checkIOCompletion();
        time++;
    }
}


int compareProcessID(const void *a_string, const void *b_string)
{
    PCB **process_A = (PCB **)a_string;   
    PCB **process_B = (PCB **)b_string;   

    PCB *first = *process_A;       
    PCB *second = *process_B;

    return first->processID - second->processID;
}

void printResults() 
{
    PCB *processArray[MAX_PROCESS];

    for (int index = 0; index < processCount; index++)
    {
        processArray[index] = processList[index];
    }

    qsort(processArray, processCount, sizeof(PCB *), compareProcessID);

    printf("\n%-6s %-12s %-6s %-6s %-16s %-12s %-8s\n",
           "PID", "Name", "CPU", "IO", "Status", "Turnaround", "Waiting");

    for (int index = 0; index < processCount; index++) 
    {
        PCB *process = processArray[index];

        if (process->killed) {
            printf("%-6d %-12s %-6d %-6d %-16s %-12s %-8s\n",
                   process->processID, process->name, process->cpu_burst, process->io_time_total,
                   "KILLED", "-", "-");
        } 
        else 
        {
            int totalAroundTime = process->completion_time;
            int wait = totalAroundTime - process->cpu_burst;

            printf("%-6d %-12s %-6d %-6d %-16s %-12d %-8d\n",
                   process->processID, process->name, process->cpu_burst, process->io_time_total,
                   "OK", totalAroundTime, wait
            );
        }
    }

    printf("\n");
}
