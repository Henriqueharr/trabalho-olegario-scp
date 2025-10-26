#include <stdlib.h>
#include <ncurses.h>
#include "doublylinkedlist.h"

void initList(List *lista)
{
   lista->head = NULL;
   lista->tail = NULL;
   lista->tam = 0;
}

void insertNode(List *lista, void *conteudo, Tipos dataType)
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