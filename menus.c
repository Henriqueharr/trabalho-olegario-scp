#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "produtos.h"
#include "pedidos.h"
#include "menus.h"

void transicao(WINDOW * tela)
{
   int maxy = getmaxy(tela);
   for(size_t i = 0; i < maxy; i++) {wmove(tela, i, 0); wclrtoeol(tela); wrefresh(tela); napms(25);};
}

void showMainMenu(MenuPrincipal *opc)
{
   wclear(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "Comandos:");
   mvwaddstr(stdscr, 2, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 3, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 4, 1, "Enter: Confirmar/Selecionar");

   
   mvwaddstr(stdscr,20,95,"███████╗ ██████╗██████╗");
   mvwaddstr(stdscr,21,95,"██╔════╝██╔════╝██╔══██╗");
   mvwaddstr(stdscr,22,95,"███████╗██║     ██████╔╝");
   mvwaddstr(stdscr,23,95,"╚════██║██║     ██╔═══╝");
   mvwaddstr(stdscr,24,95,"███████║╚██████╗██║");
   mvwaddstr(stdscr,25,95,"╚══════╝ ╚═════╝╚═╝");
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

   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}

void showCustomerMenu(MenuCliente *opc)
{
   wclear(stdscr);
   
   curs_set(FALSE);
   noecho();
   cbreak();
   mvwaddstr(stdscr, 0, 1, "Comandos:");
   mvwaddstr(stdscr, 2, 1, "Cima : Mover para cima");
   mvwaddstr(stdscr, 3, 1, "Baixo: Mover para baixo");
   mvwaddstr(stdscr, 4, 1, "Enter: Confirmar/Selecionar");

   
   mvwaddstr(stdscr,20,95,"███████╗ ██████╗██████╗");
   mvwaddstr(stdscr,21,95,"██╔════╝██╔════╝██╔══██╗");
   mvwaddstr(stdscr,22,95,"███████╗██║     ██████╔╝");
   mvwaddstr(stdscr,23,95,"╚════██║██║     ██╔═══╝");
   mvwaddstr(stdscr,24,95,"███████║╚██████╗██║");
   mvwaddstr(stdscr,25,95,"╚══════╝ ╚═════╝╚═╝");
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

   keypad(stdscr, FALSE);
   echo();
   nocbreak();
}