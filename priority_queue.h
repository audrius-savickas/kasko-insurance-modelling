/*
 * ADT - Prioritetinė eilė.
 * Autorius - Armintas Pakenis, armintas.pakenis@mif.stud.vu.lt, 4 grupė, 1 kursas.
 */

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Detalesni funcijų aprašai pateikiami žemiau prie jų aprašų. 
 * 
 * Prioretitinės eilės funkcijos ir jų trumpi aprašymai:
 * 
 * void createEmptyPQueue(PriorityQueue **pp_queue, int capacity) - sukuria tuščią prioritetinę eilę, kurios talpa capacity dydžio.
 * bool isEmptyPQueue(PriorityQueue *p_queue) - patikrina, ar prioritetinė eilė tuščia.
 * bool isFullPQueue(PriorityQueue *p_queue) - patikrina, ar prioritetinė eilė pilna.
 * 
 * void insertPQueue(PriorityQueue **pp_queue, Data value, int priority) - įterpia elementą į prioritetinę eilę.
 * void removePQueue(PriorityQueue **pp_queue) - pašalina elementą su didžiausiu prioritetu iš prioritetinės eilės.
 * 
 * Data getPQueue(PriorityQueue *p_queue) - gauna elemento, su didžiausiu prioritetu, reikšmę iš prioritetinės eilės.
 * 
 * void joinPQueue(PriorityQueue **pp_queue1, PriorityQueue **pp_queue2) - prie vienos prioritetinės eilės prijungia kitą.
 * 
 * void deletePQueue(PriorityQueue **pp_queue) - sunaikina prioritetinę eilę.
 * 
 */

/*
 * int errorno_PQueue - po funkcijos įvykdimo, indikuoja paskutinę klaidą.
 * Šios bibliotekos vartotojo atsakomybė, tikrinti, ar įvyko klaida.
 * 
 * Klaidų kodų reikšmės:
 * 0 - Klaidos neįvyko (pradinė reikšmė).
 * 1 - Prioritetinė eilė tuščia, negalima išimti daugiau elementų iš jos.
 * 2 - Prioritetinė eilė tuščia, negalima pažiūrėti pirmojo elemento.
 * 3 - Negalima apjungti tos pačios eilės į save.
 * 4 - Nebėra laisvos vietos.
 * 5 - Pirmosios eilės talpa per maža norint prijungti antrąją p. eilę prie jos.
 * 6 - Nebėra prienamos laisvos atminties, nors neviršijama talpa.
 */
#define ERROR1 "Klaida 1: prioritetinė eilė tuščia, negalima išimti daugiau elementų iš jos."
#define ERROR2 "Klaida 2: prioritetinė eilė tuščia, negalima pažiūrėti į pirmąjį elementą."
#define ERROR3 "Klaida 3: negalima apjungti tos pačios eilės į save."
#define ERROR4 "Klaida 4: nebėra laisvos vietos."
#define ERROR5 "Klaida 5: pirmosios eilės talpa per maža norint prijungti antrąją."
#define ERROR6 "Klaida 6: nebėra prieinamos laisvos atminties, nors neviršijama talpa."


extern int errorno_PQueue;


/*
 * bool print_errors_PQueue - pagal nutylėjimą, pradinė reikšmę 'false'.
 * 
 * Jei nustatoma reikšmė 'true', automatiškai įvykus kokiai nors klaidai,
 * išspausdinamas klaida į konsolę. Kitu atveju pačio vartotojo atsakomybė
 * susekti klaidą pagal klaidos kodą, kuris pateiktas errorno_PQueue.
 * 
 */
extern bool print_errors_PQueue;

// print_errors_PQueue reikšmę nustatyti galima ir priority_queue.c faile.

/*
 * typedef <tipas> Data - vietoj <tipas> įrašyti, kokį duomenų tipą norite saugoti prioritetinėj eilėj.
 * #define Data "format" - vietoj "format" įrašyti spausdinimo formatą. Pvz, jei pasrinkta
 * saugoti int tipą, "format" - "%d", float - "%f". Ši dalis skirta tik primityviom duom. struktūrom ir
 * parodomajai versijai main.c faile.
 */

typedef struct CustomType{
    int klientoNr;
    int darbuotojoNr;
    int taisyti;
    int pradejoTaisyti;
    bool VIP;
} CustomType;

#define DATA "%d"
typedef CustomType Data;



/*
 * Prioritetinės eilės kintamasis sukuriamas taip:
 * PriorityQueue kintamojo_pavadinimas.
 */
typedef struct PriorityQueue{
    Data value;
    int priority;

    int capacity;
    int member_count;

    long time;
    struct PriorityQueue* p_next;
    
} PriorityQueue;

/*
 * createEmptyPQueue(PriorityQueue **pp_queue, int capacity)
 * 
 * Parametrai:
 *  **pp_queue - nurodomas prioritetinės eilės kintamojo rodyklės adresas.
 *  capacity - nurodoma prioritetinės eilės talpa.
 * 
 * Funkcijos darbas:
 *  Sukuria tuščią prioretitinę eilę su limituota talpa.
 * 
 * Grąžina:
 *  Funkcija nieko negrąžina.
 */
void createEmptyPQueue(PriorityQueue **pp_queue, int capacity);

/*
 * isEmptyPQueue(PriorityQueue *p_queue)
 * 
 * Parametrai:
 *   *p_queue - nurodoma prioritetinės eilės kintamojo rodyklė.
 * 
 * Funkcijos darbas:
 *  Išsiaiškina, ar eilė tuščia.
 * 
 * Grąžina:
 *  true - jei eilė tuščia.
 *  false - jei eilė netuščia.
 * 
 */
bool isEmptyPQueue(PriorityQueue *p_queue);



/*
 * isFullPQueue(PriorityQueue *p_queue)
 * 
 * Parametrai:
 *   *p_queue - nurodoma prioritetinės eilės kintamojo rodyklė.
 * 
 * Funkcijos darbas:
 *  Išsiaiškina, ar eilė pilna.
 * 
 * Grąžina:
 *  true - jei eilė pilna.
 *  false - jei eilė nepilna.
 * 
 */
bool isFullPQueue(PriorityQueue *p_queue);

/*
 * insertPQueue(PriorityQueue **pp_queue, Data value, int priority)
 * 
 * Parametrai:
 *  **pp_queue - nurodomas prioritetinės eilės kintamojo rodyklės adresas.
 *  Data value - nurodomas Data tipo kintamasis, kuris bus įdėtas į prioritetinę eilę kaip reikšmė.
 *  priority - prioritetas (sveikas skaičius), pagal kurį parametras value įdedamas į eilę.
 * 
 * Funkcijos darbas:
 *  Įdeda elementą/reikšmę į prioretitinę eilę pagal prioritetą. Jei keli elementai
 *  turi vienodą prioritetą, elementas, kuris buvo sukurtas anksčiau turės didesnį prioritetą.
 * 
 * Grąžina:
 *  Funkcija nieko negrąžina.
 */
void insertPQueue(PriorityQueue **pp_queue, Data value, int priority);

/*
 * void removePQueue(PriorityQueue **pp_queue)
 * 
 * Parametrai:
 *  **pp_queue - nurodomas prioritetinės eilės kintamojo rodyklės adresas.
 * 
 * Funkcijos darbas:
 *  Išima iš prioritetinės eilės elementą, su didžiausiu prioritetu. Jei keli elementai turi
 *  vienodą didžiausią prioritetinę reikšmę, elementas, kuris buvo sukurtas anksčiausiai,
 *  laikomas turintis didžiausią prioritetą.
 * 
 * Grąžina:
 *  Funkcija nieko negrąžina.
 */
void removePQueue(PriorityQueue **pp_queue);

/*
 * Data getPQueue(PriorityQueue *p_queue)
 * 
 * Parametrai:
 *  *p_queue - nurodoma prioritetinės eilės kintamojo rodyklė.
 * 
 * Funkcijos darbas:
 *  Grąžina iš prioritetinės eilės elementą, su didžiausiu prioritetu. Jei keli elementai turi
 *  vienodą didžiausią prioritetinę reikšmę, elementas, kuris buvo sukurtas anksčiausiai,
 *  laikomas turintis didžiausią prioritetą.
 *  
 *  Funkcija iš eilės elemento nepašalina.
 * 
 * Grąžina:
 *  Data tipo kintamąjį, kuris yra su didžiausiu prioritetu esantis elementas prioritetinėj eilėj.
 */
Data getPQueue(PriorityQueue *p_queue);

/*
 * void deletePQueue(PriorityQueue **pp_queue)
 * 
 * Parametrai:
 *  **pp_queue - nurodomas prioritetinės eilės kintamojo rodyklės adresas.
 * 
 * Funkcijos darbas:
 *  Sunaikina nurodytą prioritetinę eilę.
 * 
 * Grąžina:
 *  Funkcija nieko negrąžina.
 */
void deletePQueue(PriorityQueue **pp_queue);

/*
 * joinPQueue(PriorityQueue **pp_queue1, PriorityQueue **pp_queue2)
 * 
 * Parametrai:
 *  **pp_queue1 - nurodomas prioritetinės eilės kintamojo rodyklės adresas,
 *                prie kurios bus jungiama kita prioritetinę eilė.
 *  **pp_queue2 - nurodomas prioritetinės eilės kintamojo rodyklės adresas,
 *                kuri bus jungiama prie pirmosios.
 * 
 * Funkcijos darbas:
 *  Prijungia pp_queue2 elementus prie pp_queue1.
 *  Prijungus eilę, tvarka gali pasikeisti pagal prioritetus.
 * 
 *  Po darbo, prioritetinė eilė pp_queue2 tampa tuščia.
 * 
 * Grąžina:
 *  Funkcija nieko negrąžina.
 */
void joinPQueue(PriorityQueue **pp_queue1, PriorityQueue **pp_queue2);




// Žemiau nurodytos funkcijos nėra skirtos vartotojui.


// Palygina, kurios eilės elementas (su didžiausiu prioritetu) turi didesnį prioritetą.
static char comparePQ(PriorityQueue *p_queue1, PriorityQueue *p_queue2);

// Nukopijuoja prioritetinės eilės viršūnės elementą. Su pakeitimu:
// p_next = NULL.
static PriorityQueue *copyPQNULL(PriorityQueue *p_queue);

// Nukopijuoja prioritetinės eilės viršūnės elementą be pakeitimų.
static PriorityQueue *copyPQ(PriorityQueue *p_queue);

// *p_queue viršūnęs reikšmė tampa esamos viršūnės vaikos, bei atlaisvinama dalis vietos, nes
// tėvo reikšmė išimama ir panaikinama.
static PriorityQueue *makeChildPQ(PriorityQueue **p_queue);

// Grąžina rodyklę į PriorityQueue, kuri turi value, priority reikšmes nurodytuose parametruose,
// bei time ir p_next = NULL reikšmes.
static PriorityQueue *createElementPQ(PriorityQueue **pp_queue, Data value, int priority);



// Macro, skirtas nustatyi klaidos kodą. Jei vartotojas nori, jog klaidos
// pranešimai būtų spausdinami automatiškai, jis gali nustatyti 'print_errors_PQueue'
// reikšmę į true, kas yra rekomenduojama.

#define IF_PRINT_ERROR(condition, message, error_num, ...)                    \
    if (condition)                                                            \
    {                                                                         \
        if (print_errors_PQueue)                                              \
        {                                                                     \
            printf(message "\n\n", ##__VA_ARGS__);                            \
        }                                                                     \
        errorno_PQueue = error_num;                                           \
    }                                                                         \

#endif
