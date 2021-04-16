/* Audrius Savickas 4 gr 2 pogr 3 uzduotis 13 variantas
Kasko draudimas/autoservisas (ADT: prioritetinė eilė, ilgas sveikas skaičius). 

Procesas 1.
Klientas draudžiasi mašiną kasko draudimu įmonėje, kuri turi bendradarbiavimo sutartį su autoservisu.
Tokiu atveju frančizė (minimalus kliento sumokamas mokestis įvykus avarijai, kurį jis turi sumokėti
pats) yra A litų dydžio, iš kurių A1 atitenka servisui dėl bendradarbiavimo sutarties kaip priedas, ir
klientas laikomas VIP klientu. Tokie klientai servise turi pirmumo teisę, ir jų mašinos yra tvarkomos
pirmiausiai (jeigu reikia dirbant netgi viršvalandžius - VIP kliento mašina turi būti pradedama tvarkyti
nuo pristatymo akimirkos nenutrūkstamai iki pat jos sutvarkymo, t.y. per vieną dieną). Už
viršvalandžius darbuotojams reikia mokėti dvigubai. 

Procesas 2. 
Klientas draudžiasi paprastu kasko draudimu, kurio frančizė yra B litų (B=A-A1) ir servisas 
už tai papildomų pinigų negauna, bet nereikia
dirbti ir viršvalandžių, kurie kainuoja brangiau. 

Pamodeliuoti, kokia verslo logika servisui
naudingiausia: turėti kuo daugiau bendradarbiavimo sutarčių, ar dirbti visai be jų. 

Modelio parametrai:
minėti dydžiai A, A1 ir B. Taip pat: U – serviso darbuotojo valandinė alga, K - darbuotojų skaičius.
Kiti naudotini dydžiai: T1 - tikimybė procentais, kad mašiną pristatys VIP klientas, T2 - tikimybė
procentais, kad mašiną pristatys paprastas klientas, TR - atsitiktinis dydis valandomis, kurio reikia
vienam darbuotojui sutvarkyti atvežtą mašiną (tarkime, jog 1<= TR <=5*K
*/

// TODO: Atlyginimai 1 procese daug mazesni

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "priority_queue.c"

#define A 1000    // VIP sumoka
#define A1 200    // Servisas papildomai gauna
#define B A-A1    // Paprastas sumoka
#define U 5       // Serviso darbuotojo valandine alga
#define K 4       // Serviso darbuotoju skaicius
#define T1 20     // Tikimybe kad masina atves VIP klientas
#define T2 70     // Tikimybe kad masina atves paprastas klientas
int TR;           // Atvesta masina taisys valandu (1 <= TR <= 5*K)
int vipN = 0, papN = 0;
long int servisoPajamos = 0;
int uzimtiDarbuotojai = 0;
int laukiaPaprasti = 0;
int laukiaVIP = 0;
int taisomiAutomobiliai = 0;
long int pajamos = 0;
long int algos = 0;
int sutaisytiPaprasti = 0;
int sutaisytiVIP = 0;

int sutaisytiPaprasti1 = 0, sutaisytiPaprasti2 = 0;
long int pajamos1 = 0, pajamos2 = 0;
long int algos1 = 0, algos2 = 0;

#define N 100     // Servisas dirbs 60 laiko momentu

FILE *output;     // Isvedimo failas

void outputStartData ();
void procesas1();
void procesas2();
bool checkIfFixed(PriorityQueue **eile, int currTime, bool dirbantysDarbuotojai[], int *uzimtiDarbuotojai);
void outputWaitingQueue(PriorityQueue *eile, int laukia);
int getFreeEmployeeNumber(bool dirbantysDarbuotojai[]);
void printFixingQueue(PriorityQueue *eile);
void countEmployeeWage(PriorityQueue *eile, bool overtime);
void outputResults();
void removeNonVIP(PriorityQueue **taisymoEile, PriorityQueue **laukimoEile, int *uzimtiDarbuotojai, bool dirbantysDarbuotojai[]);
bool checkIfOvertime(PriorityQueue **taisymoEile, PriorityQueue **laukimoEile);

int main () {
    output = fopen("protokolas.txt", "w");

    outputStartData();
    
    procesas1();
    procesas2();
    
    outputResults();
    fclose(output);
    return 0;
}

void outputResults() {
    fprintf(output, "\nIII DALIS - REZULTATAI.\n\n");
    fprintf(output, "Procesas 1.\n");
    fprintf(output, "\tSutaisytos paprastų klientų mašinos: %d mašinos.\n", sutaisytiPaprasti1);
    fprintf(output, "\tSutaisytos VIP klientų mašinos: %d mašinos.\n", sutaisytiVIP);
    fprintf(output, "\tPajamos: %6d€.\n", pajamos1);
    fprintf(output, "\tDarbuotojams sumokėta alga: %6d€.\n", algos1);
    fprintf(output, "\tPelnas: %6d€.\n", pajamos1-algos1);
    fprintf(output, "Procesas 2.\n");
    fprintf(output, "\tSutaisytos paprastų klientų mašinos: %d mašinos.\n", sutaisytiPaprasti2);
    fprintf(output, "\tSutaisytos VIP klientų mašinos: %d mašinos.\n", 0);
    fprintf(output, "\tPajamos: %6d€.\n", pajamos2);
    fprintf(output, "\tDarbuotojams sumokėta alga: %6d€.\n", algos2);
    fprintf(output, "\tPelnas: %6d€.\n", pajamos2-algos2);
}

void outputStartData() {
    fprintf(output, "Audrius Savickas, PS1, 4 grupė, 2 pogrupis.\n");
    fprintf(output, "Trečioji užduotis, 13 variantas.\n\n");
    fprintf(output, "Kasko draudimas/autoservisas (ADT: prioritetinė eilė, ilgas sveikas skaičius). Procesas 1.\nKlientas draudžiasi mašiną kasko draudimu įmonėje, kuri turi bendradarbiavimo sutartį su autoservisu.\nTokiu atveju frančizė (minimalus kliento sumokamas mokestis įvykus avarijai, kurį jis turi sumokėti\npats) yra A litų dydžio, iš kurių A1 atitenka servisui dėl bendradarbiavimo sutarties kaip priedas, ir\n");
    fprintf(output, "klientas laikomas VIP klientu. Tokie klientai servise turi pirmumo teisę, ir jų mašinos yra tvarkomos\npirmiausiai (jeigu reikia dirbant netgi viršvalandžius - VIP kliento mašina turi būti pradedama tvarkyti\nnuo pristatymo akimirkos nenutrūkstamai iki pat jos sutvarkymo, t.y. per vieną dieną). Už\nviršvalandžius darbuotojams reikia mokėti dvigubai. Procesas 2. Klientas draudžiasi paprastu kasko\n");
    fprintf(output, "draudimu, kurio frančizė yra B litų (B=A-A1) ir servisas už tai papildomų pinigų negauna, bet nereikia dirbti ir viršvalandžių, kurie kainuoja brangiau.\nPamodeliuoti, kokia verslo logika servisui naudingiausia: turėti kuo daugiau bendradarbiavimo sutarčių, ar dirbti visai be jų.\n");
    fprintf(output, "Modelio parametrai: minėti dydžiai A, A1 ir B. Taip pat: U – serviso darbuotojo valandinė alga, K - darbuotojų skaičius.\nKiti naudotini dydžiai: T1 - tikimybė procentais kad mašiną pristatys VIP klientas, T2 - tikimybė procentais,\nkad mašiną pristatys paprastas klientas, TR - atsitiktinis dydis valandomis, kurio reikia vienam darbuotojui sutvarkyti atvežtą mašiną\n(tarkime, jog 1<= TR <=5*K).\n\n");
    fprintf(output, "\nI DALIS - PRADINIAI DUOMENYS\n\n");
    fprintf(output, "VIP kliento frančizė A = %d€.\n", A);
    fprintf(output, "Servisas papildomai gauna A1 = %d€.\n", A1);
    fprintf(output, "Paprasto kliento frančizė B = %d€.\n", B);
    fprintf(output, "Serviso darbuotojo valandinė alga U = %d€.\n", U);
    fprintf(output, "Serviso darbuotojų skaičius K = %d darbuotojai.\n", K);
    fprintf(output, "Tikimybė, kad mašiną atveš VIP klientas T1 = %d\%%.\n", T1);
    fprintf(output, "Tikimybė, kad mašiną atveš paprastas klientas T2 = %d%%.\n", T2);
    fprintf(output, "Serviso darbo trukmė N = %d minučių.\n\n\n", N);
    fprintf(output, "II DALIS - VYKDYMAS\n\n");
}

void procesas1() {
    PriorityQueue *taisymoEile, *laukimoEile;
    bool dirbantysDarbuotojai[K] = {false};
    Data value;
    bool overtime = false;

    createEmptyPQueue(&taisymoEile, N*5);
    createEmptyPQueue(&laukimoEile, N*5);

    fprintf(output, "PROCESAS 1: VIP KLIENTAI IR PAPRASTI KLIENTAI\n\n");
    int darboLaikas = N;
    for (int i = 0; i < darboLaikas; i++) {
        if (!overtime) fprintf(output, "T = %d min.\n", i);
        else if (overtime && checkIfOvertime(&taisymoEile, &laukimoEile)) fprintf(output, "T = %d min. Viršvalandžiai.\n", i);
        int skaicius = rand() % 100 + 1;

        // JEIGU DIRBA VIRSVALANDZIUS
        if (overtime) {
            
            bool check = checkIfOvertime(&taisymoEile, &laukimoEile);
            //if (i == 102) check = false;

            if (check) {
                //printFixingQueue(taisymoEile);
                darboLaikas++;
            }
            else {
                overtime = false;
                break;
            }
        }

        // ATEJO VIP KLIENTAS
        if (skaicius <= T1 && !overtime) {
            vipN++;
            fprintf(output, "\tAtvyko VIP klientas V%d.\n", vipN);
            value.klientoNr = vipN;
            value.VIP = true;

            if (uzimtiDarbuotojai >= K) { // Nera laisvu darbuotoju
                laukiaVIP++;
                value.taisyti = rand() % (5*K) + 1;

                insertPQueue(&laukimoEile, value, 2);
                //printFixingQueue(laukimoEile);
            }
            else { // Yra laisvu darbuotoju
                int darbuotojas = getFreeEmployeeNumber(dirbantysDarbuotojai);
                uzimtiDarbuotojai++;
                dirbantysDarbuotojai[darbuotojas-1] = true;
                value.darbuotojoNr = darbuotojas;
                value.taisyti = rand() % (5*K) + 1;
                value.pradejoTaisyti = i;
                fprintf(output, "\tDarbuotojas D%d pradeda taisyti VIP kliento V%d mašiną.\n", darbuotojas, vipN);
                insertPQueue(&taisymoEile, value, 2);
            }

        }
        // ATEJO PAPRASTAS KLIENTAS
        else if (skaicius <= T2 && !overtime) {
            papN++;
            fprintf(output, "\tAtvyko paprastas klientas K%d.\n", papN);
            value.klientoNr = papN;
            value.VIP = false;
            if (uzimtiDarbuotojai >= K) { // Nera laisvu darbuotoju
                laukiaPaprasti++;
                value.taisyti = rand() % (5*K) + 1;

                insertPQueue(&laukimoEile, value, 1);
            }
            else { // Yra laisvu darbuotoju
                int darbuotojas = getFreeEmployeeNumber(dirbantysDarbuotojai);
                uzimtiDarbuotojai++;
                dirbantysDarbuotojai[darbuotojas-1] = true;
                value.darbuotojoNr = darbuotojas;
                value.taisyti = rand() % (5*K) + 1;
                value.pradejoTaisyti = i;
                fprintf(output, "\tDarbuotojas D%d pradeda taisyti paprasto kliento K%d mašiną.\n", darbuotojas, papN);
                insertPQueue(&taisymoEile, value, 1);
            }
        }
        if (checkIfFixed(&taisymoEile, i, dirbantysDarbuotojai, &uzimtiDarbuotojai)) {
            while (uzimtiDarbuotojai < K && (laukiaPaprasti+laukiaVIP) > 0) {
                int darbuotojas = getFreeEmployeeNumber(dirbantysDarbuotojai);
                uzimtiDarbuotojai++;
                dirbantysDarbuotojai[darbuotojas-1] = true;

                value = getPQueue(laukimoEile);
                value.darbuotojoNr = darbuotojas;
                value.pradejoTaisyti = i;
                
                if (value.VIP) {
                    laukiaVIP--;
                    fprintf(output, "\tDarbuotojas D%d pradeda taisyti VIP kliento V%d mašiną.\n", darbuotojas, value.klientoNr);
                    insertPQueue(&taisymoEile, value, 1);
                }
                else {
                    laukiaPaprasti--;
                    fprintf(output, "\tDarbuotojas D%d pradeda taisyti paprasto kliento K%d mašiną.\n", darbuotojas, value.klientoNr);
                    insertPQueue(&taisymoEile, value, 1);
                }
                removePQueue(&laukimoEile);
                //printFixingQueue(taisymoEile);
                //fprintf(output, "Darbuotojas D%d pradeda taisyti kliento K%d mašiną.\n", darbuotojas, value.klientoNr);
            }

        }
        outputWaitingQueue(laukimoEile, laukiaPaprasti+laukiaVIP);
        // printFixingQueue(taisymoEile);
        // fprintf(output, "UZimti darbuotojai: %d", uzimtiDarbuotojai);
        countEmployeeWage(taisymoEile, overtime);
        fprintf(output, "\n");

        // Patikrinti ar baigesi darbo diena ir yra nebaigtu VIP
        if (i == N - 1 && !overtime) {
            if (checkIfOvertime(&taisymoEile, &laukimoEile)) {
                overtime = true;
                darboLaikas++;
                laukiaPaprasti = 0;
                removeNonVIP(&taisymoEile, &laukimoEile, &uzimtiDarbuotojai, dirbantysDarbuotojai);
            }
        }
    }

    pajamos1 = pajamos;
    algos1 = algos;
    sutaisytiPaprasti1 = sutaisytiPaprasti;
    deletePQueue(&laukimoEile);
    deletePQueue(&taisymoEile);
}

void removeNonVIP(PriorityQueue **taisymoEile, PriorityQueue **laukimoEile, int *uzimtiDarbuotojai, bool dirbantysDarbuotojai[]){
    PriorityQueue *p_current = (*taisymoEile)->p_next;
    PriorityQueue *p_previous = *taisymoEile;
    Data value;
    while (p_current) {
        value = p_current->value;
        if (!value.VIP) {
            (*uzimtiDarbuotojai)--;
            dirbantysDarbuotojai[value.darbuotojoNr-1] = false;
            if (p_current == (*taisymoEile)->p_next) {
                (*taisymoEile)->p_next = p_current->p_next;
            }
            else {
                p_previous->p_next = p_current->p_next;
                p_current = p_current->p_next;
                continue;
            }
        }
        p_previous = p_current;
        p_current = p_current->p_next;
    }
    p_current = (*laukimoEile)->p_next;
    p_previous = *laukimoEile;
    while (p_current) {
        value = p_current->value;
        if (!value.VIP) {
            if (p_current == (*taisymoEile)->p_next) {
                (*taisymoEile)->p_next = p_current->p_next;
            }
            else {
                p_previous->p_next = p_current->p_next;
                p_current = p_current->p_next;
                continue;
            }
        }
        p_previous = p_current;
        p_current = p_current->p_next;
    }
}

void procesas2() {
    PriorityQueue *taisymoEile, *laukimoEile;
    bool dirbantysDarbuotojai[K] = {false};
    Data value;
    pajamos = 0;
    sutaisytiPaprasti = 0;
    laukiaPaprasti = 0;
    uzimtiDarbuotojai = 0;

    createEmptyPQueue(&taisymoEile, N*5);
    createEmptyPQueue(&laukimoEile, N*5);

    //srand(time(NULL));
    int darboLaikas = N;
    fprintf(output, "\n\nPROCESAS 2: TIK PAPRASTI KLIENTAI\n\n");
    for (int i = 0; i < darboLaikas; i++) {
        //printf("\n%d\n", i);
        fprintf(output, "T = %d min.\n", i);
        int skaicius = rand() % 100 + 1;
        if (skaicius <= T2) {
            papN++;
            fprintf(output, "\tAtvyko paprastas klientas K%d.\n", papN);

            if (uzimtiDarbuotojai >= K) { // Nera laisvu darbuotoju
                laukiaPaprasti++;
                value.klientoNr = papN;
                value.taisyti = rand() % (5*K) + 1;
                value.VIP = false;

                insertPQueue(&laukimoEile, value, 1);
            }
            else { // Yra laisvu darbuotoju
                int darbuotojas = getFreeEmployeeNumber(dirbantysDarbuotojai);
                uzimtiDarbuotojai++;
                dirbantysDarbuotojai[darbuotojas-1] = true;
                value.klientoNr = papN;
                value.darbuotojoNr = darbuotojas;
                value.taisyti = rand() % (5*K) + 1;
                value.pradejoTaisyti = i;
                value.VIP = false;
                //printf("%d %d %d %d %d\n", value.klientoNr, value.darbuotojoNr, value.taisyti, value.pradejoTaisyti, value.VIP);
                fprintf(output, "\tDarbuotojas D%d pradeda taisyti paprasto kliento K%d mašiną.\n", darbuotojas, papN);
                insertPQueue(&taisymoEile, value, 1);
            }
        }
        
        if (checkIfFixed(&taisymoEile, i, dirbantysDarbuotojai, &uzimtiDarbuotojai)) {
            while (uzimtiDarbuotojai < K && laukiaPaprasti > 0) {
                int darbuotojas = getFreeEmployeeNumber(dirbantysDarbuotojai);
                uzimtiDarbuotojai++;
                laukiaPaprasti--;
                dirbantysDarbuotojai[darbuotojas-1] = true;
                value = getPQueue(laukimoEile);
                value.darbuotojoNr = darbuotojas;
                value.pradejoTaisyti = i;
                removePQueue(&laukimoEile);
                fprintf(output, "\tDarbuotojas D%d pradeda taisyti paprasto kliento K%d mašiną.\n", darbuotojas, value.klientoNr);
                insertPQueue(&taisymoEile, value, 1);
                //fprintf(output, "Darbuotojas D%d pradeda taisyti kliento K%d mašiną.\n", darbuotojas, value.klientoNr);
            }
        }
        outputWaitingQueue(laukimoEile, laukiaPaprasti);
        //fprintf(output, "Uzimti darbuotojai: %d, ", uzimtiDarbuotojai);
        // fprintf(output, "Uzimti darbuotojai %d\n", uzimtiDarbuotojai);
        countEmployeeWage(taisymoEile, false);
        fprintf(output, "\n");
    }
    pajamos2 = pajamos;
    algos2 = algos;
    sutaisytiPaprasti2 = sutaisytiPaprasti;
    deletePQueue(&laukimoEile);
    deletePQueue(&taisymoEile);
}

bool checkIfOvertime(PriorityQueue **taisymoEile, PriorityQueue **laukimoEile) {
    PriorityQueue *p_current = (*taisymoEile)->p_next;
    PriorityQueue *p_previous = *taisymoEile;
    Data value;
    while (p_current) {
        value = p_current->value;
        if (value.VIP) {
            return true;
        }
    }
    p_current = (*laukimoEile)->p_next;
    p_previous = *laukimoEile;
    while (p_current) {
        value = p_current->value;
        if (value.VIP) {
            return true;
        }
    }
    return false;
}

bool checkIfFixed(PriorityQueue **eile, int currTime, bool dirbantysDarbuotojai[], int *uzimtiDarbuotojai) {
    PriorityQueue *p_current = (*eile)->p_next;
    PriorityQueue *p_previous = *eile;
    PriorityQueue *p_next = p_current->p_next;
    Data value;
    bool fixed = false;
    while (p_current) {
        value = p_current->value;
        if (currTime - value.pradejoTaisyti >= value.taisyti) {
            fixed = true;
            if (!value.VIP) {
                sutaisytiPaprasti++;
                pajamos += B;
                fprintf(output, "\tPaprasto kliento K%d mašiną sutaisė darbuotojas D%d.\n", value.klientoNr, value.darbuotojoNr);
            }
            else {
                sutaisytiVIP++;
                pajamos += A;
                fprintf(output, "\tVIP kliento V%d mašiną sutaisė darbuotojas D%d.\n", value.klientoNr, value.darbuotojoNr);
            }
            p_next = p_current->p_next;
            //fprintf(output, "%d %d\n", p_previous->value.klientoNr, p_current->value.klientoNr);
            //fprintf(output, "DELETE %d", p_current->value.klientoNr);
            //fprintf(output, "prev next %u current %u\n", p_previous->p_next, p_current);
            if (p_current == (*eile)->p_next) {
                (*eile)->p_next = p_current->p_next;
            }
            else {
                p_previous->p_next = p_current->p_next;
                p_current = p_current->p_next;
                dirbantysDarbuotojai[value.darbuotojoNr-1] = false;
                (*uzimtiDarbuotojai)--;
                continue;
            }
            //fprintf(output, "prev next %u current next%u\n", p_previous->p_next, p_current->p_next);
            
            dirbantysDarbuotojai[value.darbuotojoNr-1] = false;
            (*uzimtiDarbuotojai)--;
            // printFixingQueue(*eile);
            //fprintf(output, "Uzimti: %d\n", (*uzimtiDarbuotojai));
        }
        p_previous = p_current;
        p_current = p_current->p_next;
        //fprintf(output, "%u %u\n\n", p_previous, p_current);
    }
    //fprintf(output, "%d ", p_current->value.klientoNr);
    //fprintf(output,"\n");
    if (fixed) return true;
    return false;
}

void outputWaitingQueue(PriorityQueue *eile, int laukia) {
    PriorityQueue *p_current = eile->p_next;
    Data value;
    if (laukia == 0) {
        fprintf(output, "\tEilėje laukia 0 klientų. ");
    }
    else if (laukia == 1) {
        fprintf(output, "\tEilėje laukia 1 klientas. ");
    }
    else if (laukia < 10){
        fprintf(output, "\tEilėje laukia %d klientai. ", laukia);
    }
    else {
        fprintf(output, "\tEilėje laukia %d klientų. ", laukia);
    }
    while (p_current) {
        value = p_current->value;
        if (value.VIP) {
            fprintf(output, "V%d", value.klientoNr);
        }
        else fprintf(output, "K%d", value.klientoNr);
        if (p_current->p_next) fprintf(output, ", ");
        else fprintf(output, ".");
        p_current = p_current->p_next;
    }
    fprintf(output, "\n");
}

int getFreeEmployeeNumber(bool dirbantysDarbuotojai[]) {
    for (int i = 0; i < K; i++) {
        if (dirbantysDarbuotojai[i] == false) {
            dirbantysDarbuotojai[i] = true;
            return i+1;
        }
    }
}

void countEmployeeWage(PriorityQueue *eile, bool overtime) {
    PriorityQueue *p_current = eile->p_next;
    Data value;
    algos += U * K;
    if (overtime) {
        while (p_current) {
            value = p_current->value;
            //algos += U;
            p_current = p_current->p_next;
        }
    }

}

void printFixingQueue(PriorityQueue *eile) {
    PriorityQueue *p_current = eile->p_next;
    Data value;
    //fprintf(output, "Taisomu masinu klientu nr: ");
    //printf("Taisomu masinu klientu nr: ");
    //fprintf(output, "eile adr: %u  ", eile);
    while (p_current) {
        value = p_current->value;
        //fprintf(output, "K%d adr: %u  ", value.klientoNr, p_current);
        if (value.VIP) fprintf(output, "V%d, ", value.klientoNr);
        else fprintf(output, "K%d, ", value.klientoNr);
        // if (value.VIP) printf("V%d, ", value.klientoNr);
        // else printf("K%d, ", value.klientoNr);
        p_current = p_current->p_next;
    }
    fprintf(output, "\n");
}
