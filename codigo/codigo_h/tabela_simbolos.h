#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SIZE_HASH_ARRAY 20
#define MAX_SIZE_SYMBOL 32

typedef enum {INTEIRO, REAL, STRING} Tipo; //MUDAR PARA TIPO EM PT-BR

extern int contador_linhas;

//Declaracao de tipo de dados
struct symTableEntry {
        char identificador[MAX_SIZE_SYMBOL];
        Tipo tipo;
        char* conteudo;
        int len_conteudo;
};
typedef struct symTableEntry SymTableEntry;

//Tipo de dado para definicao do no
struct symTableNode {
        SymTableEntry data;
        struct symTableNode *next;

};
typedef struct symTableNode SymTableNode;

struct symTable {
        SymTableNode* array;
        int max_size;
        int size;
};
typedef struct symTable SymTable;

extern SymTable tabela_simbolos;

//Prototipos das funcoes
int initSymTable(SymTable* table);
void printSymTable(SymTable* table);
int addSymTable(SymTable* table, char* identificador, Tipo tipo);
SymTableEntry* findSymTable(SymTable* table, char* identificador);
void freeSymTable(SymTable* table);

#endif
