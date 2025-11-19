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
   initList(&LProdutos);
   initList(&LPedidos);

   
   carregarDados(&LClientes, "clientes.csv");
   carregarDados(&LProdutos, "produtos.csv");

   //Debug
   size_t last = 10123;
   // const char *seila[] = {"Pedro Migliori", "Mateus Betelle", "Thiago Bahia", "Lucas Oliver", "Davi Jaime", "Gustavo Barros", "Eduardo Valcacer", "Henrique Augusto"};
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

   // for(size_t i = 123; i < last * 5; i++)
   // {
   //    Produto *novoProd = (Produto*)malloc(sizeof(Produto));
   //    novoProd->id = i;
   //    strncpy(novoProd->description, "Placeholder", 150);
   //    novoProd->price = 25 + (i % 200) * (rand() % 5) + (rand() % 100)/100.0;
   //    novoProd->stock = 75 + (rand() % 201); 
   //    createInsertNode(&LProdutos, novoProd, PRODUTO);   
   // }

   for(size_t i = 123; i <= last; i++)
   {
      Pedido *novoPed = (Pedido*)malloc(sizeof(Pedido));
      initVector(&novoPed->itens);
      novoPed->id = i;
      novoPed->customerId = (rand() % 50482) + 124;
      strncpy(novoPed->date, "31/12/9999", 15);
      double tot = 0;
      for(size_t j = (rand() % 7) + 5; j > 0; j--)
      {
         ItemPedido *novoItem = (ItemPedido*)malloc(sizeof(ItemPedido));
         novoItem->requestId = i;
         size_t hmm = (rand() % 50482) + 124;
         novoItem->productId = hmm;
         novoItem->amount = (rand() % 100) + 20;
         novoItem->subtotal = expand_node(findByID(&LProdutos, hmm), Produto)->price * novoItem->amount;
         push_back(&novoPed->itens, novoItem);
         tot += novoItem->subtotal;
      }
      novoPed->total = tot;
      createInsertNode(&LPedidos, novoPed, PEDIDO);
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
                           if(novoCliente) 
                           {
                              if(createInsertNode(&LClientes, novoCliente, dataType))
                              {
                                 gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação bem sucedida", "Cliente salvo com sucesso");
                              }
                              else gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação fracassou", "Não foi possível adicionar o cliente aos dados");
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
                           else gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação fracassou", "Não foi possível adicionar o produto aos dados");
                        }
                     }
                     direita(stdscr, maxstdx * (95.0/teladevx) - maxstdx * (50.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 74, maxstdx * (50.0/teladevx), 10);
                  }
                  break;
                  case LISTAR_PRODUTOS:
                  {
                     abrir(stdscr, maxstdy * (20.0/teladevy) + 8, 29, 25);
                     ListarProdutos(&LProdutos);
                     cortina(stdscr, 0, 0, maxstdy, 10);
                  }
                  break;
                  case EDITAR_PRODUTO:
                  {

                  }
                  break;
                  case REMOVER_PRODUTO:
                  {
                  abrir(stdscr, maxstdy * (20.0 / teladevy) + 8, 29, 25);
                                    
                                 keypad(stdscr, TRUE);
                                 Produto *sel = selecionarProduto(&LProdutos);
                                    
                                   clear();
                                   refresh();
                                    
                                 if (sel)
                                 {
                                     // 1) Verifica se produto está em algum pedido
                                     if (produtoEstaEmUmPedido(&LPedidos, sel->id))
                                     {
                                         gerarAviso(
                                             maxstdy * (20.0 / teladevy),
                                             maxstdx * (45.0 / teladevx),
                                             maxstdy * 0.5 - (maxstdy * (15.0 / teladevy)) * 0.5,
                                             maxstdx * 0.5 - (maxstdx * (30.0 / teladevx)) * 0.5,
                                             "||~~OOOO",
                                             "Produto não pode ser removido",
                                             "Este produto está vinculado a um pedido!");
                                     }
                                     // 2) Pode remover normalmente
                                     else if (removerProduto(&LProdutos, sel->id))
                                     {
                                         gerarAviso(
                                             maxstdy * (15.0 / teladevy),
                                             maxstdx * (30.0 / teladevx),
                                             maxstdy * 0.5 - (maxstdy * (15.0 / teladevy)) * 0.5,
                                             maxstdx * 0.5 - (maxstdx * (30.0 / teladevx)) * 0.5,
                                             "||~~OOOO",
                                             "Operação bem sucedida",
                                             "Produto removido com sucesso");
                                     }
                                     // 3) Falhou por outro motivo
                                     else
                                     {
                                         gerarAviso(
                                             maxstdy * (15.0 / teladevy),
                                             maxstdx * (30.0 / teladevx),
                                             maxstdy * 0.5 - (maxstdy * (15.0 / teladevy)) * 0.5,
                                             maxstdx * 0.5 - (maxstdx * (30.0 / teladevx)) * 0.5,
                                             "||~~OOOO",
                                             "Erro",
                                             "Falha ao remover o produto");
                                     }
                                 }

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
         {
            MenuPedido escolha = ADD_PEDIDO;
            while(escolha != VOLTAR_PEDIDO)
            {
               showMenuRequest(&escolha);
               getmaxyx(stdscr, maxstdy, maxstdx);

               switch(escolha)
               {
                  case ADD_PEDIDO:
                  {
                     void *novoPedido = &escolha;
                     cortina(stdscr, 0, maxstdy * (20.0/teladevy) + 7, maxstdy * (20.0/teladevy) + 15, 25);
                     abrir(stdscr, maxstdy * (20.0/teladevy) + 2, 4, 75);
                     while(novoPedido)
                     {
                        novoPedido = criarPedido(&LPedidos, &LClientes, &LProdutos);
                        if(novoPedido) 
                        {
                           if(createInsertNode(&LPedidos, novoPedido, PEDIDO))
                           {
                              gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação bem sucedida", "Pedido salvo com sucesso");
                           }
                           else gerarAviso(maxstdy * (15.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (15.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Operação fracassou", "Não foi possível adicionar o pedido aos dados");
                        }
                     }
                  }
                  break;
                  case LISTAR_PEDIDOS:
                  {
                     abrir(stdscr, maxstdy * (20.0/teladevy) + 8, 29, 25);
                     ListarPedidos(&LPedidos);
                     cortina(stdscr, 0, 0, maxstdy, 5);
                  }
                  break;
                  case EDITAR_PEDIDO:
                  {

                  }
                  break;
                  case REMOVER_PEDIDO:
                  {

                  }
                  break;
                  case VOLTAR_PEDIDO:
                  {
                     descer(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy), maxstdy * (20.0/teladevy) + 5, 73, maxstdy * (9.0/teladevy), 15);
                     subir(stdscr, maxstdx * (95.0/teladevx), maxstdy * (20.0/teladevy) + maxstdy * (9.0/teladevy), maxstdy * (20.0/teladevy) + 5 + maxstdy * (9.0/teladevy), 73, maxstdy * (9.0/teladevy), 15);
                  }
                  break;
               }
            }
         }
         break;
         case ENCERRAR_PROGRAMA:
         mvwaddstr(stdscr, maxstdy * (33.0/teladevy), maxstdx * (95.0/teladevx), "Encerrando programa\n");
         break;
      }
      wrefresh(stdscr);
   }

   SalvarDados(&LClientes, "clientes.csv");
   SalvarDados(&LProdutos, "produtos.csv");

   endwin();

   return 0;
}