#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char alpha[]={
'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
'a','b','c','d','e','f','g','h','i','j','k','l','m',
'n','o','p','q','r','s','t','u','v','w','x','y','z',
'0','1','2','3','4','5','6','7','8','9',
'_',' ','!','#','%','&','(',')','*','+',',','-','.','/',':',';','<','=','>','?','[',']','^','{','|','}','~','\\',
'\n','\'','	'
};

enum tipos_token {
	BLOCO = 1,
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

char tp_comparador;
	
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
} tabelapp[12] = {
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
char* buff;
char* init_buff;
int size_buff;
char tp_token;

char *init;
char *prog;
int size_program;
FILE *arq;

int linha = 1, coluna = 1;

char analisador_lexico(void);
void getNumero(void), getPalavra(void), getString(void), getChar(void);
int isPalavraChave(void);

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
/*Carrega o arquivo na memória e fecha a leitura de arquivo*/
void load_program(){
	arq = fopen("teste.c", "r");
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
		if(c!=EOF){
			prog++;
		}
	}while (c != EOF);
	prog = init;
	fclose(arq);
}

void printfb(){
	buff = init_buff;
	int i;
	for(i = 0; i<size_buff; i++){
		printf("%c", *buff);
		buff++;
	}
	buff = init_buff;
}

int main(int argc, char *argv[]){
	load_program();
	
	int c = -1;
	while(c != FIM){
		c = analisador_lexico();
		tp_token = c;
		switch(c){
			case BLOCO:
				printf("%c - Bloco\n",*prog);
				break;
			case NUMERO:
				printfb();
				printf(" - Numero\n");
				break;
			case PONTUACAO:
				printf("%c - Delimitador\n",*prog);
				break;
			case PALAVRA_CHAVE:
				printfb();
				printf(" - Palavra-chave\n");
				break;
			case STRING:
				printfb();
				printf(" - String\n");
				break;
			case CHAR:
				printfb();
				printf(" - Char\n");
				break;
			case IDENTIFICADOR:
				printfb();
				printf(" - Identificador\n");
				break;
			case OPERADOR:
				printf("%c - Operador\n", *prog);
				break;
			case DELIMITADOR:
				printf("%c - Delimitador\n", *prog);
				break;
			//case COMPARADOR:
			//	printfb();
			//	printf(" - Comparador\n");
			//	break;
			//case ATRIBUICAO:
			//	printf("%c - Atribuicao\n", *prog);
			//	break;
			default:
				printf("%c - Nao identificado\n", *prog);
				break;
		}
		prog++;
	}
	return 0;
}

char analisador_lexico(){
	char ret = NULL;
	//Ignorar comentários ou espaços em branco
	while(strchr("/ \t\n",*prog)){
		if(*prog == '/'){
			if(*(prog+1) == '/'){
				while(*prog != '\n') prog++;
			}else if(*(prog+1) == '*'){
				do{
					while(*prog != '*') prog++;
					prog++;
				}while(*prog != '/');
				prog++;
			}
		}
		prog++;
	}
	//Verificar se arquivo acabou
	if(*prog == EOF){
		ret = FIM;
	}else
	//Verifica CHAR
	if(*prog == '\''){
		ret = CHAR;
		getChar();
	}else
	//Verifica #
	if(*prog == '#'){
		ret = PREPROCESSADOR;
		getChar();
	}else
	//Verifica STRING
	if(*prog == '"'){
		ret = STRING;
		getString();
	}else
	//Verifica condicionais
	if(strchr("!=<>",*prog)){
		free(buff);
		buff = (char *) malloc(2);
		size_buff = 2;
		tp_comparador = 0;
		if(*prog == '!' && *(prog+1) == '='){
			buff = "!=";
			prog++;
			tp_comparador = DF;
		}else if(*prog == '=' && *(prog+1) == '='){
			buff = "==";
			prog++;
			tp_comparador = IG;
		}else if(*prog == '<' && *(prog+1) == '='){
			buff = "<=";
			prog++;
			tp_comparador = MEIG;
		}else if(*prog == '>' && *(prog+1) == '='){
			buff = ">=";
			prog++;
			tp_comparador = MAIG;
		}else if(*prog == '<'){
			if(*(prog+1)=='<'){
				prog++;
				if(*(prog+1)=='='){
					buff = "<<=";
					size_buff = 3;
					//ret = ATRIBUICAO;
				}else{
					buff = "<<";
				//	ret = OPERADOR;
				}
			}else{
				buff = ">";
				size_buff = 1;
				tp_comparador = ME;
			}
		}else if(*prog == '>'){
			if(*(prog+1)=='>'){
				prog++;
				if(*(prog+1)=='='){
					buff = ">>=";
					size_buff = 3;
					//ret = ATRIBUICAO;
				}else{
					buff = ">>";
				//	ret = OPERADOR;
				}
			}else{
				buff = ">";
				size_buff = 1;
				tp_comparador = MA;
			}
		}else if(*prog == '='){
			buff = "=";
			//ret = OPERADOR;
		}
		if(tp_comparador != 0){
		}
			ret = OPERADOR;
	}else
	//Verifica BLOCO DE CÓDIGO
	if(strchr("{}",*prog)){
		ret = BLOCO;
	}else
	//Verifica DELIMITADOR DE CÓDIGO
	if(strchr(",;.()",*prog)){
		ret = PONTUACAO;
	}else
	if(strchr("/*-+%",*prog)){
		ret = OPERADOR;
	}else
	if(strchr("&|",*prog)){
		ret = OPERADOR_LOGICO;
		
	}else
	//Verificar Numero, caso for número guarda ele no buffer
	if(isdigit(*prog)){
		ret = NUMERO;
		getNumero();
	}else
	//Verificar Letra e guarda palavra no buffer
	if(isalpha(*prog) || *prog=='_'){
		getPalavra();
		if(isPalavraChave()>=0){
			ret = PALAVRA_CHAVE;
		}else{
			ret = IDENTIFICADOR;
		}
	}
	return ret;
}

void getNumero(){
	free(buff);
	size_buff = 0;
	char *temp = prog;
	while(isdigit(*prog) && *prog != EOF){
		size_buff++;
		prog++;
	}
	prog = temp;
	buff = (char *) malloc(size_buff);
	init_buff = buff;
	while(isdigit(*prog) && *prog != EOF){
		*buff = *prog;
		buff++;
		prog++;
	}
	buff = init_buff;
	prog--;
}

void getPalavra(){
	free(buff);
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
	buff = (char *) malloc(size_buff);
	init_buff = buff;
	int i;
	for(i = 0; i < size_buff; i++){
		*buff = *prog; 
		buff++;
		prog++;
	}	
	buff = init_buff;
	prog--;
}

void getString(){
	free(buff);
	size_buff = 0;
	char *temp = prog;
	char *temp2;
	int cont;
	do{
		do{
			size_buff++;
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
			size_buff++;
			prog++;
		}
	}while(*prog != '"' && *prog != EOF);
	size_buff++;
	prog++;
	prog = temp;
	buff = (char *) malloc(size_buff);
	init_buff = buff;
	int i;
	for(i = 0; i < size_buff; i++){
		*buff = *prog; 
		buff++;
		prog++;
	}	
	buff = init_buff;
}

void getChar(){
	free(buff);
	size_buff = 0;
	char *temp = prog;
	int max_size = 4;
	char *temp2;
	int cont;
	do{
		do{
			size_buff++;
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
			size_buff++;
			prog++;
		}
	}while(*prog != '\'' && *prog != EOF);
	size_buff++;
	prog++;
	prog = temp;
	buff = (char *) malloc(size_buff);
	init_buff = buff;
	int i;
	for(i = 0; i < size_buff; i++){
		*buff = *prog; 
		buff++;
		prog++;
	}	
	buff = init_buff;
	if(size_buff>max_size){
		printf("Tamanho invalido para char: ");
	}
}

int isPalavraChave(){
	int i;
	char* palavra;
	for(i = 0; i< 32; i++){
		if(equals(tabelapc[i].palavra, tabelapc[i].size, buff, size_buff)){
			return tabelapc[i].token;
		}
	}
	return -1;
}