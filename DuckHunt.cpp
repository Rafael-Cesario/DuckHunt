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
    int x;
    int y;
} Elemento;

typedef struct
{
    int vidas;
    int municao;
    int pontos;
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
void executarRodada(Estado *jogo);

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
    int clickX, clickY, x, y, centroX = 315, centroY = 215;
    int esperarClique = 1;

    while (esperarClique)
    {
        // Mantem o jogo parado, esperando por um clique
        while(!ismouseclick(WM_LBUTTONDOWN)) delay(10);

        // Salva a posição do clique
        getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
        clearmouseclick(WM_LBUTTONDOWN);

        // Verifica se o clique foi dentro do alvo
        x = clickX - centroX;
        y = clickY - centroY;

        if( x * x + y * y <= 26 * 26)
        {
            // Remove a mensagem inicial e o alvo
            setfillstyle(1,BLACK);
            bar(224, 161, 224 + 180, 161 + 100);
            setfillstyle(1,BLACK);
            break;
        }
    }
}

void iniciarJogo(int *pontuacao)
{
    int partidasTotal = 5, partidaAtual;

    Elemento alvo = {"Imagens/bullseye.jpg", 0, 0};
    Elemento vida = {"Imagens/heart.jpg", 16, 393};
    Elemento municao = {"Imagens/bullet.jpg", 16, 430};

    srand(time(NULL));

    for (partidaAtual = 0; partidaAtual < partidasTotal; partidaAtual++)
    {
        // Tarefa:
        // Mostrar a partida atual no topo da tela

        // Reseta a interface antes de cada partida
        exibir(vida, 5);
        exibir(municao, 5);
        exibirPontos(0);

        // Tarefa:
        // Exibir uma contagem regressiva na tela antes dos alvos começarem a aparecer
        // contagemRegressiva();
        limparAlvos();
        // Delay temporário, para simular a contagem regressiva
        delay(3000);

        jogo(&pontuacao[partidaAtual], alvo, vida, municao);
    }
}

void jogo(int *pontuacao, Elemento alvo, Elemento vida, Elemento municao)
{
    int tempoAlvo, totalAlvos, acertou;
    int clickX, clickY;

    Estado jogo = { 5, 5, 0 };

    for(totalAlvos = 0; totalAlvos < 10; totalAlvos++)
    {
        if (jogo.vidas == 0) break;

        // Gera um x e y aleatório, considerando a área útil e o tamanho do alvo
        alvo.x = rand() % (640 - 50);
        alvo.y = rand() % (370 - 50 - 30) + 30;

        // Limpa o alvo anterior, e posiciona um novo
        limparAlvos();
        readimagefile(alvo.caminho, alvo.x, alvo.y, alvo.x + 50, alvo.y + 50);

        // Para todo novo alvo em tela, o valor de acertou deverá ser falso;
        acertou = 0;

        // Tempo para o jogador tentar acertar o alvo: 1 segundo
        for(tempoAlvo = 100; tempoAlvo >= 0; tempoAlvo--)
        {
            if (jogo.municao == 0) recarregarMunicao(&jogo.municao, municao);

            // Perde uma vida por não acertar o alvo a tempo
            if (acertou == 0 && tempoAlvo == 0)
            {
                jogo.vidas--;
                exibir(vida, jogo.vidas);

                if (jogo.vidas == 0) break;
            }

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

            // Delay para diminuir o tempoAlvo
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

void recarregarMunicao(int *jogoMunicao, Elemento municao)
{
    int quantidade, pont;

    limparAlvos();

    // Escrever na tela recarregando...
    for(pont = 0; pont < 3; pont++)
    {
        setbkcolor(BLACK);
        char texto[30] = "RECARREGANDO";

        for(int j = 0; j <= pont; j++)
        {
            strcat(texto," . ");
            outtextxy((JANELA_X / 2) - 60, (JANELA_Y / 2) - 50, texto);
            delay(100);
        }
    }

    // Animação que aumenta a munição na tela
    for (quantidade = 0; quantidade <= 5; quantidade++)
    {
        exibir(municao, quantidade);
        delay(100);
    }

    // Variavel para monitorar a municao durante o jogo
    *jogoMunicao = 5;
}
