#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

// ============================================================================
// 1. UTILITÁRIOS DE TERMINAL
// ============================================================================

// Lê uma tecla do teclado sem exigir Enter e sem exibi-la na tela
// (desativa os modos ICANON e ECHO do terminal temporariamente).
int getch(void) {
    int ch;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Move o cursor do terminal para a posição (XPos, YPos) usando um código
// de escape ANSI. É usado para "redesenhar" o mapa sempre no mesmo lugar,
// sem precisar limpar a tela inteira a cada frame.
void SetCursorPos(int XPos, int YPos) {
    printf("\033[%d;%dH", YPos + 1, XPos + 1);
}

// ============================================================================
// 2. DADOS DOS MAPAS / FASES
// ============================================================================
// Cada função abaixo simplesmente copia uma matriz 20x20 fixa (o "desenho"
// da fase) para a matriz do jogo passada por referência.

void Fase1(int (&mat)[20][20]) {
    int temp[20][20]{
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,5,0,1,1,1},
        {1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,4,1,1,1,1,0,0,0,2,1,1,1,1,1,1},
        {1,1,1,1,1,3,1,1,1,1,4,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,0,0,0,0,0,8,2,8,3,9,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            mat[i][j] = temp[i][j];
}

void Fase2(int (&mat)[20][20]) {
    int temp[20][20]{
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,1,1,1,1,0,1,1,1,1,1,0,0,2,3,9,1,1,1,1},
        {1,1,1,2,0,0,8,5,1,1,3,1,2,1,1,1,1,1,1,1},
        {1,1,1,1,0,1,1,1,1,1,4,1,4,1,1,1,1,1,1,1},
        {1,1,1,1,0,0,3,2,8,0,0,2,0,0,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,2,1,3,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            mat[i][j] = temp[i][j];
}

void Fase3(int (&mat)[20][20]) {
    int temp[20][20]{
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,5,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,4,1,1,1,1,4,1,0,1,1,1,1,1,1,1,1},
        {1,1,1,1,0,0,0,0,0,2,0,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,2,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,2,8,3,8,0,0,0,2,0,3,9,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            mat[i][j] = temp[i][j];
}

// Tela de menu: mostra três "portas" (10, 11, 12) que levam às fases 1, 2 e 3.
void menu(int (&mat)[20][20]) {
    int temp[20][20]{
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13, 9,10, 0,11, 0,12, 0,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13}
    };

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            mat[i][j] = temp[i][j];
}

// Carrega a matriz da fase indicada por "jogoAtual" (0 = menu, 1..3 = fases),
// localiza o jogador na matriz recém-carregada e zera o valor "embaixo" dele.
void CarregarFase(int (&mat)[20][20], int jogoAtual, int &px, int &py, int &rerun1);
void EncontrarJogador(int (&mat)[20][20], int &px, int &py);

// ============================================================================
// 3. ROTAÇÃO DE MATRIZ (girar o mapa 90°)
// ============================================================================
// Ambas as funções primeiro transpõem a matriz (trocam mat[i][j] com
// mat[j][i]) e depois espelham metade das linhas/colunas, produzindo uma
// rotação de 90° no sentido horário (GirarMatrizQ) ou anti-horário
// (GirarMatrizE).

// Gira a matriz 90° no sentido anti-horário (tecla 'e').
void GirarMatrizE(int (&mat)[20][20]) {
    // Transposição
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }
    // Espelha cada linha na horizontal
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[i][19 - j];
            mat[i][19 - j] = temp;
        }
    }
}

// Gira a matriz 90° no sentido horário (tecla 'q').
void GirarMatrizQ(int (&mat)[20][20]) {
    // Transposição
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }
    // Espelha cada coluna na vertical
    for (int j = 0; j < 20; j++) {
        for (int i = 0; i < 10; i++) {
            int temp = mat[i][j];
            mat[i][j] = mat[19 - i][j];
            mat[19 - i][j] = temp;
        }
    }
}

// ============================================================================
// 4. LÓGICA DO JOGO
// ============================================================================

// Indica se o jogador pode caminhar sobre o tile de valor "valor".
bool ehPassavel(int valor) {
    return valor == 6 || valor == 0 || valor == 3 || valor == 5 ||
           valor == 8 || valor == 10 || valor == 11 || valor == 12;
}

// Varre a matriz procurando o tile do jogador (valor 9) e guarda sua
// posição em (px, py).
void EncontrarJogador(int (&mat)[20][20], int &px, int &py) {
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            if (mat[i][j] == 9) {
                px = i;
                py = j;
            }
}

// Tenta mover o jogador uma casa na direção indicada por "direcao"
// ('w'=cima, 's'=baixo, 'a'=esquerda, 'd'=direita).
// Retorna false se o movimento for inválido (fora do mapa ou tile bloqueado).
// Ao mover, restaura no tile de origem o valor que estava "guardado" em
// rerun1 (o que havia embaixo do jogador antes) e guarda em rerun1 o valor
// do novo tile em que o jogador está pisando agora.
bool MoverJogador(int (&mat)[20][20], int &px, int &py, int &rerun1, char direcao) {
    int dx = 0, dy = 0;

    switch (direcao) {
    case 'w': dx = -1; break;
    case 's': dx = 1;  break;
    case 'a': dy = -1; break;
    case 'd': dy = 1;  break;
    default: return false;
    }

    int nx = px + dx;
    int ny = py + dy;

    if (nx < 0 || nx >= 20 || ny < 0 || ny >= 20) return false;
    if (!ehPassavel(mat[nx][ny])) return false;

    int rerun = mat[nx][ny];  // valor do tile de destino, antes de pisar nele
    mat[px][py] = 0;
    px = nx;
    py = ny;
    mat[px - dx][py - dy] = rerun1;  // restaura o que havia na posição antiga
    mat[px][py] = 9;                 // jogador ocupa a nova posição
    rerun1 = rerun;                  // guarda o que está embaixo dele agora

    return true;
}

// Alterna os pares de terreno "visível/oculto" (4<->6 e 8<->7).
// É usado ao girar o mapa, para simular blocos que aparecem/desaparecem.
void AlternarTerrenoOculto(int (&mat)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (mat[i][j] == 4) mat[i][j] = 6;
            else if (mat[i][j] == 6) mat[i][j] = 4;

            if (mat[i][j] == 8) mat[i][j] = 7;
            else if (mat[i][j] == 7) mat[i][j] = 8;
        }
    }
}

// Faz os blocos vermelhos (valor 2) "caírem" para baixo enquanto houver
// espaço andável abaixo deles — simula gravidade após girar o mapa.
void AplicarGravidadeBlocos(int (&mat)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (mat[i][j] == 2) {
                int aux1 = 0, aux = 0, x = i, y = j;
                while (x + 1 < 20 && ehPassavel(mat[x + 1][y])) {
                    aux = mat[x + 1][y];
                    mat[x][y] = 0;
                    x = x + 1;
                    mat[x - 1][y] = aux1;
                    mat[x][y] = 2;
                    aux1 = aux;
                }
            }
        }
    }
}

// Gira o mapa inteiro (chamada quando o jogador está sobre a alavanca, tile 3,
// e pressiona 'q' ou 'e'). Depois de girar, reencontra o jogador na nova
// orientação, alterna o terreno oculto e aplica a gravidade nos blocos.
void GirarMapa(int (&mat)[20][20], int &px, int &py, bool &giro, char direcao) {
    if (direcao == 'q') {
        GirarMatrizQ(mat);
    } else if (direcao == 'e') {
        GirarMatrizE(mat);
    } else {
        return;
    }

    EncontrarJogador(mat, px, py);
    AlternarTerrenoOculto(mat);
    giro = !giro;
    AplicarGravidadeBlocos(mat);
}

// A fase é vencida quando o jogador está pisando na saída (tile 5).
bool VerificarVitoria(int rerun1) {
    return rerun1 == 5;
}

// No menu, o valor "embaixo" do jogador (10, 11 ou 12) indica qual fase
// será carregada ao pressionar Enter.
int SelecionarFase(int valorAbaixoDoJogador) {
    return valorAbaixoDoJogador;
}

// Carrega a matriz da fase pedida, localiza o jogador nela e zera rerun1
// (o valor "embaixo" do jogador começa em 0/chão vazio).
void CarregarFase(int (&mat)[20][20], int jogoAtual, int &px, int &py, int &rerun1) {
    switch (jogoAtual) {
    case 1: Fase1(mat); break;
    case 2: Fase2(mat); break;
    case 3: Fase3(mat); break;
    case 0:
    default: menu(mat); break;
    }

    EncontrarJogador(mat, px, py);
    rerun1 = 0;
}

// ============================================================================
// 5. DESENHO DO MAPA
// ============================================================================
// Percorre a matriz e imprime, para cada tile, um caractere com uma cor ANSI
// correspondente ao seu tipo.
void DesenharMapa(int (&mat)[20][20]) {
    printf("\033[37m");  // cor padrão (branco/cinza)

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            switch (mat[i][j]) {
            case 0:  cout << " "; break;                                  // chão vazio
            case 1:  printf("\033[42m"); cout << " "; printf("\033[0m"); break; // parede (fundo verde)
            case 2:  printf("\033[41m"); cout << " "; printf("\033[0m"); break; // bloco vermelho (fundo vermelho)
            case 3:  printf("\033[35m"); cout << "/"; break;               // alavanca
            case 4:  printf("\033[31m"); cout << "="; break;               // terreno visível
            case 5:  printf("\033[33m"); cout << "S"; break;               // saída
            case 6:  printf("\033[37m"); cout << ":"; break;               // terreno oculto
            case 7:  printf("\033[31m"); cout << "|"; break;               // terreno oculto (par de 8)
            case 8:  printf("\033[37m"); cout << ";"; break;               // terreno visível (par de 7)
            case 9:  printf("\033[36m"); cout << "@"; break;               // jogador
            case 10: printf("\033[32m"); cout << "1"; break;               // porta fase 1
            case 11: printf("\033[33m"); cout << "2"; break;               // porta fase 2
            case 12: printf("\033[31m"); cout << "3"; break;               // porta fase 3
            case 13: cout << " "; break;                                   // fundo do menu
            case 14: printf("\033[33m"); cout << "WIP"; break;             // não utilizado
            default: cout << " ";
            }
        }
        cout << endl;
    }

    printf("\033[37m");  // restaura cor padrão ao final
}

// ============================================================================
// 6. LOOP PRINCIPAL DO JOGO
// ============================================================================

int main() {
    int m[20][20];
    int JogoAtual = 0;      // 0 = menu, 1..3 = fases
    int faseCarregada = -1; // controla quando é preciso recarregar a matriz
    int px = 0, py = 0, rerun1 = 0;
    bool giro = true;
    bool jogando = true;

    printf("\033[?25l"); // esconde o cursor do terminal

    while (jogando) {
        // Só recarrega a matriz quando a fase realmente mudou
        if (JogoAtual != faseCarregada) {
            CarregarFase(m, JogoAtual, px, py, rerun1);
            faseCarregada = JogoAtual;
        }

        DesenharMapa(m);

        char tecla = getch();
        switch (tecla) {
        case 'w':
        case 's':
        case 'a':
        case 'd':
            // Movimento do jogador
            MoverJogador(m, px, py, rerun1, tecla);
            break;

        case 'q':
        case 'e':
            // Só gira o mapa se o jogador estiver sobre a alavanca (tile 3)
            if (rerun1 == 3) {
                GirarMapa(m, px, py, giro, tecla);
            }
            break;

        case '\n':
            if (JogoAtual == 0) {
                // No menu: Enter sobre uma porta (10/11/12) troca de fase
                switch (SelecionarFase(rerun1)) {
                case 10: JogoAtual = 1; break;
                case 11: JogoAtual = 2; break;
                case 12: JogoAtual = 3; break;
                }
            } else if (VerificarVitoria(rerun1)) {
                // Dentro de uma fase: Enter sobre a saída volta ao menu
                JogoAtual = 0;
            }
            break;

        case 'r':
            // Atalho para reiniciar/voltar ao menu a qualquer momento
            if (JogoAtual != 0) {
                JogoAtual = 0;
            }
            break;
        }

        SetCursorPos(0, 0); // reposiciona o cursor para redesenhar por cima
    }

    system("clear");
    printf("\033[33m");
    cout << "\n\n       Parabéns! Você ganhou!\n\n";

    return 0;
}
