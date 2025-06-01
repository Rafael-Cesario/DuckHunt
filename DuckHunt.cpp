#include <stdio.h>
#include <graphics.h>
#include<windows.h>

#define JANELA_X 640
#define JANELA_Y 480

void gerarInterfaceInicial();
void tamparAlvo();
void exibirVidas(int vidas);
void exibirMunicao(int municao);
void exibirAcertos(int acertos);

void timer();

int main ()
{
    initwindow(JANELA_X, JANELA_Y, "Duck Hunt", 600, 100);

    gerarInterfaceInicial();
    // tamparAlvo();
    // iniciarJogo();

    tamparAlvo();
    timer();

    // Fecha a janela quando uma tecla for pressionada
    getch();
    closegraph();

    return 0;
}

void gerarInterfaceInicial()
{
    // Timer
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 0, JANELA_X, 30);
    setbkcolor(COLOR(20,20,20));
    outtextxy(300, 8, "01:00");

    // Alvo para iniciar o jogo
    setbkcolor(BLACK);
    outtextxy(224, 161, "Acerte o alvo para come�ar");
    readimagefile("Imagens/bullseye.jpg", 291, 190, 291 + 50, 190 + 50);

    // Background na parte inferior
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 370, JANELA_X, 370 + 110);

    exibirVidas(5);
    exibirMunicao(5);
    exibirAcertos(0);
}


void tamparAlvo()
{
    int x,y;

    //esconde o alvo

    while(1)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {


            getmouseclick(WM_LBUTTONDOWN,x,y);

            if(x>=291&& x<=341 && y>=190 && y<=240)
            {
                setfillstyle(SOLID_FILL,BLACK);
                bar(224,161,200+224,100+161);

                setbkcolor(BLACK);
                outtextxy(250,161,"O jogo vai come�ar");

                delay(2000);

                //apaga a mensagem "o jogo vai come�ar"

                bar(250,161,250+150,161+150);
            }

            break;

        }
    }

}

void exibirVidas(int vidas)
{
    int i, x;

    for (i = 0; i < vidas; i++)
    {
        x = 16 + (i * 32);
        readimagefile("Imagens/heart.jpg", x, 393, x + 32, 393 + 32);
    }
}

void exibirMunicao(int municao)
{
    int i, x;

    for (i = 0; i < municao; i++)
    {
        x = 16 + (i * 32);
        readimagefile("Imagens/bullet.jpg", x, 430, x + 32, 430 + 32);
    }
}

void exibirAcertos(int acertos)
{
    char text[11];

    sprintf(text, "Acertos: %d", acertos);
    setbkcolor(COLOR(20, 20, 20));
    outtextxy(552, 447, text);
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
