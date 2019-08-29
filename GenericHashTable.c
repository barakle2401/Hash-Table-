/*
Create by Barak Levy Id 203280185
Program Objective:
-Create a generic hash table (str/int) using array of pointers and linked lists
 handling collisions by linked list and dynamic expansion of the table
-practice c and especially pointers 
*/
#ifndef _GENERIC_HASH_TABLE_H
#define _GENERIC_HASH_TABLE_H
#include "GenericHashTable.h"
Table *createTable(int size, int dType, int listLength)
{
    if (size < 0 || (dType != 0 && dType != 1) || listLength < 0)
    {
        errorMsg();
        return NULL;
    }
    Table *t = (Table *)malloc(sizeof(Table));
    t->arr = (Object **)malloc(size * (sizeof(Object *)));
    t->dType = dType;
    t->listLength = listLength;
    t->origSize = size;
    t->d = 1;
    for (int i = 0; i < size; i++) //initiate the array
    {
        t->arr[i] = NULL;
    }
    return t;
}
/* The function adds data to the hashtable by locate the right index  then call insert func
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
int add(Table *table, void *data)
{
    if (table == NULL)
    {
        errorMsg();
        return -1;
    }
    int index = -1;
    Object *obj = createObject(data);
    if (table->dType == INT_TYPE) //case integer
    {
        index = (table->d) * (intHashFun((int *)data, table->origSize));

        obj->data = ((int *)data);
    }
    else //case string type
    {
        index = (table->d) * (strHashFun((char *)data, table->origSize));

        obj->data = (char *)malloc(strlen((char *)data) + 1);
        strcpy(obj->data, (char *)data);
    }
    if (index < 0)
    {
        return -1;
    }
    insert(table, index, obj);
    return index;
}
//The function trying to insert the data if expansion needed call expandTable func and then call insert again
void insert(Table *table, int index, Object *obj)
{
    if (table == NULL)
    {
        errorMsg();
        return;
    }
    int counter;
    for (int i = index; i < index + table->d; i++) //scan the possible lists
    {
        counter = 1;
        Object *temp = table->arr[i];
        if (temp == NULL) //case no obj's
        {
            table->arr[i] = obj;

            return;
        }
        while (temp->next != NULL) //count the objects at the list
        {
            temp = temp->next;
            counter++;
        }
        if (counter < table->listLength) //if there is free space add the object o.w expand table func
        {

            temp->next = obj;
            return;
        }
    }

    expandTable(table);            //expand needed
    insert(table, index * 2, obj); //after expanded, update the index and call insert
}
//The function duplicate the pointers array to a multiply array
void expandTable(Table *table)
{
    if (table == NULL)
    {
        errorMsg();
        return;
    }
    table->d *= 2;
    int d = table->d;
    Object **temp = (Object **)malloc(sizeof(Object *) * d * (table->origSize));
    for (int i = 0; i < table->d * table->origSize; i++)
    {
        temp[i] = NULL;
    }
    for (int i = 0; i < ((d / 2)) * table->origSize; i++) //scan the previous table and and copy each elem to the index*2 cell in the new array
    {
        temp[i * 2] = table->arr[i];
    }
    free(table->arr); //free the prev array
    table->arr = temp;
}
/*
* check the equality of the data of two objects
*/
int isEqual(int type, void *data1, void *data2)
{
    if (type == INT_TYPE)
    {
        int dataInt1 = *((int *)data1), dataInt2 = *((int *)data2);
        if (dataInt1 == dataInt2)
        {
            return 0;
        }

        return -1;
    }
    else
    {

        char *c1 = (char *)data1;
        char *c2 = (char *)data2;
        if ((strcmp(c1, c2)) == 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
}
Object *createObject(void *data)
{
    Object *obj = (Object *)malloc(sizeof(Object));
    obj->next = NULL;
    return obj;
}

void freeObject(Object *obj, int type)
{
    if (obj == NULL)
    {
        errorMsg();
        return;
    }
    if (type == STR_TYPE)
    {
        free(obj->data);
    }
    free(obj);
}
int intHashFun(int *key, int origSize)
{
    if (origSize <= 0 || (key == NULL))
    {
        errorMsg();
        return -1;
    }
    return (*(key)) % origSize;
}
void printTable(Table *table)
{
    if (table == NULL)
    {
        errorMsg();
        return;
    }
    int d = table->d;
    int origSize = table->origSize;
    Object *temp = NULL;
    for (int i = 0; i < origSize * d; i++)
    {

        printf("[%d]\t", i);

        temp = table->arr[i];
        if (temp == NULL) //no objects at the list
        {
            printf("\n");
            continue;
        }
        if (table->dType == STR_TYPE)
        {

            while (temp != NULL)
            {

                printf("%s\t-->\t", (char *)temp->data);
                temp = temp->next;
            }
        }
        else
        {
            while (temp != NULL)
            {

                printf("%d\t-->\t", *((int *)temp->data));
                temp = temp->next;
            }
        }
        printf("\n");
    }
}
int strHashFun(char *key, int origSize)
{
    if (origSize <= 0 || (key == NULL))
    {
        errorMsg();
        return -1;
    }
    int assciiSum = 0;
    char *tempKey = key;
    while ((*tempKey) != '\0')
    {
        assciiSum += (int)(*(tempKey));
        tempKey++;
    }
    return assciiSum % origSize;
}
/**
* The function release all the allocated members of struct Table.
*/
void freeTable(Table *table)
{
    if (table == NULL)
    {
        printf("this table already free\n");
        return;
    }
    int d = table->d;

    for (int i = 0; i < d * (table->origSize); i++)
    {
        Object *temp = table->arr[i], *prev = NULL;

        while (temp != NULL)
        {
            prev = temp->next;
            freeObject(temp, table->dType);
            temp = prev;
        }
    }
    free(table->arr);
    free(table);
    table = NULL;
}
int removeObj(Table *table, void *data)
{
    if (table == NULL)
    {
        errorMsg();
        return -1;
    }
    int index = 0, d = table->d;
    if (table->dType == INT_TYPE)
    {
        index = d * intHashFun((int *)data, table->origSize);
    }
    else
    {
        index = d * strHashFun((char *)data, table->origSize);
    }
    for (int i = index; i < (index + d); i++)
    {

        Object *temp = table->arr[i], *prev = NULL;

        if (temp == NULL) //there is no node's at the list
        {
            continue;
        }
        else if (temp->next == NULL && ((isEqual(table->dType, data, temp->data) == 0))) //only head ,and matching
        {

            freeObject(temp, table->dType);
            table->arr[i] = NULL;
            return i;
        }
        else
        {

            while (temp != NULL && (isEqual(table->dType, data, temp->data) != 0)) //find the location of the data at the list
            {

                prev = temp;
                temp = temp->next;
            }
            if (temp == NULL) //scan the whole list and not match
            {

                continue;
            }

            if (prev == NULL) //the first node is matching
            {

                prev = temp->next;
                freeObject(temp, table->dType);
                table->arr[i] = prev;
                return i;
            }
            prev->next = temp->next; //unlink the node

            freeObject(temp, table->dType);

            temp = NULL;
            return i;
        }
    }
    return -1;
}
/**
* The function search for an object that is data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
Object *search(Table *table, void *data)
{
    if (table == NULL)
    {
        errorMsg();
        return NULL;
    }
    int index = 0, d = table->d;
    Object *temp = NULL;
    if (table->d == INT_TYPE)
    {
        index = d * (intHashFun(data, table->origSize));
    }
    else
    {
        index = d * (strHashFun(data, table->origSize));
    }
    if (index == -1)
    {
        return NULL;
    }

    for (int i = index; i < (index + d); i++)
    {
        temp = table->arr[i];
        if (temp == NULL)
        {
            continue;
        }
        while (temp != NULL)
        {
            if (isEqual(table->dType, data, temp->data) == 0)
            {
                return temp;
            }
            temp = temp->next;
        }
    }
    return NULL;
}
void errorMsg()
{
    printf("INVALID VALUE\n");
}
#endif