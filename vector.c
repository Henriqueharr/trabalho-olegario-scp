#include <stdlib.h>
#include <ncurses.h>
#include "vector.h"

void initVector(Vector *vetor)
{
   vetor->_current_pb = 0;
   vetor->_capacity = 2;
   vetor->_size = 0;
   vetor->container = (void**)malloc(sizeof(void*) * vetor->_capacity);
}

void _reserve(Vector *vetor)
{
   vetor->_capacity = vetor->_capacity * 2;
   vetor->container = (void**)realloc(vetor->container, sizeof(void*) * vetor->_capacity); 
}

void push_back(Vector *vetor, void* elemento)
{
   if(vetor->_current_pb >= vetor->_capacity) _reserve(vetor);

   vetor->container[vetor->_current_pb++] = elemento;
   vetor->_size++;
}

void erasePos(Vector *vetor, size_t pos)
{
   if(pos >= vetor->_size || pos < 0) return;
   size_t i;
   for(i = pos; i < vetor->_size - 1; i++) vetor->container[i] = vetor->container[i + 1];
   vetor->container[--vetor->_current_pb] = NULL;
   vetor->_size--;
}