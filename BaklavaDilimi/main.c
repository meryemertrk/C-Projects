#include <stdio.h>
#include <stdlib.h>

int main()
{
   int girilenDeger,i,j;
   printf("Bir sayi giriniz:");
   scanf("%d",&girilenDeger);

   for(i=1;i<=girilenDeger;i++){
    for(j=1;j<=girilenDeger-i;j++){
        printf(" ");
    }
    for(j=1;j<=2*i-1;j++){
        printf("*");
    }
    for(j=1;j<=girilenDeger-i;j++){
        printf(" ");
   }

     printf("\n");
   }

   for(i=1;i<=girilenDeger-1;i++){
    for(j=1;j<=i;j++)  {
        printf(" ");
    }
    for(j=1;j<=2*(girilenDeger-1)-2*i+1;j++){
        printf("*");
    }
     for(j=1;j<=i;j++)  {
        printf(" ");
    }

     printf("\n");
   }
    return 0;
}
