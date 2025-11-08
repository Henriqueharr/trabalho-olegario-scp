#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <math.h>
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
   init_pair(4, COLOR_CYAN, COLOR_BLACK);

   List LClientes, LPedidos, LProdutos;
   initList(&LClientes);
   initList(&LPedidos);
   initList(&LProdutos);

   
   //Debug
   // const char *seila[] = {"Pedro Migliori", "Mateus Betelle", "Thiago Bahia", "Lucas Oliver", "Davi Jaime", "Gustavo Barros", "Eduardo Valcacer", "Henrique Augusto"};
   // size_t last = 10123;
   // for(size_t i = 123; i < last * 5; i++)
   // {
   //    short hmm = rand() % 8;
   //    if(rand() % 2)
   //    {
   //       PessoaFisica* conteudo = (PessoaFisica*)malloc(sizeof(PessoaFisica));
   //       if(conteudo)
   //       {
   //          conteudo->data.id = i;
   //          strncpy(conteudo->data.name, seila[hmm], 100);
   //          strncpy(conteudo->data.address, "Barra da Tijuca\0", 200);
   //          strncpy(conteudo->data.phonenumber, "75983475\0", 20);
   //          strncpy(conteudo->cpf, "000.000.000-00\0", 15);
   //          createInsertNode(&LClientes, conteudo, PESSOA_FISICA);
   //       }
   //       continue;
   //    }
   //    PessoaJuridica* conteudo = (PessoaJuridica*)malloc(sizeof(PessoaJuridica));
   //    if(conteudo)
   //    {
   //       conteudo->data.id = i;
   //       strncpy(conteudo->data.name, seila[hmm], 100);
   //       strncpy(conteudo->data.address, "PlaceHolder\0", 200);
   //       strncpy(conteudo->data.phonenumber, "PlaceHolder\0", 20);
   //       strncpy(conteudo->cnpj, "00.000.000/0001-00\0", 20);
   //       createInsertNode(&LClientes, conteudo, PESSOA_JURIDICA);
   //    }
      
   // }
   //endDebug

   carregarDados(&LClientes, "clientes.csv");

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
                           if(novoCliente) 
                           {
                              if(createInsertNode(&LClientes, novoCliente, dataType))
                              {
                                 //gerar aviso de operação bem sucedida
                              }
                              //else gerar aviso de erro
                           }
                        }
                     }
                     slideLeft(stdscr, maxstdx * (75.0/teladevx), maxstdy * (21.0/teladevy), maxstdy * (21.0/teladevy) + 3, 30, 3);
                     direita(stdscr, maxstdx * (95.0/teladevx) - maxstdx * (50.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 74, maxstdx * (50.0/teladevx), 10);
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
                     esquerda(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 73, maxstdy * (80.0/teladevy), 5);
                     editarCliente(&LClientes);
                     abrir(stdscr, maxstdy * 0.5, 20, 15);
                  }
                  break;
                  case REMOVER_CLIENTE:
                  {
                     // subir(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 73, maxstdy * (18.0/teladevy), 15);
                     //IMPLEMENTAR DEPOIS  
                  }
                  break;
                  case VOLTAR_CLIENTE:
                  {
                     descer(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 73, maxstdy * (9.0/teladevy), 15);
                     subir(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy) + maxstdy * (9.0/teladevy), maxstdy * (20.0/teladevy) + 5 + maxstdy * (9.0/teladevy), 73, maxstdy * (9.0/teladevy), 15);
                  }
                  break;
               }
            }
         }
         break;
         case MENU_PRODUTOS:
         {
            MenuProduto escolha = ADD_PRODUTO;
            while(escolha != VOLTAR_PRODUTO)
            {
               showMenuProduct(&escolha);
               getmaxyx(stdscr, maxstdy, maxstdx);

               switch(escolha)
               {
                  case ADD_PRODUTO:
                  {
                     void *novoProduto = &opc;
                     cortina(stdscr, 0, maxstdy * (20.0/teladevy) + 7, maxstdy * (20.0/teladevy) + 15, 25);
                     slideLeft(stdscr, 0, maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, maxstdx * (50.0/teladevx), 5);
                     while(novoProduto)
                     {
                        novoProduto = criarProduto(&LProdutos); //Função fodástica de criar produto
                        if(novoProduto) 
                        {
                           if(createInsertNode(&LProdutos, novoProduto, PRODUTO))
                           {
                              gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação bem sucedida", "Produto salvo com sucesso");
                           }
                           //else gerar aviso de erro
                        }
                     }
                     direita(stdscr, maxstdx * (95.0/teladevx) - maxstdx * (50.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 74, maxstdx * (50.0/teladevx), 10);
                  }
                  break;
                  case LISTAR_PRODUTOS:
                  {

                  }
                  break;
                  case EDITAR_PRODUTO:
                  {

                  }
                  break;
                  case REMOVER_PRODUTO:
                  {

                  }
                  break;
                  case VOLTAR_PRODUTO:
                  {
                     descer(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 73, maxstdy * (9.0/teladevy), 15);
                     subir(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy) + maxstdy * (9.0/teladevy), maxstdy * (20.0/teladevy) + 5 + maxstdy * (9.0/teladevy), 73, maxstdy * (9.0/teladevy), 15);
                  }
                  break;
               }
            }
         }
         break;
         case MENU_PEDIDOS:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Pedidos selecionados\n");
         break;
         case ENCERRAR_PROGRAMA:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Encerrando programa\n");
         break;
      }
      wrefresh(stdscr);
   }

   SalvarDados(&LClientes, "clientes.csv");

   endwin();

   return 0;
}