#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "produtos.h"
#include "pedidos.h"
#include "menus.h"
#include "doublylinkedlist.h"

double teladevy = 49.0, teladevx = 211.0;

void gerarErro(unsigned short altura, unsigned short largura, unsigned short yi, unsigned short xi, const char *bordas, const char *titulo, const char *msg)
{
   WINDOW *erro = newwin(altura, largura, yi, xi);
   WINDOW *subtela = derwin(erro, altura - 2, largura - 2, 1, 1);
   wborder(erro, bordas[0], bordas[1], bordas[2], bordas[3], bordas[4], bordas[5], bordas[6], bordas[7]);
   mvwprintw(subtela, 0, 0, "%s\n\n%s\n\nPressione qualquer tecla para continuar", titulo, msg);
   keypad(erro, FALSE);
   wgetch(erro);
   cortina(erro, 0, altura, 20);
   delwin(subtela);
   delwin(erro);
}

void cortina(WINDOW * tela, short yi, short yf, unsigned short delay)
{
   curs_set(FALSE);
   unsigned short x, y;
   getyx(tela, y, x);
   for(short i = yi; i <= yf; i++) {wmove(tela, i, 0); wclrtoeol(tela); wrefresh(tela); napms(delay);};
   wmove(tela, y, x);
   curs_set(TRUE);
}

void slideLeft(WINDOW *tela, unsigned short relx, unsigned short yi, unsigned short yf, unsigned short pad, unsigned short delay)
{
   curs_set(FALSE);
   for(unsigned short i = yi; i <= yf; i++)
   {
      wmove(tela, i, relx);
      for(unsigned short j = 0; j < pad; j++)
      {
         wdelch(tela);
         wrefresh(tela);
         napms(delay);
      }
   }
   curs_set(TRUE);
}

void abrir(WINDOW *tela, unsigned short uppery, unsigned short abertura, unsigned short delay)
{
   curs_set(FALSE);
   unsigned short ys, yd;
   (abertura % 2 ? (ys = uppery, yd = uppery + 1) : (ys = yd = uppery));
   for(unsigned short i = 0; i < abertura; ys--, yd++, i++)
   {
      wmove(tela, ys, 0);
      wclrtoeol(tela);
      wmove(tela, yd, 0);
      wclrtoeol(tela);
      wrefresh(tela);
      napms(delay);
   }
   curs_set(TRUE);
}

void logo(unsigned short y, unsigned short x)
{
   mvwaddstr(stdscr, y + 0, x, "███████╗ ██████╗██████╗");
   mvwaddstr(stdscr, y + 1, x, "██╔════╝██╔════╝██╔══██╗");
   mvwaddstr(stdscr, y + 2, x, "███████╗██║     ██████╔╝");
   mvwaddstr(stdscr, y + 3, x, "╚════██║██║     ██╔═══╝");
   mvwaddstr(stdscr, y + 4, x, "███████║╚██████╗██║");
   mvwaddstr(stdscr, y + 5, x, "╚══════╝ ╚═════╝╚═╝");
}

void showMainMenu(MenuPrincipal *opc)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   werase(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 2, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 3, 1, "Enter: Confirmar/Selecionar");
   mvwaddstr(stdscr, 4, 1, "Z/z  : Voltar");

   unsigned short xpadrao = maxstdx * (95.0/teladevx), ypadrao = maxstdy * (20.0/teladevy);
   
   logo(ypadrao, xpadrao);
   mvwaddstr(stdscr,ypadrao + 7,xpadrao,"Selecione um opção: ");
   mvwaddstr(stdscr,ypadrao + 9,xpadrao + 3,"Menu de clientes");
   mvwaddstr(stdscr,ypadrao + 10,xpadrao + 3,"Menu de produtos");
   mvwaddstr(stdscr,ypadrao + 11,xpadrao + 3,"Menu de pedidos");
   mvwaddstr(stdscr,ypadrao + 12,xpadrao + 3,"Encerrar programa");
   
   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = ypadrao + 9;
   *opc = MENU_CLIENTES;

   mvwaddch(stdscr, stdy, xpadrao, '>');
   keypad(stdscr, TRUE);

   while(menuKey != '\n')
   {
      switch(menuKey)
      {
         case 'Z':
         case 'z':
         (*opc) = ENCERRAR_PROGRAMA;
         goto fim;
         break;
         case KEY_UP: 
         if(*opc > 1)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*opc)--;
         }   
         break;

         case KEY_DOWN:
         if(*opc < 4)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*opc)++;
         }
         break;
      }
      wrefresh(stdscr);
      menuKey = wgetch(stdscr);
   }
   fim:
   
   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}

void showCustomerMenu(MenuCliente *opc)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   werase(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 2, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 3, 1, "Enter: Confirmar/Selecionar");
   mvwaddstr(stdscr, 4, 1, "Z/z  : Voltar");

   unsigned short xpadrao = maxstdx * (95.0/teladevx), ypadrao = maxstdy * (20.0/teladevy);

   logo(ypadrao, xpadrao);
   mvwaddstr(stdscr,ypadrao + 7,xpadrao,"Selecione um opção: ");
   mvwaddstr(stdscr,ypadrao + 9,xpadrao + 3,"Inserir cliente");
   mvwaddstr(stdscr,ypadrao + 10,xpadrao + 3,"Listar clientes");
   mvwaddstr(stdscr,ypadrao + 11,xpadrao + 3,"Editar cliente");
   mvwaddstr(stdscr,ypadrao + 12,xpadrao + 3,"Remover cliente");
   mvwaddstr(stdscr,ypadrao + 13,xpadrao + 3,"Salvar clientes em arquivo");
   mvwaddstr(stdscr,ypadrao + 14,xpadrao + 3,"Carregar arquivo de clientes");
   mvwaddstr(stdscr,ypadrao + 15,xpadrao + 3,"Voltar ao menu principal");
   
   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = ypadrao + 9;
   *opc = ADD_CLIENTE;

   mvwaddch(stdscr, stdy, xpadrao, '>');
   keypad(stdscr, TRUE);

   while(menuKey != '\n')
   {
      switch(menuKey)
      {
         case 'Z':
         case 'z':
         (*opc) = VOLTAR_CLIENTE;
         goto fim;
         break;
         case KEY_UP: 
         if(*opc > 1)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*opc)--;
         }   
         break;

         case KEY_DOWN:
         if(*opc < 7)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*opc)++;
         }
         break;
      }
      wrefresh(stdscr);
      menuKey = wgetch(stdscr);
   }
   fim:

   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}

void showAddCustomerMenu(Tipos *dataType)
{
   curs_set(FALSE);
   noecho();
   cbreak();

   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   unsigned short ypadrao = maxstdy * (21.0/teladevy), xpadrao = maxstdx * (75.0/teladevx);

   mvwaddstr(stdscr, ypadrao, xpadrao, "Escolha o tipo de cliente:");
   mvwaddstr(stdscr, ypadrao + 2, xpadrao + 3, "Pessoa física");
   mvwaddstr(stdscr, ypadrao + 3, xpadrao + 3, "Pessoa jurídica");
   mvwaddch(stdscr, ypadrao + 2, xpadrao, '>');

   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = ypadrao + 2;
   *dataType = PESSOA_FISICA;

   keypad(stdscr, TRUE);

   while(menuKey != '\n')
   {
      switch(menuKey)
      {
         case 'Z':
         case 'z':
         (*dataType) = NULO;
         goto fim;
         break;
         case KEY_UP: 
         if(*dataType > 1)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*dataType)--;
         }   
         break;

         case KEY_DOWN:
         if(*dataType < 2)
         {
            mvwaddch(stdscr, stdy, xpadrao, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, xpadrao, '>');
            (*dataType)++;
         }
         break;
      }
      wrefresh(stdscr);
      menuKey = wgetch(stdscr);
   }
   mvwaddch(stdscr, stdy, xpadrao, ' ');
   fim:

   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}