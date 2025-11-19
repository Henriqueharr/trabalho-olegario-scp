#ifndef PRODUTOS_UTILS_H
#define PRODUTOS_UTILS_H

#include "doublylinkedlist.h"

//Essa é a estrutura de um produto na SCP
//
// typedef struct Produto
// {
//    size_t id;
//    char description[150];
//    double price;
//    size_t stock;
// } Produto;
typedef struct Produto
{
   size_t id;
   char description[150];
   double price;
   size_t stock;
} Produto;

Produto* criarProduto(List *listaProdutos);

void ListarProdutos(List *listaProd);

Produto* selecionarProduto(List *L);
int removerProduto(List *L, size_t id);
Produto* buscarProdutoPorID(List *L, size_t id);

#endif