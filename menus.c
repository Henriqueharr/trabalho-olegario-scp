#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "produtos.h"
#include "pedidos.h"
#include "menus.h"
#include "doublylinkedlist.h"

void gerarErro(unsigned short altura, unsigned short largura, unsigned short yi, unsigned short xi, const char *bordas, const char *titulo, const char *msg)
{
   WINDOW *erro = newwin(altura, largura, yi, xi);
   WINDOW *subtela = derwin(erro, altura - 2, largura - 2, 1, 1);
   wborder(erro, bordas[0], bordas[1], bordas[2], bordas[3], bordas[4], bordas[5], bordas[6], bordas[7]);
   mvwprintw(subtela, 0, 0, "%s\n\n%s\n\nPressione qualquer tecla para continuar", titulo, msg);
   keypad(erro, FALSE);
   wgetch(erro);
   cortina(erro, 0, altura, 25);
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

void slideLeft(WINDOW *tela, short yi, short yf, short pad, unsigned short delay)
{
   curs_set(FALSE);
   for(short i = yi; i <= yf; i++)
   {
      wmove(tela, i, 0);
      for(short j = 0; j < pad; j++)
      {
         wdelch(tela);
         wrefresh(tela);
         napms(delay);
      }
   }
   curs_set(TRUE);
}

void abrir(WINDOW *tela, short uppery, short abertura, unsigned short delay)
{
   curs_set(FALSE);
   short ys, yd;
   (abertura % 2 ? (ys = uppery, yd = uppery + 1) : (ys = yd = uppery));
   for(short i = 0; i < abertura; ys--, yd++, i++)
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
   werase(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 2, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 3, 1, "Enter: Confirmar/Selecionar");
   mvwaddstr(stdscr, 4, 1, "Z/z  : Voltar");

   
   logo(20, 95);
   mvwaddstr(stdscr,27,95,"Selecione um opção: ");
   mvwaddstr(stdscr,29,98,"Menu de clientes");
   mvwaddstr(stdscr,30,98,"Menu de produtos");
   mvwaddstr(stdscr,31,98,"Menu de pedidos");
   mvwaddstr(stdscr,32,98,"Encerrar programa");
   
   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = 29;
   *opc = MENU_CLIENTES;

   mvwaddch(stdscr, stdy, 95, '>');
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
         if(stdy > 29)
         {
            mvwaddch(stdscr, stdy, 95, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, 95, '>');
            (*opc)--;
         }   
         break;

         case KEY_DOWN:
         if(stdy < 32)
         {
            mvwaddch(stdscr, stdy, 95, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, 95, '>');
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
   werase(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 2, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 3, 1, "Enter: Confirmar/Selecionar");
   mvwaddstr(stdscr, 4, 1, "Z/z  : Voltar");

   
   logo(20, 95);
   mvwaddstr(stdscr,27,95,"Selecione um opção: ");
   mvwaddstr(stdscr,29,98,"Inserir cliente");
   mvwaddstr(stdscr,30,98,"Listar clientes");
   mvwaddstr(stdscr,31,98,"Editar cliente");
   mvwaddstr(stdscr,32,98,"Remover cliente");
   mvwaddstr(stdscr,33,98,"Salvar clientes em arquivo");
   mvwaddstr(stdscr,34,98,"Carregar arquivo de clientes");
   mvwaddstr(stdscr,35,98,"Voltar ao menu principal");
   
   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = 29;
   *opc = ADD_CLIENTE;

   mvwaddch(stdscr, stdy, 95, '>');
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
         if(stdy > 29)
         {
            mvwaddch(stdscr, stdy, 95, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, 95, '>');
            (*opc)--;
         }   
         break;

         case KEY_DOWN:
         if(stdy < 35)
         {
            mvwaddch(stdscr, stdy, 95, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, 95, '>');
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

   mvwaddstr(stdscr, 21, 75, "Escolha o tipo de cliente:");
   mvwaddstr(stdscr, 23, 78, "Pessoa física");
   mvwaddstr(stdscr, 24, 78, "Pessoa jurídica");
   mvwaddch(stdscr, 23, 75, '>');

   wrefresh(stdscr);

   short menuKey = KEY_UP;
   short stdy = 23;
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
         if(stdy == 24)
         {
            mvwaddch(stdscr, stdy, 75, ' ');
            stdy--;
            mvwaddch(stdscr, stdy, 75, '>');
            (*dataType)--;
         }   
         break;

         case KEY_DOWN:
         if(stdy == 23)
         {
            mvwaddch(stdscr, stdy, 75, ' ');
            stdy++;
            mvwaddch(stdscr, stdy, 75, '>');
            (*dataType)++;
         }
         break;
      }
      wrefresh(stdscr);
      menuKey = wgetch(stdscr);
   }
   mvwaddch(stdscr, stdy, 75, ' ');
   fim:

   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}