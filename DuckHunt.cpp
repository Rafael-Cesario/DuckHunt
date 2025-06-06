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
void recarregarMunicao(int *jogoMunicao, Elemento *municao);
void jogo(int *pontuacao);
void iniciarJogo(int *pontuacao);

int main ()
{
    int pontuacao[5] = {0, 0, 0, 0, 0};

    initwindow(JANELA_X, JANELA_Y, "Duck Hunt", 100, 100);

    gerarInterfaceInicial();
    aguardarInicio();
    iniciarJogo(pontuacao);
    // telaFinal(&pontuacao);

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
    outtextxy(224, 161, "Acerte o alvo para começar");
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

void iniciarJogo(int *pontuacao)
{
    int partidasTotal = 5, i;

    for (i = 0; i < partidasTotal; i++)
        jogo(&pontuacao[i]);

    for (i = 0; i < 5; i++)
        printf("Pontuacao %d: %d\n", i, pontuacao[i]);
}

void jogo(int *pontuacao)
{
    int clickX, clickY, tempoAlvo, totalAlvos = 0;

    Estado jogo = { 5, 5, 0 };

    Elemento alvo = {"Imagens/bullseye.jpg", 0, 0};
    Elemento vida = {"Imagens/heart.jpg", 16, 393};
    Elemento municao = {"Imagens/bullet.jpg", 16, 430};

    // Reseta a interface antes de cada partida
    exibir(vida, jogo.vidas);
    exibir(municao, jogo.municao);
    exibirPontos(jogo.pontos);

    srand(time(NULL));

    // Tarefa:
    // Exibir uma contagem regressiva na tela antes dos alvos começarem a aparecer
    // contagemRegressiva();
    limparAlvos();
    delay(3000);

    // O jogo termina depois que 10 alvos aparecerem na tela
    for(totalAlvos; totalAlvos < 10; totalAlvos++)
    {
        if (jogo.vidas == 0)
        {
            limparAlvos();
            break;
        };

        if (jogo.municao == 0)
            recarregarMunicao(&jogo.municao, &municao);

        // Gera um x e y, aleatório, considerando o limite da área e o tamanho do alvo
        alvo.x = rand() % (640 - 50);
        alvo.y = rand() % (370 - 50 - 30) + 30;

        // Limpa o alvo anterior, e posiciona um novo
        limparAlvos();
        readimagefile(alvo.caminho, alvo.x, alvo.y, alvo.x + 50, alvo.y + 50);

        // Tempo para o jogador tentar acertar o alvo: 0.5 segundos;
        tempoAlvo = 50;

        for(tempoAlvo; tempoAlvo >= 0; tempoAlvo--)
        {
            if (tempoAlvo == 0)
            {
                // Perde uma vida por não acertar o alvo a tempo
                jogo.vidas--;
                exibir(vida, jogo.vidas);
                break;
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
                    jogo.pontos++;
                    exibirPontos(jogo.pontos);
                    break;
                }
                else
                {
                    // Perde uma vida por errar
                    jogo.vidas--;
                    exibir(vida, jogo.vidas);
                    break;
                }
            }

            // Delay par diminuir o timer
            delay(10);
        }
    }

    *pontuacao = jogo.pontos;
}

void telaFinal()
{
    // Tarefa:
    // Tela Final

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

    // Limpa as posições antes de exibir os elementos
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

void recarregarMunicao(int *jogoMunicao, Elemento *municao)
{
    int i;
    limparAlvos();

    // Tarefa:
    // Escrever na tela recarregando...

    // Animação que aumenta a munição na tela
    for (i = 0; i <= 5; i++)
    {
        exibir(*municao, i);
        delay(200);
    }

    // Variavel para monitorar a municao durante o jogo
    *jogoMunicao = 5;
}
