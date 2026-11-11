#include "graphics.h"
#include <iostream>
#include <windows.h>
#include <cstdio>

/*
 * LEGENDA HĂRȚII (Semnificația caracterelor):
 * '.' = Spațiu liber / Aer
 * 'n' = Nor pe cer
 * 'b' = Bloc căramidă (solid, distructibil/util)
 * 'S' = Bloc secret / întrebare
 * 't' = Teava verde din mario
 * 'i' = Bloc invizibil (coliziunea invizibila)
 * 'B' = Monstrul din mario Bowser
 * 'P' = Prințesa
 * 's' = Steagul de la final
 * 'g' = Inamic Goomba
 * 'p' = Bloc de pământ solid (solul pe care calcă personajele)
 */


using namespace std;

void *imgMario;
void *imgPamant;
void *imgNor;
void *imgCaramida;
void *imgSecret;
void *imgGoomba;
void *imgprintesa;
void *imgbauzar;
void *imgMarioStanga;
void *imgteavaMica;
void *imgsteag;

const int LUNGIME_HARTA = 60;
const int DIMENSIUNE_BLOC = 40;

struct Inamic {
    int x;
    int y;
    int vitezaX;
    bool activ;
};

struct bauzar {
    int x;
    int y;
    bool activ;
    bool nrVieti;
    //de facut
    // sa sara poate, sa arunce flacari etc mai vezi tu
};

Inamic goombas[20];
int nrGoombas = 0;

void incarcaTexturaInRAM(const char *caleFisier, void *&bufferTextura, int w, int h) {
    readimagefile(caleFisier, 0, 0, w, h);
    unsigned int dimensiune = imagesize(0, 0, w, h);
    bufferTextura = malloc(dimensiune);
    getimage(0, 0, w, h, bufferTextura);
}

void pregatesteImaginile() {

    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\Mario.jpg", imgMario, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\blocPamant.jpg", imgPamant, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\nor.jpg", imgNor, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\blocCaramida.jpg", imgCaramida, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\blocSecret.jpg", imgSecret, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\goomba.jpg", imgGoomba, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\printesa.jpg", imgprintesa, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\bauzar.jpg", imgbauzar, 120, 120);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\MarioStanga.jpg", imgMarioStanga, 40, 40);
    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\steag.jpg", imgsteag, 50, 190);


    incarcaTexturaInRAM("C:\\Users\\Alin\\CLionProjects\\WinBGIm-64\\cmake-build-debug\\teavaMica.jpg", imgteavaMica, 80, 100);

    cleardevice();

}



const char cer[3][LUNGIME_HARTA + 1] = {
    "............................................................",
    ".......n.........................n..........................",
    "......nnn.......................nnn........................."
};

char mijloc[5][LUNGIME_HARTA + 1] = {
    "............................................................",
    ".........................................................s..",
    "............S..bbb.................................Bii......",
    "......................bb............ti.............iii......",
    "................g...................ii........g.P..iii.....b"
};

const char jos[2][LUNGIME_HARTA + 1] = {
    "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp",
    "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"
};

void initializeazaInamici() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            if (mijloc[i][j] == 'g') {
                goombas[nrGoombas].x = j * DIMENSIUNE_BLOC;
                goombas[nrGoombas].y = (i + 3) * DIMENSIUNE_BLOC;
                goombas[nrGoombas].vitezaX = -3;
                goombas[nrGoombas].activ = true;
                nrGoombas++;
                mijloc[i][j] = '.';
            }
        }
    }
}

char getTile(int pixX, int pixY) {
    int c = pixX / DIMENSIUNE_BLOC;
    int r = pixY / DIMENSIUNE_BLOC;

    if (c < 0 || c >= LUNGIME_HARTA || r < 0 || r >= 10) return ' ';

    if (r < 3) return cer[r][c];
    if (r < 8) return mijloc[r - 3][c];
    return jos[r - 8][c];
}

bool esteSolid(char bloc) {
    return (bloc == 'b' || bloc == 'S' || bloc == 't' || bloc == 'p' || bloc == 'i' || bloc == 'B');
}

bool seCiocneste(int x, int y) {
    char stangaSus = getTile(x + 1, y + 1);
    char dreaptaSus = getTile(x + 38, y + 1);
    char stangaJos = getTile(x + 1, y + 39);
    char dreaptaJos = getTile(x + 38, y + 39);
    return esteSolid(stangaSus) || esteSolid(dreaptaSus) || esteSolid(stangaJos) || esteSolid(dreaptaJos);
}

void deseneazaForma(char tip, int x, int y) {
    if (tip == '.' || tip == 'i') return;
    if (tip == 'p') putimage(x, y, imgPamant, COPY_PUT);
    else if (tip == 'B') putimage(x, y, imgbauzar, COPY_PUT);
    else if (tip == 'P') putimage(x, y, imgprintesa, COPY_PUT);
    else if (tip == 'n') putimage(x, y, imgNor, COPY_PUT);
    else if (tip == 'b') putimage(x, y, imgCaramida, COPY_PUT);
    else if (tip == 'S') putimage(x, y, imgSecret, COPY_PUT);
    else if (tip == 't') putimage(x, y, imgteavaMica, COPY_PUT);
    else if (tip == 's') putimage(x, y, imgsteag, COPY_PUT);
}

void deseneazaEcranul(int cameraPixX) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = i * DIMENSIUNE_BLOC;
            deseneazaForma(cer[i][j], pixX, pixY);
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = (i + 3) * DIMENSIUNE_BLOC;
            deseneazaForma(mijloc[i][j], pixX, pixY);
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = (i + 8) * DIMENSIUNE_BLOC;
            deseneazaForma(jos[i][j], pixX, pixY);
        }
    }
}

int main() {
    initwindow(800, 400, "Mario - Joc Complet", -1, -1, true);

    pregatesteImaginile();
    initializeazaInamici();

    int scor = 0;
    int vieti = 3;
    int nivel = 0;
    int subNivel = 1;

    int startX = 100, startY = 280;
    int marioX = startX;
    int marioY = startY;

    int vitezaY = 0;
    int gravitatie = 2;
    bool inSaritura = false;
    int vitezaMiscare = 10;
    bool mergStanga = false;

    bool inJoc = true;

    while (inJoc) {
        int viitorX = marioX;
        if (GetAsyncKeyState('D') & 0x8000) {
            viitorX += vitezaMiscare;
            mergStanga = false;
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            viitorX -= vitezaMiscare;
            mergStanga = true;
        }

        if (viitorX < 0) viitorX = 0;
        int capatulHartii = (LUNGIME_HARTA * DIMENSIUNE_BLOC) - DIMENSIUNE_BLOC;
        if (viitorX > capatulHartii) viitorX = capatulHartii;

        if (!seCiocneste(viitorX, marioY)) {
            marioX = viitorX;
        }

        if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
            if (!inSaritura && seCiocneste(marioX, marioY + 1)) {
                vitezaY = -25;
                inSaritura = true;
                Beep(600, 10);
            }
        }
        if (GetAsyncKeyState('X') & 0x8000) inJoc = false;

        int viitorY = marioY + vitezaY;
        vitezaY += gravitatie;

        if (!seCiocneste(marioX, viitorY)) {
            marioY = viitorY;
            inSaritura = true;
        } else {
            if (vitezaY > 0) {
                inSaritura = false;
                marioY = (viitorY / DIMENSIUNE_BLOC) * DIMENSIUNE_BLOC;
            }
            vitezaY = 0;
        }

        // Coliziuni Goomba si logica de moarte
        for (int i = 0; i < nrGoombas; i++) {
            if (goombas[i].activ) {
                int gViitorX = goombas[i].x + goombas[i].vitezaX;
                if (!seCiocneste(gViitorX, goombas[i].y)) {
                    goombas[i].x = gViitorX;
                } else {
                    goombas[i].vitezaX *= -1;
                }

                bool intersectieX = (marioX + DIMENSIUNE_BLOC - 5 > goombas[i].x) && (marioX + 5 < goombas[i].x + DIMENSIUNE_BLOC);
                bool intersectieY = (marioY + DIMENSIUNE_BLOC > goombas[i].y) && (marioY < goombas[i].y + DIMENSIUNE_BLOC);

                if (intersectieX && intersectieY) {
                    if (vitezaY > 0 && (marioY + DIMENSIUNE_BLOC - vitezaY) <= goombas[i].y + 15) {
                        goombas[i].activ = false;
                        vitezaY = -15;
                        scor += 100;
                        Beep(800, 50);
                    } else {
                        vieti--;
                        Beep(200, 300);
                        setbkcolor(BLACK);
                        cleardevice();
                        setcolor(RED);
                        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 5);

                        if (vieti <= 0) {
                            outtextxy(230, 170, (char *) "GAME OVER");
                            swapbuffers();
                            delay(3000);
                            inJoc = false;
                        } else {
                            outtextxy(260, 170, (char *) "AI MURIT!");
                            swapbuffers();
                            delay(2000);

                            marioX = startX;
                            marioY = startY;
                            vitezaY = 0;
                            while (kbhit()) getch();
                        }
                    }
                }
            }
        }

        if (!inJoc) break;

        int cameraPixX = marioX - 400;
        if (cameraPixX < 0) cameraPixX = 0;
        if (cameraPixX > (LUNGIME_HARTA * DIMENSIUNE_BLOC) - 800) cameraPixX = (LUNGIME_HARTA * DIMENSIUNE_BLOC) - 800;

        setbkcolor(COLOR(135, 206, 235));
        cleardevice();
        deseneazaEcranul(cameraPixX);

        for (int i = 0; i < nrGoombas; i++) {
            if (goombas[i].activ) {
                putimage(goombas[i].x - cameraPixX, goombas[i].y, imgGoomba, COPY_PUT);
            }
        }

        int marioEcranX = marioX - cameraPixX;
        if (mergStanga) putimage(marioEcranX, marioY, imgMarioStanga, COPY_PUT);
        else putimage(marioEcranX, marioY, imgMario, COPY_PUT);

        char textHUD[100];
        sprintf(textHUD, "NIVEL: %d-%d     SCOR: %d     VIETI: %d", nivel, subNivel, scor, vieti);
        setcolor(BLACK);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
        setbkcolor(COLOR(135, 206, 235));
        outtextxy(20, 20, textHUD);

        swapbuffers();
        delay(30);
    }

    closegraph();
    return 0;
}