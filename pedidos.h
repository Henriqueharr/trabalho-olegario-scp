#ifndef PEDIDOS_UTILS_H
#define PEDIDOS_UTILS_H

#include "vector.h"

typedef struct ItemPedido
{
   size_t requestId;
   size_t productId;
   size_t amount;
   double subtotal;
} ItemPedido;

typedef struct Pedido
{
   size_t id;
   size_t customerId;
   char date[15];
   double total;
   Vector itens;
} Pedido;

#endif