#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "safetyutils.h"
#include "menus.h"
#include <string.h>
#include <stdbool.h>
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

bool escolha_cpf();
bool escolha_cnpj();
bool todos_digitos_iguais(const char *str);

int main() {

    int escolha;

    printf("Escolha qual usar: CPF(1) ou CNPJ(2)\n");
    scanf("%d", &escolha);
    while(getchar() != '\n'); // limpa o buffer

    if (escolha == 1) {
        if (escolha_cpf()) {
            printf("CPF válido\n");
        } 
    }
    else if (escolha == 2) {
        if (escolha_cnpj()) {
            printf("CNPJ válido\n");
        } 
    } 
    else {
        printf("Escolha inválida\n");
    }

    return 0;
}

bool escolha_cpf() {
    char cpf_string[15];

    while (1) {
        printf("Digite o CPF: ");
        fgets(cpf_string, sizeof(cpf_string), stdin);
        cpf_string[strcspn(cpf_string, "\n")] = '\0';

        int n = strlen(cpf_string);
        char cpf_numeros[12];
        int j = 0;

        for (int i = 0; i < n; i++) {
            if (cpf_string[i] >= '0' && cpf_string[i] <= '9') {
                cpf_numeros[j++] = cpf_string[i];
            } else if (cpf_string[i] != '.' && cpf_string[i] != '-') {
                printf("CPF inválido: deve conter apenas números.\n");
                j = -1; // sinaliza erro
                break;
            }
        }

        if (j == -1) continue; // reinicia o loop
        cpf_numeros[j] = '\0';

        if (j != 11) {
            printf("CPF inválido: deve ter 11 dígitos.\n");
            continue;
        }

        if (todos_digitos_iguais(cpf_numeros)) {
            printf("CPF inválido: todos os dígitos iguais.\n");
            continue;
        }

        int cpf[11];
        for (int i = 0; i < 11; i++) cpf[i] = cpf_numeros[i] - '0';

        int soma = 0, mult = 10;
        for (int i = 0; i < 9; i++) {
            soma += cpf[i] * mult;
            mult--;
        }

        int digito1 = (soma * 10) % 11;
        if (digito1 == 10) digito1 = 0;

        soma = 0; mult = 11;
        for (int i = 0; i < 10; i++) {
            if (i < 9) soma += cpf[i] * mult;
            else soma += digito1 * mult;
            mult--;
        }

        int digito2 = (soma * 10) % 11;
        if (digito2 == 10) digito2 = 0;

        if (digito1 == cpf[9] && digito2 == cpf[10]) return true;
        else {
            printf("CPF inválido: dígito verificador incorreto.\n");
        }
    }
}

bool escolha_cnpj() {
    char cnpj_string[20];

    while (1) {
        printf("Digite o CNPJ: ");
        fgets(cnpj_string, sizeof(cnpj_string), stdin);
        cnpj_string[strcspn(cnpj_string, "\n")] = '\0';

        int n = strlen(cnpj_string);
        char cnpj_numeros[15];
        int j = 0;

        for (int i = 0; i < n; i++) {
            if (cnpj_string[i] >= '0' && cnpj_string[i] <= '9') {
                cnpj_numeros[j++] = cnpj_string[i];
            } else if (cnpj_string[i] != '.' && cnpj_string[i] != '-' && cnpj_string[i] != '/') {
                printf("CNPJ inválido: deve conter apenas números.\n");
                j = -1;
                break;
            }
        }

        if (j == -1) continue;
        cnpj_numeros[j] = '\0';

        if (j != 14) {
            printf("CNPJ inválido: deve ter 14 dígitos.\n");
            continue;
        }

        if (todos_digitos_iguais(cnpj_numeros)) {
            printf("CNPJ inválido: todos os dígitos iguais.\n");
            continue;
        }

        int cnpj[14];
        for (int i = 0; i < 14; i++) cnpj[i] = cnpj_numeros[i] - '0';

        int soma = 0, mult = 5;
        for (int i = 0; i < 12; i++) {
            soma += cnpj[i] * mult;
            mult--;
            if (mult < 2) mult = 9;
        }

        int digito1 = (soma % 11) < 2 ? 0 : 11 - (soma % 11);

        soma = 0; mult = 6;
        for (int i = 0; i < 13; i++) {
            if (i < 12) soma += cnpj[i] * mult;
            else soma += digito1 * mult;
            mult--;
            if (mult < 2) mult = 9;
        }

        int digito2 = (soma % 11) < 2 ? 0 : 11 - (soma % 11);

        if (digito1 == cnpj[12] && digito2 == cnpj[13]) return true;
        else {
            printf("CNPJ inválido: dígito verificador incorreto.\n");
        }
    }
}

bool todos_digitos_iguais(const char *str) {
    int n = strlen(str);
    for (int i = 1; i < n; i++) {
        if (str[i] != str[0]) return false;
    }
    return true;
}
