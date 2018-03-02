#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tabela_simbolos.h>

int hashFunction(int max_size, char* key) {
								int i = 0;
								int sum = 0;
								int factor = 0;
								while (key[i] != '\0') {
																sum += (key[i] + factor);
																factor += 113;
																i++;
								}

								// Metodo do resto da divisao
								return sum % max_size;
}

//Funcao que inicializa tabela Hash, colocando -1 em todos os numeros
int initSymTable(SymTable* table) {
								table->size = 0;
								table->max_size = MAX_SIZE_HASH_ARRAY;

								table->array = (SymTableNode*) malloc(table->max_size * sizeof(SymTableNode));
								if (table == NULL)
																return 0;

								// Inicializa cada posição do vetor
								for (int i = 0; i < table->max_size; i++) {
																table->array[i].data.identificador[0] = '\0';
																table->array[i].data.conteudo = NULL;
																table->array[i].data.len_conteudo = 0;

																table->array[i].next = NULL;
								}
								return 1;
}

// Funcao que realiza a impressao da Hash
void printSymTable(SymTable* table) {
								int i = 0;
								int cont = 0;

								while (i < table->max_size) {
																// Se nao tiver nada simplesmente imprime vazio
																if (table->array[i].data.identificador[0] == '\0') {
																								// printf("[%d]: VAZIO\n", i);
																								;
																} else {
																								SymTableNode *aux = &table->array[i];

																								while (aux != NULL) {
																																// printf("[%d]: {%s} {%d} {%s}", i, aux->data.identificador, aux->data.tipo, aux->data.conteudo);
																																// if (aux->next != NULL)
																																								// printf("\n\t| ");
																																aux = aux->next;
																								}
																								// printf("\n");
																}
																i++;
								}
}


//Funcao de insercao na tabela Hash
int addSymTable(SymTable* table, char* identificador, Tipo tipo) {
								int pos;
								pos = hashFunction(table->max_size, identificador);

								SymTableNode* aux = NULL;

								// Verifica se a posicao esta livre
								if (table->array[pos].data.identificador[0] == '\0')
																aux = &table->array[pos];
								else {
																// verificando se ja nao se trata de uma dado repetido
																aux = &table->array[pos];
																while (aux != NULL && strcmp(aux->data.identificador, identificador) != 0)
																								aux = aux->next;
																if (aux != NULL) return 0;

																aux = (SymTableNode*) malloc(sizeof(SymTableNode));
																if (aux == NULL) return 0;

																// A tabela passa a apontar para o aux alocado,
																// assim sempre será inserido no inicio e vai "empurrando a lista"
																aux->next = table->array[pos].next;
																table->array[pos].next = aux;

								}

								// Identifier
								strncpy(aux->data.identificador, identificador, MAX_SIZE_SYMBOL - 1);
								aux->data.identificador[MAX_SIZE_SYMBOL - 1] = '\0';

								// Tipo
								aux->data.tipo = tipo;

								// Incrementa contador da quantidade de elementos na tabela
								table->size += 1;

								return 1;
}

//Função que realiza a busca pelo valor na hash
SymTableEntry* findSymTable(SymTable* table, char* identificador) {
								int pos = hashFunction(table->max_size, identificador); //descobrindo a posicao onde foi inserido

								SymTableNode *aux;
								aux = &(table->array[pos]);

								while ((aux != NULL) && (strcmp(aux->data.identificador, identificador) != 0))
																aux = aux->next;

								//Ele ira para por nao ter chegado em nulo e nao ter encontrado.
								if (aux == NULL)
																return NULL;
								else
																return &aux->data;
}

void freeSymTable(SymTable* table) {
								SymTableNode* node;
								SymTableNode* aux;
								for (int i = 0; i < table->max_size; i++) {
																aux = NULL;
																node = &table->array[i];
																if (node->data.identificador[0] != '\0') {
																								free(node->data.conteudo);
																								node = node->next;

																								while (node != NULL) {
																																aux = node;
																																node = node->next;

																																free(aux->data.conteudo);
																																free(aux);
																								}
																}
								}
								free(table->array);
}
