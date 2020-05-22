#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

//Maximum musteri sayisini ve sandalya sayisini belirledik.
#define MAX_MUSTERI 10 
#define sandalyeSayisi 5

//fonksiyonları belirledik.
void *musteri(void *num);
void *berber(void *);
void bekle();
int bosSandalye=sandalyeSayisi;


//Semaforları tanimladik.

sem_t beklemeOdasi;
sem_t berberKoltugu;
//Berberin musteri gelene kadar uyumasi icin tanimladik.
sem_t berberUyu;
//Musterinin saci kesilirken beklemesi adina tanimladik.
sem_t musteriBekle;

//Butun islemler bittiginde bilmemiz adina bu degiskeni tanimladik.
int bitis = 0;

int main(int argc, char *argv[]) {
pthread_t btid;
pthread_t tid[MAX_MUSTERI];
long Randseed;
int i, musteriSayisi;
int Num[MAX_MUSTERI];


printf("Musteri sayisini giriniz:(En fazla 10 kisi girebilirsiniz!) "); scanf("%d",&musteriSayisi) ;

//Girilen kullanici sayisiyle iligli kontrol yapiyoruz.
if (musteriSayisi > MAX_MUSTERI) {
printf("En fazla 10 kisi girebilirsiniz!\n");
exit(-1);
}

//Bir dizi olusturuyoruz.
for (i=0; i<MAX_MUSTERI; i++) {
Num[i] = i+1;
}

//Semaforlari olusturuyoruz.
sem_init(&beklemeOdasi, 0, sandalyeSayisi);
sem_init(&berberKoltugu, 0, 1);
sem_init(&berberUyu, 0, 0);
sem_init(&musteriBekle, 0, 0);

printf("Berber -> dukkani acti.\n");

//Berberi olusturduk.
pthread_create(&btid, NULL, berber, NULL);

//Musterileri olusturduk.
for (i=0; i<musteriSayisi; i++) {
pthread_create(&tid[i], NULL, musteri, (void *)&Num[i]);
sleep(1);
}

//Butun musterilerle ilgilen.
for (i=0; i<musteriSayisi; i++) {
pthread_join(tid[i],NULL);
sleep(1);
}

//Butun islem bitince berberi cikar.
bitis = 1;
sem_post(&berberUyu); //Beberi uyandiriyoruz.
pthread_join(btid,NULL);
}

void *musteri(void *sayi) {
int num = *(int *)sayi;

//Musteri islemlerini burada hallediyoruz.
if(bosSandalye!=0){
printf("%d. Musteri -> dukkana geldi.\n", num);

//Bekleme odasında yer bul
sem_wait(&beklemeOdasi);
printf("%d. Musteri -> bekleme salonuna geldi.\n", num);
bosSandalye--;
}
else{
printf("%d. Musteri -> dukkandan ayrildi, yer yok.\n", num);


}
if(num!=9){
// Musteri berber koltugunun bosalmasini bekliyor
sem_wait(&berberKoltugu);



// Berber koltugu bosaldi. Musteri yerini verebilir.

sem_post(&beklemeOdasi);
printf("%d. Musteri -> berberi uyandırıyor.\n", num);
sem_post(&berberUyu);


// Berberin sac kesme suresi
sem_wait(&musteriBekle);

//Musteri koltugu bosaltabilir.
sem_post(&berberKoltugu);
printf("%d. Musteri -> dukkandan ayriliyor.\n", num);

}
}

void *berber(void *junk) {
while (!bitis) {

printf("Berber -> uyuyor.\n");
sem_wait(&berberUyu);
bosSandalye++;

if (!bitis) {

printf("Berber -> sac kesiyor.\n");
bekle(2);
printf("Berber -> sac kesimini bitirdi.\n");

//Musteriyle isimiz bitti.
sem_post(&musteriBekle);
}
else {
printf("Berber -> eve gidiyor.\n");
}
 }
  } 
void bekle(int secs) {
int len;
len = (int) ((1 * secs) + 1);
sleep(len);
}
