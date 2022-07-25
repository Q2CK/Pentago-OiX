#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct hist_ruch {
    char histCwiartka;
    char histPole;
    char histCwObr;
    char histObr;
};

//Plansza przykladowa
const char arrExample[4][3][3] = { {{' ','N',' '},
                                    {'C','C','N'},
                                    {' ','C',' '}},
                                                {{' ','C',' '},
                                                 {'C','N',' '},
                                                 {' ',' ',' '}},
                                    {{'C','C',' '},
                                     {'N',' ','C'},
                                     {' ','N',' '}},
                                                {{' ',' ',' '},
                                                 {' ','N',' '},
                                                 {' ','N','N'}} };

//Zmienne odpowiedzianle za wyglad planszy
const char sufit[26] = { 201, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 187, 201, 205, 205, 205, 203, 205, 205, 205, 203, 205, 205, 205, 187 };
const char scianka = 186;
const char poziom[26] = { 204, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 185, 204, 205, 205, 205, 206, 205, 205, 205, 206, 205, 205, 205, 185 };
const char podloga[26] = { 200, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 188, 200, 205, 205, 205, 202, 205, 205, 205, 202, 205, 205, 205, 188 };

//Parametry planszy
const int wymiar = 6;
const int cwiartki = 4;

const int pozycjaX = 0; //Przesuniecie planszy w poziomie
const int pozycjaY = 0; //Przesuniecie planszy w pionie

const int limitCzasu = 600; //Czas gry w sekundach

void odstep_x() { //Funkcja przesuwajaca plansze w poziomie
    for (int h = 0; h < pozycjaX - 1; h++) {
        cout << " ";
    }
};

void rotateR(char arr[wymiar / 2][wymiar / 2]) { //Obrot cwiartki w prawo
    int temp1 = arr[0][0];
    int temp2 = arr[1][0];
    arr[0][0] = arr[2][0];
    arr[1][0] = arr[2][1];
    arr[2][0] = arr[2][2];
    arr[2][1] = arr[1][2];
    arr[2][2] = arr[0][2];
    arr[1][2] = arr[0][1];
    arr[0][2] = temp1;
    arr[0][1] = temp2;
}

void rotateL(char arr[wymiar / 2][wymiar / 2]) { //Obrot cwiartki w lewo
    int temp1 = arr[0][0];
    int temp2 = arr[0][1];
    arr[0][0] = arr[0][2];
    arr[0][1] = arr[1][2];
    arr[0][2] = arr[2][2];
    arr[1][2] = arr[2][1];
    arr[2][2] = arr[2][0];
    arr[2][1] = arr[1][0];
    arr[2][0] = temp1;
    arr[1][0] = temp2;
}

//Funkcja zamieniajaca trojwymiarowa tablice planszy na dwuwymiarowa
void zamiana2d(char arr[cwiartki][wymiar / 2][wymiar / 2], char board[wymiar][wymiar]) {
    for (int i = 0; i < wymiar / 2; i++) {
        for (int j = 0; j < wymiar / 2; j++) {
            board[i][j] = arr[0][i][j];
            board[i][j + 3] = arr[1][i][j];
            board[i + 3][j] = arr[2][i][j];
            board[i + 3][j + 3] = arr[3][i][j];
        }
    }
}

//Funkcja sprawdzajaca wygrana danego gracza
bool win(char arr[cwiartki][wymiar / 2][wymiar / 2], char player) {
    bool win_ = false;

    char board[wymiar][wymiar] = {};
    int counterX = 0;
    int counterY = 0;
    int counterD[wymiar] = { 0, 0, 0, 0, 0, 0 }; //6 skosow mieszczacych 5 lub wiecej pol

    zamiana2d(arr, board);

    for (int i = 0; i < wymiar; i++) { //Naliczanie zetonow w rzedach i kolumnach
        counterX = 0;
        counterY = 0;
        for (int j = 0; j < wymiar; j++) {
            if (board[i][j] == player)
                counterX++;
            if (board[j][i] == player)
                counterY++;
        }
        if ((counterX == 5 && (board[i][0] != player || board[i][5] != player)) || (counterY == 5 && (board[0][i] != player || board[5][i] != player)) || counterX > 5 || counterY > 5)
            win_ = true;
        //Warunek logiczny zapewniajacy, ze 5 takich samych zetonow nie jest oddzielone jednym zetonem innego typu lub pustka, lub jest ich po prostu 6
    }

    for (int i = 0; i < wymiar && !win_; i++) { //Naliczanie zetonow po skosach 6-miejscowych (przekatne planszy)
        if (board[i][i] == player)
            counterD[0]++;
        if (board[5 - i][i] == player)
            counterD[1]++;
    }
    if ((counterD[0] == 5 && (board[0][0] != player || board[5][5] != player)) || (counterD[1] == 5 && (board[5][0] != player || board[0][5] != player)) || counterD[0] > 5 || counterY > 5) {
        win_ = true;
    }

    for (int i = 0; i < wymiar - 1 && !win_; i++) { //naliczanie zetonow po skosach 5-miejscowych (skosy sasiednie do przekatnych)
        if (board[i + 1][i] == player)
            counterD[2]++;
        if (board[i][i + 1] == player)
            counterD[3]++;
        if (board[4 - i][i] == player)
            counterD[4]++;
        if (board[5 - i][i + 1] == player)
            counterD[5]++;
    }
    for (int i = 2; i < wymiar; i++)
        if (counterD[i] >= wymiar - 1)
            win_ = true;

    return win_;
}

//Funkcja wyswietlajaca plansze gry
void wyswietl(char tab[cwiartki][wymiar / 2][wymiar / 2]) {
    for (int i = 0; i < pozycjaY; i++) { //Przesuniecie w pionie
        cout << endl;
    }
    char board[wymiar][wymiar] = {};
    zamiana2d(tab, board); //Konwersja tablicy na 2d dla latwiejszego przetwarzania
    odstep_x();
    cout << sufit << endl; //Wyswietlanie gornej czesci planszy

    for (int i = 0; i < wymiar; i++) {
        odstep_x();
        cout << scianka;
        for (int j = 0; j < wymiar; j++) {
            cout << " " << board[i][j] << " " << scianka; //Wyswietlanie zetonow i scianek miedzy nimi
            if (j == 2)
                cout << scianka;
        }
        cout << endl;
        odstep_x();
        if (i != 5 && i != 2)
            cout << poziom << endl; //Wyswietlanie poziomych linii na planszy
        else if (i == 5)
            cout << podloga << endl; //Wyswietlanie dolnej krawedzi planszy
        else if (i == 2) {
            cout << podloga << endl;
            odstep_x();
            cout << sufit << endl;
        }
    }
}

//Funkcja obslugi menu gry
void menu(char& gracz1, char& gracz2, char& gra, char historia[36][cwiartki][wymiar / 2][wymiar / 2], int hist_licznik, hist_ruch Hist_ruch[37]) {
    int licznik = 0;
    char opcja = '0';
    char opcja_4 = ' ';

    while (opcja != '3') {
        system("cls");
        cout << "===================== O P C J E =====================" << endl;
        cout << "1. Zmien rodzaj gry                                  " << endl;
        cout << "2. Wybierz zetony                                    " << endl;
        cout << "3. Zapisz i graj                                     " << endl;
        cout << "4. Pokaz podglad ostatniej gry                       " << endl;
        cin >> opcja;
        if (opcja == '1') { //Zmiana rodzaju gry
            system("cls");
            cout << "k - kolko i krzyzyk | p - pentago: ";
            cin >> gra;
            if (gra != 'k' && gra != 'p') {
                gra = 'k';
            }
            opcja = '0';
        }
        else if (opcja == '2') { //Zmiana zetonow
            do {
                system("cls");
                if (gracz1 == gracz2)
                    cout << "Zetony nie moga byc takie same!" << endl;
                cout << "Zeton gracza 1: ";
                cin >> gracz1;
                cout << "Zeton gracza 2: ";
                cin >> gracz2;
                opcja = '0';
            } while (gracz1 == gracz2);
        }
        else if (opcja == '3') { //Powrot do rozgrywki / rozpoczecie nowej rozgrywki
            system("cls");
            return;
        }
        else if (opcja == '4') { //Podglad przebiegu ostatniej rundy
            do {
                system("cls");
                wyswietl(historia[licznik]);
                cout << endl;
                odstep_x();
                if (licznik > 0)
                    cout << "POSUNIECIE NR: " << licznik << " | RUCH: " << Hist_ruch[licznik].histCwiartka << Hist_ruch[licznik].histPole << Hist_ruch[licznik].histCwObr << Hist_ruch[licznik].histObr << endl;
                else
                    cout << "PLANSZA POCZATKOWA" << endl;
                odstep_x();
                cout << "w - nast. ruch   s - poprz. ruch" << endl;
                odstep_x();
                cout << "0 - wyjdz z podgladu           ";

                cin >> opcja_4;

                if (opcja_4 == 'w' && licznik < hist_licznik) { //Nastepny ruch
                    licznik++;
                    opcja_4 = ' ';
                }
                if (opcja_4 == 's' && licznik > 0) { //Poprzedni ruch
                    licznik--;
                    opcja_4 = ' ';
                }
            } while (opcja_4 != '0');
        }
    }
    return;
}

//Funkcja wypisujaca podpowiedzi przyciskow
void legenda() {
    odstep_x();
    cout << "q, w, a, s    -   wybor cwiartki               POMOC" << endl;
    odstep_x();
    cout << "Num 1 .. 9    -   wybor pola w cwiartce" << endl;
    odstep_x();
    cout << "p - pauza   u - undo   o - wczytanie predef. planszy" << endl;
    odstep_x();
    cout << "m - wejscie do opcji   h - wejscie do opisu programu" << endl;
    odstep_x();
    cout << "z - obrot w lewo       x - obrot w prawo   m - opcje" << endl;
}

//Funkcja wypisujaca informacje o grze
void interfejs(char arr[cwiartki][wymiar / 2][wymiar / 2], string nazwa[2], char gracz[2], int kolej, bool& zajete, int czas) {
    wyswietl(arr);
    legenda();
    odstep_x();
    cout << "RUCH GRACZA: " << nazwa[kolej] << " - " << gracz[kolej] << " Pozostaly czas: " << limitCzasu - czas << " s" << endl;
    odstep_x();
    if (zajete) {
        zajete = false;
    }
}

int main()
{
    system("MODE CON COLS=80 LINES=25");
    system("color a");

    char historia[37][cwiartki][wymiar / 2][wymiar / 2] = {};
    hist_ruch Hist_ruch[37];

    char gracz[2] = { 'C', 'N' }; //Zetony i ich wartosci domyslne
    string nazwa[2] = { "","" }; //Nazwy graczy

    char gra = 'k'; //Rodzaj gry (kolki o krzyzyk / pentago)

    bool wylacznik = false;
    bool petla = true;

    char opcja = ' ';
    int kolej = 0; //Kolej ruchów

    int cwiartka = 1;
    int pole = 0;

    bool blad = false;
    bool poprawny = true;
    bool zajete = false;
    bool remis = false;

    //Zapelnianie pierwszej pozycji w tablicy historii spacjami
    int hist_licznik = 0;
    for (int i = 0; i < cwiartki; i++) {
        for (int j = 0; j < wymiar / 2; j++) {
            for (int k = 0; k < wymiar / 2; k++) {
                if (hist_licznik < 37)
                    historia[hist_licznik][i][j][k] = ' ';
            }
        }
    }

    //Glowna petla
    while (!wylacznik) {
        time_t czas1;
        time_t czas2;
        time_t czasPauza1;
        time_t czasPauza2;

        int czasPauza = 0;

        int czas[2] = { 0, 0 };

        char zetonStary1 = ' ';
        char zetonStary2 = ' ';

        petla = true;

        menu(gracz[0], gracz[1], gra, historia, hist_licznik, Hist_ruch);
        hist_licznik = 0;

        char arr[cwiartki][wymiar / 2][wymiar / 2] = { {{' ',' ',' '},
                                                       {' ',' ',' '},
                                                       {' ',' ',' '}},
                                                                 {{' ',' ',' '},
                                                                  {' ',' ',' '},
                                                                  {' ',' ',' '}},
                                                      {{' ',' ',' '},
                                                       {' ',' ',' '},
                                                       {' ',' ',' '}},
                                                                 {{' ',' ',' '},
                                                                  {' ',' ',' '},
                                                                  {' ',' ',' '}} }; //Glowna plansza

        for (int i = 0; i < 2; i++) {
            cout << "Podaj nazwe gracza " << i + 1 << ": ";
            cin >> nazwa[i];
        }
        system("cls");

        while (petla) { // Glowna petla
            poprawny = false;

            time(&czas1);

            interfejs(arr, nazwa, gracz, kolej, zajete, czas[kolej]);
            cout << "Wybierz opcje lub cwiartke do ruchu: ";
            cin >> opcja;

            if (opcja == 'o') { //Wczytanie przykladowej planszy
                hist_licznik = 16; //Ilosc ruchow w domysle wykonanych na planszy przykladowej
                for (int i = 0; i < cwiartki; i++) {
                    for (int j = 0; j < wymiar / 2; j++) {
                        for (int k = 0; k < wymiar / 2; k++) {
                            arr[i][j][k] = arrExample[i][j][k];
                            for (int l = 0; l < hist_licznik; l++) {
                                historia[l][i][j][k] = ' '; //Nieznane ruchy przed wczytaniem planszy przykladowej
                                Hist_ruch[l].histCwiartka = '-';
                                Hist_ruch[l].histPole = '-';
                                Hist_ruch[l].histCwObr = '-';
                                Hist_ruch[l].histObr = '-';
                            }
                            if (hist_licznik < 37) {
                                Hist_ruch[hist_licznik].histCwiartka = 'o';
                                Hist_ruch[hist_licznik].histPole = '-';
                                Hist_ruch[hist_licznik].histCwObr = '-';
                                Hist_ruch[hist_licznik].histObr = '-';
                                historia[hist_licznik][i][j][k] = arr[i][j][k]; //Zapisanie ruchu w historii
                            }
                        }
                    }
                }
                kolej = 0;
                gracz[0] = 'C';
                gracz[1] = 'N';
                poprawny = true;
            }

            if (opcja == 'p') {
                time(&czasPauza1);
                opcja = ' ';
                while (opcja != 'p') {
                    system("cls");
                    cout << "====================== PAUZA =======================" << endl;
                    cout << "p - powrot do gry           Ruch gracza z zetonem: " << gracz[kolej] << endl;
                    cin >> opcja;
                }
                time(&czasPauza2);
                czasPauza = difftime(czasPauza2, czasPauza1);
                poprawny = true;
            }

            if (opcja == 'h') {
                opcja = ' ';
                while (opcja != '0') {
                    system("cls");
                    cout << "Gra OX + Pentago" << endl;
                    cout << "Autor: Maciej Heimowski" << endl;
                    cout << "0 - powrot" << endl;
                    cin >> opcja;
                    odstep_x();
                    cout << " q | w      7 8 9" << endl;
                    odstep_x();
                    cout << " -----      4 5 6" << endl;
                    odstep_x();
                    cout << " a | s      1 2 3" << endl;
                }
            }

            if (opcja == 'm') { //Opcje
                zetonStary1 = gracz[0];
                zetonStary2 = gracz[1];

                menu(gracz[0], gracz[1], gra, historia, hist_licznik, Hist_ruch);
                for (int i = 0; i < cwiartki; i++) { //Zamiana zetonow na planszy
                    for (int j = 0; j < wymiar / 2; j++) {
                        for (int k = 0; k < wymiar / 2; k++) {
                            if (arr[i][j][k] == zetonStary1)
                                arr[i][j][k] = gracz[0];
                            if (arr[i][j][k] == zetonStary2)
                                arr[i][j][k] = gracz[1];
                            for (int l = 0; l < 37; l++) {
                                if (historia[l][i][j][k] == zetonStary1)
                                    historia[l][i][j][k] = gracz[0];
                                if (historia[l][i][j][k] == zetonStary2)
                                    historia[l][i][j][k] = gracz[1];
                            }
                        }
                    }
                }
                interfejs(arr, nazwa, gracz, kolej, zajete, czas[kolej]);
                poprawny = true;
            }

            if (opcja == 'u') { //Cofanie ruchu
                if (hist_licznik > 0) {
                    hist_licznik--;
                    for (int i = 0; i < cwiartki; i++) {
                        for (int j = 0; j < wymiar / 2; j++) {
                            for (int k = 0; k < wymiar / 2; k++) {
                                arr[i][j][k] = historia[hist_licznik][i][j][k]; //Przywracanie planszy z historii
                            }
                        }
                    }

                }
                else {
                    kolej = 1;
                }
                system("cls");
                interfejs(arr, nazwa, gracz, kolej, zajete, czas[kolej]);
                opcja = ' ';
                poprawny = true;
            }

            if (opcja == 'q' || opcja == 'w' || opcja == 'a' || opcja == 's') { //Wykonywanie ruchu
                Hist_ruch[hist_licznik + 1].histCwiartka = opcja;
                if (opcja == 'q')
                    cwiartka = 0;
                else if (opcja == 'w')
                    cwiartka = 1;
                else if (opcja == 'a')
                    cwiartka = 2;
                else if (opcja == 's')
                    cwiartka = 3;
                odstep_x();
                cout << "Wybierz pole: ";
                do {
                    if (blad) { //Wykrywanie blednego numeru pola
                        system("cls");
                        odstep_x();
                        interfejs(arr, nazwa, gracz, kolej, zajete, czas[kolej]);
                        odstep_x();
                        cout << "Wybierz pole: ";
                        blad = false;
                    }
                    cin >> opcja;
                    Hist_ruch[hist_licznik + 1].histPole = opcja;
                    pole = opcja - '0';
                    if (pole < 1 || pole > 9) {
                        blad = true;
                    }

                    if (pole > 0 && pole < 10 && cwiartka >= 0 && cwiartka < 4 && arr[cwiartka][2 - ((pole - 1) / 3)][(pole - 1) % 3] == ' ') { //ZAPISANIE POLA
                        arr[cwiartka][2 - ((pole - 1) / 3)][(pole - 1) % 3] = gracz[kolej];
                        hist_licznik++;
                        for (int i = 0; i < cwiartki; i++) {
                            for (int j = 0; j < wymiar / 2; j++) {
                                for (int k = 0; k < wymiar / 2; k++) {
                                    if (hist_licznik < 37)
                                        historia[hist_licznik][i][j][k] = arr[i][j][k]; //Zapisanie do historii ruchow
                                }
                            }
                        }
                    }

                    else
                        zajete = true;
                } while (blad);

                Hist_ruch[hist_licznik].histCwObr = '-';
                Hist_ruch[hist_licznik].histObr = '-';

                if (gra == 'p' && !zajete) { //Obrot cwiartki tylko wtedy, jesli wybrano gre pentago
                    odstep_x();
                    cout << "Wybierz cwiartke do obrocenia: ";
                    cin >> opcja;
                    Hist_ruch[hist_licznik].histCwObr = opcja;
                    if (opcja == 'q' || opcja == 'w' || opcja == 'a' || opcja == 's') {
                        if (opcja == 'q')
                            cwiartka = 0;
                        else if (opcja == 'w')
                            cwiartka = 1;
                        else if (opcja == 'a')
                            cwiartka = 2;
                        else if (opcja == 's')
                            cwiartka = 3;
                    }
                    else
                        cwiartka = 0; //Opcja domyslna w wypadku zlego wejscia

                    odstep_x();
                    cout << "Wybierz kierunek obrotu: ";
                    cin >> opcja;
                    Hist_ruch[hist_licznik].histObr = opcja;
                    if (opcja == 'z') {
                        rotateL(arr[cwiartka]);
                    }
                    else if (opcja == 'x') {
                        rotateR(arr[cwiartka]);
                    }
                    else {
                        rotateR(arr[cwiartka]); //Opcja domyslna w wypadku zlego wejscia
                    }
                    for (int i = 0; i < cwiartki; i++) {
                        for (int j = 0; j < wymiar / 2; j++) {
                            for (int k = 0; k < wymiar / 2; k++) {
                                if (hist_licznik < 37)
                                    historia[hist_licznik][i][j][k] = arr[i][j][k]; //Zapisanie do historii ruchow
                            }
                        }
                    }
                }
                poprawny = true;
            }

            system("cls");

            time(&czas2);
            czas[kolej] += (int)(difftime(czas2, czas1) - czasPauza);
            czasPauza = 0;

            if (!zajete && opcja != 'o' && opcja != 'm' && opcja != 'u' && opcja != 'p' && poprawny) { //Zmiana kolei graczy
                kolej++;
                if (kolej == 2)
                    kolej = 0;
            }

            if (win(arr, gracz[0]) || czas[1] >= limitCzasu) { //Sprawdzanie wygranej
                cout << endl;
                odstep_x();
                cout << "GRACZ " << nazwa[0] << " WYGRAL ";
                if (czas[1] >= limitCzasu)
                    cout << "NA CZAS";
                cout << "!" << endl;
                odstep_x();
                cout << "Nacisnij dowolny klawisz... ";
                cin >> opcja;
                petla = false;
                system("cls");
            }
            else if (win(arr, gracz[1]) || czas[0] >= limitCzasu) {
                cout << endl;
                odstep_x();
                cout << "GRACZ " << nazwa[1] << " WYGRAL ";
                if (czas[0] >= limitCzasu)
                    cout << "NA CZAS";
                cout << "!" << endl;
                odstep_x();
                cout << "Nacisnij dowolny klawisz... ";
                cin >> opcja;
                petla = false;
                system("cls");
            }

            if (hist_licznik >= 36 && !win(arr, gracz[0]) && !win(arr, gracz[1])) { //Sprawdzanie remisu
                cout << endl;
                odstep_x();
                cout << "REMIS!" << endl;
                odstep_x();
                cout << "Nacisnij dowolny klawisz... ";
                cin >> opcja;
                petla = false;
                hist_licznik = 36;
                system("cls");
            }
        }
    }
    return 0;
}