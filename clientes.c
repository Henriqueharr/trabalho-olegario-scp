#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "doublylinkedlist.h"
#include "menus.h"
#include "safetyutils.h"

extern double teladevy, teladevx;

void* criarCliente(Tipos dataType, List *lista)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);

   const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};

   short y = maxstdy * (7.0/teladevy);

   WINDOW *telas[5], *subtelas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(y, maxstdx * (30.0/teladevx), (i + 1) * y, maxstdx * (150.0/teladevx));
      wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
      waddstr(telas[i], label[i]);
      if(dataType == PESSOA_JURIDICA && i == 4)
      {
         mvwaddstr(telas[i], 0, 0, label[5]);
      }
      subtelas[i] = derwin(telas[i], y * (3.0/7.0), maxstdx * (30.0/teladevx) - 4, y * (2.0/y), maxstdx * (30.0/teladevx) * (2.0/(maxstdx * (30.0/teladevx))));
      wrefresh(telas[i]);
      wrefresh(stdscr);
      napms(25);
   }


   void *novoCliente;

   if(dataType == PESSOA_FISICA)
   {
      novoCliente = (PessoaFisica*)calloc(1, sizeof(PessoaFisica));
   }
   else
   {
      novoCliente = (PessoaJuridica*)calloc(1, sizeof(PessoaJuridica));
   }

   if(!novoCliente) goto escape;

   unsigned short key = KEY_UP;
   MenuCriarCliente opc = ID;
   //ID//Nome//Endereço//Telefone//CPF ou CNPJ//confirmar saida
   unsigned short booleans[6] = {};
   unsigned short tudofeito = 0;
   unsigned short stdy = y + y * 0.5;
   mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, '>');
   curs_set(FALSE);
   keypad(stdscr, TRUE);
   noecho();
   cbreak();
   while(!booleans[5])
   {
      key = wgetch(stdscr);
      switch(key)
      {
         case 'Z':
         case 'z':
         mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, ' ');
         wrefresh(stdscr);
         goto escape;
         case KEY_UP:
         if(opc > 1)
         {
            mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, ' ');
            stdy -= y;
            mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, '>');
            opc--;
         }
         break;
         case KEY_DOWN:
         if(opc < 5 || (opc == 5 && tudofeito))
         {
            mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, ' ');
            stdy += y;
            mvwaddch(stdscr, stdy, maxstdx * (150.0/teladevx) - 2, '>');
            opc++;
         }
         break;
         case '\n':
         echo();
         nocbreak();
         switch(opc)
         {
            case ID:
            {
               cortina(subtelas[opc - 1], 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               Node* repetido = findCByID(lista, get_ctype(novoCliente,GenericCast)->data.id);
               while(check != 1 || repetido)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == 0) {gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                  if(check == -1) {gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                  if(check == -2) {gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                  if(repetido) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "Este ID de cliente já existe");
                  erroDuplo:
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
                  curs_set(FALSE);
                  repetido = findCByID(lista, get_ctype(novoCliente,GenericCast)->data.id);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case NOME:
            {
               cortina(subtelas[opc - 1], 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case ENDERECO:
            {
               cortina(subtelas[opc - 1], 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");                  werase(subtelas[opc - 1]);
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case TELEFONE:
            {
               cortina(subtelas[opc - 1], 0, y * (3.0/7.0), 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 0, 0);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case CPF_CNPJ:
            {
               cbreak();
               cortina(subtelas[opc - 1], 0, y * (3.0/7.0), 10);
               if(dataType == PESSOA_FISICA)
               {
                  wmove(subtelas[opc - 1], 0, 0);
                  keypad(subtelas[opc - 1], TRUE);
                  short confirmou = lerCPF(get_ctype(novoCliente,PessoaFisica)->cpf, subtelas[opc - 1]);
                  keypad(subtelas[opc - 1], FALSE);
                  if(!confirmou)
                  {
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     goto skip;
                  }
                  while(!validar_cpf(get_ctype(novoCliente,PessoaFisica)->cpf))
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     cbreak();
                     keypad(subtelas[opc - 1], TRUE);
                     confirmou = lerCPF(get_ctype(novoCliente,PessoaFisica)->cpf, subtelas[opc - 1]);
                     keypad(subtelas[opc - 1], FALSE);
                     if(!confirmou)
                     {
                        werase(subtelas[opc - 1]);
                        wrefresh(subtelas[opc - 1]);
                        goto skip;
                     }
                  }
               }
               else
               {
                  wmove(subtelas[opc - 1], 0, 0);
                  keypad(subtelas[opc - 1], TRUE);
                  short confirmou = lerCNPJ(get_ctype(novoCliente,PessoaJuridica)->cnpj, subtelas[opc - 1]);
                  keypad(subtelas[opc - 1], FALSE);
                  if(!confirmou)
                  {
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     goto skip;
                  }
                  while(!validar_cnpj(get_ctype(novoCliente,PessoaJuridica)->cnpj))
                  {
                     cbreak();
                     noecho();
                     curs_set(FALSE);
                     gerarErro(maxstdy * (10.0/teladevy), maxstdx * (36.0/teladevx), maxstdy * (1.0/teladevy), maxstdx * (86.0/teladevx), "||~~OOOO", "Erro de validação", "O CNPJ digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 0, 0);
                     cbreak();
                     keypad(subtelas[opc - 1], TRUE);
                     confirmou = lerCNPJ(get_ctype(novoCliente,PessoaJuridica)->cnpj, subtelas[opc - 1]);
                     keypad(subtelas[opc - 1], FALSE);
                     if(!confirmou)
                     {
                        werase(subtelas[opc - 1]);
                        wrefresh(subtelas[opc - 1]);
                        goto skip;
                     }
                  }
               }
               booleans[opc - 1] = 1;
            }
            skip:
            break;
            case FINALIZAR:
            booleans[opc - 1] = 1;
            break;
         }
         noecho();
         cbreak();
         tudofeito = 1;
         for(unsigned short i = 0; i < 5; i++)
         {
            if(!booleans[i])
            {
               tudofeito = 0;
            }
         }
         break;
      }
      curs_set(FALSE);

      if(tudofeito)
      {
         unsigned short x, y;
         getyx(stdscr, y, x);
         attron(COLOR_PAIR(1));
         mvwaddstr(stdscr, maxstdy * (45.0/teladevy), maxstdx * (150.0/teladevx), "Salvar cliente");
         attroff(COLOR_PAIR(1));
         wmove(stdscr, y, x);
      }
      wrefresh(stdscr);
   }

   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, y, 20);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   cortina(stdscr, maxstdy * (45.0/teladevy) - 1, maxstdy * (45.0/teladevy) + 1, 25);

   return novoCliente;

   escape:
   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, y, 20);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
   free(novoCliente);

   cortina(stdscr, maxstdy * (45.0/teladevy) - 1, maxstdy * (45.0/teladevy) + 1, 25);

   return NULL;
}

void listarClientes(List *listaClientes)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   noecho();
   cbreak();
   unsigned int n, cont;
   size_t ind = 0;
   (listaClientes->tam % 6 == 0 ? (n = listaClientes->tam / 6) : (n = listaClientes->tam / 6 + 1));
   
   Node *pagina[n];
   
   if(!listaClientes->head) goto listavazia;
   
   Node *atual = listaClientes->head;

   for(cont = 1; atual; atual = atual->next, cont++)
   {
      if(cont % 6 == 1)
      {
         pagina[ind] = atual;
         ind++;
      }
   }
   ind = 1;
   atual = pagina[0];

   mvwaddstr(stdscr, 0, 1, "!Comandos!");
   mvwaddstr(stdscr, 1, 1, "Direita : Avançar página");
   mvwaddstr(stdscr, 2, 1, "Esquerda: Retroceder página");
   mvwaddstr(stdscr, 3, 1, "P/p     : Consultar por página");
   mvwaddstr(stdscr, 4, 1, "I/i     : Consultar por ID");
   mvwaddstr(stdscr, 5, 1, "Enter   : Confirmar");
   mvwaddstr(stdscr, 6, 1, "Z/z     : Voltar");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 0, maxstdx * (2.0/teladevx), "  ██╗");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 1, maxstdx * (2.0/teladevx), " ██╔╝");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 2, maxstdx * (2.0/teladevx), "██╔╝ ");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 3, maxstdx * (2.0/teladevx), "╚██╗ ");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 4, maxstdx * (2.0/teladevx), " ╚██╗");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 5, maxstdx * (2.0/teladevx), "  ╚═╝");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 0, maxstdx * (204.0/211.0), "██╗  ");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 1, maxstdx * (204.0/211.0), "╚██╗ ");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 2, maxstdx * (204.0/211.0), " ╚██╗");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 3, maxstdx * (204.0/211.0), " ██╔╝");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 4, maxstdx * (204.0/211.0), "██╔╝ ");
   mvwaddstr(stdscr, maxstdy * (28.0/teladevy) + 5, maxstdx * (204.0/211.0), "╚═╝  ");

   wrefresh(stdscr);

   unsigned short yi = maxstdy * (15.0/teladevy), xi[] = {maxstdx * (15.0/teladevx), maxstdx * 0.5 - (maxstdx * (50.0/teladevx)) * 0.5, maxstdx * (146.0/teladevx)};

   logo(maxstdy * (4.0/teladevy), maxstdx * 0.5 - 11);
   
   WINDOW *selecaoPagina = newwin(3, 19, yi - 3, xi[0]);
   WINDOW *subsel = derwin(selecaoPagina, 1, 8, 1, 1);
   wborder(selecaoPagina, '|', '|', '~', '~', 'O', 'O', 'O', 'O');
   mvwaddstr(selecaoPagina, 0, 0, "Página");
   mvwprintw(selecaoPagina, 1, 1, "        /%d", n);
   wrefresh(selecaoPagina);
   
   WINDOW *telas[6];
   WINDOW *subtelas[6];
   
   for(unsigned short i = 0; i < 6; i++)
   {
      telas[i] = newwin(maxstdy * (15.0/teladevy), maxstdx * (50.0/teladevx), yi, xi[i % 3]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], maxstdy * (15.0/teladevy) - 2, maxstdx * (50.0/teladevx) - 2, 1, 1);
      wrefresh(telas[i]);
      if(i == 2) yi += (maxstdy * (15.0/teladevy) + 2);
   }
   
   wrefresh(stdscr);

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
               wrefresh(subsel);
               atual = pagina[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                   get_content(atual, GenericCast)->data.id,
                                                   get_content(atual, GenericCast)->data.name,
                                                   get_content(atual, GenericCast)->data.address,
                                                   get_content(atual, GenericCast)->data.phonenumber,
                                                   ((atual->dataType == PESSOA_FISICA) ? get_content(atual, PessoaFisica)->cpf : get_content(atual, PessoaJuridica)->cnpj));
                     wrefresh(subtelas[i]);
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
               wrefresh(subsel);
               atual = pagina[ind];
               for(unsigned short i = 0; i < 6; i++)
               {
                  werase(subtelas[i]);
                  wrefresh(subtelas[i]);
                  if(atual)
                  {
                     mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                   get_content(atual, GenericCast)->data.id,
                                                   get_content(atual, GenericCast)->data.name,
                                                   get_content(atual, GenericCast)->data.address,
                                                   get_content(atual, GenericCast)->data.phonenumber,
                                                   ((atual->dataType == PESSOA_FISICA) ? get_content(atual, PessoaFisica)->cpf : get_content(atual, PessoaJuridica)->cnpj));
                     wrefresh(subtelas[i]);
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
                  gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                  goto duploerro;
               }
               if(check == -1) 
               {
                  gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                  goto duploerro;
               }
               if(check == -2) 
               {
                  gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                  goto duploerro;
               }
               if(ind > n || ind <= 0)
               {
                  gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "A página requisitada não existe");
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
            wrefresh(subsel);
            atual = pagina[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wrefresh(subtelas[i]);
               if(atual)
               {
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                get_content(atual, GenericCast)->data.id,
                                                get_content(atual, GenericCast)->data.name,
                                                get_content(atual, GenericCast)->data.address,
                                                get_content(atual, GenericCast)->data.phonenumber,
                                                ((atual->dataType == PESSOA_FISICA) ? get_content(atual, PessoaFisica)->cpf : get_content(atual, PessoaJuridica)->cnpj));
                  wrefresh(subtelas[i]);
                  atual = atual->next;
               }
            }
         }
         break;
         case 'I':
         case 'i':
         {
            WINDOW *selid = newwin(3, 25, maxstdy * (15.0/teladevy) - 6, xi[0]);
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
            while(check != 1)
            {
               cbreak();
               noecho();
               curs_set(FALSE); 
               if(check == 0) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
               if(check == -1) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
               if(check == -2) gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
               nocbreak();
               echo();
               curs_set(TRUE);
               werase(subselid);
               wrefresh(subselid);
               wmove(subselid, 1, 3);
               check = lerSizeT(&targetID, subselid);
            }
            int dist = pathToCNode(listaClientes, targetID);
            if(dist == -1)
            {
               cbreak();
               noecho();
               curs_set(FALSE);
               gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * (2.0/teladevy), xi[2] + (maxstdx * (30.0/teladevx) - maxstdx * (50.0/teladevx)) * 0.5, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
               nocbreak();
               echo();
               curs_set(TRUE);
               cortina(selid, 0, 3, 25);
               delwin(subselid);
               delwin(selid);
               noecho();
               cbreak();
               break;
            }
            ind = dist / 6;
            werase(subsel);
            mvwprintw(subsel, 0, 0, "%zu", ind + 1);
            wrefresh(subsel);
            atual = pagina[ind];
            for(unsigned short i = 0; i < 6; i++)
            {
               werase(subtelas[i]);
               wrefresh(subtelas[i]);
               if(atual)
               {
                  if(get_content(atual, GenericCast)->data.id == targetID) wattron(subtelas[i], COLOR_PAIR(1));
                  mvwprintw(subtelas[i], 0, 0, "ID: %zu\n\nNome: %s\n\nEndereço : %s\n\nTelefone: %s\n\nCPF/CNPJ: %s",
                                                get_content(atual, GenericCast)->data.id,
                                                get_content(atual, GenericCast)->data.name,
                                                get_content(atual, GenericCast)->data.address,
                                                get_content(atual, GenericCast)->data.phonenumber,
                                                ((atual->dataType == PESSOA_FISICA) ? get_content(atual, PessoaFisica)->cpf : get_content(atual, PessoaJuridica)->cnpj));
                  wrefresh(subtelas[i]);
                  wattroff(subtelas[i], COLOR_PAIR(1));
                  atual = atual->next;
               }
            }
            cortina(selid, 0, 3, 25);
            delwin(subselid);
            delwin(selid);
            noecho();
            cbreak();
         }
         break;
      }
      curs_set(FALSE);
      cmd = wgetch(stdscr);
   }

   for(unsigned short i = 0; i < 6; i++)
   {
      abrir(telas[i], 8, 15, 15);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   return;
   listavazia:
   curs_set(FALSE);
   gerarErro(maxstdy * (10.0/teladevy), maxstdx * (30.0/teladevx), maxstdy * 0.5 - (maxstdy * (10.0/teladevy)) * 0.5, maxstdx * 0.5 - (maxstdx * (30.0/teladevx)) * 0.5, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
   curs_set(TRUE);
}

void editarCliente(List *listaClientes)
{
   unsigned short maxstdy, maxstdx;
   getmaxyx(stdscr, maxstdy, maxstdx);
   noecho();
   cbreak();

   if(!listaClientes) goto listavazia;

   

   return;

   listavazia:
   curs_set(FALSE);
   gerarErro(10, 30, maxstdy/2 - 5, maxstdx/2 - 15, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
   curs_set(TRUE);
}

Node* findCByID(List *lista, size_t targetID)
{
   if(!lista->head) return NULL;
   
   for(Node* begin = lista->head; begin; begin = begin->next) if(get_content(begin,GenericCast)->data.id == targetID) return begin;   

   return NULL;
}

int pathToCNode(List *lista, size_t targetID)
{
   if(!lista->head) return -1;
   
   int dist = 0;
   
   for(Node* begin = lista->head; begin; begin = begin->next, dist++) if(get_content(begin,GenericCast)->data.id == targetID) return dist;   

   return -1;
}