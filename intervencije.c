#define _CRT_SECURE_NO_WARNINGS
#include "intervencije.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int broj = 0;

int izbornik() {
    int uvjet = 0;
    INTERVENCIJE* polje = NULL;
    int brojIntervencija = 0;

    do {
        printf("\n===== IZBORNIK =====\n");
        printf("1. Dodaj intervenciju\n");
        printf("2. Azuriraj intervenciju\n");
        printf("3. Ispis intervencija\n");
        printf("4. Pretraga (ID / Vrsta / Lokacija)\n");
        printf("5. Sortiraj po trajanju\n");
        printf("6. Obrisi intervenciju\n");
        printf("7. Obrisi datoteku\n");
        printf("8. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &uvjet);
        getchar();  // ciscenje novog reda iz buffer-a


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
            polje = ucitaj_intervencije(&brojIntervencija);
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
            polje = ucitaj_intervencije(&brojIntervencija);
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
            obrisi_datoteku();
            break;
        case 8:
            printf("Izlazak iz programa.\n");
            exit(0);
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (uvjet != 8);

    return 0;
}

void kreirajdatoteku() {
    FILE* fp = fopen("intervencije.txt", "w");
    if (!fp) {
        perror("Greška pri kreiranju datoteke");
        return;
    }
    fprintf(fp, "0\n");
    fclose(fp);
}

void dodaj_intervenciju() {
    static int lokalniBrojac = 0;

    FILE* fp = fopen("intervencije.txt", "r+");
    if (!fp) {
        perror("Greška pri otvaranju datoteke");
        return;
    }

    fscanf(fp, "%d\n", &broj);

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

    printf("Unesite trajanje (min): ");
    scanf("%d", &nova.trajanje);
    getchar();

    printf("Unesite lokaciju: ");
    fgets(nova.lokacija, sizeof(nova.lokacija), stdin);
    nova.lokacija[strcspn(nova.lokacija, "\n")] = 0;

    printf("Unesite broj vatrogasaca: ");
    scanf("%d", &nova.brojVatrogasaca);
    getchar();

    printf("Unesite broj vozila: ");
    scanf("%d", &nova.brojVozila);
    getchar();

    // pomice pokazivac na kraj datoteke i upisuje novu intervenciju
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%d|%s|%s|%s|%d|%s|%d|%d\n",
        nova.id, nova.vrsta, nova.datum, nova.vrijeme,
        nova.trajanje, nova.lokacija, nova.brojVatrogasaca, nova.brojVozila);

    // azuriranje broja intervencija u datoteci s tim  da se broj intervencija poveca za 1
    broj++;
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", broj);

    fclose(fp);

    lokalniBrojac++;  // poveca se svaki put kad se doda nova intervencija
    printf("Intervencija uspjesno dodana. (Lokalno dodano ukupno: %d)\n", lokalniBrojac);
}


INTERVENCIJE* ucitaj_intervencije(int* brojOut) {
    FILE* fp = fopen("intervencije.txt", "r");
    if (!fp) {
        *brojOut = 0;
        return NULL;
    }

    if (fscanf(fp, "%d\n", brojOut) != 1 || *brojOut <= 0) {
        *brojOut = 0;
        fclose(fp);
        return NULL;
    }

    INTERVENCIJE* polje = malloc(*brojOut * sizeof(INTERVENCIJE));
    if (!polje) {
        perror("Alokacija memorije neuspješna");
        fclose(fp);
        *brojOut = 0;
        return NULL;
    }

    for (int i = 0; i < *brojOut; i++) {
        char linija[256];

        if (!fgets(linija, sizeof(linija), fp)) {
            *brojOut = i;
            break;
        }

        // Èitamo podatke iz reda koristeæi sscanf
        sscanf(linija, "%d|%19[^|]|%11[^|]|%9[^|]|%d|%49[^|]|%d|%d",
            &polje[i].id,
            polje[i].vrsta,
            polje[i].datum,
            polje[i].vrijeme,
            &polje[i].trajanje,
            polje[i].lokacija,
            &polje[i].brojVatrogasaca,
            &polje[i].brojVozila
        );
    }

    fclose(fp);
    return polje;
}


void azuriraj_intervenciju() {
    int brojIntervencija;
    INTERVENCIJE* polje = ucitaj_intervencije(&brojIntervencija);
    if (!polje || brojIntervencija == 0) {
        printf("Nema intervencija za azuriranje.\n");
        free(polje);
        return;
    }

    int id;
    printf("Unesite ID intervencije za azuriranje: ");
    scanf("%d", &id);
    getchar();

    int found = 0;
    for (int i = 0; i < brojIntervencija; i++) {
        if (polje[i].id == id) {
            found = 1;

            printf("Nova vrsta: ");
            fgets(polje[i].vrsta, sizeof(polje[i].vrsta), stdin);
            polje[i].vrsta[strcspn(polje[i].vrsta, "\n")] = 0;

            printf("Novi datum: ");
            fgets(polje[i].datum, sizeof(polje[i].datum), stdin);
            polje[i].datum[strcspn(polje[i].datum, "\n")] = 0;

            printf("Novo vrijeme: ");
            fgets(polje[i].vrijeme, sizeof(polje[i].vrijeme), stdin);
            polje[i].vrijeme[strcspn(polje[i].vrijeme, "\n")] = 0;

            printf("Novo trajanje (min): ");
            scanf("%d", &polje[i].trajanje);
            getchar();

            printf("Nova lokacija: ");
            fgets(polje[i].lokacija, sizeof(polje[i].lokacija), stdin);
            polje[i].lokacija[strcspn(polje[i].lokacija, "\n")] = 0;

            printf("Novi broj vatrogasaca: ");
            scanf("%d", &polje[i].brojVatrogasaca);
            getchar();

            printf("Novi broj vozila: ");
            scanf("%d", &polje[i].brojVozila);
            getchar();

            break;
        }
    }

    if (!found) {
        printf("Intervencija s ID %d nije pronadena.\n", id);
    }
    else {
        FILE* fp = fopen("intervencije.txt", "w");
        if (!fp) {
            perror("Greška pri otvaranju datoteke");
            free(polje);
            return;
        }

        fprintf(fp, "%d\n", brojIntervencija);
        for (int i = 0; i < brojIntervencija; i++) {
            fprintf(fp, "%d|%s|%s|%s|%d|%s|%d|%d\n",
                polje[i].id, polje[i].vrsta, polje[i].datum, polje[i].vrijeme,
                polje[i].trajanje, polje[i].lokacija, polje[i].brojVatrogasaca, polje[i].brojVozila);
        }
        fclose(fp);
        printf("Intervencija azurirana.\n");
    }

    free(polje);
}

void rekurzivni_ispis(INTERVENCIJE* polje, int n) {
    if (n <= 0) return;

    rekurzivni_ispis(polje, n - 1);

    printf("\nIntervencija #%d\n", n);
    printf("ID: %d\n", polje[n - 1].id);
    printf("Vrsta: %s\n", polje[n - 1].vrsta);
    printf("Datum: %s\n", polje[n - 1].datum);
    printf("Vrijeme: %s\n", polje[n - 1].vrijeme);
    printf("Trajanje: %d min\n", polje[n - 1].trajanje);
    printf("Lokacija: %s\n", polje[n - 1].lokacija);
    printf("Broj vatrogasaca: %d\n", polje[n - 1].brojVatrogasaca);
    printf("Broj vozila: %d\n", polje[n - 1].brojVozila);
}

void ispis_intervencije() {
    INTERVENCIJE* polje = ucitaj_intervencije(&broj);
    if (!polje) {
        printf("Nema intervencija za ispis.\n");
        return;
    }

    rekurzivni_ispis(polje, broj);

    free(polje);
}


void brisanje_intervencije() {
    int brojIntervencija;
    INTERVENCIJE* polje = ucitaj_intervencije(&brojIntervencija);
    if (!polje || brojIntervencija == 0) {
        printf("Datoteka ne postoji ili nema intervencija.\n");
        free(polje);
        return;
    }

    int id, noviBroj = 0;
    printf("Unesite ID za brisanje: ");
    scanf("%d", &id);
    getchar();

    INTERVENCIJE* noviPolje = malloc(brojIntervencija * sizeof(INTERVENCIJE));
    if (!noviPolje) {
        perror("Alokacija memorije neuspješna");
        free(polje);
        return;
    }

    for (int i = 0; i < brojIntervencija; i++) {
        if (polje[i].id != id) {
            noviPolje[noviBroj++] = polje[i];
        }
    }

    if (noviBroj == brojIntervencija) {
        printf("ID nije pronaðen.\n");
        free(polje);
        free(noviPolje);
        return;
    }

    FILE* fpTemp = fopen("temp_intervencije.txt", "w");
    if (!fpTemp) {
        perror("Greška pri otvaranju privremene datoteke");
        free(polje);
        free(noviPolje);
        return;
    }

    fprintf(fpTemp, "%d\n", noviBroj);
    for (int i = 0; i < noviBroj; i++) {
        fprintf(fpTemp, "%d|%s|%s|%s|%d|%s|%d|%d\n",
            noviPolje[i].id, noviPolje[i].vrsta, noviPolje[i].datum, noviPolje[i].vrijeme,
            noviPolje[i].trajanje, noviPolje[i].lokacija, noviPolje[i].brojVatrogasaca, noviPolje[i].brojVozila);
    }
    fclose(fpTemp);

    // Brisanje originalne i preimenovanje temp datoteke
    if (remove("intervencije.txt") != 0) {
        perror("Greška pri brisanju originalne datoteke");
    }
    else if (rename("temp_intervencije.txt", "intervencije.txt") != 0) {
        perror("Greska pri preimenovanju datoteke");
    }
    else {
        printf("Intervencija uspjesno obrisana.\n");
    }

    free(polje);
    free(noviPolje);
}



void pretraga_po_ID(INTERVENCIJE* polje, int broj) {
    int id;
    printf("Unesite ID: ");
    scanf("%d", &id);

    for (int i = 0; i < broj; i++) {
        if (polje[i].id == id) {
            printf("Pronadena intervencija: %s (%d min)\n", polje[i].vrsta, polje[i].trajanje);
            return;
        }
    }

    printf("Intervencija nije pronadena.\n");
}

void pretraga_po_vrsti(INTERVENCIJE* polje, int broj) {
    char vrsta[50];
    printf("Unesite vrstu: ");
    fgets(vrsta, sizeof(vrsta), stdin);
    vrsta[strcspn(vrsta, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < broj; i++) {
        // Radi obiènu usporedbu bez ignoriranja velikih/malih slova
        if (strcmp(polje[i].vrsta, vrsta) == 0) {
            printf("ID: %d | Lokacija: %s | Trajanje: %d min\n", polje[i].id, polje[i].lokacija, polje[i].trajanje);
            found = 1;
        }
    }

    if (!found)
        printf("Nema pronadenih intervencija te vrste.\n");
}

void pretraga_po_lokaciji(INTERVENCIJE* polje, int broj) {
    char lokacija[50];
    printf("Unesite lokaciju: ");
    fgets(lokacija, sizeof(lokacija), stdin);
    // uklanjamo znak novog reda na kraju unosa
    lokacija[strcspn(lokacija, "\n")] = 0;

    int found = 0;

    for (int i = 0; i < broj; i++) {
        if (strcmp(polje[i].lokacija, lokacija) == 0) {
            printf("ID: %d | Vrsta: %s | Trajanje: %d min\n",
                polje[i].id, polje[i].vrsta, polje[i].trajanje);
            found = 1;
        }
    }

    if (!found) {
        printf("Nema pronadenih intervencija na toj lokaciji.\n");
    }
}

static inline void zamjena(INTERVENCIJE* a, INTERVENCIJE* b) {
    INTERVENCIJE temp = *a;
    *a = *b;
    *b = temp;
}

int usporediTrajanje(const void* a, const void* b) {
    const INTERVENCIJE* ia = (const INTERVENCIJE*)a;
    const INTERVENCIJE* ib = (const INTERVENCIJE*)b;
    return ia->trajanje - ib->trajanje;
}

void sortiranje_po_trajanju(INTERVENCIJE* polje, int broj) {
    qsort(polje, broj, sizeof(INTERVENCIJE), usporediTrajanje);

    printf("--- Sortirane intervencije po trajanju ---\n");
    for (int i = 0; i < broj; i++) {
        printf("ID: %d | Trajanje: %d min | Vrsta: %s\n",
            polje[i].id, polje[i].trajanje, polje[i].vrsta);
    }
}
void obrisi_datoteku() {
    char potvrda;

    printf("Jeste li sigurni da zelite obrisati datoteku 'intervencije.txt'? (D/N): ");
    scanf(" %c", &potvrda);

    if (potvrda == 'D' || potvrda == 'd') {
        if (remove("intervencije.txt") == 0) {
            printf("Datoteka je uspjesno obrisana.\n");
        }
        else {
            perror("Greska pri brisanju datoteke");
        }
    }
    else {
        printf("Brisanje datoteke je otkazano.\n");
    }
}
void oslobodi_memoriju(INTERVENCIJE* polje) {
    if (polje != NULL) {
        free(polje);
        polje = NULL;
    }
}