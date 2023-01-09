/*
* arquivo: piramide10.c
* ------------------------
* Este programa imprime uma tabela triangular como a sguinte:
*
* 0
* 0 1 2
* 0 1 2 3 4
* 0 1 2 3 4 5 6
* 0 1 2 3 4 5 6 7 8
* 0 1 2 3 4 5 6 7 8 9 10
*
* Exercicio: mudar o programa abaixo de modo a imprimir
* uma tabela semelhante que vai ae' 20.
*/
#include <stdio.h>
int main()
{
int i, j, spaces;
spaces = 5;
for (i = 0; i <= 10; i = i + 2) {
for (j = 0; j < spaces; j++)
printf(" "); // Para fazer um deslocamento de 'spaces' brancos.
// Aqui cada branco e' o total de casas entre as aspas
for (j = 0; j <= i; j++)
printf("%3d", j);
printf("\n");
spaces--;
}
return 0;
}