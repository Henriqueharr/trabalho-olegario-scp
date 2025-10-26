#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include "vector.h"
#include "doublylinkedlist.h"
#include "menus.h"
#include "clientes.h"
#include "produtos.h"
#include "pedidos.h"

int main()
{
   setlocale(LC_ALL,"");
   initscr();

   List listaClientes, listaPedidos, listaProdutos;
   initList(&listaClientes);
   initList(&listaPedidos);
   initList(&listaProdutos);
   
   MenuPrincipal opc = MENU_CLIENTES;
   
   while(opc != ENCERRAR_PROGRAMA)
   {
      
      showMainMenu(&opc);
      
      transicao(stdscr);
      
      switch(opc)
      {
         case MENU_CLIENTES:
         {
            MenuCliente escolha = ADD_CLIENTE;
            while(escolha != VOLTAR_CLIENTE)
            {
               showCustomerMenu(&escolha);

               transicao(stdscr);

               switch(escolha)
               {
                  case ADD_CLIENTE:
                  mvwaddstr(stdscr, 33, 95, "Adicionar clientes\n");
                  break;
                  case LISTAR_CLIENTES:
                  mvwaddstr(stdscr, 33, 95, "Listar clientes\n");
                  break;
                  case EDITAR_CLIENTE:
                  mvwaddstr(stdscr, 33, 95, "Editar clientes\n");
                  break;
                  case REMOVER_CLIENTE:
                  mvwaddstr(stdscr, 33, 95, "Remover cliente\n");
                  break;
                  case SALVAR_CLIENTES_CSV:
                  mvwaddstr(stdscr, 33, 95, "Salvar clientes\n");
                  break;
                  case CARREGAR_CLIENTES_CSV:
                  mvwaddstr(stdscr, 33, 95, "Carregar clientes\n");
                  break;
                  case VOLTAR_CLIENTE:
                  mvwaddstr(stdscr, 33, 95, "Saindo do menu de clientes\n");
                  break;
               }

               wgetch(stdscr);
            }
         }
         break;
         case MENU_PRODUTOS:
         mvwaddstr(stdscr, 33, 95, "Produtos selecionados\n");
         wgetch(stdscr);
         break;
         case MENU_PEDIDOS:
         mvwaddstr(stdscr, 33, 95, "Pedidos selecionados\n");
         wgetch(stdscr);
         break;
         case ENCERRAR_PROGRAMA:
         mvwaddstr(stdscr, 33, 95, "Encerrando programa\n");
         wgetch(stdscr);
         break;
      }
   }



   endwin();

   return 0;
}