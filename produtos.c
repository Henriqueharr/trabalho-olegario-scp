#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "safetyutils.h"
#include "doublylinkedlist.h"
#include "produtos.h"
#include "menus.h"

extern double teladevy, teladevx;

Produto* criarProduto(List *listaProdutos)
{
   curs_set(FALSE);
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   const char* label[] = {"ID", "Descrição", "Preço", "Estoque"};

   unsigned short tamabay = maxstdy * (15.0/teladevy);
   unsigned short tamabax = maxstdx * (35.0/teladevx);
   unsigned short ypos[] = {maxstdy * (7.0/teladevy), maxstdy * (7.0/teladevy), maxstdy * (26.0/teladevy), maxstdy * (26.0/teladevy)};
   unsigned short xpos[] = {maxstdx * 0.5 - (tamabax * 0.5), maxstdx * 0.5 + (tamabax * 0.5) + tamabax * 0.35, maxstdx * 0.5 - (tamabax * 0.5), maxstdx * 0.5 + (tamabax * 0.5) + tamabax * 0.35};
   unsigned short erroposy = maxstdy * (20.0/teladevy) + 6;
   unsigned short erroposx = maxstdx * (95.0/teladevx) - maxstdx * (50.0/teladevx);

   WINDOW *telas[4];
   WINDOW *subtelas[4];

   for(short i = 0; i < 4; i++)
   {
      telas[i] = newwin(tamabay, tamabax, ypos[i], xpos[i]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], tamabay * (11.0/15.0), tamabax * (31.0/35.0), tamabay * (2.0/15.0), tamabax * (2.0/35.0));
      wattron(telas[i], COLOR_PAIR(4));
      mvwaddstr(telas[i], 0, 0, label[i]);
      wattroff(telas[i], COLOR_PAIR(4));
      wrefresh(telas[i]);
      overlay(telas[i], stdscr);
      overlay(subtelas[i], stdscr);
      napms(15);
   }

   unsigned short baixo = 0, direita = 0;
   unsigned short setaposy[2][2] = {{ypos[0] + tamabay * 0.5, ypos[1] + tamabay * 0.5},
                                    {ypos[2] + tamabay * 0.5, ypos[3] + tamabay * 0.5}};
   unsigned short setaposx[2][2] = {{xpos[0] - 2, xpos[1] - 2}, 
                                    {xpos[2] - 2, xpos[3] - 2}};


   Produto *novoProduto = (Produto*)malloc(sizeof(Produto));
   novoProduto->price = 0;
   novoProduto->id = 0;

   MenuCriarProduto opc = IDPROD;
   unsigned short cmd = KEY_UP;
   unsigned short preenchidos[5] = {};
   unsigned short tudofeito = 0;
   mvwaddch(stdscr, setaposy[0][0], setaposx[0][0], '>');
   keypad(stdscr, TRUE);
   noecho();
   cbreak();
   while(!preenchidos[4])
   {
      cmd = wgetch(stdscr);

      switch(cmd)
      {
         case 'Z':
         case 'z':
         {
            mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
            wrefresh(stdscr);
            goto escape;
         }
         break;
         case KEY_UP:
         {
            if(opc == FINALIZARPROD)
            {
               mvwaddch(stdscr, maxstdy * (44.0/teladevy), maxstdx * 0.5 - (tamabax * 0.5) + (maxstdx * (33.0/teladevx)) - 2, ' ');
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
               opc = ADD_PRODUTO;
            }
            else 
            {
               if(baixo)
               {
                  mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
                  baixo = !baixo;
                  mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
               }
            }
         }
         break;
         case KEY_DOWN:
         {
            if(!baixo)
            {
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
               baixo = !baixo;
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
            }
            else
            {
               if(baixo && tudofeito)
               {
                  mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
                  mvwaddch(stdscr, maxstdy * (44.0/teladevy), maxstdx * 0.5 - (tamabax * 0.5) + (maxstdx * (33.0/teladevx)) - 2, '>');
                  opc = FINALIZARPROD;
               }
            }
         }
         break;
         case KEY_RIGHT:
         {
            if(!direita && opc != FINALIZARPROD)
            {
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
               direita = !direita;
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
            }
         }
         break;
         case KEY_LEFT:
         {
            if(direita && opc != FINALIZARPROD)
            {
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
               direita = !direita;
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
            }
         }
         break;
         case '\n':
         {
            nocbreak();
            echo();
            curs_set(TRUE);
            wmove(subtelas[opc - 1], 0, 0);
            werase(subtelas[opc - 1]);
            wrefresh(subtelas[opc - 1]);
            switch(opc)
            {
               case IDPROD:
               {
                  short check = lerSizeT(&novoProduto->id, subtelas[opc - 1]);
                  if(check == -3) goto pulouID;
                  Node *repetido = findByID(listaProdutos, novoProduto->id);
                  while(check != 1 || repetido)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     if(check == 0) {gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                     if(check == -1) {gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                     if(check == -2) {gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                     if(repetido) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este ID de produto já existe");
                     erroDuplo:
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     check = lerSizeT(&novoProduto->id, subtelas[opc - 1]);
                     if(check == -3) goto pulouID;
                     curs_set(FALSE);
                     repetido = findByID(listaProdutos, novoProduto->id);
                  }
                  werase(subtelas[opc - 1]);
                  mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoProduto->id);
                  wrefresh(subtelas[opc - 1]);
                  preenchidos[opc - 1] = 1;
               }
               break;

               pulouID:
               werase(subtelas[opc - 1]);
               if(preenchidos[opc - 1]) mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoProduto->id);
               wrefresh(subtelas[opc - 1]);
               break;

               case DESCRICAO:
               {
                  short check = lerStr(novoProduto->description, sizeof(novoProduto->description), subtelas[opc - 1]);
                  while(check != 1)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     check = lerStr(novoProduto->description, sizeof(novoProduto->description), subtelas[opc - 1]);
                  }
                  preenchidos[opc - 1] = 1;
               }
               break;
               case PRECO:
               {
                  short check = lerDouble(subtelas[opc - 1], &novoProduto->price);
                  if(check == -3) goto pulouPreco;
                  while(check != 1)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     if(check == 0) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                     if(check == -1) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                     if(check == -2) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     check = lerDouble(subtelas[opc - 1], &novoProduto->price);
                     if(check == -3) goto pulouPreco;
                  }
                  werase(subtelas[opc - 1]);
                  mvwprintw(subtelas[opc - 1], 0, 0, "%.2lf", novoProduto->price);
                  wrefresh(subtelas[opc - 1]);
                  preenchidos[opc - 1] = 1;
               }
               break;

               pulouPreco:
               werase(subtelas[opc - 1]);
               if(preenchidos[opc - 1]) mvwprintw(subtelas[opc - 1], 0, 0, "%.2lf", novoProduto->price);
               wrefresh(subtelas[opc - 1]);
               break;

               case ESTOQUE:
               {
                  short check = lerSizeT(&novoProduto->stock, subtelas[opc - 1]);
                  if(check == -3) goto pulouEstoque;
                  while(check != 1)
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     if(check == 0) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                     if(check == -1) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                     if(check == -2) gerarAviso(tamabay, tamabax, erroposy, erroposx, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     check = lerSizeT(&novoProduto->stock, subtelas[opc - 1]);
                     if(check == -3) goto pulouID;
                  }
                  werase(subtelas[opc - 1]);
                  mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoProduto->stock);
                  wrefresh(subtelas[opc - 1]);
                  preenchidos[opc - 1] = 1;
               }
               break;

               pulouEstoque:
               werase(subtelas[opc - 1]);
               if(preenchidos[opc - 1]) mvwprintw(subtelas[opc - 1], 0, 0, "%zu", novoProduto->stock);
               wrefresh(subtelas[opc - 1]);
               break;

               case FINALIZARPROD:
               preenchidos[opc - 1] = 1;
               break;
            }
         }
         tudofeito = 1;
         for(short i = 0; i < 4; i++) if(!preenchidos[i]) tudofeito = 0;
         if(tudofeito)
         {
            wattron(stdscr, COLOR_PAIR(1));
            mvwaddstr(stdscr, maxstdy * (44.0/teladevy), maxstdx * 0.5 - (tamabax * 0.5) + (maxstdx * (33.0/teladevx)), "Salvar produto");
            wattroff(stdscr, COLOR_PAIR(1));
         }
         break;
      }

      if(opc != FINALIZARPROD)
      {
         opc = IDPROD;
         if(direita) opc++;
         if(baixo) opc += 2;
      }

      keypad(stdscr, TRUE);
      noecho();
      cbreak();
      curs_set(FALSE);
      wrefresh(stdscr);
   }

   curs_set(FALSE);

   wmove(stdscr, maxstdy * (44.0/teladevy), 0);
   wclrtoeol(stdscr);
   wrefresh(stdscr);

   for(short i = 0; i < 4; i++)
   {
      cortina(telas[i], 0, 0, tamabay, 10);
      delwin(telas[i]);
      delwin(subtelas[i]);
   }

   return novoProduto;

   escape:

   for(short i = 0; i < 4; i++)
   {
      cortina(telas[i], 0, 0, tamabay, 10);
      delwin(telas[i]);
      delwin(subtelas[i]);
   }

   free(novoProduto);

   return NULL;
}

void ListarProdutos(List *listaProd)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   unsigned short ytamerro = maxstdy * (10.0/teladevy);
   unsigned short xtamerro = maxstdx * (30.0/teladevx);
   unsigned short yposerro = maxstdy * (2.0/teladevy);

   unsigned int n;
   size_t ind = 0;
   (listaProd->tam % 6 == 0 ? (n = listaProd->tam / 6) : (n = listaProd->tam / 6 + 1));

   Node *paginas[n];

   if(!listaProd->head) goto listaVazia;

   Node* atual = listaProd->head;

   for(unsigned int cont = 1; atual; atual = atual->next, cont++)
   {
      if(cont % 6 == 1)
      {
         paginas[ind] = atual;
         ind++;
      }
   }

   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Direita : Avançar página");
   mvwaddstr(stdscr, 2, 1, "Esquerda: Retroceder página");
   mvwaddstr(stdscr, 3, 1, "P/p     : Consultar por página");
   mvwaddstr(stdscr, 4, 1, "I/i     : Consultar por ID");
   mvwaddstr(stdscr, 5, 1, "Enter   : Confirmar");
   mvwaddstr(stdscr, 6, 1, "Z/z     : Voltar");

   unsigned short yascii = maxstdy * (28.0/teladevy);
   unsigned short xascii1 = maxstdx * (2.0/teladevx);
   unsigned short xascii2 = maxstdx * (204.0/teladevx);

   mvwaddstr(stdscr, yascii + 0, xascii1, "  ██╗");
   mvwaddstr(stdscr, yascii + 1, xascii1, " ██╔╝");
   mvwaddstr(stdscr, yascii + 2, xascii1, "██╔╝ ");
   mvwaddstr(stdscr, yascii + 3, xascii1, "╚██╗ ");
   mvwaddstr(stdscr, yascii + 4, xascii1, " ╚██╗");
   mvwaddstr(stdscr, yascii + 5, xascii1, "  ╚═╝");
   mvwaddstr(stdscr, yascii + 0, xascii2, "██╗  ");
   mvwaddstr(stdscr, yascii + 1, xascii2, "╚██╗ ");
   mvwaddstr(stdscr, yascii + 2, xascii2, " ╚██╗");
   mvwaddstr(stdscr, yascii + 3, xascii2, " ██╔╝");
   mvwaddstr(stdscr, yascii + 4, xascii2, "██╔╝ ");
   mvwaddstr(stdscr, yascii + 5, xascii2, "╚═╝  ");

   wnoutrefresh(stdscr);

   unsigned short yi = maxstdy * (15.0/teladevy), xi[] = {maxstdx * (15.0/teladevx), maxstdx * 0.5 - (maxstdx * (50.0/teladevx)) * 0.5, maxstdx * (146.0/teladevx)};

   logo(maxstdy * (4.0/teladevy), maxstdx * 0.5 - 11);
   
   WINDOW *selecaoPagina = newwin(3, 19, yi - 3, xi[0]);
   WINDOW *subsel = derwin(selecaoPagina, 1, 8, 1, 1);
   wborder(selecaoPagina, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
   mvwaddstr(selecaoPagina, 0, 0, "Página");
   mvwprintw(selecaoPagina, 1, 1, "        /%d", n);
   wnoutrefresh(selecaoPagina);
   
   WINDOW *telas[6];
   WINDOW *subtelas[6];

   unsigned short ytamaba = yi;
   unsigned short xtamaba = maxstdx * (50.0/teladevx);
   
   for(unsigned short i = 0; i < 6; i++)
   {
      telas[i] = newwin(ytamaba, xtamaba, yi, xi[i % 3]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], ytamaba - 2, xtamaba - 2, 1, 1);
      wnoutrefresh(telas[i]);
      if(i == 2) yi += (ytamaba + 2);
   }
   
   wnoutrefresh(stdscr);

   ind = 1;
   atual = paginas[0];

   cbreak();
   noecho();
   keypad(stdscr, TRUE);
   unsigned short cmd = KEY_LEFT;
   while(cmd != 'Z' && cmd != 'z')
   {
      switch(cmd)
      {
         case KEY_LEFT:
         {
            if(ind > 0)
            {
               ind--;
               werase(subsel);
               mvwprintw(subsel, 0, 0, "%zu", ind + 1);
               wnoutrefresh(subsel);
               atual = paginas[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wnoutrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nDescrição: %s\n\nPreço: %.2lf\n\nEm estoque: %zu",
                                                   expand_node(atual, Produto)->id,
                                                   expand_node(atual, Produto)->description,
                                                   expand_node(atual, Produto)->price,
                                                   expand_node(atual, Produto)->stock);
                     
                     wnoutrefresh(subtelas[i]);
                     atual = atual->next;
                  }
               }
            }
         }
         break;
         case KEY_RIGHT:
         {
            if(ind < n - 1)
            {
               ind++;
               werase(subsel);
               mvwprintw(subsel, 0, 0, "%zu", ind + 1);
               wnoutrefresh(subsel);
               atual = paginas[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wnoutrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nDescrição: %s\n\nPreço: %.2lf\n\nEm estoque: %zu",
                                                   expand_node(atual, Produto)->id,
                                                   expand_node(atual, Produto)->description,
                                                   expand_node(atual, Produto)->price,
                                                   expand_node(atual, Produto)->stock);
                                                   
                     wnoutrefresh(subtelas[i]);
                     atual = atual->next;
                  }
               }
            }
         }
         break;
         case 'P':
         case 'p':
         {
            echo();
            nocbreak();
            curs_set(TRUE);
            werase(subsel);
            wrefresh(subsel);
            wmove(subsel, 0, 0);
            short check = lerSizeT(&ind, subsel);
            while(check != 1 || ind > n || ind <= 0)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               if(check == 0) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                  goto duploerro;
               }
               if(check == -1) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                  goto duploerro;
               }
               if(check == -2) 
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                  goto duploerro;
               }
               if(ind > n || ind <= 0)
               {
                  gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "A página requisitada não existe");
               }
               duploerro:
               werase(subsel);
               wrefresh(subsel);
               nocbreak();
               echo();
               curs_set(TRUE);
               check = lerSizeT(&ind, subsel);
            }
            curs_set(FALSE);
            cbreak();
            noecho();
            ind--;
            werase(subsel);
            mvwprintw(subsel, 0, 0, "%zu", ind + 1);
            wnoutrefresh(subsel);
            atual = paginas[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wnoutrefresh(subtelas[i]);
               if(atual)
               {
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nDescrição: %s\n\nPreço: %.2lf\n\nEm estoque: %zu",
                                                expand_node(atual, Produto)->id,
                                                expand_node(atual, Produto)->description,
                                                expand_node(atual, Produto)->price,
                                                expand_node(atual, Produto)->stock);
                                                
                  wnoutrefresh(subtelas[i]);
                  atual = atual->next;
               }
            }
         }
         break;
         case 'I':
         case 'i':
         {
            WINDOW *selid = newwin(3, 25, ytamaba - 6, xi[0]);
            WINDOW *subselid = derwin(selid, 1, 23, 1, 1);
            wborder(selid, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
            mvwaddstr(selid, 0, 0, "Consultar ID");
            wrefresh(selid);
            echo();
            nocbreak();
            curs_set(TRUE);
            wmove(subselid, 0, 0);
            size_t targetID;
            short check = lerSizeT(&targetID, subselid);
            if(check == -3) goto cancelarID;
            while(check != 1)
            {
               cbreak();
               noecho();
               curs_set(FALSE); 
               if(check == 0) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
               if(check == -1) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
               if(check == -2) gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
               nocbreak();
               echo();
               curs_set(TRUE);
               repetirID:
               werase(subselid);
               wrefresh(subselid);
               wmove(subselid, 1, 3);
               check = lerSizeT(&targetID, subselid);
               if(check == -3) goto cancelarID;
            }
            int dist = pathToCNode(listaProd, targetID);
            if(dist == -1)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               gerarAviso(ytamerro, xtamerro, yposerro, xi[2] + (xtamaba - xtamerro) * 0.5, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
               nocbreak();
               echo();
               curs_set(TRUE);
               goto repetirID;
            }
            ind = dist / 6;
            werase(subsel);
            mvwprintw(subsel, 0, 0, "%zu", ind + 1);
            wnoutrefresh(subsel);
            atual = paginas[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wnoutrefresh(subtelas[i]);
               if(atual)
               {
                  if(expand_node(atual, Produto)->id == targetID) wattron(subtelas[i], COLOR_PAIR(1));
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nDescrição: %s\n\nPreço: %.2lf\n\nEm estoque: %zu",
                                                expand_node(atual, Produto)->id,
                                                expand_node(atual, Produto)->description,
                                                expand_node(atual, Produto)->price,
                                                expand_node(atual, Produto)->stock);
                                                
                  wnoutrefresh(subtelas[i]);
                  wattroff(subtelas[i], COLOR_PAIR(1));
                  atual = atual->next;
               }
            }
            cancelarID:
            cortina(selid, 0, 0, 3, 25);
            delwin(subselid);
            delwin(selid);
            noecho();
            cbreak();
         }
         break;
      }
      curs_set(FALSE);
      doupdate();
      cmd = wgetch(stdscr);
   }

   for(unsigned short i = 0; i < 6; i++)
   {
      abrir(telas[i], round(ytamaba * 0.5), round(ytamaba * 0.5), 10);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   return;
   listaVazia:
   curs_set(FALSE);
   gerarAviso(ytamerro, xtamerro, maxstdy * 0.5 - (ytamerro) * 0.5, maxstdx * 0.5 - (xtamerro) * 0.5, "||~~OOOO", "Erro de carregamento", "A lista de produtos encontra-se vazia no momento");
   curs_set(TRUE);
}