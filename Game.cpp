#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

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

// Limpa a tela inteira do terminal. Usado ao trocar de "modo" de exibição
// (jogo <-> créditos, fase <-> menu) para não deixar caracteres antigos
// sobrando na tela.
void LimparTela() {
    printf("\033[2J\033[H");
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
        {1,1,1,1,14,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,4,1,1,1,1,4,1,0,1,1,1,1,1,1,1,1},
        {1,1,1,1,0,2,0,0,0,2,0,2,1,1,1,1,1,1,1,1},
        {1,1,1,1,2,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,2,8,3,8,2,0,0,2,0,3,9,1,1,1,1,1},
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
           valor == 8 || valor == 10 || valor == 11 || valor == 12 || valor == 14;
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
//
// CORREÇÃO: antes, a condição do laço tinha um erro de precedência de
// operadores ("a && b || c" era interpretado como "(a && b) || c"), o que
// fazia o código acessar mat[x+1][y] fora dos limites da matriz quando
// x+1 == 20, além de usar uma condição de queda errada ("continua caindo
// enquanto a casa de baixo não for outro bloco", em vez de "continua caindo
// enquanto a casa de baixo for andável"). Também percorríamos a matriz de
// cima para baixo, o que impedia blocos empilhados de caírem corretamente
// (o de cima tentava cair antes do de baixo terminar de assentar).
//
// Agora percorremos cada coluna de baixo para cima: assim, o bloco mais
// baixo de uma pilha assenta primeiro, e os de cima caem sobre ele em
// seguida, até que nenhum bloco tenha mais espaço andável abaixo dele.
void AplicarGravidadeBlocos(int (&mat)[20][20]) {
    for (int j = 0; j < 20; j++) {
        for (int i = 18; i >= 0; i--) {
            if (mat[i][j] == 2) {
                int x = i;
                while (x + 1 < 20 && ehPassavel(mat[x + 1][j])) {
                    mat[x][j] = mat[x + 1][j];
                    mat[x + 1][j] = 2;
                    x++;
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

// Copia o conteúdo de uma matriz 20x20 para outra. Usada para salvar/restaurar
// o estado da fase quando o jogador dá uma "espiada" no menu com 'm'.
void CopiarMatriz(int (&origem)[20][20], int (&destino)[20][20]) {
    memcpy(destino, origem, sizeof(int) * 20 * 20);
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
            case 14: printf("\033[33m"); cout << "F"; break;               // saída final (Fase 3): encerra o jogo
            default: cout << " ";
            }
        }
        cout << endl;
    }

    printf("\033[37m");  // restaura cor padrão ao final

    // Rodapé com dica dos comandos, sempre visível durante o jogo.
    printf("\033[36m");
    cout << "[p] creditos  [r] reiniciar fase  [m] menu" << endl;
    printf("\033[37m");
}

// Desenha a tela de créditos. Preenche as mesmas 20 linhas usadas pelo mapa
// (com padding) para não deixar sobras de caracteres do frame anterior.
void DesenharCreditos() {
    printf("\033[33m");

    cout<<"===================================="<<endl<<"              CREDITOS"<<endl<<"===================================="<<endl<<" Jogo de quebra-cabeca com rotacao"<<endl<<" de mapa, alavancas e gravidade."<<endl<<endl<<" Desenvolvido em C++"<<endl<<endl<<" Por Vitor Chilanti"<<endl<<endl<<" Pressione 'p' para voltar ao jogo"<<endl;

    // Preenche as linhas restantes em branco para cobrir totalmente
    // o que estava desenhado antes (mapa tem 20 linhas de 20 colunas
    // + rodapé, então cobrimos uma área generosa).
    for (int i = 9; i < 22; i++) {
        cout << "                                        " << endl;
    }

    printf("\033[37m");
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

    // --- Estado da tela de créditos ('p') ---
    bool emCreditos = false;

    // --- Estado da "espiada" no menu ('m') ---
    // Quando o jogador está numa fase e pressiona 'm', o estado atual da
    // fase é salvo aqui; pressionando 'm' de novo, esse estado é restaurado
    // e o jogador volta exatamente para onde estava.
    bool emMenuTemporario = false;
    int matrizSalva[20][20];
    int pxSalvo = 0, pySalvo = 0, rerun1Salvo = 0;
    int jogoAtualSalvo = 0, faseCarregadaSalva = -1;

    printf("\033[?25l"); // esconde o cursor do terminal

    while (jogando) {
        if (emCreditos) {
            DesenharCreditos();
        } else {
            // Só recarrega a matriz quando a fase realmente mudou
            if (JogoAtual != faseCarregada) {
                CarregarFase(m, JogoAtual, px, py, rerun1);
                faseCarregada = JogoAtual;
            }

            DesenharMapa(m);
        }

        char tecla = getch();
        switch (tecla) {
        case 'w':
        case 's':
        case 'a':
        case 'd':
            // Movimento do jogador (desabilitado durante os créditos)
            if (!emCreditos) {
                MoverJogador(m, px, py, rerun1, tecla);
            }
            break;

        case 'q':
        case 'e':
            // Só gira o mapa se o jogador estiver sobre a alavanca (tile 3)
            if (!emCreditos && rerun1 == 3) {
                GirarMapa(m, px, py, giro, tecla);
            }
            break;

        case '\n':
            if (emCreditos == false) {
                // CORREÇÃO: o tile 14 ('S', usado na Fase 3) fica embaixo do
                // jogador da mesma forma que qualquer outro tile durante o
                // jogo, então esse caso precisa ser checado independente da
                // fase atual — antes, "case 14" só era avaliado dentro do
                // bloco do menu (JogoAtual == 0), onde o tile 14 nunca existe,
                // então pressionar Enter sobre ele na Fase 3 não fazia nada.
                if (rerun1 == 14) {
                    jogando = false;
                } else if (JogoAtual == 0 && !emMenuTemporario) {
                    // No menu "de verdade" (não é uma espiada): Enter sobre
                    // uma porta (10/11/12) troca de fase
                    switch (SelecionarFase(rerun1)) {
                    case 10: JogoAtual = 1; break;
                    case 11: JogoAtual = 2; break;
                    case 12: JogoAtual = 3; break;
                    }
                } else if (JogoAtual != 0 && VerificarVitoria(rerun1)) {
                    // Dentro de uma fase: Enter sobre a saída volta ao menu
                    // definitivamente (não há mais fase salva para restaurar)
                    JogoAtual++;
                    emMenuTemporario = false;
                }
            }
            break;

        case 'r':
            // Reinicia a fase atual (não faz nada no menu ou durante
            // uma espiada temporária no menu)
            if (!emCreditos && JogoAtual != 0 && !emMenuTemporario) {
                LimparTela();
                faseCarregada = -1; // força CarregarFase a recarregar a mesma fase
            }
            break;

        case 'm':
            // Alterna entre a fase atual e o menu, preservando o progresso
            if (!emCreditos) {
                if (!emMenuTemporario) {
                    if (JogoAtual != 0) {
                        // Salva o estado completo da fase antes de ir ao menu
                        CopiarMatriz(m, matrizSalva);
                        pxSalvo = px;
                        pySalvo = py;
                        rerun1Salvo = rerun1;
                        jogoAtualSalvo = JogoAtual;
                        faseCarregadaSalva = faseCarregada;

                        emMenuTemporario = true;
                        LimparTela();
                        JogoAtual = 0; // exibe o menu (será carregado no próximo loop)
                    }
                    // Se já estava no menu "de verdade", 'm' não faz nada.
                } else {
                    // Restaura a fase exatamente de onde o jogador parou
                    CopiarMatriz(matrizSalva, m);
                    px = pxSalvo;
                    py = pySalvo;
                    rerun1 = rerun1Salvo;
                    JogoAtual = jogoAtualSalvo;
                    faseCarregada = faseCarregadaSalva;

                    emMenuTemporario = false;
                    LimparTela();
                }
            }
            break;

        case 'p':
            // Alterna para a tela de créditos e de volta ao que estava sendo
            // exibido (jogo ou menu), sem alterar nenhum estado do jogo.
            emCreditos = !emCreditos;
            LimparTela();
            break;
        }

        SetCursorPos(0, 0); // reposiciona o cursor para redesenhar por cima
    }

    system("clear");
    printf("\033[33m");
    cout << "\n\n       Parabéns! Você ganhou!\n\n";

    return 0;
}
