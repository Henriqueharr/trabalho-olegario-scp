#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include "doublylinkedlist.h"
#include "produtos.h"
#include "pedidos.h"
#include "clientes.h"
#include "menus.h"
#include "safetyutils.h"

extern double teladevy, teladevx;

Pedido* criarPedido(List* listaPedidos, List* listaClientes, List* listaProdutos)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   unsigned short dist = maxstdy * (8.0/teladevy);

   const char* label[] = {"ID", "Data", "ID do Cliente", "Produtos do pedido", "Total do pedido"};
   const char* otalabel[] = {"Cliente", "Produto"};

   unsigned short posx[] = {maxstdx * (36.0/teladevx), 
                            maxstdx * (36.0/teladevx), 
                            maxstdx * (36.0/teladevx), 
                            maxstdx * (36.0/teladevx) + maxstdx * (32.0/teladevx),
                            maxstdx * (36.0/teladevx) + maxstdx * (32.0/teladevx)};

   unsigned short posy[] = {maxstdy * (10.0/teladevy), 
                            maxstdy * (10.0/teladevy) + dist + 1, 
                            maxstdy * (10.0/teladevy) + dist * 2 + 2, 
                            maxstdy * (10.0/teladevy),
                            maxstdy * (10.0/teladevy) + dist * 3 + 2};
   
   unsigned short tamabay[] = {maxstdy * (7.0/teladevy), 
                               maxstdy * (7.0/teladevy), 
                               maxstdy * (7.0/teladevy), 
                               maxstdy * (23.0/teladevy),
                               maxstdy * (5.0/teladevy)};

   unsigned short tamabax[] = {maxstdx * (30.0/teladevx),
                               maxstdx * (30.0/teladevx),
                               maxstdx * (30.0/teladevx),
                               maxstdx * (45.0/teladevx),
                               maxstdx * (45.0/teladevx)};

   float refy[] = {3.0,
                   3.0,
                   3.0,
                   19.0,
                   3.0};

   float refx[] = {26.0,
                   26.0,
                   26.0,
                   41.0,
                   41.0};

   
   unsigned short xtamerro = maxstdx * (33.0/teladevx);
   unsigned short ytamerro = maxstdy * (23.0/teladevy);
   unsigned short xposerro = maxstdx * (1.0/teladevx);
   unsigned short yposerro = posy[0];
   
   WINDOW *telas[5];
   WINDOW *subtelas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(tamabay[i], tamabax[i], posy[i], posx[i]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], tamabay[i] * (refy[i]/tamabay[i]), tamabax[i] * (refx[i]/tamabax[i]), tamabay[i] * (2.0/tamabay[i]), tamabax[i] * (2.0/tamabax[i]));
      wattron(telas[i], COLOR_PAIR(4));
      mvwaddstr(telas[i], 0, 0, label[i]);
      wattroff(telas[i], COLOR_PAIR(4));
      wrefresh(telas[i]);
      napms(15);
   }

   Pedido *novoPedido = (Pedido*)malloc(sizeof(Pedido));
   initVector(&novoPedido->itens);
   novoPedido->id = 0;

   unsigned short setapos = posy[0] + round(tamabay[0] * 0.5) + dist;
   unsigned short cmd = KEY_UP;
   MenuCriarPedido opc = DATA;
   unsigned short preenchidos[5] = {};
   unsigned short tudofeito = 0;

   mvwaddstr(stdscr, posy[0] + tamabay[0] * 0.5 + dist * 3 + 3, posx[0], "Adicionar produtos");
   
   keypad(stdscr, TRUE);
   curs_set(FALSE);
   cbreak();
   noecho();

   while(!preenchidos[4])
   {
      switch(cmd)
      {
         case 'z':
         case 'Z':
         goto escape;
         break;
         case KEY_UP:
         {
            if(opc == FINALIZAR_PEDIDO)
            {
               mvwaddch(stdscr, setapos, posx[0] - 2, ' ');
               setapos -= 3;
               mvwaddch(stdscr, setapos, posx[0] - 2, '>');
               opc--;
               break;
            }
            if(opc > IDPEDIDO)
            {
               mvwaddch(stdscr, setapos, posx[0] - 2, ' ');
               setapos -= (dist + 1);
               mvwaddch(stdscr, setapos, posx[0] - 2, '>');
               opc--;
            }
         }
         break;
         case KEY_DOWN:
         {
            if(opc == ESCOLHER_PRODUTO && tudofeito)
            {
               mvwaddch(stdscr, setapos, posx[0] - 2, ' ');
               setapos += 3;
               mvwaddch(stdscr, setapos, posx[0] - 2, '>');
               opc++;
               break;
            }
            if(opc < ESCOLHER_PRODUTO)
            {
               mvwaddch(stdscr, setapos, posx[0] - 2, ' ');
               setapos += (dist + 1);
               mvwaddch(stdscr, setapos, posx[0] - 2, '>');
               opc++;
            }
         }
         break;
         case '\n':
         echo();
         nocbreak();
         curs_set(TRUE);
         werase(subtelas[opc - 1]);
         switch(opc)
         {
            case IDPEDIDO:
            {
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerSizeT(&novoPedido->id, subtelas[opc - 1]);
               if(check == -3) goto pulouID;
               Node* repetido = findByID(listaPedidos, novoPedido->id);
               while(check != 1 || repetido)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == 0) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                  if(check == -1) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                  if(check == -2) {gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                  if(repetido) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este ID de pedido já existe");
                  erroDuplo:
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerSizeT(&novoPedido->id, subtelas[opc - 1]);
                  if(check == -3) goto pulouID;
                  curs_set(FALSE);
                  repetido = findByID(listaPedidos, novoPedido->id);
               }
               werase(subtelas[opc - 1]);
               mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoPedido->id);
               wrefresh(subtelas[opc - 1]);
               preenchidos[opc - 1] = 1;
            }
            break;
            pulouID:
            werase(subtelas[opc - 1]);
            if(preenchidos[opc - 1]) mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoPedido->id);
            wrefresh(subtelas[opc - 1]);
            break;
            case DATA:
            {
               char data[15];
               cbreak();
               werase(subtelas[opc - 1]);
               wmove(subtelas[opc - 1], 0, 0);
               keypad(subtelas[opc - 1], TRUE);
               short confirmou = lerData(data, subtelas[opc - 1]);
               if(!confirmou) goto pulouData;
               strcpy(novoPedido->date, data);
               preenchidos[opc - 1] = 1;
            }
            break;
            pulouData:
            werase(subtelas[opc - 1]);
            if(preenchidos[opc - 1]) mvwaddstr(subtelas[opc - 1], 0, 0, novoPedido->date);
            wrefresh(subtelas[opc - 1]);
            break;
            case ESCOLHER_CLIENTE:
            {
               WINDOW *cliente = newwin(maxstdy * (18.0/teladevy), maxstdx * (39.0/teladevx), posy[0], maxstdx * (148.0/teladevx));
               WINDOW *subcliente = derwin(cliente, maxstdy * (18.0/teladevy) * (16.0/18.0), maxstdx * (39.0/teladevx) * (37.0/39.0), 1, 1);
               wborder(cliente, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
               wattron(cliente, COLOR_PAIR(4));
               mvwaddstr(cliente, 0, 0, otalabel[0]);
               wattroff(cliente, COLOR_PAIR(4));
               wrefresh(cliente);

               Node* atual = listaClientes->head;

               cmd = KEY_LEFT;
               cbreak();
               noecho();
               curs_set(FALSE);
               while(cmd != '\n')
               {
                  switch(cmd)
                  {
                     case KEY_LEFT:
                     {
                        if(atual->prev)
                        {
                           atual = atual->prev;
                        }
                     }
                     break;
                     case KEY_RIGHT:
                     {
                        if(atual->next)
                        {
                           atual = atual->next;
                        }
                     }
                     break;
                     case 'Z':
                     case 'z':
                     goto pulouCliente;
                     break;
                     case 'I':
                     case 'i':
                     {
                        WINDOW *selid = newwin(maxstdy * (5.0/teladevy), maxstdx * (39.0/teladevx), posy[0] + maxstdy * (18.0/teladevy), maxstdx * (148.0/teladevx));
                        WINDOW *subselid = derwin(selid,maxstdy * (5.0/teladevy) * (3.0/5.0), maxstdx * (37.0/teladevx) * (37.0/39.0), 1, 1);
                        wborder(selid, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
                        mvwaddstr(selid, 0, 0, "Consultar ID");
                        wrefresh(selid);
                        echo();
                        nocbreak();
                        curs_set(TRUE);
                        wmove(subselid, 1, 1);
                        size_t targetID;
                        short check = lerSizeT(&targetID, subselid);
                        if(check == -3) goto cancelarID;
                        while(check != 1)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE); 
                           if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                           if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                           if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                           nocbreak();
                           echo();
                           curs_set(TRUE);
                           repetirID:
                           werase(subselid);
                           wrefresh(subselid);
                           wmove(subselid, 1, 1);
                           check = lerSizeT(&targetID, subselid);
                           if(check == -3) goto cancelarID;
                        }
                        Node* tmp = findByID(listaClientes, targetID);
                        if(!tmp)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE);
                           gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
                           nocbreak();
                           echo();
                           curs_set(TRUE);
                           goto repetirID;
                        }
                        atual = tmp;
                        cancelarID:
                        cortina(selid, 0, 0, maxstdy * (5.0/teladevy), 10);
                        delwin(subselid);
                        delwin(selid);
                        noecho();
                        cbreak();
                     }
                     break;
                  }

                  mvwprintw(subcliente, 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                   expand_node(atual, GenericCast)->data.id,
                                                   expand_node(atual, GenericCast)->data.name,
                                                   expand_node(atual, GenericCast)->data.address,
                                                   expand_node(atual, GenericCast)->data.phonenumber,
                                                   ((atual->dataType == PESSOA_FISICA) ? expand_node(atual, PessoaFisica)->cpf : expand_node(atual, PessoaJuridica)->cnpj));

                  wrefresh(subcliente);
                  curs_set(FALSE);
                  cmd = wgetch(stdscr);
               }
               novoPedido->customerId = expand_node(atual, GenericCast)->data.id;
               mvwprintw(subtelas[2], 0, 0, "%zu", novoPedido->customerId);
               wrefresh(subtelas[2]);
               preenchidos[opc - 1] = 1;

               pulouCliente:

               cortina(cliente, 0, 0, maxstdy * (18.0/teladevy), 5);
               delwin(subcliente);
               delwin(cliente);
            }
            break;
            case ESCOLHER_PRODUTO:
            {
               if(novoPedido->id == 0)
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de preenchimento", "Não há como adicionar produtos sem incluir um ID ao pedido antes");
                  break;
               }

               WINDOW *produto = newwin(maxstdy * (18.0/teladevy), maxstdx * (39.0/teladevx), posy[0], maxstdx * (148.0/teladevx));
               WINDOW *subproduto = derwin(produto, maxstdy * (18.0/teladevy) * (16.0/18.0), maxstdx * (39.0/teladevx) * (37.0/39.0), 1, 1);
               wborder(produto, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
               wattron(produto, COLOR_PAIR(4));
               mvwaddstr(produto, 0, 0, otalabel[1]);
               wattroff(produto, COLOR_PAIR(4));
               wrefresh(produto);

               Node* atual = listaProdutos->head;

               cmd = KEY_LEFT;
               cbreak();
               noecho();
               curs_set(FALSE);
               while(cmd != 'z' && cmd != 'Z')
               {
                  switch(cmd)
                  {
                     case KEY_RIGHT:
                     {
                        if(atual->next)
                        {
                           atual = atual->next;
                        }
                     }
                     break;
                     case KEY_LEFT:
                     {
                        if(atual->prev)
                        {
                           atual = atual->prev;
                        }
                     }
                     break;
                     case 'i':
                     case 'I':
                     {
                        WINDOW *selid = newwin(maxstdy * (5.0/teladevy), maxstdx * (39.0/teladevx), posy[0] + maxstdy * (18.0/teladevy), maxstdx * (148.0/teladevx));
                        WINDOW *subselid = derwin(selid,maxstdy * (5.0/teladevy) * (3.0/5.0), maxstdx * (37.0/teladevx) * (37.0/39.0), 1, 1);
                        wborder(selid, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
                        mvwaddstr(selid, 0, 0, "Consultar ID");
                        wrefresh(selid);
                        echo();
                        nocbreak();
                        curs_set(TRUE);
                        wmove(subselid, 1, 1);
                        size_t targetID;
                        short check = lerSizeT(&targetID, subselid);
                        if(check == -3) goto cancelarIDProd;
                        while(check != 1)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE); 
                           if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                           if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                           if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                           repetirIDProd:
                           nocbreak();
                           echo();
                           curs_set(TRUE);
                           werase(subselid);
                           wrefresh(subselid);
                           wmove(subselid, 1, 1);
                           check = lerSizeT(&targetID, subselid);
                           if(check == -3) goto cancelarIDProd;
                        }
                        Node* tmp = findByID(listaProdutos, targetID);
                        if(!tmp)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE);
                           gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
                           goto repetirIDProd;
                        }
                        atual = tmp;
                        cancelarIDProd:
                        cortina(selid, 0, 0, maxstdy * (5.0/teladevy), 10);
                        delwin(subselid);
                        delwin(selid);
                     }
                     break;
                     case '\n':
                     {
                        if(expand_node(atual, Produto)->stock == 0)
                        {
                           gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de requisição", "Este produto não está mais disponível");
                           break;
                        }

                        unsigned short ind = 0;
                        for(ind = 0; ind < novoPedido->itens._size; ind++)
                        {
                           if(access_index(novoPedido->itens, ItemPedido, ind).productId == expand_node(atual, Produto)->id)
                           {
                              goto itemRepetido;
                           }
                        }
                        
                        ItemPedido *novoItem = (ItemPedido*)malloc(sizeof(ItemPedido));
                        novoItem->requestId = novoPedido->id;
                        novoItem->productId = expand_node(atual, Produto)->id;

                        {
                           WINDOW *quantidade = newwin(maxstdy * (5.0/teladevy), maxstdx * (39.0/teladevx), posy[0] + maxstdy * (18.0/teladevy), maxstdx * (148.0/teladevx));
                           WINDOW *subquantidade = derwin(quantidade,maxstdy * (5.0/teladevy) * (3.0/5.0), maxstdx * (37.0/teladevx) * (37.0/39.0), 1, 1);
                           wborder(quantidade, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
                           mvwaddstr(quantidade, 0, 0, "Adicionar quantidade");
                           wrefresh(quantidade);
                           echo();
                           nocbreak();
                           wmove(subquantidade, 1, 1);
                           short check = lerSizeT(&novoItem->amount, subquantidade);
                           if(check == -3) goto pulouItem;
                           while(check != 1)
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                              if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                              if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                              repetirQuant:
                              nocbreak();
                              echo();
                              curs_set(TRUE);
                              werase(subquantidade);
                              wrefresh(subquantidade);
                              wmove(subquantidade, 1, 1);
                              check = lerSizeT(&novoItem->amount, subquantidade);
                              if(check == -3) goto pulouItem;
                           }
                           if(novoItem->amount > expand_node(atual, Produto)->stock)
                           {
                              cbreak();
                              noecho();
                              curs_set(FALSE);
                              gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de requisição", "A quantidade requisitada é maior que a quantidade disponível em estoque");
                              goto repetirQuant;
                           }
                           expand_node(atual, Produto)->stock -= novoItem->amount;
                           novoItem->subtotal = novoItem->amount * expand_node(atual, Produto)->price;
                           push_back(&novoPedido->itens, novoItem);
                           cortina(quantidade, 0, 0, maxstdy * (5.0/teladevy), 10);
                           delwin(subquantidade);
                           delwin(quantidade);
                           break;
                           pulouItem:
                           cortina(quantidade, 0, 0, maxstdy * (5.0/teladevy), 10);
                           delwin(quantidade);
                           delwin(subquantidade);
                           free(novoItem);
                        }
                        
                        break;
                        itemRepetido:
                        size_t quant = 0;
                        WINDOW *quantidade = newwin(maxstdy * (5.0/teladevy), maxstdx * (39.0/teladevx), posy[0] + maxstdy * (18.0/teladevy), maxstdx * (148.0/teladevx));
                        WINDOW *subquantidade = derwin(quantidade,maxstdy * (5.0/teladevy) * (3.0/5.0), maxstdx * (37.0/teladevx) * (37.0/39.0), 1, 1);
                        wborder(quantidade, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
                        mvwaddstr(quantidade, 0, 0, "Adicionar quantidade");
                        wrefresh(quantidade);
                        echo();
                        nocbreak();
                        wmove(subquantidade, 1, 1);
                        short check = lerSizeT(&quant, subquantidade);
                        if(check == -3) goto pulouItemRepetido;
                        while(check != 1)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE);
                           if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                           if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                           if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                           repetirQuantRep:
                           nocbreak();
                           echo();
                           curs_set(TRUE);
                           werase(subquantidade);
                           wrefresh(subquantidade);
                           wmove(subquantidade, 1, 1);
                           check = lerSizeT(&quant, subquantidade);
                           if(check == -3) goto pulouItemRepetido;
                        }
                        if(quant > expand_node(atual, Produto)->stock)
                        {
                           cbreak();
                           noecho();
                           curs_set(FALSE);
                           gerarAviso(ytamerro, xtamerro, yposerro, xposerro, "||~~OOOO", "Erro de requisição", "A quantidade requisitada é maior que a quantidade disponível em estoque");
                           goto repetirQuantRep;
                        }
                        expand_node(atual, Produto)->stock -= quant;
                        access_index(novoPedido->itens, ItemPedido, ind).amount += quant;
                        access_index(novoPedido->itens, ItemPedido, ind).subtotal = access_index(novoPedido->itens, ItemPedido, ind).amount * expand_node(atual, Produto)->price;
                        cortina(quantidade, 0, 0, maxstdy * (5.0/teladevy), 10);
                        delwin(subquantidade);
                        delwin(quantidade);
                        break;
                        pulouItemRepetido:
                        cortina(quantidade, 0, 0, maxstdy * (5.0/teladevy), 10);
                        delwin(quantidade);
                        delwin(subquantidade);
                        break;
                     }
                  }
                  werase(subproduto);
                  mvwprintw(subproduto, 0, 0, "ID: %zu\n\nDescrição: %s\n\nPreço: %.2lf\n\nEm estoque: %zu",
                                                   expand_node(atual, Produto)->id,
                                                   expand_node(atual, Produto)->description,
                                                   expand_node(atual, Produto)->price,
                                                   expand_node(atual, Produto)->stock);

                  wrefresh(subproduto);
                  curs_set(FALSE);
                  cbreak();
                  noecho();
                  werase(subtelas[3]);
                  wmove(subtelas[3], 0, 0);
                  double soma = 0;
                  for(unsigned short i = 0; i < novoPedido->itens._size; i++)
                  {
                     
                     soma += access_index(novoPedido->itens, ItemPedido, i).subtotal;
                     wprintw(subtelas[3], "Produto: %zu - Quantidade: %zu\nSubtotal: R$ %.2lf\n", 
                        access_index(novoPedido->itens, ItemPedido, i).productId,
                        access_index(novoPedido->itens, ItemPedido, i).amount,
                        access_index(novoPedido->itens, ItemPedido, i).subtotal);
                  }
                  mvwprintw(subtelas[4], (tamabay[4] * (refy[4]/tamabay[4])) * 0.5 - 1, 3, "R$ %.2lf", soma);
                  wnoutrefresh(subtelas[3]);
                  wnoutrefresh(subtelas[4]);
                  doupdate();
                  cmd = wgetch(stdscr);
               }
               cortina(produto, 0, 0, maxstdy * (18.0/teladevy), 10);
               delwin(subproduto);
               delwin(produto);
            }
            if(novoPedido->itens._size != 0) preenchidos[opc - 1] = 1;
            break;
            case FINALIZAR_PEDIDO:
            preenchidos[opc - 1] = 1;
            break;
         }
         tudofeito = 1;
         for(short i = 0; i < 4; i++) if(!preenchidos[i]) tudofeito = 0;
         if(tudofeito)
         {
            wattron(stdscr, COLOR_PAIR(1));
            mvwaddstr(stdscr, posy[0] + tamabay[0] * 0.5 + dist * 3 + 6, posx[0], "Salvar pedido");
            wattroff(stdscr, COLOR_PAIR(1));
         }
         cbreak();
         noecho();
         break;
      }
      wrefresh(stdscr);
      curs_set(FALSE);
      if(!preenchidos[4]) cmd = wgetch(stdscr);
   }

   for(int i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 0, tamabay[i], 10);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   cortina(stdscr, 0, setapos - 1, setapos + 1, 1);
   cortina(stdscr, 0, posy[0] + tamabay[0] * 0.5 + dist * 3 - 2, maxstdy, 1);

   return novoPedido;

   escape:

   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 0, tamabay[i], 10);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   cortina(stdscr, 0, setapos - 1, setapos + 1, 1);
   cortina(stdscr, 0, posy[0] + tamabay[0] * 0.5 + dist * 3 - 2, maxstdy, 1);

   for(short i = 0; i < novoPedido->itens._size; i++) 
   {  
      expand_node(findByID(listaProdutos, access_index(novoPedido->itens, ItemPedido, i).productId), Produto)->stock += access_index(novoPedido->itens, ItemPedido, i).amount;
      free(novoPedido->itens.container[i]);
   }

   free(novoPedido);

   return NULL;
}

void ListarPedidos(List *listaPedidos)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   unsigned int n;
   (listaPedidos->tam % 3 == 0 ? (n = listaPedidos->tam / 3) : (n = listaPedidos->tam / 3 + 1));

   Node *paginas[n];

   // if(!listaPedidos->head) goto listaVazia;

   Node *atual = listaPedidos->head;
   size_t ind = 0;
   for(unsigned int cont = 1; atual; cont++, atual = atual->next)
   {
      if(cont % 3 == 1)
      {
         paginas[ind] = atual;
         ind++;
      }
   }

   unsigned short posy[] = {maxstdy * (15.0/teladevy),
                            maxstdy * (22.0/teladevy),
                            maxstdy * (29.0/teladevy)};

   unsigned short posx[] = {maxstdx * (10.0/teladevx),
                            posx[0],
                            posx[0]};

   WINDOW *telas[7];
   WINDOW *subtelas[7];

   for(short i = 0; i < 3; i++)
   {
      telas[i] = newwin(6, 35, posy[i], posx[i]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], 4, 33, 1, 1);
      wnoutrefresh(telas[i]);
   }

   doupdate();

   getch();

   for(short i = 0; i < 3; i++)
   {
      cortina(telas[i], 0, 0, 6, 5);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
}