/*
* arquivo: fatorial.c
* -------------------
* Dado n (n >= 0), este programa imprime n!=n*(n-1)*...*1.
* Fornecer n <13
*/
#include <stdio.h>
int main()
{
int n, i, fatorial;
printf("Entre o valor de n (0 <= n < 13): ");
scanf("%d", &n);
fatorial = 1;
i = 1;
while (i <= n) {
fatorial = fatorial * i;
i++; /* equivalente a: i = i + 1 */
}
printf("%d! = %d\n", n, fatorial);
return 0;
}