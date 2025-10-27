#ifndef CLIENTES_UTILS_H
#define CLIENTES_UTILS_H

#include "doublylinkedlist.h"

typedef struct Cliente
{
   size_t id;
   char name[100];
   char address[200];
   char phonenumber[20];
} Cliente;

typedef struct PessoaFisica
{
   Cliente data;
   char cpf[20];
} PessoaFisica;

typedef struct PessoaJuridica
{
   Cliente data;
   char cnpj[20];
} PessoaJuridica;

void* criarCliente(Tipos dataType);

#endif