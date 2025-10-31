#include <stdlib.h>
#include <ncurses.h>
#include "clientes.h"
#include "doublylinkedlist.h"
#include "menus.h"
#include "safetyutils.h"

void* criarCliente(Tipos dataType, List *lista)
{
   const char* label[] = {"ID", "Nome", "Endereço", "Telefone", "CPF", "CNPJ"};
   short y = 7;
   WINDOW *telas[5], *subtelas[5];
   for(short i = 0; i < 5; i++)
   {
      telas[i] = newwin(7, 30, (i + 1) * y, 150);
      wborder(telas[i],  '|', '|', '-', '-', 'O', 'O', 'O', 'O');
      waddstr(telas[i], label[i]);
      if(dataType == PESSOA_JURIDICA && i == 4)
      {
         mvwaddstr(telas[i], 0, 0, label[5]);
      }
      subtelas[i] = derwin(telas[i], 3, 26, 2, 2);
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
   unsigned short stdy = 10;
   mvwaddch(stdscr, stdy, 148, '>');
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
         mvwaddch(stdscr, stdy, 148, ' ');
         wrefresh(stdscr);
         goto escape;
         case KEY_UP:
         if(opc > 1)
         {
            mvwaddch(stdscr, stdy, 148, ' ');
            stdy -= 7;
            mvwaddch(stdscr, stdy, 148, '>');
            opc--;
         }
         break;
         case KEY_DOWN:
         if(opc < 5 || (opc == 5 && tudofeito))
         {
            mvwaddch(stdscr, stdy, 148, ' ');
            stdy += 7;
            mvwaddch(stdscr, stdy, 148, '>');
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
               cortina(subtelas[opc - 1], 0, 3, 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
               Node* repetido = findCByID(lista, get_ctype(novoCliente,GenericCast)->data.id);
               while(check != 1 || repetido)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == 0) {gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos"); goto erroDuplo;}
                  if(check == -1) {gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos"); goto erroDuplo;}
                  if(check == -2) {gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Entrada não encontrada"); goto erroDuplo;}
                  if(repetido) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "Este ID de cliente já existe");
                  erroDuplo:
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerSizeT(&get_ctype(novoCliente,GenericCast)->data.id, subtelas[opc - 1]);
                  repetido = findCByID(lista, get_ctype(novoCliente,GenericCast)->data.id);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case NOME:
            {
               cortina(subtelas[opc - 1], 0, 3, 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.name, sizeof(get_ctype(novoCliente,GenericCast)->data.name), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case ENDERECO:
            {
               cortina(subtelas[opc - 1], 0, 3, 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");                  werase(subtelas[opc - 1]);
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.address, sizeof(get_ctype(novoCliente,GenericCast)->data.address), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case TELEFONE:
            {
               cortina(subtelas[opc - 1], 0, 3, 10);
               wmove(subtelas[opc - 1], 0, 0);
               short check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               while(check != 1)
               {
                  cbreak();
                  noecho();
                  curs_set(FALSE);
                  if(check == -1) gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de formatação", "Este campo não aceita entradas vazias ou iniciadas com espaço");
                  nocbreak();
                  echo();
                  curs_set(TRUE);
                  werase(subtelas[opc - 1]);
                  wrefresh(subtelas[opc - 1]);
                  wmove(subtelas[opc - 1], 1, 3);
                  check = lerStr(get_ctype(novoCliente,GenericCast)->data.phonenumber, sizeof(get_ctype(novoCliente,GenericCast)->data.phonenumber), subtelas[opc - 1]);
               }
               booleans[opc - 1] = 1;
            }
            break;
            case CPF_CNPJ:
            {
               cbreak();
               cortina(subtelas[opc - 1], 0, 3, 10);
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
                     gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "O CPF digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 1, 3);
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
                     gerarErro(10, 36, 1, 86, "||~~OOOO", "Erro de validação", "O CNPJ digitado não é válido");
                     nocbreak();
                     echo();
                     curs_set(TRUE);
                     werase(subtelas[opc - 1]);
                     wrefresh(subtelas[opc - 1]);
                     wmove(subtelas[opc - 1], 1, 3);
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
         mvwaddstr(stdscr, 45, 150, "Salvar cliente");
         wmove(stdscr, y, x);
      }

      wrefresh(stdscr);
   }

   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 7, 15);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }

   cortina(stdscr, 43, 45, 25);

   return novoCliente;

   escape:
   for(short i = 0; i < 5; i++)
   {
      cortina(telas[i], 0, 7, 20);
      delwin(subtelas[i]);
      delwin(telas[i]);
   }
   free(novoCliente);

   cortina(stdscr, 43, 45, 25);

   return NULL;
}

void listarClientes(List *listaClientes)
{
   unsigned short maxy, maxx;
   getmaxyx(stdscr, maxy, maxx);
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
   mvwaddstr(stdscr, maxy - 21, 2, "  ██╗");
   mvwaddstr(stdscr, maxy - 20, 2, " ██╔╝");
   mvwaddstr(stdscr, maxy - 19, 2, "██╔╝ ");
   mvwaddstr(stdscr, maxy - 18, 2, "╚██╗ ");
   mvwaddstr(stdscr, maxy - 17, 2, " ╚██╗");
   mvwaddstr(stdscr, maxy - 16, 2, "  ╚═╝");
   mvwaddstr(stdscr, maxy - 21, maxx - 7, "██╗  ");
   mvwaddstr(stdscr, maxy - 20, maxx - 7, "╚██╗ ");
   mvwaddstr(stdscr, maxy - 19, maxx - 7, " ╚██╗");
   mvwaddstr(stdscr, maxy - 18, maxx - 7, " ██╔╝");
   mvwaddstr(stdscr, maxy - 17, maxx - 7, "██╔╝ ");
   mvwaddstr(stdscr, maxy - 16, maxx - 7, "╚═╝  ");

   wrefresh(stdscr);

   unsigned short yi = maxy - 34, xi[] = {15, maxx/2 - 25, maxx - 65};

   logo(4, maxx/2 - 11);
   
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
      telas[i] = newwin(15, 50, yi, xi[i % 3]);
      wborder(telas[i], '|', '|', '~', '~', 'O', 'O', 'O', 'O');
      subtelas[i] = derwin(telas[i], 13, 48, 1, 1);
      wrefresh(telas[i]);
      if(i == 2) yi += 17;
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
                  gerarErro(10, 36, 2, xi[2] + 7, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
                  goto duploerro;
               }
               if(check == -1) 
               {
                  gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
                  goto duploerro;
               }
               if(check == -2) 
               {
                  gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
                  goto duploerro;
               }
               if(ind > n || ind <= 0)
               {
                  gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "A página requisitada não existe");
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
            WINDOW *selid = newwin(3, 25, maxy - 41, xi[0]);
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
               if(check == 0) gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "Este campo não aceita valores negativos");
               if(check == -1) gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "Este campo não aceita caracteres não numéricos");
               if(check == -2) gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de validação", "Entrada não encontrada");
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
               gerarErro(10, 36, 1, xi[2] + 7, "||~~OOOO", "Erro de busca", "O identificador solicitado não existe");
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
   gerarErro(10, 30, maxy/2 - 5, maxx/2 - 15, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
   curs_set(TRUE);
}

void editarCliente(List *listaClientes)
{
   noecho();
   cbreak();
   unsigned short maxy = getmaxy(stdscr), maxx = getmaxx(stdscr);

   if(!listaClientes) goto listavazia;

   

   return;

   listavazia:
   curs_set(FALSE);
   gerarErro(10, 30, maxy/2 - 5, maxx/2 - 15, "||~~OOOO", "Erro de carregamento", "A lista de clientes encontra-se vazia no momento");
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