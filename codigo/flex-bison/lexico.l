/* Seção das Definicões */
%{
	#include <string.h>
	#include <stdio.h>
	#include <y.tab.h>
	#include <tabela_simbolos.h>
	void yyerror(char *);
%}

espaco 	[ \t]
letra 	[A-Za-z]
digito 	[0-9]
aspas 	['\"']
string 	[^'\"']


%%	/* Seção das regras*/

"//".*\n  {
	contador_linhas++;
}

"inteiro"	{
	strcpy(yylval.simbolo.conteudo, yytext);
	return TIPO_INTEIRO;
}

"real" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return TIPO_REAL;
}

"string" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return TIPO_STRING;
}

"faca" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return FACA;
}

"leia" {
	strcpy(yylval.simbolo.conteudo,yytext);
	return LEIA;
}

"escreva" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return ESCREVA;
}

"se" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return SE;
}

"entao" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return ENTAO;
}

"senao" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return SENAO;
}

"enquanto" {
	strcpy(yylval.simbolo.conteudo, yytext);
	return ENQUANTO;
}

"<=" {
	return MENOR_IGUAL;
}

">=" {
	return MAIOR_IGUAL;
}

"==" {
	return IGUAL;
}

"!=" {
	return DIFERENTE;
}

">" {
	return MAIOR;
}

"<" {
	return MENOR;
}

{digito}+ {
	strcpy(yylval.simbolo.conteudo, yytext);
	return NUM;
}

{digito}+"."{digito}+ {
	strcpy(yylval.simbolo.conteudo, yytext);
	return NUM;
}

{letra}({letra}|{digito}|_)* {
	strcpy(yylval.simbolo.conteudo, yytext);
	return ID;
}

{aspas}{string}*{aspas} {
	strcpy(yylval.simbolo.conteudo, yytext);
	return LITERAL_STRING;
}

[-;:={}+*()/] {
	return *yytext;
}

{espaco} {

}

\n {
	contador_linhas++;
}

. {
	yyerror("Caractere inválido!");
}

%%	/* Seção das Subrotinas */

int yywrap (void)
{
 	return 1;
}
