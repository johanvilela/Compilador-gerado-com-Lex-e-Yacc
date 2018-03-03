#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tabela_simbolos.h>
#include <y.tab.h>

// Variáveis Globais
FILE* saida_asm = NULL;
SymTable tabela_simbolos;
int contador_linhas = 1;

int main(int argc, char const *argv[]) {

        // Detalhes de verificacão da entrada e Geracao no nome da saída
        if (argc < 2) {
                printf("Erro: Não encontrou a entrada!\n");
                printf("$ ./ccc <código_entrada.txt>\n");
                exit(0);
        }
        int n = strlen(argv[1]);
        char s[n + 10];

        int i;
        for (i = 0; i < (n + 10); i++)
                s[i] = '\0';

        if (argc >= 4 && strcmp(argv[2], "-o") == 0)
                strcpy(s, argv[3]);
        else {
                for (i = n - 1; i >= 0 && argv[1][i] != '.'; i--) {}
                if (i == -1) {
                        fprintf(stderr, "Arquivo de entrada incompatível\n");
                        exit(0);
                }
                strncpy(s, argv[1], i);
                strcat(s, ".asm");
        }
        // Fim das verificacoes

        // Gerando o arquivo <nome_saida.asm>
        saida_asm = fopen(s, "w");
        fprintf(saida_asm, "extern printf\n");
        fprintf(saida_asm, "extern scanf\n");
        fprintf(saida_asm, "section .data\n");

        // Strings usadas para a quebra de linha
        fprintf(saida_asm, "fmt_dln: db \"%%ld\", 10, 0\n");
        fprintf(saida_asm, "fmt_fln: db \"%%f\", 10, 0\n");
        fprintf(saida_asm, "fmt_sln: db \"%%s\", 10, 0\n");
        fprintf(saida_asm, "\n");

        iniciaTabelaSimbolos(&tabela_simbolos);

        FILE* teclado = stdin;
        stdin = fopen(argv[1], "r");

        int resultado = yyparse();  // Chamada do Analisador Sintático
        // Encerra o programa em caso de erro na análise sintática
        if (resultado == 1) {
                fclose(saida_asm);
                remove(s);
                return 1;
        }

        fprintf(saida_asm, "mov rax,0\n");
        fprintf(saida_asm, "ret\n");

        imprimeTabelaSimbolos(&tabela_simbolos);

        fclose(saida_asm);
        fclose(stdin);

        return 0;
}
