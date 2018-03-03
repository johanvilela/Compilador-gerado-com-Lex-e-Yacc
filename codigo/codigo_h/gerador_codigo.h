#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>

// Tamanho de um string nao inicializada
#define TAM_STRING 256

extern FILE* saida_asm;

void criaRotulo(char* rotulo_saida);
void geraCodigoDeclaracao(char* destino, char* identificador, Tipo tipo);
void inicioSecaoCodigo();

int geraCodigoLeia(char* destino, char *id);
int geraCodigoEscreva(char* destino, char *id);

int geraCodigoAtribuicao(char* destino, char* id, char* expressao);
int geraCodigo_termo_e_fator(char* destino, char* id, int ref);

void geraCodigoSoma(char* destino, char* conteudo);
void geraCodigoSubtracao(char* destino, char* conteudo);
void geraCodigoMultiplicacao(char* destino, char* conteudo2);
void geraCodigoDivisao(char* destino, char* conteudo2);

int geraCodigoComparacao(char* destino, char* id, char* expressao);
void geraCodigoSe(char* destino, char* codigo_expressao, char * tipo_jump, char* bloco_codigo);
void geraCodigoEnquanto(char* destino, char* codigo_expressao, char * tipo_jump, char* bloco_codigo);
void makeCodeIfElse(char* destino, char* codigo_expressao, char * tipo_jump,char* bloco_codigo_se, char* bloco_codigo_senao);

#endif
