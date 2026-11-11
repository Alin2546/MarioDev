#include "graphics.h"
#include <iostream>
#include <windows.h>

using namespace std;

const int LUNGIME_HARTA = 60;
const int DIMENSIUNE_BLOC = 40;


const char cer[3][LUNGIME_HARTA + 1] = {
    "............................................................",
    ".......n.........................n..........................",
    "......nnn.......................nnn........................."
};

const char mijloc[5][LUNGIME_HARTA + 1] = {
    "...........................................................s",
    "...........................................................s",
    "............S..b.b.........................................s",
    ".....................................t.....................s",
    ".........g...........................t........g............s"
};

const char jos[2][LUNGIME_HARTA + 1] = {
    "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp",
    "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"
};

void deseneazaForma(char tip, int x, int y) {
    if (tip == '.') return;

    if (tip == 'p') {
        setfillstyle(SOLID_FILL, BROWN);
        bar(x, y, x + DIMENSIUNE_BLOC, y + DIMENSIUNE_BLOC);
        setcolor(GREEN);
        line(x, y, x + DIMENSIUNE_BLOC, y);
    }

    //nori
    else if (tip == 'n') {
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        fillellipse(x + DIMENSIUNE_BLOC/2, y + DIMENSIUNE_BLOC/2, DIMENSIUNE_BLOC/3, DIMENSIUNE_BLOC/2);
    }
//blockuri
    else if (tip == 'b') {
        setfillstyle(SOLID_FILL, RED);
        bar(x, y, x + DIMENSIUNE_BLOC, y + DIMENSIUNE_BLOC);
        setcolor(BLACK);
        rectangle(x, y, x + DIMENSIUNE_BLOC, y + DIMENSIUNE_BLOC);
    }

    //block secret
    else if (tip == 'S') {
        setfillstyle(SOLID_FILL, YELLOW);
        bar(x, y, x + DIMENSIUNE_BLOC, y + DIMENSIUNE_BLOC);
    }

    //teava
    else if (tip == 't') {
        setfillstyle(SOLID_FILL, GREEN);
        bar(x, y, x + DIMENSIUNE_BLOC, y + DIMENSIUNE_BLOC);
    }

//goompa
    else if (tip == 'g') {
        setfillstyle(SOLID_FILL, MAGENTA);
        fillellipse(x + DIMENSIUNE_BLOC/2, y + DIMENSIUNE_BLOC/2, DIMENSIUNE_BLOC/2 - 5, DIMENSIUNE_BLOC/2 - 5);
    }

//steag
    else if (tip == 's') {
        setfillstyle(SOLID_FILL, WHITE);
        bar(x + DIMENSIUNE_BLOC/2 - 2, y, x + DIMENSIUNE_BLOC/2 + 2, y + DIMENSIUNE_BLOC);
    }
}


void deseneazaEcranul(int cameraPixX) {
    //  Cerul
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = i * DIMENSIUNE_BLOC;
            deseneazaForma(cer[i][j], pixX, pixY);
        }
    }

    //  Mijlocul
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = (i + 3) * DIMENSIUNE_BLOC;
            deseneazaForma(mijloc[i][j], pixX, pixY);
        }
    }

    // Jos
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < LUNGIME_HARTA; j++) {
            int pixX = (j * DIMENSIUNE_BLOC) - cameraPixX;
            int pixY = (i + 8) * DIMENSIUNE_BLOC;
            deseneazaForma(jos[i][j], pixX, pixY);
        }
    }
}

int main() {

    initwindow(800, 400, "Mario - Miscare si Saritura", -1, -1, true);

    int marioX = 100;
    int marioY = 280;

    int vitezaY = 0;
    int gravitatie = 2;
    bool inSaritura = false;
    int vitezaMiscare = 10;

    bool inJoc = true;

    while (inJoc) {


        if (GetAsyncKeyState('D') & 0x8000) {
            marioX += vitezaMiscare;
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            marioX -= vitezaMiscare;
        }

        if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000)) {

            if (!inSaritura) {
                vitezaY = -25;
                inSaritura = true;
            }
        }

        if (GetAsyncKeyState('X') & 0x8000) {
            inJoc = false;
        }

        // fizica
        marioY += vitezaY;
        vitezaY += gravitatie;

        // Podeaua y=280
        if (marioY >= 280) {
            marioY = 280;       // Nu-l lasam sa cada sub pamant
            vitezaY = 0;        // Se opreste din cazut
            inSaritura = false; // A atins pamantul, deci are voie sa sara iar
        }


        if (marioX < 0) marioX = 0;
        int capatulHartii = LUNGIME_HARTA * DIMENSIUNE_BLOC - DIMENSIUNE_BLOC;
        if (marioX > capatulHartii) marioX = capatulHartii;



        // centrare camera pe mario
        int cameraPixX = marioX - 400;
        if (cameraPixX < 0) cameraPixX = 0; // Camera nu are voie sa se duca mai in stanga decat 0


        // grafica
        setbkcolor(COLOR(100, 150, 235));
        cleardevice();

        deseneazaEcranul(cameraPixX);

        int marioEcranX = marioX - cameraPixX;



        setfillstyle(SOLID_FILL, BLUE);
        bar(marioEcranX, marioY, marioEcranX + DIMENSIUNE_BLOC, marioY + DIMENSIUNE_BLOC);

        setcolor(WHITE);
        rectangle(marioEcranX, marioY, marioEcranX + DIMENSIUNE_BLOC, marioY + DIMENSIUNE_BLOC);

        swapbuffers();

//set custom fps: 40 - 24fps  30 - 30fps   15 - 60fps
        delay(40);
    }

    closegraph();
    return 0;
}