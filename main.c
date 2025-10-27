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
#include "safetyutils.h"

int main()
{
   setlocale(LC_ALL,"");
   initscr();

   short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   List LClientes, LPedidos, LProdutos;
   initList(&LClientes);
   initList(&LPedidos);
   initList(&LProdutos);
   
   MenuPrincipal opc = MENU_CLIENTES;
   
   while(opc != ENCERRAR_PROGRAMA)
   {
      
      showMainMenu(&opc);

      //testando função
      size_t a;
      lerSizeT(&a, stdscr);
      
      transicao1(stdscr, 0, maxstdy);

      
      switch(opc)
      {
         case MENU_CLIENTES:
         {
            MenuCliente escolha = ADD_CLIENTE;
            while(escolha != VOLTAR_CLIENTE)
            {
               showCustomerMenu(&escolha);

               switch(escolha)
               {
                  case ADD_CLIENTE:
                  {
                     Tipos dataType;
                     transicao1(stdscr, 27, maxstdy);
                     transicao2(stdscr, 20, 25, 50);
                     showAddCustomerMenu(&dataType);
                     criarCliente(dataType);
                  }
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
               wrefresh(stdscr);
               napms(500);
            }
         }
         break;
         case MENU_PRODUTOS:
         mvwaddstr(stdscr, 33, 95, "Produtos selecionados\n");
         break;
         case MENU_PEDIDOS:
         mvwaddstr(stdscr, 33, 95, "Pedidos selecionados\n");
         break;
         case ENCERRAR_PROGRAMA:
         mvwaddstr(stdscr, 33, 95, "Encerrando programa\n");
         break;
      }
      wrefresh(stdscr);
      napms(500);
   }



   endwin();

   return 0;
}