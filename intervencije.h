#ifndef INTERVENCIJE_H
#define INTERVENCIJE_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void sortiranje_po_trajanju(INTERVENCIJE* polje, int broj); // od najkraæe do najduže
void zamjena(INTERVENCIJE* a, INTERVENCIJE* b);
void obrisi_datoteku();
INTERVENCIJE* ucitaj_intervencije(int* broj);

extern int broj; // deklaracija globalne varijable broj (broj intervencija)
#endif
