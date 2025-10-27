#ifndef LINKEDLIST_UTILS_H
#define LINKEDLIST_UTILS_H

typedef enum Tipos {PESSOA_FISICA = 1, PESSOA_JURIDICA, PEDIDO, PRODUTO, ITEM_PEDIDO} Tipos;

#define get_content(node,type) ((type*)(node->content))

typedef struct Node 
{
   void *content;
   Tipos dataType;
   struct Node *next;
   struct Node *prev;
} Node;

typedef struct List
{
   size_t tam;
   Node *head;
   Node *tail;
} List;

void initList(List *lista);
void insertNode(List *lista, void *conteudo, Tipos dataType);

#endif