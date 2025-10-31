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

#include <string.h>

//Essa é a main :D
int main()
{
   setlocale(LC_ALL,"");
   initscr();

   start_color();
   init_pair(1, COLOR_GREEN, COLOR_BLACK);

   short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   List LClientes, LPedidos, LProdutos;
   initList(&LClientes);
   initList(&LPedidos);
   initList(&LProdutos);

   //Debug
   size_t last = 1000123;
   for(size_t i = 123; i <= last * 3; i++)
   {
      if(i % 2)
      {
         PessoaFisica* conteudo = (PessoaFisica*)malloc(sizeof(PessoaFisica));
         if(conteudo)
         {
            conteudo->data.id = i;
            strncpy(conteudo->data.name, "PlaceHolder", 100);
            strncpy(conteudo->data.address, "PlaceHolder\0", 200);
            strncpy(conteudo->data.phonenumber, "PlaceHolder\0", 20);
            strncpy(conteudo->cpf, "000.000.000-00\0", 15);
            createInsertNode(&LClientes, conteudo, PESSOA_FISICA);
         }
         continue;
      }
      PessoaJuridica* conteudo = (PessoaJuridica*)malloc(sizeof(PessoaJuridica));
      if(conteudo)
      {
         conteudo->data.id = i;
         strncpy(conteudo->data.name, "PlaceHolder", 100);
         strncpy(conteudo->data.address, "PlaceHolder\0", 200);
         strncpy(conteudo->data.phonenumber, "PlaceHolder\0", 20);
         strncpy(conteudo->cnpj, "00.000.000/0001-00\0", 15);
         createInsertNode(&LClientes, conteudo, PESSOA_JURIDICA);
      }
      
   }
   //endDebug
   
   MenuPrincipal opc = MENU_CLIENTES;
   
   while(opc != ENCERRAR_PROGRAMA)
   {
      
      showMainMenu(&opc);
      
      cortina(stdscr, 27, maxstdy, 25);

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
                     Tipos dataType = PESSOA_FISICA;
                     cortina(stdscr, 27, maxstdy, 25);
                     slideLeft(stdscr, 20, 25, 50, 5);
                     while(dataType != NULO)
                     {
                        showAddCustomerMenu(&dataType);
                        if(dataType != NULO)
                        {
                           void *novoCliente = criarCliente(dataType, &LClientes);
                           if(novoCliente) createInsertNode(&LClientes, novoCliente, dataType);
                        }
                     }
                  }
                  break;
                  case LISTAR_CLIENTES:
                  {
                     abrir(stdscr, 28, 29, 25);
                     listarClientes(&LClientes);
                     cortina(stdscr, 0, maxstdy, 10);
                  }
                  break;
                  case EDITAR_CLIENTE:
                  {
                     abrir(stdscr, 27, 9, 10);
                     editarCliente(&LClientes);
                  }
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