
 # CCC - Compilador que Compila Código
 
Esse compilador foi desenvolvido para o propósito didádico na disciplina de Compiladores.

A saída do compilador é um arquivo ASM 64bits, para obter o executável use o montador NASM e um linker.

--------------------------------------------
### Trabalho final da disciplina de Compiladores - 2017/02

__Descrição:__ 
O objetivo do trabalho é desenvolver um compilador que implemente a gramática abaixo.  
Os símbolos em *itálico* são não terminais e os símbolos em **negrito** são terminais.

* _programa_ -> _declaracoes  bloco_
* _declaracoes_ -> _declaracao declaracoes_ | ɛ
* _declaracao_ -> _declaracao_inteiro_ | _declaracao_real_  | _declaracao_string_
* _declaracao_inteiro_ -> __inteiro  id__; 
* _declaracao_real_ -> __real  id__  ; 
* _declaracao_string_ -> __string   id__; 
* _bloco_ -> __{__ _comandos_ __}__
* _comandos_ -> _comando comandos_ | _ɛ_
* _comando_ ->  _comando_se_ | _comando_se_senao_ | _comando_enquanto_ | _comando_atribuicao_ | _comando_leitura_ | _comando_escrita_
* _comando_se_ -> __se__ **(** _expressao_booleana_ **)** __entao__  _bloco_
* _comando_se_senao_ -> __se__ **(** _expressao_booleana_ **)** __entao__ _bloco_ __senao__ _bloco_
* _comando_enquanto_ -> __enquanto__ **(** _expressao_booleana_ **)** __faca__ _bloco_
* _comando_atribuicao_ -> __id__  =  _expressao_numerica_;
* _comando_leitura_ -> **leia** __id__;*
* _comando_escrita_ -> **escreva** __id__;* 
* _expressao_booleana_ -> __id__  _operador_relacional_  _expressao_numerica_
* _operador_relacional_ ->  __>__ | __<__ | **>=** | **=<** | **==** | **!=**
* *expressao_numerica* -> *expressao_numerica* + *expressao_numerica*  | *expressao_numerica* - *expressao_numerica*
                      | *termo*  | *termo* * *fator* | *termo* / *fator*
* _termo_ -> __num__ | __id__ 
* *fator* -> __num__ | __id__ | ( *expressao_numerica* )

As análises léxica e sintática serão realizadas utilizando as ferramentas _Flex_ e _Bison_.  
O código gerado deverá ser um código __assembly de 64 bits__ para processadores da família __x86__ e o arquivo gerado deverá ser lido pelo montador _nasm_ para geração do código executável.

--------------------------------------------

### Dependências
* BISON - Versão 3
* FLEX
* NASM
* GCC

### Como compilar o CCC
    $ make

### Como usar o CCC para compilar código da linguagem descrita
    $ ./ccc <código.txt>
    ou
    $ make compila var=<código>

### Limpar os arquivos gerados na compilacao do Bison/Flex (exceto o executável)
    $ make clean

### Limpar os arquivos gerados na compilacao do Bison/Flex (inclusive o executável)
	$ make clean-all

### Montar o arquivo ASM de saída do compilador
    $ nasm -f elf64 <código.asm>
	ou
	$ make monta var=<código>

### Linkar o arquivo O de saída do montador
    $ gcc -g <código.o> -o <código>
    ou
    $ make linka var=<código>

### Executar o programa
	$ ./<código>
