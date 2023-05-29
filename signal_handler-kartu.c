#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define MANY_CARD 56
#define NAME_CARD 14

// misalkan ada jeda tiap pengocokan kartu
// misalkan di suruh 10
// setiap 3 atau 4 detik kartu keluar
// pengeluaran kartu pertama
// pengkocokkan . . . .
// keluar -> misalkan sesuatu dari kartu yang di suffle
// Lanjut pengkocokkan
// pengkocokkan . . . .
// lalu jika ada joker
// minta inputan user, apakah ingin lanjut atau enggak?

typedef struct card
{
    const char *nameCard;
    const char *suits;
} Card;

void fillcard(Card *const cardptr, const char *nameCard[], const char *suits[]);
void sufflecard(Card *const cardptr);
void printcard(Card *const cardptr);
void firstPrint(Card *const cardptr);
void signalHandler(int signalValue);
void hapus_terminal();

int main()
{
    Card temp[MANY_CARD];
    const char *name[] = {"Ace",
                          "Deuce",
                          "Three",
                          "Four",
                          "Five",
                          "Six",
                          "Seven",
                          "Eight",
                          "Nine",
                          "Ten",
                          "Jack",
                          "Queen",
                          "King",
                          "Joker"};
    const char *suits[] = {"Heart", "Diamonds", "Clubs", "Spades"};

    srand(time(NULL));

    fillcard(temp, name, suits);
    sufflecard(temp);
    firstPrint(temp);
}

void fillcard(Card *const cardptr, const char *nameCards[], const char *suits[])
{
    int i = 0;
fill:
    while (i < MANY_CARD)
    {
        cardptr[i].nameCard = nameCards[i % NAME_CARD];
        cardptr[i].suits = suits[i / NAME_CARD];
        i++;
        goto fill;
    }
}

void sufflecard(Card *const cardptr)
{
    int i = 0;
suffle:
    while (i < MANY_CARD)
    {
        unsigned int j = rand() % MANY_CARD;
        Card temp = cardptr[i];
        cardptr[i] = cardptr[j];
        cardptr[j] = cardptr[i];
        i++;
        goto suffle;
    }
}

void signalHandler(int signalValue)
{
    printf("Kamu Mendapatkan joker");
    printf("\n%s%d%s\n%s",
           "Interrupt joker (", signalValue, ") sorry you have joker.",
           "Do you wish to continue (1 = yes or 2 = no)? ");
    int a;
    scanf("%d", &a);

    while (a != 1 && a != 2)
    {
        printf("%s", "(1 = yes or 2 = no)? ");
        scanf("%d", &a);
    }

    if (a == 1)
    {
        signal(SIGINT, signalHandler);
    }
    else
    {
        exit(1);
    }
}

void firstPrint(Card *const cardptr)
{
    hapus_terminal();
    for (int j = 0; j < MANY_CARD; j++)
    {
        printf("=");
    }
    printf("\n\t%s\n", "Selamat Datang di Permainan Hajo");
    printf("\t\t%s\n", "Hati-hati Dapet Joker");
    for (int j = 0; j < MANY_CARD; j++)
    {
        printf("=");
    }
    puts("");
    puts("Akan ada pengocokkan kartu, dikeluarkan 10 kartu");
    puts("untuk sekali bermain. Kartu akan di suffle oleh ");
    puts("program jika tidak mendapatkan joker, maka akan ");
    puts("mendapatkan tambahan poin 10. Jika mendapatkan joker ");
    puts("maka akan berakhir permainannya");
    printf("1. Mulai Game\n2. Exit Game");
    int a;
    printf("\n>> ");
    scanf("%d", &a);
    if (a == 1)
    {
        printcard(cardptr);
    }
    else
    {
        exit(0);
    }
}

void printcard(Card *const cardptr)
{
    hapus_terminal();
    int i = 0;
    signal(SIGINT, signalHandler);
    srand(time(NULL));
print:
    while (i < 10)
    {
        sleep(1);
        printf("\nKeluaran %d", (1 + i));
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".\n");
        printf("|%*.5s of %-*.8s|%s", 10, cardptr[i].nameCard, 15, cardptr[i].suits, 0 ? " " : "\n");
        if (strcmp(cardptr[i].nameCard, "Joker") == 0)
        {
            raise(SIGINT);
        }
        i++;
        goto print;
    }

    puts("");
    puts("Selamat bagi yang tidak dapat Joker kamu dapat poin 10");
    puts("Maaf yang mendapat joker, bisa coba run lagi");
    puts("Terimakasih");
    exit(2);
}
void hapus_terminal()
{
    if (system("cls"))
        system("clear");
}
