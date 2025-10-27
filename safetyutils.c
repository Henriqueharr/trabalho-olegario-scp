#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "safetyutils.h"
#include "menus.h"

unsigned int validarStr(const char *str)
{
   if(*str == ' ') return 0;
   while(*str++)
   {
      if(*str != ' ') return 1;
   }
   return 0;
}

void lerSizeT(size_t *valor, WINDOW *tela)
{
   short y, x, maxy;
   long long intermediario;
   getyx(tela, y, x);
   maxy = getmaxy(tela);
   keypad(tela, FALSE);
   echo();
   nocbreak();
   short verificarLeitura = 0;
   while(verificarLeitura <= 0 || intermediario < 0)
   {
      curs_set(TRUE);
      intermediario = 0;
      transicao1(tela, 0, maxy);
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

void lerStr(char *str, size_t buffer, WINDOW *tela)
{
   unsigned short x, y, maxy = getmaxy(tela);
   getyx(tela, y, x);
   keypad(tela, FALSE);
   echo();
   nocbreak();
   mvwgetnstr(tela, y, x, str, buffer);
   while(!validarStr(str))
   {
      curs_set(TRUE);
      transicao1(tela, 0, maxy);
      
      if(!validarStr(str))
      {
         curs_set(FALSE);
         noecho();
         cbreak();
         WINDOW *erro = newwin(10, 36, 1, 86);
         wborder(erro, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
         mvwaddstr(erro, 1, 1, "Erro de formatação");//////////*
         mvwaddstr(erro, 3, 1, "Este campo não aceita entradas");
         mvwaddstr(erro, 4, 1, "vazias ou iniciadas com espaço");
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
      mvwgetnstr(tela, y, x, str, buffer);
   }
   keypad(tela, TRUE);
   noecho();
   cbreak();
}