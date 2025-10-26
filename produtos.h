#ifndef PRODUTOS_UTILS_H
#define PRODUTOS_UTILS_H

typedef struct Produto
{
   char name[100];
   size_t id;
   char description[150];
   double price;
   size_t stock;
} Produto;

#endif