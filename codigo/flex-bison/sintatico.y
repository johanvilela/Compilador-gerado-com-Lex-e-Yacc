%{
  #include <string.h>
  #include <stdio.h>
  #include <tabela_simbolos.h>
  #include <gerador_codigo.h>

  void yyerror(char*);
  int yylex();

  extern SymTable tabela_simbolos;
%}

%union {
	struct simbolo {
		char conteudo[2044];  // Armazena código ASM
    char tipo_jump[4];
	} simbolo;
}

%type <simbolo> programa
%type <simbolo> declaracoes
%type <simbolo> declaracao bloco
%type <simbolo> declaracao_inteiro
%type <simbolo> declaracao_real
%type <simbolo> declaracao_string
%type <simbolo> comandos
%type <simbolo> comando
%type <simbolo> comando_escrita
%type <simbolo> comando_leitura
%type <simbolo> comando_atribuicao
%type <simbolo> expressao_numerica
%type <simbolo> termo fator
%type <simbolo> expressao_booleana
%type <simbolo> operador_relacional
%type <simbolo> comando_se
%type <simbolo> comando_se_senao
%type <simbolo> comando_enquanto

%token <simbolo> VAR
%token <simbolo> ID
%token <simbolo> NUM
%token <simbolo> LITERAL_STRING
%token <simbolo> TIPO_INTEIRO
%token <simbolo> TIPO_REAL
%token <simbolo> TIPO_STRING
%token <simbolo> LEIA
%token <simbolo> ESCREVA
%token <simbolo> SE
%token <simbolo> ENTAO
%token <simbolo> SENAO
%token <simbolo> ENQUANTO
%token <simbolo> FACA
%token <simbolo> MENOR
%token <simbolo> MAIOR
%token <simbolo> MENOR_IGUAL
%token <simbolo> MAIOR_IGUAL
%token <simbolo> IGUAL
%token <simbolo> DIFERENTE

%left '+' '-'
%left '*' '/'

%%	/* Seção das regras de produção */


programa:             declaracoes bloco {
                    		fprintf(saida_asm, "%s", $1.conteudo);
                    		inicioSecaoCodigo();
                    		fprintf(saida_asm, "%s", $2.conteudo);
                    	};


declaracoes:          declaracao declaracoes {
                    		strcpy($$.conteudo, $1.conteudo);
                    		sprintf($$.conteudo + strlen($$.conteudo), "%s", $2.conteudo);
                    	}
                    	| %empty {
                        $$.conteudo[0] = '\0';
                      };


declaracao:           declaracao_inteiro {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | declaracao_real {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | declaracao_string {
                        strcpy($$.conteudo, $1.conteudo);
                      };


declaracao_inteiro:   TIPO_INTEIRO ID ';' {
                        addSymTable(&tabela_simbolos, $2.conteudo, INTEIRO);
                        geraCodigoDeclaracao($$.conteudo, $2.conteudo, INTEIRO);
                      };


declaracao_real:      TIPO_REAL ID ';' {
                        addSymTable(&tabela_simbolos, $2.conteudo, REAL);
                        geraCodigoDeclaracao($$.conteudo, $2.conteudo, REAL);
                      };


declaracao_string:    TIPO_STRING ID ';' {
                        addSymTable(&tabela_simbolos, $2.conteudo, STRING);
                        geraCodigoDeclaracao($$.conteudo, $2.conteudo, STRING);
                      };


bloco :               '{' comandos '}' {
                        strcpy($$.conteudo, $2.conteudo);
                      };


comandos :            comando comandos {
                        sprintf($$.conteudo + strlen($$.conteudo), "%s", $2.conteudo);
                      }
                      | %empty {
                        $$.conteudo[0] = '\0';
                      };


comando:              comando_se {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | comando_se_senao {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | comando_enquanto {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | comando_leitura {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | comando_atribuicao {
                        strcpy($$.conteudo, $1.conteudo);
                      }
                      | comando_escrita {
                        strcpy($$.conteudo, $1.conteudo);
                      };


comando_leitura:      LEIA ID ';'  {
                        if (!geraCodigoLeia($$.conteudo, $2.conteudo))
                        YYABORT;
                      };


comando_escrita:      ESCREVA ID ';' {
  		                  if (!geraCodigoEscreva($$.conteudo, $2.conteudo))
                          YYABORT;
                      };


comando_atribuicao:   ID '=' expressao_numerica ';' {
                        if (!geraCodigoAtribuicao($$.conteudo, $1.conteudo, $3.conteudo))
                        YYABORT;
                      };

                      /* REGRA OK*/ /* NÃO FUNCIONA COM NUMERO REAL*/
expressao_numerica:   expressao_numerica '+' expressao_numerica {
                        geraCodigoSoma($$.conteudo, $3.conteudo);
                      }
                      | expressao_numerica '-' expressao_numerica {
                    		geraCodigoSubtracao($$.conteudo, $3.conteudo);
                    	}
                      | termo  {
                    		strcpy($$.conteudo, $1.conteudo);
                    	}
                      | termo '*' fator  {
                      	geraCodigoMultiplicacao($1.conteudo, $3.conteudo);
                      	strcpy($$.conteudo, $1.conteudo);
                      }
                      | termo '/' fator  {
                      	geraCodigoDivisao($$.conteudo, $3.conteudo);
                      };


termo:                NUM {
              		      makeCodeLoad($$.conteudo, $1.conteudo, 0);
                      }
                      | ID  {
                        if (!makeCodeLoad($$.conteudo, $1.conteudo, 1))
                        YYABORT;
                      };


fator:                NUM {
              		      makeCodeLoad($$.conteudo, $1.conteudo, 0);
                      }
                      | ID {
                        makeCodeLoad($$.conteudo, $1.conteudo, 1);
                      }
                      | '(' expressao_numerica ')'  {
                        strcpy($$.conteudo, $2.conteudo);
                      };


comando_se:           SE '(' expressao_booleana ')' ENTAO bloco {
                        geraCodigoSe($$.conteudo, $3.conteudo, $3.tipo_jump, $6.conteudo);
                      };


comando_se_senao:     SE '(' expressao_booleana ')' ENTAO bloco SENAO bloco {
                        makeCodeIfElse($$.conteudo, $3.conteudo, $3.tipo_jump, $6.conteudo, $8.conteudo);
                      };


comando_enquanto:     ENQUANTO '(' expressao_booleana ')' FACA bloco {
                        geraCodigoEnquanto($$.conteudo, $3.conteudo, $3.tipo_jump, $6.conteudo);
                      };


expressao_booleana:   ID operador_relacional expressao_numerica {
                        strcpy($$.tipo_jump,$2.tipo_jump);
                        if (!geraCodigoComparacao($$.conteudo, $1.conteudo, $3.conteudo))
                          YYABORT;
                      };


operador_relacional:  MENOR {
                        strcpy($$.tipo_jump,"jnl");
                      }
  	                  | MAIOR {
                        strcpy($$.tipo_jump,"jng");
                      }
  	                  | MENOR_IGUAL {
                        strcpy($$.tipo_jump,"jg");
                      }
  	                  | MAIOR_IGUAL {
                        strcpy($$.tipo_jump,"jl");
                      }
  	                  | IGUAL {
                        strcpy($$.tipo_jump,"jne");
                      }
  	                  | DIFERENTE {
                        strcpy($$.tipo_jump,"je");
                      };

%%	/* Seção das Subrotinas */

void yyerror(char *tipo_do_erro) {
   fprintf(stderr, "ERRO NA LINHA %d: %s\n", contador_linhas, tipo_do_erro);
}
