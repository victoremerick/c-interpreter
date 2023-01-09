#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAMVET(vet) (sizeof(vet)/sizeof((vet)[0]))

char alfabeto[]={
'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
'a','b','c','d','e','f','g','h','i','j','k','l','m',
'n','o','p','q','r','s','t','u','v','w','x','y','z',
'0','1','2','3','4','5','6','7','8','9',
'_',' ','!','#','%','&','(',')','*','+',',','-','.','/',':',';','<','=','>','?','[',']','^','{','|','}','~','\\',
'\n','\'','\t','\"'
};

enum tipos_token {
	CHAVES = 1,
	PARENTESES,
	INICIO,
	DELIMITADOR, 
	NUMERO,
	PONTUACAO,
	PALAVRA_CHAVE,
	STRING,
	IDENTIFICADOR,
	COMPARADOR,
	OPERADOR,
	OPERADOR_LOGICO,
	ATRIBUICAO,
	PREPROCESSADOR,
	FIM
};

enum comparadores {
	MEIG = 1, IG, MAIG, ME, MA, DF 
};
	
enum token_palavras_chave{
	AUTO = 1, DOUBLE, INT, STRUCT, BREAK, ELSE, LONG, SWITCH,
	CASE, ENUM, REGISTER, TYPEDEF, CHAR, EXTERN, RETURN,
	UNION, CONST, FLOAT, SHORT, UNSIGNED, CONTINUE, FOR,
	SIGNED, VOID, DEFAULT, GOTO, SIZEOF, VOLATILE, DO, IF,
	STATIC, WHILE
};

enum token_pre_processador{
	DEFINE = 1, INCLUDE, IFDEF, IFNDEF, ELIF, ENDIF,
	UNDEF, LINE, ERROR, PRAGMA, DEFINED
};

struct PreProcessador{
	char* palavra;
	int size;
	char token;
} tabelapp[13] = {
	"if", 2, DEFINE,
	"else", 4, ELSE,
	"elif", 4, IF,
	"line", 4, LINE,
	"ifdef", 5, IFDEF,
	"endif", 5, ENDIF,
	"undef", 5, UNDEF,
	"error", 5, ERROR,
	"pragma", 6, PRAGMA,
	"define", 6, DEFINE,
	"ifndef", 6, IFNDEF,
	"defined", 7, DEFINED,
	"include", 7, INCLUDE
};

struct PalavraChave{
	char* palavra;
	char token;
	int size;
} tabelapc[32] = {
	"do", DO, 2,
	"if", IF, 2,
	"int", INT, 3,
	"for", FOR, 3,
	"auto", AUTO, 4,
	"else", ELSE, 4,
	"long", LONG, 4,
	"case", CASE, 4,
	"char", CHAR, 4,
	"enum", ENUM, 4,
	"void", VOID, 4,
	"goto", GOTO, 4,
	"break", BREAK, 5,
	"union", UNION, 5,
	"const", CONST, 5,
	"float", FLOAT, 5,
	"short", SHORT, 5,
	"while", WHILE, 5,
	"double", DOUBLE, 6,
	"struct", STRUCT, 6,
	"switch", SWITCH, 6,
	"extern", EXTERN, 6,
	"return", RETURN, 6,
	"signed", SIGNED, 6,
	"sizeof", SIZEOF, 6,
	"static", STATIC, 6,
	"typedef", TYPEDEF, 7,
	"default", DEFAULT, 7,
	"register", REGISTER, 8,
	"unsigned", UNSIGNED, 8,
	"continue", CONTINUE, 8,
	"volatile", VOLATILE, 8
};

typedef struct {
	char* token;
	char type;
	int innerToken;
	int backToken;
	int nextToken;
} Token;

char* buffer;
char* init_buffer;
int size_buffer;

char *init;
char *prog;
int size_program;

FILE *arq;
char* name_arq;

int row = 1, col = 1;
char* init_row;

/*
	*******equals*******
	
	compara dois vetores de chars se são iguais

*/

int equals(char* s1, int t1, char* s2, int t2){
	if(t1 == t2){
		int i;
		for(i = 0; i< t1; i++){
			if(*s1 == *s2){
				s1++;
				s2++;
			}else{
				return 0;
			}
		}
		return 1;
	}else{
		return 0;
	}
}

/*Imprime na tela sem saltar linha no final*/

void print(char* text){
	int size_text = TAMVET(text);
	int i;
	for(i = 0; i<size_text; i++){
		printf("%c", *text);
		text++;
	}
}

/*Imprime na tela com salto de linha no final*/

void println(char* text){
	int size_text = TAMVET(text);
	int i;
	for(i = 0; i<size_text; i++){
		printf("%c", *text);
		text++;
	}
	printf("\n");
}

/*
	******load_program******

	Carrega o arquivo do programa na memória.
*/
void load_program(){
	arq = fopen(name_arq, "r");
	if (arq == NULL)
	{
		printf("Problemas na leitura do arquivo\n");
		return;
	}
	size_program = 0;
	char c;
	do{
		c = fgetc(arq);
		size_program++;
	}while (c != EOF);
	rewind(arq);
	prog = (char *) malloc(size_program);
	init = (char *) prog;
	do{
		c = fgetc(arq);
		*prog = c;
		prog++;
	}while (c != EOF);
	prog = init;
	fclose(arq);
}

/*
	*******next*******
	
	pega o proximo caracter e atualiza localização do ponteiro
	no codigo-fonte lido (row e col).

	p -> primeira execucao do next para iniciar debug corretamente.
*/

int p = 0;
void next(){	
	if(*prog == EOF){
		printf("\n");
		return;
	}
	if(p == 0){
		printf("%d\t",row);
		init_row = prog;
		p = 1;
	}
	printf("%c", *prog);
	if(*prog == '\n'){
		row++;
		col = 0;
		printf("%d\t",row);
		init_row = prog+1;
	}
	prog++;
	col++;
}

/*
	******ignore_coments******
	
	ignora comentarios e espaços em branco
*/

void ignore_coments(){
	while(strchr("/ \t",*prog)){
		if(*prog == '/'){
			if(*(prog+1) == '/'){
				while(*prog != '\n'){
					next();			
				}
				next();
			}else if(*(prog+1) == '*'){
				do{
					while(*prog != '*'){
						next();
					}
					next();
				}while(*prog != '/');
				next();
			}else{
				next();
			}
		}
		while(*prog == ' ' || *prog == '\t'){
			next();
		}
	}
}

/*
	******ignore_string*******
	
	ignora os caracteres que estiverem dentro da string ate
	encontrar outra \", que não seja uma aspas para escrita
	na string.
*/

int get_string(){
	if(*prog == '\"'){
		char* temp = prog;
		char* temp2;
		int cont;
		do{
			do{
				size_buffer++;
				prog++;
			}while(*prog != '"');
			temp2 = prog;
			prog--;
			cont = 0;
			while(*prog == '\\'){
				prog--;
				cont++;
			}
			prog = temp2;
			if(cont%2 == 1){
				size_buffer++;
				prog++;
			}
		}while(*prog != '"' && *prog != '\n' && *prog != EOF);
		if(*prog == '\n' || *prog == EOF){
			return -1;
		}
		size_buffer++;
		prog = temp;
		buffer = (char *) malloc(size_buffer);
		init_buffer = buffer;
		int i;
		for(i = 0; i< size_buffer; i++){
			*buffer = *prog;
			buffer++;
			next();
		}
		buffer = init_buffer;
		next();
		return 1;
	}
	return 0;
}


/*
	******ignore_char*******
	
	ignora os caracteres que estiverem dentro da string ate
	encontrar outra \", que não seja uma aspas para escrita
	na string.
*/

void get_char(){
	if(*prog == '\''){
		char* temp = prog;
		char* temp2;
		int cont;
		do{
			do{
				size_buffer++;
				prog++;
			}while(*prog != '\'');
			temp2 = prog;
			prog--;
			cont = 0;
			while(*prog == '\\'){
				prog--;
				cont++;
			}
			prog = temp2;
			if(cont%2 == 1){
				size_buffer++;
				prog++;
			}
		}while(*prog != '\'' && *prog != '\n' && *prog != EOF);
		if(*prog == '\n' || *prog == EOF){
			return -2;
		}
		size_buffer++;
		prog = temp;
		buffer = (char *) malloc(size_buffer);
		init_buffer = buffer;
		int i;
		for(i = 0; i< size_buffer; i++){
			*buffer = *prog;
			buffer++;
			next();
		}
		buffer = init_buffer;
		next();
		return 1;
	}
	return 0;
}

void get_Palavra(){
	free(buffer);
	size_buff = 0;
	char *temp = prog;
	do{
		size_buff++;
		prog++;
		if(*prog == '_'){
			size_buff++;
			prog++;
		}
	}while(isalpha(*prog) && *prog != EOF);
	prog = temp;
	buffer = (char *) malloc(size_buff);
	init_buffer = buffer;
	int i;
	for(i = 0; i < size_buff; i++){
		*buffer = *prog; 
		buffer++;
		next();
	}	
	buffer = init_buffer;
}

int get_preprocessador(){
	
}

void tokenizar(){
	
}

/*
	******verf_row_lex******

	Análise Léxica linha por linha;	

*/

char *verf_row_lex(Token t){
	char* t = prog;
	int aspas = 0;
	char* temp;
	int continua;
	do{		
		free(buffer);
		size_buffer = 0;
		continua = 1;
		ignore_coments();
		continua = get_string();
		if(continua == -1){
			return -1;
		}else if(continua == 1){
			//Criar Token
		}
		if(size_buffer == 0){
			continua = get_char();
			if(continua == -2){
				return -2;
			}
		}
		if(size_buffer == 0){
			if(*prog == '#'){
				
			}
		}
		if(*prog == EOF){
			return 0;
		}
		if(!strchr(alfabeto, *prog)){
			next();
			return t;
		}
		next();
	}while(!strchr("\n", *prog) && *prog != EOF);
	return 0;
}

/*
	******analise_lexica*******
	
	Análise léxica do arquivo inteiro.
*/

void analise_lexica(){
	char* colt;
	char* inicio;
	char* fim;
	Token t = malloc(sizeof(Token));
	t.type = INICIO;
	t.token = "";
	t.backToken = -1;
	t.innerToken = -1;
	t.nextToken = -1;
	do{
		inicio = prog;
		colt = verf_row_lex();
	}while(colt == 0 && *prog != EOF);
	if(colt!=0){
		char* t = init_row;
		int i;
		printf("\nErro %s -> ", name_arq);
		printf("Row:%d,Col:%d: Caracter Invalido\n", row, col);
		for(i = 0; i < col; i++){
			printf("%c", *t);
			t++;
		}
		printf("\n");
		for(i = 0; i < col-2; i++){
			printf(" ");
			t++;
		}
		printf("^\n", *t);
	}
}

int interpretar(){
	analise_lexica();
}

int main(){
	name_arq = "teste1.c";
	load_program();
	interpretar();
	system("PAUSE");
	return 0;
}