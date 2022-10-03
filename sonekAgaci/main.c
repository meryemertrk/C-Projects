#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define maxchar 256

/*Oncelikle Ukkonen'in sonek agac yapisi algoritmasini isleyen bir programi, sonrasinda da sonek agacimizi olusturuyoruz.
Bundan sonra gerceklesecek islemler, komut satirindan alinan probleme gore sekillenecektir.
(Her problem fonksiyonunun icinde sonek agacini en bastan olusturarak is yuku yaratmamak adina problem fonksiyonlarimizdan once sonek agacimizi olusturduk.)*/


struct sonekagacidugumu
{
    struct sonekagacidugumu *children[maxchar];
    struct sonekagacidugumu *sonekbaglantisi;

    int baslangic;
    int *bitis;
    //parent/child dugumlerinin birbirine baglanmasi icin baslangic ve bitis olusturduk.

    int sonekindeks; //kokten yapraga gidis yolu

};

typedef struct  sonekagacidugumu Dugum; //structimiza yeni ad atadik

char text[100]; //girilecek stringimiz
Dugum *kok = NULL;

Dugum *sonYeniDugum = NULL;
Dugum *aktifDugum = NULL;
int sayac=0;

int olanKenar = -1; //olanKenar= girilecek string karakterin indexi
int olanUzunluk = 0;

int kalansonek = 0;
int yaprakBitis = -1;
int *kokBitis = NULL;
int *ucBitis = NULL;
int size = -1; //girilecek stringin uzunlugu

//yeni dugum acan fonksiyonumuz
Dugum *yeniDugum(int baslangic, int *bitis)
{
    sayac++;
    Dugum *dugum = (Dugum*) malloc(sizeof(Dugum));
    int i;

    for(i=0; i<maxchar; i++)
        dugum->children[i] = NULL;

    dugum->sonekbaglantisi=kok;
    dugum->baslangic=baslangic;
    dugum->bitis=bitis;

    dugum->sonekindeks = -1;

    return dugum;

}

int kenarUzunluk(Dugum *d)
{
    return *(d->bitis) - (d->baslangic)+1;

}

//dugumde dolasma fonksiyonumuz
int dugumDolas(Dugum *mevcutdugum)
{
    if (olanUzunluk >= kenarUzunluk(mevcutdugum))
    {
        olanKenar = (int)text[olanKenar+kenarUzunluk(mevcutdugum)]-(int)' ';
        olanUzunluk -= kenarUzunluk(mevcutdugum);
        aktifDugum=mevcutdugum;
        return 1;

    }
    return 0;

}

//suffix ekleyen fonksiyonumuz
void sonekagaciuzatma(int a)
{
    yaprakBitis=a;

    kalansonek++;

    sonYeniDugum=NULL;

    //henuz eklenmemis tum suffixleri teker teker ekleniyor
    while(kalansonek>0)
    {
        if(olanUzunluk==0)
        {
            olanKenar=(int)text[a]-(int)' ';
        }

        if(aktifDugum->children[olanKenar] == NULL)
        {
            aktifDugum->children[olanKenar]=yeniDugum(a, &yaprakBitis);

            if(sonYeniDugum!=NULL)
            {
                sonYeniDugum->sonekbaglantisi=aktifDugum;
                sonYeniDugum=NULL;
            }
        }

        else
        {
            Dugum *sonra = aktifDugum->children[olanKenar];


            if (dugumDolas(sonra))
            {
                //sonraki dugumden baslaniyor
                continue;
            }

            if(text[sonra->baslangic + olanUzunluk] == text[a]) //islenen karakter kenarda zaten isleniyorsa
            {
                if(sonYeniDugum!=NULL && aktifDugum != kok)
                {
                    sonYeniDugum->sonekbaglantisi=aktifDugum;
                    sonYeniDugum=NULL;
                }

                olanUzunluk++; //bu asamadaki diger islemler durur ve sonraki asamaya gecilir
                break;
            }

            ucBitis=(int*)malloc(sizeof(int));
            *ucBitis=sonra->baslangic+olanUzunluk-1;

            Dugum *ayir=yeniDugum(sonra->baslangic, ucBitis);
            aktifDugum->children[olanKenar]=ayir;

            ayir->children[(int)text[a]-(int)' ']=yeniDugum(a, &yaprakBitis);
            sonra->baslangic+=olanUzunluk;
            ayir->children[olanKenar]=sonra;

            if(sonYeniDugum!=NULL)
            {
                sonYeniDugum->sonekbaglantisi=ayir;
            }

            sonYeniDugum=ayir;
        }

        kalansonek--;

        if(aktifDugum==kok && olanUzunluk>0)
        {
            olanUzunluk--;
            olanKenar=(int)text[a -kalansonek + 1]-(int)' ';
        }

        else if(aktifDugum!=kok)
        {
            aktifDugum=aktifDugum->sonekbaglantisi;
        }


    }

}

//print fonksiyonumuz
void bastir(int x, int y)
{
    int z;
    for(z=x; z<=y; z++)
        printf("%c", text[z]);

}

//her kenar sonek indeksi ile birlikte yazdirilacak
//suffix index koyan fonksiyonumuz
void sonekagaciindexi(Dugum *d, int uzunluk)
{
    if(d==NULL)
        return;

    if(d->baslangic!=-1)
    {
        bastir(d->baslangic, *(d->bitis));
    }

    int yaprak=1;
    int i;
    for(i=0; i<maxchar; i++)
    {
        if(d->children[i]!=NULL)
        {
            if(yaprak==1 && d->baslangic != -1)
                printf("[%d]\n", d->sonekindeks);
            yaprak=0;
            sonekagaciindexi(d->children[i], uzunluk + kenarUzunluk(d->children[i]));
        }
    }

    if(yaprak==1)
    {
        d->sonekindeks=size-uzunluk;
        printf("[%d]\n", d->sonekindeks);
    }
}



//serbest suffix fonksiyonumuz
void serbestsonekagaci(Dugum *d)
{
    if(d==NULL)
        return;

    int i;
    for(i=0; i<maxchar; i++)
    {
        if(d->children[i] != NULL)
        {
            serbestsonekagaci(d->children[i]);
        }
    }
    if(d->sonekindeks== -1)
        free(d->bitis);
    free(d);
}


//suffix agacimizi kurma fonksiyonumuz
void sonekagacikurma()
{
    size=strlen(text);
    int i;
    kokBitis=(int*)malloc(sizeof(int));
    *kokBitis=-1;

    kok=yeniDugum(-1, kokBitis);

    aktifDugum=kok;
    for(i=0; i<size; i++)
        sonekagaciuzatma(i);
    int uzunluk=0;
    sonekagaciindexi(kok, uzunluk);



}

//1.PROBLEM
int suffixKontrol(char giris[50],int sizegiris)
{
    char s[10][20];
    char p[10][20];
    int i,j,k;
    for(j=0; j<sizegiris; j++)
    {
        for(i=0; 1; i++)
        {
            if(giris[i+j]=='\0')
            {
                s[j][i]=giris[i+j];
                break;
            }
            s[j][i]=giris[i+j];
        }
    }

    int control1=0;
    int sayac=0;
    int gecici;
    for(j=0; j<sizegiris; j++)
    {
        for(i=0; i<=j; i++)
        {
            p[j][i]=giris[i];
            gecici=i;
        }
        p[j][gecici+1]='\0';
    }

    int control2=0;
    for(i=1; i<sizegiris; i++)
    {
        for(k=0; k<sizegiris; k++)
        {
            if(!(strcmp(s[i],p[k])))
                control2=1;
        }
        if(control2==1)
            break;
    }
    if(control2==1)
        printf("Girilen string ile sonek agaci olusturulamaz\n");
    else
        printf("Girilen string ile sonek agaci olusturulabilir\n");

}


//2.PROBLEM
int kenarDolas(char *katar, int index, int baslangic, int bitis)
{
    int k = 0;
    for(k=baslangic; k<=bitis && katar[index] != '\0'; k++, index++)
    {
        if(text[k] != katar[index])
            return -1;
    }
    if(katar[index] == '\0')
        return 1;
    return 0;
}

int altkatarKonum(Dugum *d)
{
    if(d == NULL)
        return 0;
    if(d->sonekindeks > -1)
    {
        printf("Bu pozisyonda bulundu: %d\n", d->sonekindeks);
        return 1;
    }
    int sayac = 0;
    int i = 0;
    for (i = 0; i < maxchar; i++)
    {
        if(d->children[i] != NULL)
        {
            sayac += altkatarKonum(d->children[i]);
        }
    }
    return sayac;
}

int yaprakSay(Dugum *d)
{
    if(d == NULL)
        return 0;
    return altkatarKonum(d);
}

int gecisYap(Dugum *d, char* katar, int index)
{
    if(d == NULL)
    {
        return -1;
    }
    int sonuc = -1;
    if(d->baslangic != -1)
    {
        sonuc = kenarDolas(katar, index, d->baslangic, *(d->bitis));
        if(sonuc == -1)
            return -1;
        if(sonuc == 1)
        {
            if(d->sonekindeks> -1)
                printf("Katar pozisyonu: %d\n",d->sonekindeks);
            else
                printf("Katar sayaci: %d\n", yaprakSay(d));
            return 1;
        }
    }

    index = index + kenarUzunluk(d);
    if(d->children[(int)katar[index]-(int)' '] != NULL)
        return gecisYap(d->children[(int)katar[index]-(int)' '], katar, index);
    else
        return -1;
}






void altkatarKontrol(char* katar)
{
    int sonuc = gecisYap(kok, katar, 0);
    if(sonuc == 1)
        printf("Girilen string <%s> alt katardir\n", katar);
    else
        printf("Girilen string <%s> alt katar degildir\n", katar);
}




//3.PROBLEM
void aramayap(Dugum *d, int uzunluk2, int *maxuzunluk, int* altkatarbaslangic)
{
    if(d==NULL)
    {
        return;
    }
    int i=0;
    if(d->sonekindeks==-1)
    {
        for(i=0; i<maxchar; i++)
        {
            if(d->children[i]!=NULL)
            {
                aramayap(d->children[i], uzunluk2 + kenarUzunluk(d->children[i]), maxuzunluk, altkatarbaslangic);
            }
        }
    }

    else if(d->sonekindeks>-1 && (*maxuzunluk<uzunluk2-kenarUzunluk(d)))
    {
        *maxuzunluk=uzunluk2-kenarUzunluk(d);
        *altkatarbaslangic=d->sonekindeks;
    }
}

void enuzunaltkatar()
{
    int maxuzunluk=0;
    int altkatarbaslangic=0;

    aramayap(kok, 0, &maxuzunluk, &altkatarbaslangic);

    printf("%s deki tekrar eden en uzun altkatar: \n", text);
    int k;
    for(k=0; k<maxuzunluk; k++)
        printf("%c", text[k + altkatarbaslangic]);
    if(k==0)
        printf("Tekrar eden altkatar yoktur.\n");
    printf("\n");

}

//4.PROBLEM
void encokaltkatar()
{
    int encokyaprak=0;
    int encokaltkatar=0;
    int yapraksayi=0;
    int tekrar=0;
    int i;

    for(i=0; i<maxchar; i++)
    {
        if(kok->children[i]==NULL || kok->children[i]->sonekindeks!=-1)
            continue;
        Dugum *at=(Dugum*)malloc(sizeof(Dugum));

        yapraksayi=0;
        tekrar=0;
        at=kok->children[i];

        int k;

        for(k=0; k<maxchar; k++)
        {
            if(at->children[k]==NULL)
                continue;

            if(at->children[k]->sonekindeks>-1)
                yapraksayi++;

            else if(at->children[k]->sonekindeks==-1)
            {
                at=at->children[k];
                k=0;
                continue;
            }
        }

        if(yapraksayi>encokyaprak)
        {
            encokaltkatar=i;
            encokyaprak=yapraksayi;
        }

    }

    printf("En cok tekrar eden altkatar: ");
    for(int x=kok->children[encokaltkatar]->baslangic; x<=*(kok->children[encokaltkatar]->bitis); x++)
    {
        printf("%c", text[x]);
    }

    printf(" dir ve %d kere tekrar etmektedir.\n", encokyaprak);
}


int main()
{
    strcpy(text, "xaaabacc");
    size=strlen(text);
    sonekagacikurma();

    int secim;

    while(1)
    {
        printf("Gerceklestirmek istediginiz islemi seciniz.\n");
        printf ("1- s katari icin sonek agaci olusturulabilir mi?\n");
        printf ("2- Sonek agaci olusturulan bir s katari icinde p katari geciyor mu, geciyorsa ilk bulundugu yerin baslangic pozisyonu nedir, kac kez tekrar etmektedir?\n");
        printf ("3- Sonek agaci olusturulan bir s katari icinde tekrar eden en uzun altkatar nedir, kac kez tekrar etmektedir?\n");
        printf ("4- Sonek agaci olusturulan bir s katari icinde en cok tekrar eden altkatar nedir, kac kez tekrar etmektedir?\n");
        printf ("5- Cikis\n");

        printf("Seciminiz: \n");
        scanf("%d", &secim);

        switch(secim)
        {
        case 1:
            system("cls");
            suffixKontrol(text,size);
            break;

        case 2:
            system("cls");
            altkatarKontrol("xa");
            break;

        case 3:
            system("cls");
            enuzunaltkatar();
            break;

        case 4:
            system("cls");
            encokaltkatar();
            break;

        case 5:
            system("cls");
            printf("Cikis yapiliyor...\n");
            return 0;


        default:
            printf("Yanlis secim yaptiniz.\n");
            break;


        }


    }

}
