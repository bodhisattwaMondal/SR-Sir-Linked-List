#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ----------------------------- Structure(s) -----------------------------
struct Log
{
    int time;
    struct Name *userName;
    struct Log *nextLog;
};

struct Name
{
    char *fullName;
    struct Name *nextName;
};

// ----------------------------- Functions -----------------------------
// ----------- Insertion -----------
struct Name *insertName(struct Name *nameHead, char *name)
{
    /*
    Create Name node and will return
    */

    // Creating Name node
    struct Name *newNameNode = (struct Name *)malloc(sizeof(struct Name));
    newNameNode->fullName = (char *)malloc(sizeof(char) * 20);
    strcpy(newNameNode->fullName, name); // putting fullName to Name node

    // linking newNode
    if (nameHead == NULL) // if empty Name LL - inserting new node at first
    {
        newNameNode->nextName = nameHead; // nameHead is NULL (here)
        nameHead = newNameNode;
    }
    else
    {
        struct Name *temptr = nameHead;
        // Traversing the LL
        while (temptr->nextName != NULL)
            temptr = temptr->nextName;
        // inserting new node at last
        temptr->nextName = newNameNode;
        newNameNode->nextName = NULL;
    }
    return nameHead;
}

void insertLog(struct Log **logHead, int time, char *name)
{
    /*
    Create Log node and will insert in the Log LL.

    Insertion will'be sorted in ascending order of the time.

    Check node with same time already exists or not,
    if found it's corresponding Name LL will get appended.
    */

    // Creating Log node
    struct Log *newLogNode = (struct Log *)malloc(sizeof(struct Log));
    // inserting time
    newLogNode->time = time;
    // inserting userName
    struct Name *nameHead = NULL;
    newLogNode->userName = insertName(nameHead, name);

    // if empty Log LL - inserting new node at first
    if (*logHead == NULL)
    {
        newLogNode->nextLog = *logHead;
        *logHead = newLogNode;
    }
    else
    {
        // inserting at first bcz new time is greater than logHead time
        if ((*logHead)->time > time)
        {
            newLogNode->nextLog = *logHead;
            *logHead = newLogNode;
        }
        else
        {
            struct Log *temptr = *logHead;
            // traversing the Log LL
            while (temptr != NULL)
            {
                // if multiple userName join at the same time
                if (temptr->time == time)
                {
                    // getting the nameHead of of the name LL joined with that log LL
                    struct Name *nameHead = temptr->userName;
                    // appending the name LL with the new userName
                    temptr->userName = insertName(nameHead, name);
                    break;
                }
                // appending the new log node at the last
                else if (temptr->nextLog == NULL)
                {
                    newLogNode->nextLog = temptr->nextLog; // temptr->nextLog is NULL here
                    temptr->nextLog = newLogNode;
                    break;
                }
                else
                {
                    if (temptr->nextLog->time > time)
                    {
                        newLogNode->nextLog = temptr->nextLog;
                        temptr->nextLog = newLogNode;
                        break;
                    }
                    temptr = temptr->nextLog;
                }
            }
        }
    }
}

// ----------- Display -----------
void display(struct Log *logPtr)
{
    printf("\nOUTPUT\n");
    if (logPtr == NULL)
        printf("Empty Log\n");
    else
    {
        // Traversing the Log Node(s)
        while (logPtr != NULL)
        {
            printf("%d -> ", logPtr->time);
            struct Name *namePtr = logPtr->userName;
            // Traversing the Name Node(s) of above Log Node
            while (namePtr != NULL)
            {
                printf("%s ", namePtr->fullName);
                namePtr = namePtr->nextName;
            }
            logPtr = logPtr->nextLog;
            printf("\n");
        }
    }
}

// ----------------------------- Update 2022-12-25 onwards  -----------------------------
// ----------- Deletion -----------
void dltLog(struct Log **logHead, int time)
{
    /*
    Delete Log node based on the given time value.
    If no node with the time found - specify.
    */
    int flag = 0;
    if (*logHead == NULL)
        printf("Can't dlt. - The log is empty");
    else
    {
        // if the node to be dlt is the head node itself
        if ((*logHead)->time == time)
        {
            *logHead = (*logHead)->nextLog;
            flag = 1;
        }
        else
        {
            struct Log *temptr = *logHead;
            // Traversing the log node until the given time is found
            while (temptr->nextLog != NULL)
            {
                // if the node lies anywhere in b/w head and last node
                if (temptr->nextLog->time == time)
                {
                    temptr->nextLog = (temptr->nextLog)->nextLog;
                    flag = 1;
                    return;
                }

                // if the node to be dlt is the last node
                else if (((temptr->nextLog)->nextLog == NULL) && (temptr->nextLog->time == time))
                {
                    temptr->nextLog = NULL;
                    flag = 1;
                    return;
                }

                // else loop proceed
                else
                    temptr = temptr->nextLog;
            }
        }
    }
    if (flag == 0)
        printf("\nCan't dlt - No Log entry found with the time %d.\n", time);
    return;
}

void dltNameElement(struct Log **logHead, char *name)
{
    /*
    Delete Name element based on userName,

    If only one name element is present and it matches,
    then dlt it's corresponding Log node.

    If no entry found specify
    */
    int flag = 0;
    if (*logHead == NULL)
        printf("Can't dlt - Empty log !!");

    else
    {
        struct Log *logPtr = *logHead;
        // Traversing the log node
        while (logPtr != NULL)
        {
            // Getting the head pointer of the linked Name LL
            struct Name *namePtr = logPtr->userName;
            // Traversing the Name LL
            while (namePtr->nextName != NULL)
            {
                // * NOTE: strcmp returns 0 if Strings are same
                // Case 1: if the name node to be dlt is the head node itself
                if (strcmp(name, namePtr->fullName) == 0 && namePtr == logPtr->userName)
                // NOTE : Here, logPtr->userName is carrying ref of the head node of name LL
                {
                    flag = 1;
                    logPtr->userName = namePtr->nextName;
                }

                // Case 2: if the name node lies anywhere in b/w head and last node
                else if (strcmp(name, namePtr->nextName->fullName) == 0)
                {
                    flag = 1;
                    namePtr->nextName = (namePtr->nextName)->nextName;
                }

                // Case 3: if the name node to be dlt is the last node
                else if ((strcmp(name, namePtr->nextName->fullName) == 0) && (namePtr->nextName)->nextName == NULL)
                {
                    flag = 1;
                    namePtr->nextName = NULL;
                }

                else
                    // else loop proceed
                    namePtr = namePtr->nextName;
            }

            // Exclusive Case :
            // if only one name node is present under current log node and it's fullName matches
            if ((strcmp(name, namePtr->fullName) == 0) && namePtr->nextName == NULL)
            {
                flag = 1;
                dltLog(logHead, logPtr->time);
            }

            // Move to nxt log node
            logPtr = logPtr->nextLog;
        }
    }
    if (flag == 0)
        printf("\nCan't dlt - No entry found by the name %s!!\n", name);

    return;
}

// ----------- Searching -----------
void searchWhoCameAtTime(struct Log *logHead, int time)
{
    /*
    userName(s) joined at given time
    */
    if (logHead == NULL)
        printf("Can't Search - Empty Log\n");
    else
    {
        // if require name of logHead itself
        if (logHead->time == time)
        {
            struct Name *namePtr = logHead->userName;
            printf("\nFollowing username(s) joined at %d:\n", time);
            while (namePtr != NULL)
            {
                printf("%s ", namePtr->fullName);
                namePtr = namePtr->nextName;
            }
            printf("\n");
            return;
        }
        else
        {
            // traverse the log LL
            while (logHead != NULL)
            {
                if (logHead->time == time)
                {
                    struct Name *namePtr = logHead->userName;
                    printf("\nFollowing username(s) joined at %d:\n", time);
                    while (namePtr != NULL)
                    {
                        printf("%s ", namePtr->fullName);
                        namePtr = namePtr->nextName;
                    }
                    printf("\n");
                    return;
                }
                else
                    logHead = logHead->nextLog;
            }
        }
    }
    printf("No log entry with time %d found !!\n");
}

void searchHowManyCameAtTime(struct Log *logHead, int time)
{
    if (logHead == NULL)
        printf("Can't Search - Empty Log\n");
    else
    {
        int count = 0;
        // if require count of logHead itself
        if (logHead->time == time)
        {
            struct Name *namePtr = logHead->userName;
            while (namePtr != NULL)
            {
                count += 1;
                namePtr = namePtr->nextName;
            }
        }
        else
        {
            // traverse the log LL
            while (logHead != NULL)
            {
                if (logHead->time == time)
                {
                    struct Name *namePtr = logHead->userName;
                    while (namePtr != NULL)
                    {
                        count += 1;
                        namePtr = namePtr->nextName;
                    }
                    break;
                }
                else
                    logHead = logHead->nextLog;
            }
        }
        printf("\nNo. of username(s) joined at %d: %d\n", logHead->time, count);
        return;
    }
    printf("No log entry with time %d found !!\n");
}

void searchWhoFirstAtTime(struct Log *logHead, int time)
{
    if (logHead == NULL)
        printf("Can't Search - Empty Log!!\n");
    else
    {
        if (logHead->time == time){
            // Getting the name LL of head Log and printing the first UserName
            printf("\n%s joined first at %d\n", logHead->userName->fullName, logHead->time);
            return;
        }
        else
        {
            while (logHead != NULL)
            {
                if (logHead->time == time)
                {
                    // Getting the name LL of head Log and printing the first UserName
                    printf("\n%s joined first at %d\n", logHead->userName->fullName, logHead->time);
                    return;
                }
                else
                    logHead = logHead->nextLog;
            }
        }
    }
    printf("No log entry with time %d found !!\n", time);
}

void searchWhoLastAtTime(struct Log *logHead, int time)
{
    if (logHead == NULL)
        printf("Can't Search - Empty Log!!\n");
    else
    {
        if (logHead->time == time)
        {
            // Getting the name LL of head Log
            struct Name *nameHead = logHead->userName;
            // Traversing to the last name of the name LL
            while (nameHead->nextName != NULL)
                nameHead = nameHead->nextName;

            printf("\n%s joined last at %d\n", nameHead->fullName, logHead->time);
            return;
        }
        else
        {
            while (logHead != NULL)
            {
                if (logHead->time == time)
                {
                    // Getting the name LL of head Log
                    struct Name *nameHead = logHead->userName;
                    // Traversing to the last name of the name LL
                    while (nameHead->nextName != NULL)
                        nameHead = nameHead->nextName;

                    printf("\n%s joined last at %d\n", nameHead->fullName, logHead->time);
                    return;
                }
                else
                    logHead = logHead->nextLog;
            }
        }
    }
    printf("No log entry with time %d found !!\n", time);
}

void searchWhenUserNameCame(struct Log *logHead, char *name)
{
    if (logHead == NULL)
        printf("Can't Search - Empty Log !!\n");
    else
    {
        // if head node itself
        if (strcmp(name, logHead->userName->fullName) == 0){
            printf("\n%s came at %d\n", logHead->userName->fullName, logHead->time);
            return;
        }
        else
        {
            while (logHead != NULL)
            {
                struct Name *nameHead = logHead->userName;
                while (nameHead != NULL)
                {
                    if (strcmp(name, nameHead->fullName) == 0)
                    {
                        printf("\n%s came at %d\n", nameHead->fullName, logHead->time);
                        return;
                    }
                    else
                        nameHead = nameHead->nextName;
                }
                logHead = logHead->nextLog;
            }
        }
    }
    printf("No search enty with name %s found !!\n", name);
}


// ----------------------------- Sir'dlt Algorithm 2023-01-11 (Not tested) -----------------------------
void getLogHeadStatus(struct Log **logHead, char *name)
{
    while ((*logHead)->userName != NULL)
    {
        if (strcmp(name, (*logHead)->userName->fullName) == 0)
            (*logHead)->userName = (*logHead)->userName->nextName;
        else
            break;
    }
    if ((*logHead)->userName == NULL)
        (*logHead) = NULL;
    else
    {
        while ((*logHead)->userName->nextName != NULL)
        {
            if (strcmp(name, (*logHead)->userName->nextName->fullName) == 0)
                (*logHead)->userName->nextName = (*logHead)->userName->nextName->nextName;
            else
                (*logHead)->userName = (*logHead)->userName->nextName;
        }
    }
}

void dltByName(struct Log **logHead, char *name)
{
    while (*(logHead) != NULL)
    {
        getLogHeadStatus(logHead, name);
        if ((*logHead)->userName == NULL)
            (*logHead) = (*logHead)->nextLog;
        else
            break;
    }

    if ((*logHead) == NULL)
        printf("Empty List !!");
    else
    {
        struct Log *temptr = *logHead;
        while (temptr->nextLog != NULL)
        {
            getLogHeadStatus(&temptr, name);
            if (temptr->userName == NULL)
                temptr->nextLog = temptr->nextLog->nextLog;
            else
                temptr = temptr->nextLog;
        }
    }
}

// ----------------------------- Driver Code  -----------------------------
int main()
{
    struct Log *logHead = NULL;

    // display(logHead);

    // ----------- Insertion -----------
    // insertLog(&logHead, 1040, "Animesh");
    // insertLog(&logHead, 1020, "Sriza");
    // insertLog(&logHead, 1030, "Saikat");
    // insertLog(&logHead, 1020, "Joyitree");
    // // display(logHead);

    // insertLog(&logHead, 1055, "CR7");
    // insertLog(&logHead, 1045, "Pritwish");
    // insertLog(&logHead, 1020, "Bodhi");
    // insertLog(&logHead, 1030, "Ankur");
    // insertLog(&logHead, 1058, "Rajdeep");
    // insertLog(&logHead, 1055, "Messi");
    // display(logHead);

    // ----------- Deletion -----------
    // dltLog(&logHead, 1020); // head log node
    // dltLog(&logHead, 1045); // intermediate log node
    // dltLog(&logHead, 1058); // last log node

    // dltNameElement(&logHead, "Saikat"); // head name node
    // dltNameElement(&logHead, "Joyitree"); // intermediate name node
    // dltNameElement(&logHead, "Ankur"); // last name node
    // dltNameElement(&logHead, "Messi"); // intermediate Log node
    // dltNameElement(&logHead, "Pritiwsh");
    // display(logHead);

    // ----------- Searching -----------
    // searchWhoCameAtTime(logHead, 1020);
    // searchWhoCameAtTime(logHead, 1058);

    // searchHowManyCameAtTime(logHead, 1020);

    // searchWhoFirstAtTime(logHead, 1020);
    // searchWhoLastAtTime(logHead, 1020);

    // searchWhenUserNameCame(logHead, "Rajdeep");
    // searchWhenUserNameCame(logHead, "CR7");

    // ------- Date: 23/01/08 <Test Case> -------
    insertLog(&logHead, 1010, "Sriza");
    insertLog(&logHead, 1010, "Bodhi");
    insertLog(&logHead, 1010, "Bodhi");
    insertLog(&logHead, 1040, "Rajdeep");
    insertLog(&logHead, 1030, "Bodhi");
    insertLog(&logHead, 1030, "Saikat");
    insertLog(&logHead, 1030, "Joyitree");
    insertLog(&logHead, 1020, "Bodhi");
    insertLog(&logHead, 1020, "Bodhi");
    insertLog(&logHead, 1040, "Bodhi");
    insertLog(&logHead, 1040, "Pritwish");
    insertLog(&logHead, 1050, "Ankur");


    display(logHead);


    // ------- Executing my dlt Algo -------
    // dltLog(&logHead, 1050);
    // dltNameElement(&logHead, "Ronaldo");
    // dltNameElement(&logHead, "Bodhi");
    // display(logHead);

    // Test Case - No entry with given i/p found :
    // searchWhenUserNameCame(logHead,"Sharukh");
    // searchWhoFirstAtTime(logHead, 1212);
    // searchWhoLastAtTime(logHead, 1212);
    // searchWhoCameAtTime(logHead, 1234);
    // searchHowManyCameAtTime(logHead, 1067);
    return 0;
}
