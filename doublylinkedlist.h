#ifndef LINKEDLIST_UTILS_H
#define LINKEDLIST_UTILS_H

typedef enum Tipos {PESSOA_FISICA = 1, PESSOA_JURIDICA, PEDIDO, PRODUTO, ITEM_PEDIDO, NULO} Tipos;

#define get_content(node,type) ((type*)(node->content))

//Essa é a estrutura de um nó para listas duplamente encadeadas
//Use o macro get_content para acessar o conteúdo do nó
//
// typedef struct Node 
// {
//    void *content;
//    Tipos dataType;
//    struct Node *next;
//    struct Node *prev;
// } Node;
typedef struct Node 
{
   void *content;
   Tipos dataType;
   struct Node *next;
   struct Node *prev;
} Node;

// typedef struct List
// {
//    size_t tam;
//    Node *head;
//    Node *tail;
// } List;
typedef struct List
{
   size_t tam;
   Node *head;
   Node *tail;
} List;

void initList(List *lista);
void createInsertNode(List *lista, void *conteudo, Tipos dataType);

#endif