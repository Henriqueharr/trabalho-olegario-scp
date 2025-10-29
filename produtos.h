#ifndef PRODUTOS_UTILS_H
#define PRODUTOS_UTILS_H

//Essa é a estrutura de um produto na SCP
//
// typedef struct Produto
// {
//    char name[100];
//    size_t id;
//    char description[150];
//    double price;
//    size_t stock;
// } Produto;
typedef struct Produto
{
   char name[100];
   size_t id;
   char description[150];
   double price;
   size_t stock;
} Produto;

#endif