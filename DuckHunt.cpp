#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define JANELA_X 640
#define JANELA_Y 480
#define LIMITE_AREA_ALVO_X (640 - 50)
#define LIMITE_AREA_ALVO_Y (370 - 50 - 30) + 30

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

typedef struct
{
    char titulo[8];
    int cor;
    int tempoLimite;
} Dificuldade;

typedef struct
{
    char caminho[27];
    int valor;
    int x;
    int y;
} Objeto;

void exibir(Elemento item, int quantidade);
void exibirPontos(int pontos);
void atualizarTimer(int segundos);
void gerarInterfaceInicial();
void aguardarInicio();
void limparAlvos();
void recarregarMunicao(int *jogoMunicao, Elemento municao);
void jogo(int *pontuacao, Dificuldade dificuldade, Elemento vida, Elemento municao);
void iniciarJogo(int *pontuacao);
void telaFinal(int pontuacao[3]);
void executarRodada(Estado *jogo);
void aumentarPontos(Estado *jogo, int valor);
void diminuirVidas(Elemento vida, Estado *jogo);
void reiniciarInterface(Elemento vida, Elemento municao);
void contagemRegressiva(int partidaAtual);
void exibirPartidaAtual(int partidaAtual);

int main ()
{
    int pontuacao[3] = {0, 0, 0};

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

    // Partida Atual
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 0, JANELA_X, 30);
    setbkcolor(COLOR(20,20,20));
    outtextxy(300, 8, "Partida:  ");

    // Alvo para iniciar o jogo
    setbkcolor(BLACK);
    outtextxy(224, 161, "Acerte o alvo para come�ar");
    readimagefile("Imagens/bullseye.jpg", 291, 190, 291 + 50, 190 + 50);

    // Background na parte suerior
    setfillstyle(1, COLOR(20,20,20));
    bar(0, 0, JANELA_X, 30);

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

        // Salva a posi��o do clique
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
    int partidaAtual = 0, partidasTotal = 3;

    srand(time(NULL));

    // Elemento = { Caminho, x, y }
    Elemento vida = {"Imagens/heart.jpg", 16, 393};
    Elemento municao = {"Imagens/bullet.jpg", 16, 430};

    // Dificuldade = { Titulo, Cor, TempoLimite }
    Dificuldade facil = {"F�cil", 2, 100};
    Dificuldade medio = {"M�dio", 14, 80 };
    Dificuldade dificil = {"Dif�cil", 4, 50};

    Dificuldade dificuldades[3] = { facil, medio, dificil };

    for (partidaAtual; partidaAtual < partidasTotal; partidaAtual++)
    {
        reiniciarInterface(vida, municao);

        // contagemRegressiva();
        char texto1[10];
        char tempo[6];

        for(int i = 3; i > 0; i--)
        {
            setbkcolor(BLACK);
            sprintf(texto1, "Partida: %d", partidaAtual +1);
            outtextxy(285, 161, texto1);
            sprintf(tempo,"%d", i);
            outtextxy(308, 190, tempo);

            delay(1000);
        }

        setbkcolor(COLOR(20,20,20));
        bar(300,8,300+65,8+15);

        exibirPartidaAtual(partidaAtual);
        jogo(&pontuacao[partidaAtual], dificuldades[partidaAtual], vida, municao);
    }
}

void jogo(int *pontuacao, Dificuldade dificuldade, Elemento vida, Elemento municao)
{
    int clickX, clickY;
    int alvoSorteado;

    // Objeto = { Caminho[27], valor, x, y }
    Objeto alvo = {"Imagens/bullseye.jpg", 1, 0, 0};
    Objeto alvoDourado = {"Imagens/bullseyeGolden.jpg", 5, 0, 0};
    Objeto alvoAtual;

    Estado jogo;
    jogo.vidas = 5;
    jogo.municao = 5;
    jogo.pontos = 0;
    jogo.alvosExibidos = 0;
    jogo.alvosPorPartida = 10;

    alvoSorteado = rand() % jogo.alvosPorPartida;

    for(jogo.alvosExibidos; jogo.alvosExibidos < jogo.alvosPorPartida; jogo.alvosExibidos++)
    {
        limparAlvos();

        if (jogo.vidas == 0) break;

        if (jogo.alvosExibidos == alvoSorteado) alvoAtual = alvoDourado;
        else alvoAtual = alvo;

        // Para todo novo alvo em tela, o valor de acertou dever� ser falso;
        jogo.acertou = 0;

        // Posiciona o alvo de forma aleat�ria
        alvo.x = rand() % LIMITE_AREA_ALVO_X; // (640 - 50)
        alvo.y = rand() % LIMITE_AREA_ALVO_Y; // (370 - 50 - 30) + 30

        readimagefile(alvoAtual.caminho, alvo.x, alvo.y, alvo.x + 50, alvo.y + 50);

        // Tempo limite de acordo com a dificuldade atual
        jogo.tempoLimite = dificuldade.tempoLimite;

        for(jogo.tempoLimite; jogo.tempoLimite >= 0; jogo.tempoLimite--)
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

                if (jogo.acertou) aumentarPontos(&jogo, alvoAtual.valor);
                else diminuirVidas(vida, &jogo);
            }

            // Perde uma vida por n�o acertar o alvo a tempo
            if (jogo.tempoLimite == 0 && jogo.acertou == 0) diminuirVidas(vida, &jogo);

            // Delay para diminuir o tempo limite
            delay(10);
        }
    }

    *pontuacao = jogo.pontos;
}

void telaFinal(int pontuacao[3])
{
    // Tarefa:
    // Tela Final

    for (int i = 0; i < 3; i++)
        printf("Pontuacao %d: %d\n", i, pontuacao[i]);

    clearviewport();
    setbkcolor(BLACK);
    setfillstyle(1, WHITE);
    outtextxy(10, 10, "O jogo terminou");
}

void exibir(Elemento item, int quantidade)
{
    int x, i = 0;

    // Limpa as posi��es
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

    // Anima��o que aumenta a muni��o na tela
    for (quantidade = 0; quantidade <= maximo; quantidade++)
    {
        exibir(municao, quantidade);
        delay(50);
    }

    // Variavel para monitorar a municao durante o jogo
    *jogoMunicao = 5;
}

void aumentarPontos(Estado *jogo, int valor)
{
    jogo->pontos += valor;
    limparAlvos();
    exibirPontos(jogo->pontos);
}

void diminuirVidas(Elemento vida, Estado *jogo)
{
    jogo->vidas--;
    exibir(vida, jogo->vidas);
}

void reiniciarInterface(Elemento vida, Elemento municao)
{

    exibir(vida, 5);
    exibir(municao, 5);
    exibirPontos(0);
    limparAlvos();
}

void exibirPartidaAtual(int partidaAtual)
{
    char texto[11];
    setbkcolor(COLOR(20,20,20));
    sprintf(texto, "Partida: %d", partidaAtual + 1);
    outtextxy(300, 8, texto);
}
