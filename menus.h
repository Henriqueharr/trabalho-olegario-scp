#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <ncurses.h>
#include "doublylinkedlist.h"

//Enum para menu principal
//
// typedef enum MenuPrincipal {MENU_CLIENTES = 1, 
//                             MENU_PRODUTOS, 
//                             MENU_PEDIDOS, 
//                             ENCERRAR_PROGRAMA} MenuPrincipal;
typedef enum MenuPrincipal {MENU_CLIENTES = 1, 
                            MENU_PRODUTOS, 
                            MENU_PEDIDOS, 
                            ENCERRAR_PROGRAMA} MenuPrincipal;

//Enum para menu principal da aba clientes
//
// typedef enum MenuCliente {ADD_CLIENTE = 1, 
//                           LISTAR_CLIENTES, 
//                           EDITAR_CLIENTE, 
//                           REMOVER_CLIENTE, 
//                           SALVAR_CLIENTES_CSV, 
//                           CARREGAR_CLIENTES_CSV, 
//                           VOLTAR_CLIENTE} MenuCliente;
typedef enum MenuCliente {ADD_CLIENTE = 1, 
                          LISTAR_CLIENTES, 
                          EDITAR_CLIENTE, 
                          REMOVER_CLIENTE, 
                          VOLTAR_CLIENTE} MenuCliente;

//Enum para menu principal da aba produtos
//
// typedef enum MenuProduto {ADD_PRODUTO = 1, 
//                           LISTAR_PRODUTOS, 
//                           EDITAR_PRODUTO, 
//                           REMOVER_PRODUTO, 
//                           SALVAR_PRODUTOS_CSV, 
//                           CARREGAR_PRODUTOS_CSV, 
//                           VOLTAR_PRODUTO} MenuProduto;
typedef enum MenuProduto {ADD_PRODUTO = 1, 
                          LISTAR_PRODUTOS, 
                          EDITAR_PRODUTO, 
                          REMOVER_PRODUTO, 
                          VOLTAR_PRODUTO} MenuProduto;

//Enum para menu principal da aba pedidos
//
// typedef enum MenuPedido {ADD_PEDIDO = 1, 
//                          LISTAR_PEDIDOS, 
//                          DETALHAR_PEDIDO, 
//                          SALVAR_PEDIDOS_CSV, 
//                          CARREGAR_PEDIDOS_CSV, 
//                          VOLTAR_PEDIDO} MenuPedido;
typedef enum MenuPedido {ADD_PEDIDO = 1, 
                         LISTAR_PEDIDOS, 
                         DETALHAR_PEDIDO, 
                         VOLTAR_PEDIDO} MenuPedido;

//Enum para menu de criação de clientes
//
// typedef enum MenuCriarCliente{ID = 1,
//                               NOME,
//                               ENDERECO,
//                               TELEFONE,
//                               CPF_CNPJ,
//                               FINALIZAR} MenuCriarCliente;
typedef enum MenuCriarCliente{ID = 1,
                              NOME,
                              ENDERECO,
                              TELEFONE,
                              CPF_CNPJ,
                              FINALIZAR} MenuCriarCliente;

//Função para geração de telas de erro personalizadas e que somem em 1 click!
//Parâmetros:
//Altura da tela de erro
//Largura da tela de erro
//Linha inicial da tela de erro (A partir de seu canto superior esquerdo)
//Coluna inicial da tela de erro (A partir de seu canto superior esquerdo)
//String de 8 exatamente 8 caracteres para definir caracteres da lateral esquerda e direita, teto, chão e cantos em sentido horário, respectivamente
//String com o título do erro
//String com a mensagem do erro
void gerarAviso(unsigned short altura, unsigned short largura, unsigned short yi, unsigned short xi, const char *bordas, const char *titulo, const char *msg);

//Transição de tela no estilo de cortina
//Escolha a janela, a altura inicial e até onde descerá, além da demora para executar
void cortina(WINDOW * tela, unsigned short relx, short yi, short yf, unsigned short delay);

//Mova todos os caracteres de um range de linhas para a esquerda, linha a linha
//pad é quantas colunas para esquerda as linhas irão
//relx é a coluna que irá atrair tudo que está a direita
//As linhas se movem igualmente 
void slideLeft(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short pad, unsigned short delay);

//Transição no estilo de abertura a partir do centro com a demora escolhida
//Se a qauntidade de linhas a abrir for impar, escolha a altura do meio para o inicio, e a altura imediatamente acima do centro caso contrário
void abrir(WINDOW *tela, unsigned short uppery, unsigned short abertura, unsigned short delay);

void subir(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short QCarac, unsigned short dist, unsigned short delay);

void descer(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short QCarac, unsigned short dist, unsigned short delay);

void esquerda(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short QCarac, unsigned short dist, unsigned short delay);

void direita(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short QCarac, unsigned short dist, unsigned short delay);

//Gera a logo da SCP a partir de na coordenada escolhida a partir de seu canto superior esquerdo
//A logo tem 6 de altura por 24 de largura
void logo(unsigned short y, unsigned short x);

//Seleção de valor (por referência) enum de MenuPrincipal com interface
void showMainMenu(MenuPrincipal *opc);

//Seleção de valor (por referência) enum de MenuCliente com interface
void showCustomerMenu(MenuCliente *opc);

//Seleção de tipo de cliente (por referência) com interface
void showAddCustomerMenu(Tipos *dataType);

void showMenuProduct(MenuProduto *opc);

#endif