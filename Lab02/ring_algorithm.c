#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int processId;
    int status;        // 1 if active, 0 if failed
    int isCoordinator; // 1 if coordinator, 0 otherwise
    struct Process *next;
};

struct Process *head = NULL;

void displayProcesses()
{
    struct Process *temp = head;

    printf("| %-14s | %-14s | %-14s |\n", "Process Number", "Status", "Coordinator");
    printf("|---------------|---------------|---------------|\n");

    while (temp != NULL)
    {
        printf("| %-14d | %-14s | %-14s |\n", temp->processId, (temp->status == 1) ? "Active" : "Failed",
               (temp->isCoordinator == 1) ? "Yes" : "No");
        temp = temp->next;
    }
}

void startElection()
{
    if (head == NULL)
    {
        printf("No processes available.\n");
        return;
    }

    int startingProcessId;
    printf("Enter the process ID to start the election: ");
    scanf("%d", &startingProcessId);

    struct Process *current = head;
    while (current != NULL && current->processId != startingProcessId)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Process not found.\n");
        return;
    }

    printf("Election started by Process %d\n", startingProcessId);

    int initiatorId = startingProcessId;
    int coordinatorId = startingProcessId;
    current = current->next;
    while (current != NULL && current->processId != initiatorId)
    {
        printf("Process %d sends election message to Process %d\n", initiatorId, current->processId);

        if (current->processId > coordinatorId)
        {
            printf("Process %d acknowledges and becomes the new coordinator\n", current->processId);
            coordinatorId = current->processId;
        }
        else
        {
            printf("Process %d ignores the election message\n", current->processId);
        }

        current = current->next;
    }
    current = head;
    while (current != NULL)
    {
        if (current->processId == coordinatorId)
        {
            current->isCoordinator = 1;
        }
        else
        {
            current->isCoordinator = 0;
        }
        current = current->next;
    }

    printf("Process %d is the final coordinator.\n", coordinatorId);
}

void failProcess()
{
    if (head == NULL)
    {
        printf("No processes available.\n");
        return;
    }

    int processToFail;
    printf("Enter the process ID to fail: ");
    scanf("%d", &processToFail);

    struct Process *current = head, *prev = NULL;
    while (current != NULL && current->processId != processToFail)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Process not found.\n");
        return;
    }

    if (prev == NULL)
    {
        head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    current->status = 0;

    free(current);
    printf("Process %d failed.\n", processToFail);
}

int main()
{
    int choice;

    do
    {
        printf("\nMenu:\n");
        printf("1. Enter number of processes\n");
        printf("2. Start election\n");
        printf("3. Display active processes\n");
        printf("4. Fail a process\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int numProcesses;
            printf("Enter the number of processes: ");
            scanf("%d", &numProcesses);

            for (int i = numProcesses; i >= 1; --i)
            {
                struct Process *newProcess = (struct Process *)malloc(sizeof(struct Process));
                newProcess->processId = i;
                newProcess->status = 1;        // 1 for active
                newProcess->isCoordinator = 0; // 0 for not coordinator
                newProcess->next = head;
                head = newProcess;
            }

            int coordinatorId = rand() % numProcesses + 1;
            printf("Process %d is randomly selected as the coordinator.\n", coordinatorId);

            struct Process *current = head;
            while (current != NULL)
            {
                if (current->processId == coordinatorId)
                {
                    current->isCoordinator = 1;
                }
                current = current->next;
            }

            printf("%d processes added.\n", numProcesses);
            break;
        }
        case 2:
            startElection();
            break;
        case 3:
            displayProcesses();
            break;
        case 4:
            failProcess();
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    struct Process *current = head;
    while (current != NULL)
    {
        struct Process *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
