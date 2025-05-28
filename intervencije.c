#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intervencije.h"

static int broj = 0; //broj intervenija

int izbornik() {
    int uvjet = 0;
    INTERVENCIJE* polje = NULL;
    int brojIntervencija = 0;

    do {
        printf("\n===== IZBORNIK =====\n");
        printf("1. Dodaj intervenciju\n");
        printf("2. Ažuriraj intervenciju\n");
        printf("3. Ispis intervencija\n");
        printf("4. Pretraga (ID / Vrsta / Lokacija)\n");
        printf("5. Sortiraj po trajanju\n");
        printf("6. Obriši intervenciju\n");
        printf("7. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &uvjet);
        getchar(); // flush enter

        switch (uvjet) {
        case 1:
            dodaj_intervenciju();
            break;

        case 2:
            azuriraj_intervenciju();
            break;

        case 3:
            ispis_intervencije();
            break;

        case 4:
            brojIntervencija = ucitaj_intervencije(&polje);
            if (brojIntervencija == 0) {
                printf("Nema intervencija za pretragu.\n");
                break;
            }
            printf("Pretraga po:\n1. ID\n2. Vrsti\n3. Lokaciji\nOdabir: ");
            int izbor;
            scanf("%d", &izbor);
            getchar();
            if (izbor == 1) pretraga_po_ID(polje, brojIntervencija);
            else if (izbor == 2) pretraga_po_vrsti(polje, brojIntervencija);
            else if (izbor == 3) pretraga_po_lokaciji(polje, brojIntervencija);
            else printf("Nepoznata opcija.\n");
            free(polje);
            break;

        case 5:
            brojIntervencija = ucitaj_intervencije(&polje);
            if (brojIntervencija == 0) {
                printf("Nema intervencija za sortiranje.\n");
                break;
            }
            sortiranje_po_trajanju(polje, brojIntervencija);
            free(polje);
            break;

        case 6:
            brisanje_intervencije();
            break;

        case 7:
            printf("Izlazak iz programa.\n");
            break;

        default:
            printf("Nepoznata opcija.\n");
        }

    } while (uvjet != 7);

    return 0;
}

void kreirajdatoteku() {
    FILE* fp = fopen("intervencije.bin", "wb");
    if (fp == NULL) {
        perror("Pogreska pri kreiranju datoteke");
        return;
    }
    broj = 0;
    fwrite(&broj, sizeof(int), 1, fp);
    fclose(fp);
}

void dodaj_intervenciju() {
    FILE* fp = fopen("intervencije.bin", "rb+");
    if (fp == NULL) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    fread(&broj, sizeof(int), 1, fp);
    fseek(fp, 0, SEEK_END);            

    INTERVENCIJE nova;

    printf("Unesite ID intervencije: ");
    scanf("%d", &nova.id);
    getchar();

    printf("Unesite vrstu intervencije: ");
    fgets(nova.vrsta, sizeof(nova.vrsta), stdin);
    nova.vrsta[strcspn(nova.vrsta, "\n")] = 0;

    printf("Unesite datum (DD.MM.GGGG): ");
    fgets(nova.datum, sizeof(nova.datum), stdin);
    nova.datum[strcspn(nova.datum, "\n")] = 0;

    printf("Unesite vrijeme (HH:MM): ");
    fgets(nova.vrijeme, sizeof(nova.vrijeme), stdin);
    nova.vrijeme[strcspn(nova.vrijeme, "\n")] = 0;

    printf("Unesite trajanje intervencije (u minutama): ");
    scanf("%d", &nova.trajanje);
    getchar();

    printf("Unesite lokaciju intervencije: ");
    fgets(nova.lokacija, sizeof(nova.lokacija), stdin);
    nova.lokacija[strcspn(nova.lokacija, "\n")] = 0;

    printf("Unesite broj vatrogasaca: ");
    scanf("%d", &nova.brojVatrogasaca);
    getchar();

    printf("Unesite broj vozila: ");
    scanf("%d", &nova.brojVozila);
    getchar();

    fwrite(&nova, sizeof(INTERVENCIJE), 1, fp);

    broj++;
    fseek(fp, 0, SEEK_SET);
    fwrite(&broj, sizeof(int), 1, fp);

    fclose(fp);

    printf("Intervencija uspjesno dodana.\n");
}

INTERVENCIJE* ucitaj_intervencije(int* broj) {
    FILE* fp = fopen("intervencije.bin", "rb");
    if (!fp) {
        *broj = 0;
        return NULL;
    }

    fread(broj, sizeof(int), 1, fp);
    if (*broj == 0) {
        fclose(fp);
        return NULL;
    }

    INTERVENCIJE* polje = malloc(*broj * sizeof(INTERVENCIJE));
    if (!polje) {
        perror("Greska pri alokaciji memorije");
        fclose(fp);
        *broj = 0;
        return NULL;
    }

    fread(polje, sizeof(INTERVENCIJE), *broj, fp);
    fclose(fp);
    return polje;
}

void azuriraj_intervenciju() {
    FILE* fp = fopen("intervencije.bin", "rb+");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    fread(&broj, sizeof(int), 1, fp);
    if (broj == 0) {
        printf("Nema intervencija za azurirati.\n");
        fclose(fp);
        return;
    }

    INTERVENCIJE* polje = malloc(broj * sizeof(INTERVENCIJE));
    if (!polje) {
        perror("Greska pri alokaciji memorije");
        fclose(fp);
        return;
    }

    fread(polje, sizeof(INTERVENCIJE), broj, fp);

    int trazeniID;
    printf("Unesite ID intervencije koju zelite azurirati: ");
    scanf("%d", &trazeniID);
    getchar();

    int i, pronadeno = 0;
    for (i = 0; i < broj; i++) {
        if (polje[i].id == trazeniID) {
            pronadeno = 1;

            printf("Unesite novu vrstu intervencije: ");
            fgets(polje[i].vrsta, sizeof(polje[i].vrsta), stdin);
            polje[i].vrsta[strcspn(polje[i].vrsta, "\n")] = 0;

            printf("Unesite novi datum (DD.MM.GGGG): ");
            fgets(polje[i].datum, sizeof(polje[i].datum), stdin);
            polje[i].datum[strcspn(polje[i].datum, "\n")] = 0;

            printf("Unesite novo vrijeme (HH:MM): ");
            fgets(polje[i].vrijeme, sizeof(polje[i].vrijeme), stdin);
            polje[i].vrijeme[strcspn(polje[i].vrijeme, "\n")] = 0;

            printf("Unesite novo trajanje (u minutama): ");
            scanf("%d", &polje[i].trajanje);
            getchar();

            printf("Unesite novu lokaciju: ");
            fgets(polje[i].lokacija, sizeof(polje[i].lokacija), stdin);
            polje[i].lokacija[strcspn(polje[i].lokacija, "\n")] = 0;

            printf("Unesite novi broj vatrogasaca: ");
            scanf("%d", &polje[i].brojVatrogasaca);
            getchar();

            printf("Unesite novi broj vozila: ");
            scanf("%d", &polje[i].brojVozila);
            getchar();

            break;
        }
    }

    if (!pronadeno) {
        printf("Intervencija s ID %d nije pronadena.\n", trazeniID);
    }
    else {
        // Upisujemo promijenjeni zapis natrag u datoteku
        fseek(fp, sizeof(int), SEEK_SET);
        fwrite(polje, sizeof(INTERVENCIJE), broj, fp);
        printf("Intervencija azurirana.\n");
    }

    free(polje);
    fclose(fp);
}

void ispis_intervencije() {
    FILE* fp = fopen("intervencije.bin", "rb");
    if (!fp) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    fread(&broj, sizeof(int), 1, fp);
    if (broj == 0) {
        printf("Nema unesenih intervencija za ispis.\n");
        fclose(fp);
        return;
    }

    INTERVENCIJE* polje = malloc(broj * sizeof(INTERVENCIJE));
    if (!polje) {
        perror("Greska pri alokaciji memorije");
        fclose(fp);
        return;
    }

    fread(polje, sizeof(INTERVENCIJE), broj, fp);

    for (int i = 0; i < broj; i++) {
        printf("Intervencija #%d\n", i + 1);
        printf("ID: %d\n", polje[i].id);
        printf("Vrsta: %s\n", polje[i].vrsta);
        printf("Datum: %s\n", polje[i].datum);
        printf("Vrijeme: %s\n", polje[i].vrijeme);
        printf("Trajanje: %d minuta\n", polje[i].trajanje);
        printf("Lokacija: %s\n", polje[i].lokacija);
        printf("Broj vatrogasaca: %d\n", polje[i].brojVatrogasaca);
        printf("Broj vozila: %d\n", polje[i].brojVozila);
        printf("--------------------------\n");
    }

    free(polje);
    fclose(fp);
}

void brisanje_intervencije() {
    FILE* fp = fopen("intervencije.bin", "rb");
    if (!fp) {
        printf("Datoteka ne postoji.\n");
        return;
    }

    int brojIntervencija;
    fread(&brojIntervencija, sizeof(int), 1, fp);
    if (brojIntervencija == 0) {
        printf("Nema intervencija za brisanje.\n");
        fclose(fp);
        return;
    }

    INTERVENCIJE* niz = malloc(brojIntervencija * sizeof(INTERVENCIJE));
    fread(niz, sizeof(INTERVENCIJE), brojIntervencija, fp);
    fclose(fp);

    int id, noviBroj = 0;
    printf("Unesite ID za brisanje: ");
    scanf("%d", &id);

    for (int i = 0; i < brojIntervencija; i++) {
        if (niz[i].id != id)
            niz[noviBroj++] = niz[i];
    }

    if (noviBroj == brojIntervencija) {
        printf("ID nije pronaden.\n");
        free(niz);
        return;
    }

    fp = fopen("intervencije.bin", "wb");
    fwrite(&noviBroj, sizeof(int), 1, fp);
    fwrite(niz, sizeof(INTERVENCIJE), noviBroj, fp);
    fclose(fp);
    free(niz);

    printf("Intervencija je obrisana.\n");
}

void pretraga_po_ID(INTERVENCIJE* polje, int broj) {
    int trazeniID, brojac = 0;
    printf("Upisite ID intervencije koju zelite pronaci:\n");
    scanf("%d", &trazeniID);
    getchar();

    for (int i = 0; i < broj; i++) {
        if (polje[i].id == trazeniID) {
            printf("\nIntervencija je pronađena!\n\n");
            printf("ID: %d\tVrsta: %s\tDatum: %s\tVrijeme: %s\tTrajanje: %d min\tLokacija: %s\tBroj vatrogasaca: %d\tBroj vozila: %d\n\n",
                polje[i].id,
                polje[i].vrsta,
                polje[i].datum,
                polje[i].vrijeme,
                polje[i].trajanje,
                polje[i].lokacija,
                polje[i].brojVatrogasaca,
                polje[i].brojVozila);
            brojac++;
        }
    }

    if (brojac == 0)
        printf("\nIntervencija s tim ID-em nije pronađena!\n\n");
}

void pretraga_po_vrsti(INTERVENCIJE* polje, int broj) {
    char trazena_vrsta[50];
    int brojac = 0;
    printf("Upisi vrstu intervenciju koju zelis pronaci:\n");
    fgets(trazena_vrsta, sizeof(trazena_vrsta), stdin);
    trazena_vrsta[strcspn(trazena_vrsta, "\n")] = 0;

    for (int i = 0; i < broj; i++) {
        if (strcmp(trazena_vrsta, polje[i].vrsta) == 0) {
            printf("\nVrsta intervencije je pronađena!\n\n");
            printf("ID: %d\tVrsta: %s\tDatum: %s\tVrijeme: %s\tTrajanje: %d min\tLokacija: %s\tBroj vatrogasaca: %d\tBroj vozila: %d\n\n",
                polje[i].id,
                polje[i].vrsta,
                polje[i].datum,
                polje[i].vrijeme,
                polje[i].trajanje,
                polje[i].lokacija,
                polje[i].brojVatrogasaca,
                polje[i].brojVozila);
            brojac++;
        }
    }
    if (brojac == 0)
        printf("\nVrsta intervencije nije pronađena!\n\n");
}

void pretraga_po_lokaciji(INTERVENCIJE* polje, int broj) {
    char trazena_lokacija[50];
    int brojac = 0;
    printf("Upisi lokaciju intervencije koju zelis pronaci:\n");
    fgets(trazena_lokacija, sizeof(trazena_lokacija), stdin);
    trazena_lokacija[strcspn(trazena_lokacija, "\n")] = 0;

    for (int i = 0; i < broj; i++) {
        if (strcmp(trazena_lokacija, polje[i].lokacija) == 0) {
            printf("\nIntervencija na trazenoj lokaciji je pronadena!\n\n");
            printf("ID: %d\tVrsta: %s\tDatum: %s\tVrijeme: %s\tTrajanje: %d min\tLokacija: %s\tBroj vatrogasaca: %d\tBroj vozila: %d\n\n",
                polje[i].id,
                polje[i].vrsta,
                polje[i].datum,
                polje[i].vrijeme,
                polje[i].trajanje,
                polje[i].lokacija,
                polje[i].brojVatrogasaca,
                polje[i].brojVozila);
            brojac++;
        }
    }
    if (brojac == 0)
        printf("\nNa lokaciji %s nije pronadena nijedna intervencija.\n\n", trazena_lokacija);
}

void zamjena(INTERVENCIJE* a, INTERVENCIJE* b) {
    INTERVENCIJE tmp = *a;
    *a = *b;
    *b = tmp;
}

void sortiranje_po_trajanju(INTERVENCIJE* polje, int broj) {
    if (broj <= 1) return;

    for (int i = 0; i < broj - 1; i++) {
        int minIndeks = i;
        for (int j = i + 1; j < broj; j++) {
            if (polje[j].trajanje < polje[minIndeks].trajanje) {
                minIndeks = j;
            }
        }
        if (minIndeks != i) {
            zamjena(&polje[i], &polje[minIndeks]);
        }
    }

    printf("\nINTERVENCIJE SORTIRANE PO TRAJANJU (od najkrace do najduze)\n");
    for (int i = 0; i < broj; i++) {
        printf("ID: %d\tVrsta: %s\tDatum: %s\tVrijeme: %s\tTrajanje: %d min\tLokacija: %s\tBroj vatrogasaca: %d\tBroj vozila: %d\n",
            polje[i].id,
            polje[i].vrsta,
            polje[i].datum,
            polje[i].vrijeme,
            polje[i].trajanje,
            polje[i].lokacija,
            polje[i].brojVatrogasaca,
            polje[i].brojVozila);
    }
}
