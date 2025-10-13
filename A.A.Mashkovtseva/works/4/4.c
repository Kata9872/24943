#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    char *string;
    struct node *next;
} node;

node* create(char *str){
    node *new = malloc(sizeof(node));
    new->string = malloc(strlen(str) + 1);
    strcpy(new->string, str);
    new->next = NULL;
    return new;
}

int main(){
    node *head = malloc(sizeof(node));
    head->next = NULL;
    node *tail = head;

    char line[BUFSIZ];
    printf("Write string or write . to get all written strings\n");
    while (fgets(line, BUFSIZ, stdin)) {
        if (line[0] == '.') break;
        tail->next = create(line);
        tail = tail->next;
        printf("Write string or write . to get all written strings\n");
    }

    for (node *i = head->next; i != NULL; i = i->next){
        printf("Your strings:\n%s", i->string);
    }

    return 0;
}