#include <stdlib.h>
#include <ncurses.h>
#include "vector.h"
#include "doublylinkedlist.h"
             

void initList(List *lista)
{
   lista->head = NULL;
   lista->tail = NULL;
   lista->tam = 0;
}

void createInsertNode(List *lista, void *conteudo, Tipos dataType)
{
   Node *novo = (Node*)malloc(sizeof(Node));
   novo->content = conteudo;
   novo->dataType = dataType;
   novo->next = NULL;
   novo->prev = NULL;

   if(!lista->tail)
   {
      lista->head = novo;
      lista->tail = novo;
      lista->tam++;
      return;
   }

   lista->tail->next = novo;
   novo->prev = lista->tail;
   lista->tail = novo;
   lista->tam++;
}

void removeNode(List *lista, Node *target)
{
   if(!target) return;
   if(!lista->head) return;
   if(lista->head->dataType != target->dataType && lista->head->dataType != PESSOA_FISICA && target->dataType != PESSOA_JURIDICA && lista->head->dataType != PESSOA_JURIDICA && target->dataType != PESSOA_FISICA) return;
   
   if(target->dataType == PEDIDO)
   {

   }

   if(!target->prev)
   {
      lista->head = lista->head->next;
      if(lista->head) lista->head->prev = NULL;
      if(!(--lista->tam) == 0) lista->tail = NULL;
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