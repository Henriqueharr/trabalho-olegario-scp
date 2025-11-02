#ifndef CLIENTES_UTILS_H
#define CLIENTES_UTILS_H

#include "doublylinkedlist.h"

#define get_ctype(void_ptr,type) ((type*)void_ptr)

//Estrutura que representa clientes da SCP
//
// typedef struct Cliente
// {
//    size_t id;
//    char name[100];
//    char address[200];
//    char phonenumber[20];
// } Cliente;
typedef struct Cliente
{
   size_t id;
   char name[100];
   char address[200];
   char phonenumber[21];
} Cliente;

//Estrutura de um cliente identificado como pessoa física
//
// typedef struct PessoaFisica
// {
//    Cliente data;
//    char cpf[15];
// } PessoaFisica;
typedef struct PessoaFisica
{
   Cliente data;
   char cpf[15];
} PessoaFisica;

//Estrutura de um cliente identificado como empresa
//
// typedef struct PessoaJuridica
// {
//    Cliente data;
//    char cnpj[20];
// } PessoaJuridica;
typedef struct PessoaJuridica
{
   Cliente data;
   char cnpj[20];
} PessoaJuridica;


//Estrutura para fins de cast abrangente de pessoas física e jurídicas
//
// typedef struct GenericCast
// {
//    Cliente data;
// } GenericCast;
typedef struct GenericCast
{
   Cliente data;
} GenericCast;

//Função para criar tanto estruturas de Pessoa Física como Jurídicas com interface
void* criarCliente(Tipos dataType, List *lista);

//Função para listagem e busca de clientes com interface
void listarClientes(List *listaClientes);

//Comentário fodástico
void editarCliente(List *listaClientes);

//Função para achar um nó pelo CPF ou CNPJ
Node* findCByCPF_CNPJ(List *lista, const char *cpf_cnpj);

//Funçaõ para achar o endereço na memória de um cliente a partir de seu ID
Node* findCByID(List *lista, size_t targetID);

//Função que retorna a distância de um cliente até o começo da lista
int pathToCNode(List *lista, size_t targetID);

#endif