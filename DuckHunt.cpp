#include <graphics.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define JANELA_X 640
#define JANELA_Y 480

typedef struct
{
    char caminho[22];
    int x, y;
} Elemento;

typedef struct
{
    int vidas, municao, pontos;
} Estado;

void exibir(Elemento item, int quantidade);
void exibirPontos(int pontos);
void atualizarTimer(int segundos);
void gerarInterfaceInicial();
void aguardarInicio();
void limparAlvos();
void recarregarMunicao(int *jogoMunicao, Elemento municao);
void jogo(int *pontuacao, Elemento alvo, Elemento vida, Elemento municao);
void iniciarJogo(int *pontuacao);
void telaFinal(int pontuacao[5]);

int main ()
{
    int pontuacao[5] = {0, 0, 0, 0, 0};

    initwindow(JANELA_X, JANELA_Y, "Duck Hunt", 100, 100);

    gerarInterfaceInicial();
    aguardarInicio();
    iniciarJogo(pontuacao);
    telaFinal(pontuacao);

    getch();
    closegraph();

    return 0;
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
    outtextxy(224, 161, "Acerte o alvo para come�ar");
    readimagefile("Imagens/bullseye.jpg", 291, 190, 291 + 50, 190 + 50);

    // Background na parte inferior
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 370, JANELA_X, 370 + 110);

    exibir(vida, 5);
    exibir(municao, 5);
    exibirPontos(0);
}

void aguardarInicio()
{
    int clickX, clickY;

    while (1)
    {
        // Mantem o jogo parado, esperando por um clique
        while(!ismouseclick(WM_LBUTTONDOWN)) delay(10);

        // Salva a posi��o do clique
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

void iniciarJogo(int *pontuacao)
{
    int partidasTotal = 5, i;

    Elemento alvo = {"Imagens/bullseye.jpg", 0, 0};
    Elemento vida = {"Imagens/heart.jpg", 16, 393};
    Elemento municao = {"Imagens/bullet.jpg", 16, 430};

    srand(time(NULL));

    for (i = 0; i < partidasTotal; i++)
    {
        // Reseta a interface antes de cada partida
        exibir(vida, 5);
        exibir(municao, 5);
        exibirPontos(0);

        // Tarefa:
        // Exibir uma contagem regressiva na tela antes dos alvos come�arem a aparecer
        // contagemRegressiva();
        limparAlvos();

        // Delay tempor�rio, para simular a contagem regressiva
        delay(3000);

        jogo(&pontuacao[i], alvo, vida, municao);
    }
}

void jogo(int *pontuacao, Elemento alvo, Elemento vida, Elemento municao)
{
    int clickX, clickY, tempoAlvo, totalAlvos = 0, acertou;

    Estado jogo = { 5, 5, 0 };

    // O jogo termina depois que 10 alvos aparecerem na tela
    for(totalAlvos; totalAlvos < 10; totalAlvos++)
    {
        if (jogo.vidas == 0) break;

        // Gera um x e y, aleat�rio, considerando o limite da �rea e o tamanho do alvo
        alvo.x = rand() % (640 - 50);
        alvo.y = rand() % (370 - 50 - 30) + 30;

        // Limpa o alvo anterior, e posiciona um novo
        limparAlvos();
        readimagefile(alvo.caminho, alvo.x, alvo.y, alvo.x + 50, alvo.y + 50);

        acertou = 0;

        // Tempo para o jogador tentar acertar o alvo: 0.5 segundos;
        for(tempoAlvo = 50; tempoAlvo >= 0; tempoAlvo--)
        {
            if (jogo.municao == 0) recarregarMunicao(&jogo.municao, municao);

            // Perde uma vida por n�o acertar o alvo a tempo
            if (tempoAlvo == 0 && acertou == 0)
            {
                jogo.vidas--;
                exibir(vida, jogo.vidas);

                if (jogo.vidas == 0) break;
            };

            if (ismouseclick(WM_LBUTTONDOWN))
            {
                getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
                clearmouseclick(WM_LBUTTONDOWN);

                jogo.municao--;
                exibir(municao, jogo.municao);

                // Verifica se o clique foi dentro do alvo
                if (clickX >= alvo.x && clickY >= alvo.y && clickX <= alvo.x + 50 && clickY <= alvo.y + 50)
                {
                    // Ganha um ponto por acertar
                    acertou = 1;
                    jogo.pontos++;
                    limparAlvos();
                    exibirPontos(jogo.pontos);
                }
                else
                {
                    // Perde uma vida por errar
                    jogo.vidas--;
                    exibir(vida, jogo.vidas);

                    if (jogo.vidas == 0) break;
                }
            }

            // Delay par diminuir o timer
            delay(10);
        }
    }

    *pontuacao = jogo.pontos;
}

void telaFinal(int pontuacao[5])
{
    // Tarefa:
    // Tela Final

    for (int i = 0; i < 5; i++)
        printf("Pontuacao %d: %d\n", i, pontuacao[i]);

    clearviewport();
    setbkcolor(BLACK);
    setfillstyle(1, WHITE);
    outtextxy(10, 10, "O jogo terminou");
}

void atualizarTimer(int segundos)
{
    char tempo[6];
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 0, JANELA_X, 30);
    setbkcolor(COLOR(20,20,20));
    sprintf(tempo,"00:%02d", segundos);
    outtextxy(300, 8, tempo);
}

void exibir(Elemento item, int quantidade)
{
    int x, i = 0;

    // Limpa as posi��es antes de exibir os elementos
    setfillstyle(1, COLOR(20, 20, 20));
    bar(item.x, item.y, item.x + 160, item.y + 32);

    for (i = 0; i < quantidade; i++)
    {
        x = item.x + (i * 32);
        readimagefile(item.caminho, x, item.y, x + 32, item.y + 32);
    }
}

void exibirPontos(int pontos)
{
    char text[11];

    sprintf(text, "Acertos: %02d", pontos);
    setbkcolor(COLOR(20, 20, 20));
    outtextxy(552, 447, text);
}

void limparAlvos()
{
    setfillstyle(1, BLACK);
    bar(0, 30, 640, 370);
}

void recarregarMunicao(int *jogoMunicao, Elemento municao)
{
    int i;
    limparAlvos();

    // Tarefa:
    // Escrever na tela recarregando...

    // Anima��o que aumenta a muni��o na tela
    for (i = 0; i <= 5; i++)
    {
        exibir(municao, i);
        delay(200);
    }

    // Variavel para monitorar a municao durante o jogo
    *jogoMunicao = 5;
}
