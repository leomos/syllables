#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _elem {
    char* key;
    int data;
    struct _elem* next;
} elem;

elem* get(elem* d, char* key);
void insert(elem** head, char* key, int d);
void print_dict(elem* head);
void inc(elem** head, char* key);
int len(elem* head);
elem** dict2array(elem* head);
void sort(elem** a, int l);
