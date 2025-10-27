#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "safetyutils.h"
#include "menus.h"

void lerSizeT(size_t *valor, WINDOW *tela)
{
   short y, x;
   long long intermediario;
   getyx(tela, y, x);
   keypad(tela, FALSE);
   echo();
   nocbreak();
   short verificarLeitura = 0;
   while(verificarLeitura <= 0 || intermediario < 0)
   {
      curs_set(TRUE);
      intermediario = 0;
      wmove(tela, y, x);
      wclrtoeol(tela);
      verificarLeitura = mvwscanw(tela, y, x, "%lld", &intermediario);
      
      if(verificarLeitura <= 0 || intermediario < 0)
      {
         curs_set(FALSE);
         noecho();
         cbreak();
         WINDOW *erro = newwin(10, 36, 1, 86);
         wborder(erro, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
         mvwaddstr(erro, 1, 1, "Erro de formatação");
         mvwaddstr(erro, 3, 1, "Este campo aceita apenas números");
         mvwaddstr(erro, 4, 1, "inteiros não negativos");
         mvwaddstr(erro, 6, 1, "Pressione qualquer tecla");
         mvwaddstr(erro, 7, 1, "para prosseguir");
         keypad(erro, FALSE);
         wgetch(erro);
         transicao1(erro, 0, 10);
         delwin(erro);
         wrefresh(tela);
         echo();
         nocbreak();
      }
   }
   // while(wgetch(tela) != '\n');
   *valor = intermediario;
   keypad(tela, TRUE);
   noecho();
   cbreak();
}