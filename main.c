// Zofia Tunova DSA zadanie 2: vlastna implementacia AVL stromu s funkciami search a insert

#include <stdio.h>
#include <stdlib.h>

//struktura ktora tvori jednotlive uzly stromu obsahuje vkladanu/hladanu hodnotu, vysku uzlu a smerniky na praveho a laveho potomka a rodica
typedef struct Vrchol
{
    int hodnota;
    int vyska;
    struct Vrchol* rodic;
    struct Vrchol* pravy;
    struct Vrchol* lavy;
} VRCHOL;

//funkcia ktora zisti vysku uzlu tym, ze porovna vysky v lavom a pravom potomkovi a vrati tu vacsiu z nich
// (vyska uzlu ktoreho vysku chceme zistovat je potom vacsia z vysok laveho a praveho podstromu +1)
int zisti_vysku(VRCHOL* meneny){
    int prava_vyska= 0;
    int lava_vyska= 0;
    int vysledna;

    if(meneny->pravy != NULL){
        prava_vyska = meneny->pravy->vyska;
    }
    if(meneny->lavy != NULL){
        lava_vyska = meneny->lavy->vyska;
    }

    if(prava_vyska >= lava_vyska){
        vysledna= prava_vyska+1;
    }
    else{
        vysledna= lava_vyska+1;
    }
    return vysledna;
}

VRCHOL* rotacia_vlavo(VRCHOL* problematicky, VRCHOL* koren)
{
    VRCHOL* novy_vrch = problematicky->pravy;
    VRCHOL* zvysok= novy_vrch->lavy;
    int nova_vyska;
    if(problematicky == koren){
     //   printf("zmena korena\n");
        koren= novy_vrch;
     //   printf("hodnota v novom vrchu %d\n",koren->hodnota);
    }
    else{
//najskor sa skontroluje na ktorej strane od rodicovskeho vrcholu je nevyvazeny podstrom - tam sa pripoji novy vrch
//(novy vrch je pravy potom uzlu s pokazenym balance factorom - po rotacii sa dostane "o uroven vyssie")
        if(problematicky->rodic->lavy == problematicky){
            problematicky->rodic->lavy = novy_vrch;
        }
        else{
            problematicky->rodic->pravy = novy_vrch;
        }
    }
//presmerovanie ostatnych smernikov: rodic noveho vrchu bude rodic vrcholu s pokazenym balance factorom
//rodic pokazeneho vrcholu bude novy vrch
//pokazeny vrchol sa dostane nalavo od noveho vrcholu
//prava cast noveho vrcholu sa nemeni
//to co bolo povodne nalavo od noveho vrcholu sa dostane na pravo od pokazeneho vrcholu
//v pripade ze nalavo od noveho vrchu povodne vobec nieco bolo rodicom tohto zvysku sa stane pokazeny vrchol
//(ten sa akoby vsunul medzi novy vrch a to co bolo pod nim)
    novy_vrch->rodic = problematicky->rodic;
    problematicky->rodic = novy_vrch;
    novy_vrch->lavy = problematicky;
    problematicky->pravy = zvysok;
    if(zvysok != NULL){
        zvysok->rodic = problematicky;
    }

//este sa aktualizuju vysky vrcholov ktore sa nejako mohli zmenit - to su vysky noveho vrcholu a pokazeneho vrcholu
    nova_vyska= zisti_vysku(problematicky);
    problematicky->vyska = nova_vyska;
    nova_vyska= zisti_vysku(novy_vrch);
    novy_vrch->vyska= nova_vyska;

    return novy_vrch;
}

//rotacia vpravo spociva v rovnakom principe ako rotacia vlavo len sa uzly "prevesivaju" na ine strany
VRCHOL* rotacia_vpravo(VRCHOL* problematicky, VRCHOL* koren)
{
    VRCHOL* novy_vrch= problematicky->lavy;
    VRCHOL* zvysok= novy_vrch->pravy;
    int nova_vyska;
    if(problematicky == koren){
     //   printf("zmena korena\n");
        koren= novy_vrch;
    }
    else{
        if(problematicky->rodic->lavy == problematicky){
            problematicky->rodic->lavy = novy_vrch;
        }
        else{
            problematicky->rodic->pravy = novy_vrch;
        }
    }
    novy_vrch->rodic = problematicky->rodic;
    problematicky->rodic = novy_vrch;
    novy_vrch->pravy = problematicky;
    problematicky->lavy = zvysok;
    if(zvysok != NULL){
        zvysok->rodic = problematicky;
    }

    nova_vyska= zisti_vysku(problematicky);
    problematicky->vyska = nova_vyska;
    nova_vyska= zisti_vysku(novy_vrch);
    novy_vrch->vyska= nova_vyska;

    return novy_vrch;
}

VRCHOL* insertAVL(int vkladane, VRCHOL* koren)
{
//ak este nebol vytvoreny koren stromu vkladana hodnota sa ulozi do korena => vytvori sa koren
    if (koren == NULL){
     //   printf("vkladam koren s hodnotou %d\n",vkladane);
        koren= (VRCHOL*)malloc(sizeof(VRCHOL));
        koren->hodnota= vkladane;
        koren->vyska= 1;
        koren->rodic= NULL;
        koren->pravy= NULL;
        koren->lavy= NULL;
    }
    else{
        VRCHOL* akt= koren;
        int miesto= 0;
        int prava_vyska, lava_vyska, nova_vyska;
        int BF=0;

// ak uz nejaky koren je vytvoreny hlada sa miesto kam vlozit novu hodnotu
// ak je vkladana hodnota vacsia ako hodnota v aktualnom vrchole smernik akt (ktory hlada miesto) sa posuva do prava
// ak je vkladana hodnota mensia ako hodnota v aktualnom vrchole posuva sa do lava
//takto sa posuva cez vrcholy az kym nenajde vrchol za ktory moze pripojit strukturu s vkladanou hodnotou - na nom zastane
        while (miesto == 0){
            if(akt->hodnota < vkladane){
                if(akt->pravy != NULL){
                    akt= akt->pravy;
                }
                else{
                    miesto= 1;
                }
            }
            if(akt->hodnota > vkladane){
                if(akt->lavy != NULL){
                    akt= akt->lavy;
                }
                else{
                    miesto= 1;
                }
            }
//v pripade ze sa vklada hodnota ktora v strome uz je funkcia vrati smernik na koren a skonci
            if(akt->hodnota == vkladane){
          //      printf("vkladana hodnota %d uz v strome je\n", vkladane);
                return koren;
            }
        }
//skontroluje este ci je vkladana hodnota mensia alebo vacsia ako hodnota v aktualnom vrchole a podla toho vytvori novu strukturu
// bud nalavo alebo napravo od aktualneho vrcholu
        if(akt->hodnota < vkladane){
        //    printf("%d vkladam napravo od %d\n", vkladane, akt->hodnota);
            akt->pravy= (VRCHOL*)malloc(sizeof(VRCHOL));
            akt->pravy->hodnota = vkladane;
            akt->pravy->vyska = 1;
            akt->pravy->rodic = akt;
            akt->pravy->pravy = NULL;
            akt->pravy->lavy = NULL;
        }
        else{
         //   printf("%d vkladam nalavo od %d\n", vkladane, akt->hodnota);
            akt->lavy= (VRCHOL*)malloc(sizeof(VRCHOL));
            akt->lavy->hodnota = vkladane;
            akt->lavy->vyska = 1;
            akt->lavy->rodic = akt;
            akt->lavy->pravy = NULL;
            akt->lavy->lavy = NULL;
        }

// nasledne po tom, co pripoji novu strukturu postupuje akt smernik smerom nahor po smernokoch na rodica ulozeneho v strukture
        while(akt != NULL){
            prava_vyska= 0;
            lava_vyska= 0;
            if(akt->pravy != NULL){
                prava_vyska= akt->pravy->vyska;
            }
            if(akt->lavy != NULL){
                lava_vyska= akt->lavy->vyska;
            }

//cestou nahor aktualizuje vysky jednotlivych uzlov pomocou funkcie zisti_vysku ktora porovna vysku laveho a praveho potomka =>
// vyska aktualneho uzlu je potom ta vacsia +1
            nova_vyska= zisti_vysku(akt);
            akt->vyska= nova_vyska;

//pre kazdy uzol sa vypocitava balance factor (BF) ako vyska laveho podstromu - vyska praveho podstromu
//ak je balance factor < -1 "tazsia" je prava strana, ak je >1 "tazsia" je lava strana podla coho sa vykonaju prislusne rotacie
            BF= lava_vyska - prava_vyska;
         //   printf("BF vrcholu %d je %d\n", akt->hodnota, BF);
            if (BF > 1 && vkladane < akt->lavy->hodnota){
         //       printf("problematicky vrchol %d rotacia vpravo\n", akt->hodnota);
                akt= rotacia_vpravo(akt, koren);
                if(akt->rodic == NULL){
         //           printf("novy koren\n");
                    koren= akt;
                }
            }
            if (BF > 1 && vkladane > akt->lavy->hodnota){
           //     printf("problematicky vrchol %d rotacia vlavo+vpravo\n", akt->hodnota);
                akt->lavy = rotacia_vlavo(akt->lavy, koren);
                akt= rotacia_vpravo(akt, koren);
                if(akt->rodic == NULL){
            //        printf("novy koren\n");
                    koren= akt;
                }
            }
            if(BF < -1 && vkladane > akt->pravy->hodnota){
            //    printf("problematicky vrchol %d rotacia vlavo\n", akt->hodnota);
                akt= rotacia_vlavo(akt, koren);
             //   printf("po rotacii som na %d\n",akt->hodnota);
                if(akt->rodic == NULL){
             //       printf("novy koren\n");
                    koren= akt;
                }
            }
            if (BF < -1 && vkladane < akt->pravy->hodnota){
           //     printf("problematicky vrchol %d rotacia vpravo+vlavo\n", akt->hodnota);
                akt->pravy= rotacia_vpravo(akt->pravy, koren);
                akt= rotacia_vlavo(akt, koren);
                if(akt->rodic == NULL){
            //        printf("novy rodic\n");
                    koren= akt;
                }
            }
//posun smerom nahor
            akt= akt->rodic;
        }
    }
//funkcia vracia koren stromu
    return koren;
}

//funkcia search prechadza stromom, podla toho ci je hladana hodnota mensia alebo vacsia ako hodnota v aktualnom vrchole
//sa posuva bud dolava alebo doprava az kym hladany prvok nenajde alebo nedojde na koniec stromu
//funkcia vracia smernik na vrchol s hladanou hodntou, v pripade ze sa v strome hladany prvok nenachadza je tento smernik NULL
VRCHOL* searchAVL (int hladane, VRCHOL* koren)
{
    VRCHOL* akt= koren;
    int najdene= 0;
    while(akt != NULL && najdene == 0){
        if (akt != NULL && akt->hodnota > hladane){
        //    printf("idem nalavo od %d s vyskou %d\n", akt->hodnota, akt->vyska);
            akt= akt->lavy;
        }
        if (akt != NULL && akt->hodnota < hladane){
        //    printf("idem napravo od %d s vyskou %d\n", akt->hodnota, akt->vyska);
            akt= akt->pravy;
        }
        if (akt != NULL && akt->hodnota == hladane) {
        //    printf("najdene, vyska %d\n", akt->vyska);
            najdene = 1;
        }
    }

    if(najdene==1){
        printf("hladany prvok %d najdeny\n", hladane);
    } else{
        printf("hladany prvok %d tu nie je\n", hladane);
      //  printf("%p\n",akt);
    }

    return akt;
}

int main() {
    VRCHOL* koren= NULL;
    koren=insertAVL(10, koren);

    koren=insertAVL(5, koren);
    koren=insertAVL(3, koren);
    koren=insertAVL(20,koren);
    koren= insertAVL(30,koren);
    koren=insertAVL(7,koren);
    koren=insertAVL(15,koren);

    searchAVL(10,koren);
    searchAVL(5, koren);
    searchAVL(3, koren);
    searchAVL(20,koren);
    searchAVL(30,koren);
    searchAVL(7,koren);
    searchAVL(15,koren);

    searchAVL(1,koren);

    return 0;
}