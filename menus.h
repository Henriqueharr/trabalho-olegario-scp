#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <ncurses.h>
#include "doublylinkedlist.h"

typedef enum MenuPrincipal {MENU_CLIENTES = 1, 
                            MENU_PRODUTOS, 
                            MENU_PEDIDOS, 
                            ENCERRAR_PROGRAMA} MenuPrincipal;
                            
typedef enum MenuCliente {ADD_CLIENTE = 1, 
                          LISTAR_CLIENTES, 
                          EDITAR_CLIENTE, 
                          REMOVER_CLIENTE, 
                          SALVAR_CLIENTES_CSV, 
                          CARREGAR_CLIENTES_CSV, 
                          VOLTAR_CLIENTE} MenuCliente;

typedef enum MenuProduto {ADD_PRODUTO = 1, 
                          LISTAR_PRODUTOS, 
                          EDITAR_PRODUTO, 
                          REMOVER_PRODUTO, 
                          SALVAR_PRODUTOS_CSV, 
                          CARREGAR_PRODUTOS_CSV, 
                          VOLTAR_PRODUTO} MenuProduto;

typedef enum MenuPedido {ADD_PEDIDO = 1, 
                         LISTAR_PEDIDOS, 
                         DETALHAR_PEDIDO, 
                         SALVAR_PEDIDOS_CSV, 
                         CARREGAR_PEDIDOS_CSV, 
                         VOLTAR_PEDIDO} MenuPedido;

typedef enum MenuCriarCliente{ID = 1,
                              NOME,
                              ENDERECO,
                              TELEFONE,
                              CPF_CNPJ,
                              FINALIZAR} MenuCriarCliente;

void gerarErro(unsigned short altura, unsigned short largura, unsigned short yi, unsigned short xi, const char *bordas, const char *titulo, const char *msg);
void cortina(WINDOW * tela, short yi, short yf, unsigned short delay);
void slideLeft(WINDOW *tela, short yi, short yf, short pad, unsigned short delay);
void abrir(WINDOW *tela, short uppery, short abertura, unsigned short delay);
void logo(unsigned short y, unsigned short x);
void showMainMenu(MenuPrincipal *opc);
void showCustomerMenu(MenuCliente *opc);
void showAddCustomerMenu(Tipos *dataType);

#endif