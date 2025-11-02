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

//Tela dev
// 49.0 // Y
// 211.0 // X
extern double teladevy, teladevx;


//Essa é a main :D
int main()
{
   setlocale(LC_ALL,"");
   initscr();

   unsigned short maxstdy, maxstdx;

   start_color();
   init_pair(1, COLOR_GREEN, COLOR_BLACK);
   init_pair(2, COLOR_YELLOW, COLOR_BLACK);
   init_pair(3, COLOR_RED, COLOR_BLACK);

   List LClientes, LPedidos, LProdutos;
   initList(&LClientes);
   initList(&LPedidos);
   initList(&LProdutos);

   //Debug
   size_t last = 1000123;
   for(size_t i = 123; i <= last * 3; i++)
   {
      if(rand() % 2)
      {
         PessoaFisica* conteudo = (PessoaFisica*)malloc(sizeof(PessoaFisica));
         if(conteudo)
         {
            conteudo->data.id = i;
            strncpy(conteudo->data.name, "Lucas Oliver", 100);
            strncpy(conteudo->data.address, "Barra da Tijuca\0", 200);
            strncpy(conteudo->data.phonenumber, "75983475\0", 20);
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
         strncpy(conteudo->cnpj, "00.000.000/0001-00\0", 20);
         createInsertNode(&LClientes, conteudo, PESSOA_JURIDICA);
      }
      
   }
   //endDebug
   
   MenuPrincipal opc = MENU_CLIENTES;
   
   while(opc != ENCERRAR_PROGRAMA)
   {
      showMainMenu(&opc);
      getmaxyx(stdscr, maxstdy, maxstdx);
      cortina(stdscr, 0, maxstdy * (20.0/teladevy) + 7, maxstdy * (20.0/teladevy) + 12, 25);

      switch(opc)
      {
         case MENU_CLIENTES:
         {
            MenuCliente escolha = ADD_CLIENTE;
            while(escolha != VOLTAR_CLIENTE)
            {
               showCustomerMenu(&escolha);
               getmaxyx(stdscr, maxstdy, maxstdx);

               switch(escolha)
               {
                  case ADD_CLIENTE:
                  {
                     Tipos dataType = PESSOA_FISICA;
                     cortina(stdscr, 0, maxstdy * (20.0/teladevy) + 7, maxstdy * (20.0/teladevy) + 15, 25);
                     slideLeft(stdscr, 0, maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, maxstdx * (50.0/teladevx), 5);
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
                     abrir(stdscr, maxstdy * (20.0/teladevy) + 8, 29, 25);
                     listarClientes(&LClientes);
                     cortina(stdscr, 0, 0, maxstdy, 10);
                  }
                  break;
                  case EDITAR_CLIENTE:
                  {
                     cortina(stdscr, 0, maxstdy * (20.0/teladevy) + 6, maxstdy, 5);
                     slideLeft(stdscr, 0, maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, maxstdx * (75.0/teladevx), 2);
                     editarCliente(&LClientes);
                  }
                  break;
                  case REMOVER_CLIENTE:
                  mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Remover cliente\n");
                  break;
                  case SALVAR_CLIENTES_CSV:
                  mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Salvar clientes\n");
                  break;
                  case CARREGAR_CLIENTES_CSV:
                  mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Carregar clientes\n");
                  break;
                  case VOLTAR_CLIENTE:
                  mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Saindo do menu de clientes\n");
                  break;
               }
               wrefresh(stdscr);
               napms(500);
            }
         }
         break;
         case MENU_PRODUTOS:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Produtos selecionados\n");
         break;
         case MENU_PEDIDOS:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Pedidos selecionados\n");
         break;
         case ENCERRAR_PROGRAMA:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Encerrando programa\n");
         break;
      }
      wrefresh(stdscr);
      napms(500);
   }



   endwin();

   return 0;
}