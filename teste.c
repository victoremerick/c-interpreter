#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char alfabeto[]={
'A','B','C','D','E','F','G','H','I','J','K','L','M',
'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
'a','b','c','d','e','f','g','h','i','j','k','l','m',
'n','o','p','q','r','s','t','u','v','w','x','y','z',
'0','1','2','3','4','5','6','7','8','9',
'_',' ','!','#','%','&','(',')','*','+',',','-','.','/',':',';','<','=','>','?','[',']','^','{','|','}','~','\\',
'\n','\'','	','"'};


" � "

"  '\"' � '\"' "

'\"' � '\"'

int main(int argc, char *argv[]) {
	char ch = 'a';
	/* � 

*/
	FILE *arq;
	int barra = 0;
	int linha = 1;
	int testeStr = 0;
	arq = fopen(argv[1],"rt");
	if(arq == NULL){
		printf("%s","N foi possivel � abrir arquivo, possivelmente vazio.\n");
	}else{
		while((ch != EOF)&&((ch=fgetc(arq))!= EOF)){
			if(ch == '\n'){
				linha++;
			}
			//Programa Aqui!!!!
			int antes = 0;
			if(strchr(alfabeto,ch)){
				//Se ele pertence ao alfabeto 
				//Teste de coment�rio
				if(ch == '/'){
					if(((ch=fgetc(arq))!= EOF)&&(ch == '/')){	
					while(((ch=fgetc(arq))!= EOF)&&(ch != '\n')){
						//faz a verifica��o de coment�rio
					}
					linha++;
				}else if(ch == '*'){ // caiu em /*
							while(1){
							  if(((ch=fgetc(arq))!= EOF)&&(ch == '*')){
							  	 if(((ch=fgetc(arq))!= EOF)&&(ch == '/')){
							  	 	break;
								   }
							  }
							  if(ch == '\n'){
							  	linha++;
							  }
							} 
					}else{
					//n�o era, foi puxado mais um char
					antes = 1;
					}
				}
				//Teste de '"'
				if(ch == '\''){
					if((ch=fgetc(arq))=='"'){
						if(((ch=fgetc(arq))!= EOF)&&(ch == '\'')){
							//caso acontece '"'	
						}else{
							//caso seja string "  "
							while(((ch=fgetc(arq))!= EOF)&&(ch != '"')){
							//faz a verifica��o de String
								if(ch == '\n'){
								linha++;
								}	
							}
						}
					}else{
						antes = 1;
						//n�o era, foi puxado mais um char
					}
				}
				
				if(ch == '"'){
					while(((ch=fgetc(arq))!= EOF)&&(ch != '"')){
						if(ch == '\n'){
							linha++;
						}
					}
				}
				

				
				if((antes == 1)&&(!strchr(alfabeto,ch))){
					printf("%s%c%s%d","Char invalido -- ",ch," -- na linha ",linha);
					break;
				}
				
			}else{
				//Se ele n�o pertence
				printf("%s%c%s%d","Char invalido -- ",ch," -- na linha ",linha);
				break;		
			}
			
			//Termina aqui
		}
	}
	fclose(arq);
	return 0;
}
