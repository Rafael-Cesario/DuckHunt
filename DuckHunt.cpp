#include <stdio.h>
#include <graphics.h>
#include<windows.h>

#define JANELA_X 640
#define JANELA_Y 480

typedef struct
{
    char caminho[22];
    int x, y;
} Elemento;

void gerarInterfaceInicial();
void timer();
void exibirAcertos(int acertos);
void exibir(Elemento item, int quantidade);
void aguardarInicio();

int main ()
{
    initwindow(JANELA_X, JANELA_Y, "Duck Hunt", 100, 100);

    gerarInterfaceInicial();
    aguardarInicio();

    getch();
    closegraph();

    return 0;
}

void aguardarInicio()
{
    int clickX, clickY;

    while (1)
    {
        // Mantem o jogo parado, esperando por um clique
        while(!ismouseclick(WM_LBUTTONDOWN)) delay(10);

        // Salva a posição do clique
        getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
        clearmouseclick(WM_LBUTTONDOWN);

        // Verifica se o clique foi dentro do alvo
        if (clickX >= 291 && clickX <= 341 && clickY >= 190 && clickY <= 240)
        {
            // Tampa a mensagem inicial e o alvo
            setfillstyle(1, BLACK);
            bar(224, 161, 224 + 180, 161 + 100);
            break;
        }
    }
}

void gerarInterfaceInicial()
{
    Elemento vida = {"Imagens/heart.jpg", 16, 393};
    Elemento municao = {"Imagens/bullet.jpg", 16, 430};

    // Timer
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 0, JANELA_X, 30);
    setbkcolor(COLOR(20,20,20));
    outtextxy(300, 8, "01:00");

    // Alvo para iniciar o jogo
    setbkcolor(BLACK);
    outtextxy(224, 161, "Acerte o alvo para começar");
    readimagefile("Imagens/bullseye.jpg", 291, 190, 291 + 50, 190 + 50);

    // Background na parte inferior
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 370, JANELA_X, 370 + 110);

    exibir(vida, 5);
    exibir(municao, 5);
    exibirAcertos(0);
}

void timer()
{

    //faz o timer ir a 0

    char tempo[10];
    int i;

    for(i=59; i>=0; i--)
    {

        setbkcolor(COLOR(20,20,20));
        sprintf(tempo,"00:%02d",i);

        outtextxy(300,8,tempo);

        delay(1000);
    }

    setbkcolor(BLACK);
    outtextxy(280,161,"Fim de jogo");
}

void exibir(Elemento item, int quantidade)
{
    int x, i = 0;
    for (i = 0; i < quantidade; i++)
    {
        x = item.x + (i * 32);
        readimagefile(item.caminho, x, item.y, x + 32, item.y + 32);
    }
}

void exibirAcertos(int acertos)
{
    char text[11];
    sprintf(text, "Acertos: %d", acertos);
    setbkcolor(COLOR(20, 20, 20));
    outtextxy(552, 447, text);
}
