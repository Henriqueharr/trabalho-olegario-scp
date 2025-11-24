#include <stdlib.h>
#include <ncurses.h>
#include "vector.h"
#include "doublylinkedlist.h"
#include "clientes.h"
#include "pedidos.h"
#include "produtos.h"
             
void initList(List *lista)
{
   lista->head = NULL;
   lista->tail = NULL;
   lista->tam = 0;
}

int createInsertNode(List *lista, void *conteudo, Tipos dataType)
{
   Node *novo = (Node*)malloc(sizeof(Node));

   if(!novo) return 0;

   novo->content = conteudo;
   novo->dataType = dataType;
   novo->next = NULL;
   novo->prev = NULL;

   if(!lista->tail)
   {
      lista->head = novo;
      lista->tail = novo;
      lista->tam++;
      return 1;
   }

   lista->tail->next = novo;
   novo->prev = lista->tail;
   lista->tail = novo;
   lista->tam++;

   return 1;
}

void removeNode(List *lista, Node *target)
{
   if(!target) return;
   if(!lista->head) return;
   if(lista->head->dataType != target->dataType && lista->head->dataType != PESSOA_FISICA && target->dataType != PESSOA_JURIDICA && lista->head->dataType != PESSOA_JURIDICA && target->dataType != PESSOA_FISICA) return;
   
   if(target->dataType == PEDIDO)
   {
      Vector* itens = &expand_node(target, Pedido)->itens;

      for(short i = 0; i < itens->_size; i++)
      {
         free(itens->container[i]);
      }
      free(itens->container);
   }

   if(!target->prev)
   {
      lista->head = lista->head->next;
      if(lista->head) lista->head->prev = NULL;
      if(!(--lista->tam)) lista->tail = NULL;
      free(target->content);
      free(target);
      return;
   }

   if(!target->next)
   {
      lista->tail = lista->tail->prev;
      lista->tail->next = NULL;
      lista->tam--;
      free(target->content);
      free(target);
      return;
   }

   target->prev->next = target->next;
   target->next->prev = target->prev;
   lista->tam--;
   free(target->content);
   free(target);
}

Node* findByID(List *lista, size_t targetID)
{
   if(!lista->head) return NULL;

   Node* begin = lista->head;
   
   if(begin->dataType == PESSOA_FISICA || begin->dataType == PESSOA_JURIDICA) for( ; begin; begin = begin->next) if(expand_node(begin,GenericCast)->data.id == targetID) return begin;   

   begin = lista->head;

   if(begin->dataType == PRODUTO) for( ; begin; begin = begin->next) if(expand_node(begin,Produto)->id == targetID) return begin;
   
   begin = lista->head;

   if(begin->dataType == PEDIDO) for( ; begin; begin = begin->next) if(expand_node(begin,Pedido)->id == targetID) return begin;

   return NULL;
}

int pathToCNode(List *lista, size_t targetID)
{
   if(!lista->head) return -1;

   int dist = 0;
   
   Node* path = lista->head;
   
   if(path->dataType == PESSOA_FISICA || path->dataType == PESSOA_JURIDICA) for( ; path; path = path->next, dist++) if(expand_node(path,GenericCast)->data.id == targetID) return dist;   

   path = lista->head;
   
   if(path->dataType == PRODUTO) for( ; path; path = path->next, dist++) if(expand_node(path,Produto)->id == targetID) return dist;

   path = lista->head;
   
   if(path->dataType == PEDIDO) for( ; path; path = path->next, dist++) if(expand_node(path,Pedido)->id == targetID) return dist; 

   return -1;
}