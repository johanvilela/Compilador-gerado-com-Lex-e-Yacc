#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tabela_simbolos.h>
#include <gerador_codigo.h>

// Rótulos usados nos jumps
void criaRotulo(char* rotulo_saida) {
        static int contador_rotulo = 0;
        strcpy(rotulo_saida, "rotulo");
        char s[10];
        sprintf(s, "%d", contador_rotulo);
        strcat(rotulo_saida, s);
        contador_rotulo++;
}

void geraCodigoDeclaracao(char* destino, char* identificador, Tipo tipo) {
        if (tipo == INTEIRO) {
                sprintf(destino, "%s: dq 0\n", identificador);
        } else if (tipo == REAL) {
                sprintf(destino, "%s: dq 0\n", identificador);
        } else if (tipo == STRING) {
                sprintf(destino, "%s: times %d db 0 \n", identificador, TAM_STRING);
        }
}

// Termino da secao de dados e comeco da secao de codigo
void inicioSecaoCodigo() {
        fprintf(saida_asm, "\nsection .text\n");
        fprintf(saida_asm, "global main\n");
        fprintf(saida_asm, "\nmain:\n");
}

int geraCodigoLeia(char* destino, char *id) {
        SymTableEntry* retorno = findSymTable(&tabela_simbolos,id);

        destino[0] = '\0';

        if (retorno == NULL) {
                fprintf(stderr, "Erro na linha %d: %s\n", contador_linhas, id);
                return 0;
        }

        if (retorno->tipo == INTEIRO) {
                sprintf(destino + strlen(destino), "mov rdi,fmt_d\n");
                sprintf(destino + strlen(destino), "mov rsi,%s\n", retorno->identificador);
        } else if (retorno->tipo == REAL) {
                sprintf(destino + strlen(destino), "mov rdi,fmt_f\n");
                sprintf(destino + strlen(destino), "mov rsi,%s\n", retorno->identificador);
        } else {
                sprintf(destino + strlen(destino), "mov rdi,fmt_s\n");
                sprintf(destino + strlen(destino), "mov rsi,%s\n", retorno->identificador);
        }

        sprintf(destino + strlen(destino), "mov rax,0\n");
        sprintf(destino + strlen(destino), "call scanf\n");

        return 1;
}

int geraCodigoEscreva(char* destino, char *id) {
        SymTableEntry* retorno = findSymTable(&tabela_simbolos,id);

        destino[0] = '\0';

        if (retorno == NULL) {
                fprintf(stderr, "Erro na linha %d: %s\n", contador_linhas, id);
                return 0;
        }

        if (retorno->tipo == INTEIRO) {
                sprintf(destino + strlen(destino), "mov rdi,fmt_dln\n");
                sprintf(destino + strlen(destino), "mov rsi,[%s]\n", retorno->identificador);
        } else if (retorno->tipo == REAL) {
                sprintf(destino + strlen(destino), "mov rdi,fmt_fln\n");
                sprintf(destino + strlen(destino), "mov rsi,[%s]\n", retorno->identificador);
        } else {
                sprintf(destino + strlen(destino), "mov rdi,fmt_sln\n");
                sprintf(destino + strlen(destino), "mov rsi,%s\n", retorno->identificador);
        }

        sprintf(destino + strlen(destino), "mov rax,0\n");
        sprintf(destino + strlen(destino), "call printf\n");

        return 1;
}

int geraCodigoAtribuicao(char* destino, char* id, char* expressao) {
        SymTableEntry* retorno = findSymTable(&tabela_simbolos, id);
        destino[0] = '\0';

        if (retorno == NULL) {
                fprintf(stderr, "Erro na linha %d: %s\n", contador_linhas, id);
                return 0;
        }

        if (retorno->tipo == INTEIRO) {
                sprintf(destino + strlen(destino), "%s", expressao);
                sprintf(destino + strlen(destino), "pop rbx\n");
                sprintf(destino + strlen(destino), "mov [%s],rbx\n", retorno->identificador);
        } else {
                fprintf(stderr, "Operaćão não suportada com string ou real na linha %d\n",contador_linhas);
                return 0;
        }
        return 1;
}
// MUDAR AQUI
int geraCodigo_termo_e_fator(char* destino, char* id, int ref) {
        destino[0] = '\0';

        if (ref == 0) {
                sprintf(destino + strlen(destino), "mov rbx,%s\n", id);
                sprintf(destino + strlen(destino), "push rbx\n");
                return 1;
        }

        SymTableEntry* retorno = findSymTable(&tabela_simbolos, id);

        if (retorno == NULL) {
                fprintf(stderr, "Erro na linha %d: %s\n", contador_linhas, id);
                return 0;
        }

        if (retorno->tipo == STRING)
                sprintf(destino + strlen(destino), "mov rbx,%s\n", retorno->identificador);
        else
                sprintf(destino + strlen(destino), "mov rbx,[%s]\n", retorno->identificador);

        sprintf(destino + strlen(destino), "push rbx\n");
        return 1;
}

void geraCodigoSoma(char* destino, char* conteudo) {
        sprintf(destino + strlen(destino), "%s", conteudo);
        sprintf(destino + strlen(destino), "pop rcx\n");
        sprintf(destino + strlen(destino), "pop rbx\n");
        sprintf(destino + strlen(destino), "add rbx,rcx\n");
        sprintf(destino + strlen(destino), "push rbx\n");
}

void geraCodigoSubtracao(char* destino, char* conteudo) {
        sprintf(destino + strlen(destino), "%s", conteudo);
        sprintf(destino + strlen(destino), "pop rcx\n");
        sprintf(destino + strlen(destino), "pop rbx\n");
        sprintf(destino + strlen(destino), "sub rbx,rcx\n");
        sprintf(destino + strlen(destino), "push rbx\n");
}

void geraCodigoMultiplicacao(char* destino, char* conteudo2) {
        sprintf(destino + strlen(destino), "%s", conteudo2);
        sprintf(destino + strlen(destino), "pop rcx\npop rbx\nimul rbx,rcx\npush rbx\n");
}

void geraCodigoDivisao(char* destino, char* conteudo2) {
        sprintf(destino + strlen(destino), "%s", conteudo2);
        sprintf(destino + strlen(destino), "pop r8\n");
        sprintf(destino + strlen(destino), "pop rax\n");
        sprintf(destino + strlen(destino), "xor rdx,rdx\n");
        sprintf(destino + strlen(destino), "idiv r8\n");
        sprintf(destino + strlen(destino), "push rax\n");
}

int geraCodigoComparacao(char* destino, char* id, char* expressao) {
        SymTableEntry* retorno = findSymTable(&tabela_simbolos, id);
        destino[0] = '\0';

        if (retorno == NULL) {
                fprintf(stderr, "Erro na linha %d: %s\n", contador_linhas, id);
                return 0;
        }

        if (retorno->tipo != INTEIRO) {
                fprintf(stderr, "Operaćão não suportada com string ou real na linha %d\n",contador_linhas);
                return 0;
        }

        sprintf(destino + strlen(destino), "%s", expressao);
        sprintf(destino + strlen(destino), "pop rcx\n");
        sprintf(destino + strlen(destino), "mov rbx, [%s]\n", retorno->identificador);
        sprintf(destino + strlen(destino), "cmp rbx, rcx\n");

        return 1;
}

void geraCodigoSe(char* destino, char* codigo_expressao, char * tipo_jump, char* bloco_codigo) {
        char rotulo[16];
        criaRotulo(rotulo);

        destino[0] = '\0';

        sprintf(destino + strlen(destino), "%s", codigo_expressao);
        sprintf(destino + strlen(destino), "%s %s\n", tipo_jump,rotulo);
        sprintf(destino + strlen(destino), "%s", bloco_codigo);
        sprintf(destino + strlen(destino), "%s:\n", rotulo);
}

void makeCodeIfElse(char* destino, char * codigo_expressao, char * tipo_jump, char* bloco_codigo_se, char* bloco_codigo_senao) {
        char rotulo_senao[16];
        char rotulo_fim[16];

        criaRotulo(rotulo_senao);
        criaRotulo(rotulo_fim);

        destino[0] = '\0';

        sprintf(destino + strlen(destino), "%s", codigo_expressao);
        sprintf(destino + strlen(destino), "%s %s\n", tipo_jump,rotulo_senao);

        sprintf(destino + strlen(destino), "%s", bloco_codigo_se);
        sprintf(destino + strlen(destino), "jmp %s\n", rotulo_fim);
        sprintf(destino + strlen(destino), "%s:\n", rotulo_senao);
        sprintf(destino + strlen(destino), "%s", bloco_codigo_senao);
        sprintf(destino + strlen(destino), "%s:\n", rotulo_fim);
}

void geraCodigoEnquanto(char* destino, char* codigo_expressao, char * tipo_jump, char* bloco_codigo) {
        char rotulo_loop[16];
        char rotulo_fim[16];

        criaRotulo(rotulo_loop);
        criaRotulo(rotulo_fim);

        destino[0] = '\0';

        sprintf(destino + strlen(destino), "%s:\n", rotulo_loop);
        sprintf(destino + strlen(destino), "%s", codigo_expressao);
        sprintf(destino + strlen(destino), "%s %s\n", tipo_jump,rotulo_fim);
        sprintf(destino + strlen(destino), "%s", bloco_codigo);
        sprintf(destino + strlen(destino), "jmp %s\n", rotulo_loop);
        sprintf(destino + strlen(destino), "%s:\n", rotulo_fim);
}
