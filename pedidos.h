#ifndef PEDIDOS_UTILS_H
#define PEDIDOS_UTILS_H

#include "vector.h"

//Essa é a estrutura de um item de um pedido na SCP
//
// typedef struct ItemPedido
// {
//    size_t requestId;
//    size_t productId;
//    size_t amount;
//    double subtotal;
// } ItemPedido;
typedef struct ItemPedido
{
   size_t requestId;
   size_t productId;
   size_t amount;
   double subtotal;
} ItemPedido;

//Essa é a estrutura de um pedido na SCP
//Note que ela usa um Vector. Para mais informações, consultar Vector
//
// typedef struct Pedido
// {
//    size_t id;
//    size_t customerId;
//    char date[15];
//    double total;
//    Vector itens;
// } Pedido;
typedef struct Pedido
{
   size_t id;
   size_t customerId;
   char date[15];
   double total;
   Vector itens;
} Pedido;

#endif