#include <stdlib.h>
#include <ncurses.h>
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
      subtelas[i] = derwin(telas[i], tamabay * (11.0/tamabay), tamabax * (31.0/tamabax), tamabay * (2.0/tamabay), tamabax * (2.0/tamabax));
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
            if(!direita)
            {
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], ' ');
               direita = !direita;
               mvwaddch(stdscr, setaposy[baixo][direita], setaposx[baixo][direita], '>');
            }
         }
         break;
         case KEY_LEFT:
         {
            if(direita)
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


   return NULL;
}