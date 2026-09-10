#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

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

void SetCursorPos(int XPos, int YPos) {
    printf("\033[%d;%dH", YPos + 1, XPos + 1);
}

void LimparTela() {
    printf("\033[2J\033[H");
}

void Fase1(int (&mat)[20][20]) {
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

void CarregarFase(int (&mat)[20][20], int jogoAtual, int &px, int &py, int &rerun1);
void EncontrarJogador(int (&mat)[20][20], int &px, int &py);

void GirarMatrizE(int (&mat)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[i][19 - j];
            mat[i][19 - j] = temp;
        }
    }
}

void GirarMatrizQ(int (&mat)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            int temp = mat[i][j];
            mat[i][j] = mat[j][i];
            mat[j][i] = temp;
        }
    }
    for (int j = 0; j < 20; j++) {
        for (int i = 0; i < 10; i++) {
            int temp = mat[i][j];
            mat[i][j] = mat[19 - i][j];
            mat[19 - i][j] = temp;
        }
    }
}

bool ehPassavel(int valor) {
    return valor == 6 || valor == 0 || valor == 3 || valor == 5 ||
           valor == 8 || valor == 10 || valor == 11 || valor == 12 || valor == 14;
}

void EncontrarJogador(int (&mat)[20][20], int &px, int &py) {
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
            if (mat[i][j] == 9) {
                px = i;
                py = j;
            }
}

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

    int rerun = mat[nx][ny];
    mat[px][py] = 0;
    px = nx;
    py = ny;
    mat[px - dx][py - dy] = rerun1;
    mat[px][py] = 9;
    rerun1 = rerun;

    return true;
}

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

bool VerificarVitoria(int rerun1) {
    return rerun1 == 5;
}

int SelecionarFase(int valorAbaixoDoJogador) {
    return valorAbaixoDoJogador;
}

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

void CopiarMatriz(int (&origem)[20][20], int (&destino)[20][20]) {
    memcpy(destino, origem, sizeof(int) * 20 * 20);
}

void DesenharMapa(int (&mat)[20][20]) {
    printf("\033[37m");

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            switch (mat[i][j]) {
            case 0:  cout << " "; break;
            case 1:  printf("\033[42m"); cout << " "; printf("\033[0m"); break;
            case 2:  printf("\033[41m"); cout << " "; printf("\033[0m"); break;
            case 3:  printf("\033[35m"); cout << "/"; break;
            case 4:  printf("\033[31m"); cout << "="; break;
            case 5:  printf("\033[33m"); cout << "S"; break;
            case 6:  printf("\033[37m"); cout << ":"; break;
            case 7:  printf("\033[31m"); cout << "|"; break;
            case 8:  printf("\033[37m"); cout << ";"; break;
            case 9:  printf("\033[36m"); cout << "@"; break;
            case 10: printf("\033[32m"); cout << "1"; break;
            case 11: printf("\033[33m"); cout << "2"; break;
            case 12: printf("\033[31m"); cout << "3"; break;
            case 13: cout << " "; break;
            case 14: printf("\033[33m"); cout << "F"; break;
            default: cout << " ";
            }
        }
        cout << endl;
    }

    printf("\033[37m");

    printf("\033[36m");
    cout << "[p] creditos  [r] reiniciar fase  [m] menu" << endl;
    printf("\033[37m");
}

void DesenharCreditos() {
    printf("\033[33m");

    cout<<"===================================="<<endl<<"              CREDITOS"<<endl<<"===================================="<<endl<<" Jogo de quebra-cabeca com rotacao"<<endl<<" de mapa, alavancas e gravidade."<<endl<<endl<<" Desenvolvido em C++"<<endl<<endl<<" Por Vitor Chilanti"<<endl<<endl<<" Pressione 'p' para voltar ao jogo"<<endl;

    for (int i = 9; i < 22; i++) {
        cout << "                                        " << endl;
    }

    printf("\033[37m");
}

int main() {
    int m[20][20];
    int JogoAtual = 0;
    int faseCarregada = -1;
    int px = 0, py = 0, rerun1 = 0;
    bool giro = true;
    bool jogando = true;

    bool emCreditos = false;

    bool emMenuTemporario = false;
    int matrizSalva[20][20];
    int pxSalvo = 0, pySalvo = 0, rerun1Salvo = 0;
    int jogoAtualSalvo = 0, faseCarregadaSalva = -1;

    printf("\033[?25l");

    while (jogando) {
        if (emCreditos) {
            DesenharCreditos();
        } else {
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
            if (!emCreditos) {
                MoverJogador(m, px, py, rerun1, tecla);
            }
            break;

        case 'q':
        case 'e':
            if (!emCreditos && rerun1 == 3) {
                GirarMapa(m, px, py, giro, tecla);
            }
            break;

        case '\n':
            if (emCreditos == false) {
                if (rerun1 == 14) {
                    jogando = false;
                } else if (JogoAtual == 0) {
                    switch (SelecionarFase(rerun1)) {
                    case 10: JogoAtual = 1; break;
                    case 11: JogoAtual = 2; break;
                    case 12: JogoAtual = 3; break;
                    }
                } else if (JogoAtual != 0 && VerificarVitoria(rerun1)) {
                    JogoAtual++;
                    emMenuTemporario = false;
                }
            }
            break;

        case 'r':
            if (!emCreditos && JogoAtual != 0 && !emMenuTemporario) {
                LimparTela();
                faseCarregada = -1;
            }
            break;

        case 'm':
            if (!emCreditos) {
                if (!emMenuTemporario) {
                    if (JogoAtual != 0) {
                        CopiarMatriz(m, matrizSalva);
                        pxSalvo = px;
                        pySalvo = py;
                        rerun1Salvo = rerun1;
                        jogoAtualSalvo = JogoAtual;
                        faseCarregadaSalva = faseCarregada;

                        emMenuTemporario = true;
                        LimparTela();
                        JogoAtual = 0;
                    }
                } else {
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
            emCreditos = !emCreditos;
            LimparTela();
            break;
        }

        SetCursorPos(0, 0);
    }

    LimparTela();
    printf("\033[33m");
    cout << "\n\n       Parabéns! Você ganhou!\n\n";

    return 0;
}
