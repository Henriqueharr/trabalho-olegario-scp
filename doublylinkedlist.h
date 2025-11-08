#ifndef LINKEDLIST_UTILS_H
#define LINKEDLIST_UTILS_H

//Tipos de dados pretendidos para armazenamento em listas da SCP
typedef enum Tipos {PESSOA_FISICA = 1, PESSOA_JURIDICA, PEDIDO, PRODUTO, ITEM_PEDIDO, NULO} Tipos;

#define expand_node(node,type) ((type*)(node->content))

//Essa é a estrutura de um nó para listas duplamente encadeadas
//Use o macro expand_node para acessar o conteúdo do nó rapidamente
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

//Estrutura que guarda o início e o fim de uma lista, além da quantidade de nós
//Para mais informações em nós, consultar Node
//
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

//Inicializa a lista duplamente encadeada dada
void initList(List *lista);

//Cria e insere um nó numa lista duplamente encadeada dada
//Para mais informações em nós, consultar Node
int createInsertNode(List *lista, void *conteudo, Tipos dataType);

//Remove um nó de uma lista duplamente encadeada
//O nó deve pertencer a lista passada, comportamento indefinido caso contrário
void removeNode(List *lista, Node *target);

//Função para achar o endereço na memória de um nó a partir de seu ID
Node* findByID(List *lista, size_t targetID);

#endif