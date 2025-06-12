#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define JANELA_X 640
#define JANELA_Y 480

typedef struct
{
    int vidas;
    int municao;
    int pontos;
    int acertou;
    int alvosExibidos;
    int alvosPorPartida;
    int tempoLimite;
} Estado;

typedef struct
{
    char caminho[22];
    int x;
    int y;
} Elemento;

void exibir(Elemento item, int quantidade);
void exibirPontos(int pontos);
void gerarInterfaceInicial();
void aguardarInicio();
void limparAlvos();
void recarregarMunicao(int *jogoMunicao, Elemento municao);
void jogo(int *pontuacao, Elemento alvo, Elemento vida, Elemento municao);
void iniciarJogo(int *pontuacao);
void telaFinal(int pontuacao[5]);
void executarRodada(Estado *jogo);
void aumentarPontos(Estado *jogo);
void diminuirVidas(Elemento vida, Estado *jogo);
void calcularTempoPassado(int *tempoLimite, int *tempoInicio);

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
    outtextxy(300, 8, "00:10");

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
    int clickAlvo = 0;

    while (!clickAlvo)
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
    int clickX, clickY, tempoInicio;

    Estado jogo;

    jogo.vidas = 5;
    jogo.municao = 5;
    jogo.pontos = 0;
    jogo.alvosExibidos = 0;
    jogo.alvosPorPartida = 10;

    for(jogo.alvosExibidos; jogo.alvosExibidos < jogo.alvosPorPartida; jogo.alvosExibidos++)
    {
        if (jogo.vidas == 0) break;

        // Gera um x e y aleatório, considerando a área útil e o tamanho do alvo
        alvo.x = rand() % (640 - 50);
        alvo.y = rand() % (370 - 50 - 30) + 30;

        // Limpa o alvo anterior, e posiciona um novo
        limparAlvos();
        readimagefile(alvo.caminho, alvo.x, alvo.y, alvo.x + 50, alvo.y + 50);

        // Para todo novo alvo em tela, o valor de acertou deverá ser falso;
        jogo.acertou = 0;

        // TempoLimite = 0.5 segundos
        jogo.tempoLimite = 50 * 16;
        tempoInicio = clock();

        while(jogo.tempoLimite > 0)
        {
            if (jogo.vidas == 0) break;
            if (jogo.municao == 0) recarregarMunicao(&jogo.municao, municao);

            if (ismouseclick(WM_LBUTTONDOWN))
            {
                jogo.municao--;
                exibir(municao, jogo.municao);

                getmouseclick(WM_LBUTTONDOWN, clickX, clickY);
                clearmouseclick(WM_LBUTTONDOWN);

                // Verifica se o clique foi dentro do alvo
                jogo.acertou = clickX >= alvo.x
                               && clickY >= alvo.y
                               && clickX <= alvo.x + 50
                               && clickY <= alvo.y + 50;

                if (jogo.acertou) aumentarPontos(&jogo);
                else diminuirVidas(vida, &jogo);
            }

            calcularTempoPassado(&jogo.tempoLimite, &tempoInicio);
        }

        // Perde uma vida por não acertar o alvo a tempo
        if (jogo.acertou == 0) diminuirVidas(vida, &jogo);
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

    // Limpa as posições
    setfillstyle(1, COLOR(20, 20, 20));
    bar(item.x, item.y, item.x + 160, item.y + 32);

    // Exibi os elementos
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
    int quantidade, pont, maximo = 5;
    char texto[30];

    limparAlvos();

    // Escrever na tela recarregando...
    for(pont = 0; pont < 3; pont++)
    {
        setbkcolor(BLACK);
        strcpy(texto, "RECARREGANDO");

        for(int j = 0; j <= pont; j++)
        {
            strcat(texto, " . ");
            outtextxy((JANELA_X / 2) - 60, (JANELA_Y / 2) - 50, texto);
            delay(50);
        }
    }

    // Animação que aumenta a munição na tela
    for (quantidade = 0; quantidade <= maximo; quantidade++)
    {
        exibir(municao, quantidade);
        delay(50);
    }

    // Variavel para monitorar a municao durante o jogo
    *jogoMunicao = 5;
}

void aumentarPontos(Estado *jogo)
{
    jogo->pontos++;
    limparAlvos();
    exibirPontos(jogo->pontos);
}

void diminuirVidas(Elemento vida, Estado *jogo)
{
    jogo->vidas--;
    exibir(vida, jogo->vidas);
}

void calcularTempoPassado(int *tempoLimite, int *tempoInicio)
{
    int tempoAtual = clock();
    int tempoPassado = tempoAtual - *tempoInicio;

    *tempoLimite -= tempoPassado;
    *tempoInicio = tempoAtual;
}
