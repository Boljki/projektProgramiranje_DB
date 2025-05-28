#ifndef INTERVENCIJE_H
#define INTERVENCIJE_H

typedef struct intervencije {
    int id;
    char vrsta[50];
    char datum[50];   // DD.MM.GGGG
    char vrijeme[50]; // HH:MM
    int trajanje;     // u minutama
    char lokacija[50];
    int brojVatrogasaca;
    int brojVozila;
} INTERVENCIJE;

int izbornik();
void kreirajdatoteku();
void dodaj_intervenciju();
void azuriraj_intervenciju();
void ispis_intervencije();
void brisanje_intervencije();
void pretraga_po_ID(INTERVENCIJE* polje, int broj);
void pretraga_po_vrsti(INTERVENCIJE* polje, int broj);
void pretraga_po_lokaciji(INTERVENCIJE* polje, int broj);
void sortiranje_po_trajanju(INTERVENCIJE* polje, int broj); // od najkrace do najduze
void zamjena(INTERVENCIJE* a, INTERVENCIJE* b);
INTERVENCIJE* ucitaj_intervencije(int* broj);
#endif

