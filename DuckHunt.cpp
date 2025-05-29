#include <stdio.h>
#include <graphics.h>

#define JANELA_X 640
#define JANELA_Y 480

void gerarInterfaceInicial();

int main ()
{
    initwindow(JANELA_X, JANELA_Y, "Duck Hunt", 600, 100);

    gerarInterfaceInicial();

    // Fecha a janela quando uma tecla for pressionada
    getch();
    closegraph();

    return 0;
}

void gerarInterfaceInicial()
{
    int i, x;

    // Timer
    setfillstyle(1, COLOR(20, 20, 20));
    bar(0, 0, JANELA_X, 30);
    setbkcolor(COLOR(20, 20, 20));
    outtextxy(300, 8, "01:00");

    // Alvo para iniciar o jogo
    setbkcolor(BLACK);
    outtextxy(224, 161, "Acerte o alvo para começar");
    readimagefile("Imagens/bullseye.jpg", 291, 190, 291 + 50, 190 + 50);

    // Background
    setfillstyle(1, COLOR(20, 20, 20));
    bar(0, 370, JANELA_X, 370 + 110);

    // Vidas
    for (i = 0; i < 5; i++)
    {
        // O tamanho da imagem é somado a x, posicionando as imagens lado a lado.
        x = 16 + (i * 32);
        readimagefile("Imagens/heart.jpg", x, 393, x + 32, 393 + 32);
    }

    // Munição
    for (i = 0; i < 5; i++)
    {
        // O tamanho da imagem é somado a x, posicionando as imagens lado a lado.
        x = 16 + (i * 32);
        readimagefile("Imagens/bullet.jpg", x, 430, x + 32, 430 + 32);
    }

    // Acertos
    setbkcolor(COLOR(20, 20, 20));
    outtextxy(552, 447, "Acertos: 0");
}
