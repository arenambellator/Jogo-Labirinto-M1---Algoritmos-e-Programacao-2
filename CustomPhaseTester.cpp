/*
 * ============================================================================
 *  PROTÓTIPO DO JOGO DE PUZZLE EM MODO TEXTO (TERMINAL)
 * ============================================================================
 *
 *  Este é o protótipo inicial (uma única fase, sem menu). A lógica de
 *  movimento e rotação ainda está toda dentro do main(), então os
 *  comentários abaixo servem de "mapa" para quem for MODIFICAR o jogo:
 *  onde mexer para adicionar uma fase nova, um tile novo, uma tecla nova etc.
 *
 *  ESTRUTURA DO ARQUIVO:
 *    1. Utilitários de terminal (leitura de tecla, cursor)
 *    2. Dados da fase (matriz 20x20)
 *    3. Rotação de matriz (girar o mapa 90°)
 *    4. Regras de movimento (quais tiles são andáveis)
 *    5. main() -> contém o loop do jogo, o desenho do mapa e TODA a lógica
 *       de movimento/rotação/gravidade (ainda não separada em funções)
 *
 *  LEGENDA DOS VALORES DA MATRIZ:
 *    0  -> chão vazio, andável
 *    1  -> parede (bloco verde, intransponível)
 *    2  -> bloco vermelho, cai por gravidade após girar o mapa
 *    3  -> alavanca ('/'), permite girar o mapa com 'q'/'e' quando o
 *          jogador está em cima dela (rerun1 == 3)
 *    4  -> terreno visível ('='), alterna com 6 ao girar o mapa
 *    5  -> saída da fase ('S'), pressionar Enter em cima dela termina o jogo
 *    6  -> terreno oculto (':'), alterna com 4 ao girar o mapa
 *    7  -> terreno oculto ('|'), alterna com 8 ao girar o mapa
 *    8  -> terreno visível (';'), alterna com 7 ao girar o mapa
 *    9  -> jogador ('@')
 *    13 -> "fundo"/moldura ao redor da área jogável (não andável, não
 *          desenhado com nenhum case específico -> cai no "default" e
 *          aparece como espaço em branco)
 *
 *  GUIA RÁPIDO PARA MODDERS:
 *    - Quer criar uma fase nova? Copie a função Fase1(), renomeie (ex:
 *      Fase2()) e troque os números da matriz "temp". Depois, no main(),
 *      troque a chamada Fase1(m) pela nova função (ou crie um sistema de
 *      seleção de fases, como no menu do jogo completo).
 *    - Quer adicionar um tipo de tile novo? 1) escolha um número livre;
 *      2) adicione um "case" no switch de desenho (dentro do while, seção
 *      de impressão do mapa) definindo cor e caractere; 3) se o tile deve
 *      ser andável, adicione o número em ehPassavel().
 *    - Quer mudar o comportamento de gravidade dos blocos vermelhos (2)?
 *      Procure o bloco "APLICAÇÃO DE GRAVIDADE" repetido dentro dos cases
 *      'q' e 'e' — nesta versão prototípica, o mesmo trecho é duplicado
 *      em ambos, então uma mudança precisa ser feita nos dois lugares
 *      (na versão final do jogo isso foi extraído para uma função única,
 *      AplicarGravidadeBlocos()).
 *    - Quer mudar as teclas de movimento? Veja os cases 'w'/'a'/'s'/'d'
 *      dentro do switch principal do while — cada um repete manualmente a
 *      lógica de mover o jogador; o número de referência do tile embaixo
 *      do jogador é sempre guardado em "rerun1".
 * ============================================================================
 */

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

// ============================================================================
// 1. UTILITÁRIOS DE TERMINAL
// ============================================================================

// Lê uma tecla sem precisar de Enter e sem ecoar na tela (desliga ICANON e
// ECHO do terminal só durante a leitura). Não precisa mexer aqui para modar
// o jogo — isso é só a "ponte" com o sistema operacional.
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

// Move o cursor do terminal para (XPos, YPos) via código de escape ANSI.
// Usado no fim de cada loop para redesenhar o mapa sempre no mesmo lugar,
// em vez de "rolar" a tela a cada frame.
void SetCursorPos(int XPos, int YPos) {
    printf("\033[%d;%dH", YPos + 1, XPos + 1);
}

// ============================================================================
// 2. DADOS DA FASE
// ============================================================================
// Este protótipo tem só uma fase fixa. PARA CRIAR NOVAS FASES: duplique esta
// função, dê um nome novo (Fase2, Fase3...) e edite os números da matriz.
// Lembre-se de manter exatamente um tile "9" (jogador) e ao menos um "5"
// (saída), senão o jogo trava sem jogador ou sem forma de vencer.
void Fase1(int (&mat)[20][20]) {
    int temp[20][20]{
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
        {13,13,13,13,13,13,13, 1, 1, 1, 1, 1, 1, 1,13,13,13,13,13,13}, // parede de cima do quarto
        {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
        {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
        {13,13,13,13,13,13,13, 1, 0, 0, 9, 0, 0, 1,13,13,13,13,13,13}, // jogador (9) começa no centro
        {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
        {13,13,13,13,13,13,13, 1, 0, 0, 5, 0, 0, 1,13,13,13,13,13,13}, // saída (5)
        {13,13,13,13,13,13,13, 1, 1, 1, 1, 1, 1, 1,13,13,13,13,13,13}, // parede de baixo do quarto
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

// ============================================================================
// 3. ROTAÇÃO DE MATRIZ (girar o mapa 90°)
// ============================================================================
// As duas funções fazem transposição (trocar mat[i][j] com mat[j][i]) e
// depois espelham metade das linhas ou colunas, resultando numa rotação de
// 90°. Se for mexer aqui, tenha cuidado: qualquer alteração de sentido de
// giro afeta diretamente como o jogador reencontra sua posição depois de
// girar (feito manualmente dentro do main(), casos 'q' e 'e').

// Gira 90° no sentido anti-horário (tecla 'e').
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

// Gira 90° no sentido horário (tecla 'q').
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
// 4. REGRAS DE MOVIMENTO
// ============================================================================

// Lista de tiles em que o jogador PODE andar. Para tornar um tile novo
// andável, basta adicionar seu número aqui (não esqueça de também tratar
// o desenho dele no switch de impressão, dentro do main()).
bool ehPassavel(int valor) {
    return valor == 6 || valor == 0 || valor == 3 || valor == 5 || valor == 8;
}

// ============================================================================
// 5. LOOP PRINCIPAL DO JOGO
// ============================================================================
// AVISO PARA MODDERS: nesta versão prototípica, tudo (desenho, movimento,
// rotação, gravidade) está dentro do main(). Isso funciona, mas dificulta
// reaproveitar a lógica (por isso, na versão final do jogo, cada bloco foi
// extraído para funções como MoverJogador(), GirarMapa(),
// AplicarGravidadeBlocos() etc.). Se for expandir bastante este protótipo,
// considere fazer essa extração antes de continuar — vai poupar bastante
// código duplicado, principalmente nos casos 'q' e 'e' abaixo.

int main() {
    printf("\033[?25l"); // esconde o cursor do terminal

    int m[20][20];
    Fase1(m);

    // "atual" é apenas um apelido (ponteiro) para a mesma matriz "m".
    // Existe provavelmente para permitir, no futuro, trocar de fase
    // apontando "atual" para outra matriz sem duplicar o restante do código.
    int (*atual)[20] = m;

    // px, py -> posição do jogador na matriz
    // rerun  -> valor temporário do tile para onde o jogador está indo
    // rerun1 -> valor do tile que está "guardado" embaixo do jogador
    //           (é ele que decide se o jogador está sobre uma alavanca (3)
    //           ou sobre a saída (5), por exemplo)
    int px = 0, py = 0, rerun = 0, rerun1 = 0;

    // Localiza a posição inicial do jogador (tile 9) na matriz
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (atual[i][j] == 9) {
                px = i;
                py = j;
            }
        }
    }

    int F = 1;        // F controla o loop principal: enquanto F == 1, o jogo roda
    bool giro = true;  // controla a fase de alternância do terreno oculto/visível

    while (F == 1) {
        // ---------------------------------------------------------------
        // DESENHO DO MAPA
        // Para adicionar um tile novo, crie um "case" aqui com a cor ANSI
        // (\033[NNm) e o caractere desejado.
        // ---------------------------------------------------------------
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                switch (m[i][j]) {
                case 0: cout << " "; break;                                     // chão vazio
                case 1: printf("\033[42m"); cout << " "; printf("\033[0m"); break; // parede (fundo verde)
                case 2: printf("\033[41m"); cout << " "; printf("\033[0m"); break; // bloco vermelho (fundo vermelho)
                case 3: printf("\033[35m"); cout << "/"; break;                  // alavanca
                case 4: printf("\033[31m"); cout << "="; break;                  // terreno visível
                case 5: printf("\033[33m"); cout << "S"; break;                  // saída
                case 6: printf("\033[37m"); cout << ":"; break;                  // terreno oculto
                case 7: printf("\033[31m"); cout << "|"; break;                  // terreno oculto (par de 8)
                case 8: printf("\033[37m"); cout << ";"; break;                  // terreno visível (par de 7)
                case 9: printf("\033[36m"); cout << "@"; break;                  // jogador
                default: cout << " ";                                           // inclui o tile 13 (fundo/moldura)
                }
            }
            cout << endl;
        }

        char x;
        x = getch();

        switch (x) {

        // -------------------------------------------------------------
        // MOVIMENTO (w/a/s/d)
        // Cada case repete o mesmo padrão: checa limite do mapa + se o
        // tile de destino é andável; se sim, troca o jogador de lugar e
        // "empurra" o valor antigo de rerun1 para o tile que ficou para
        // trás. Para adicionar uma tecla de movimento diagonal, por
        // exemplo, copie um destes blocos e ajuste os deltas de px/py.
        // -------------------------------------------------------------
        case 'w': // cima
            if (px - 1 >= 0 && ehPassavel(atual[px - 1][py])) {
                rerun = atual[px - 1][py];
                atual[px][py] = 0;
                px = px - 1;
                atual[px + 1][py] = rerun1;
                atual[px][py] = 9;
                rerun1 = rerun;
            }
            break;

        case 's': // baixo
            if (px + 1 < 20 && ehPassavel(atual[px + 1][py])) {
                rerun = atual[px + 1][py];
                atual[px][py] = 0;
                px = px + 1;
                atual[px - 1][py] = rerun1;
                atual[px][py] = 9;
                rerun1 = rerun;
            }
            break;

        case 'a': // esquerda
            if (py - 1 >= 0 && ehPassavel(atual[px][py - 1])) {
                rerun = atual[px][py - 1];
                atual[px][py] = 0;
                py = py - 1;
                atual[px][py + 1] = rerun1;
                atual[px][py] = 9;
                rerun1 = rerun;
            }
            break;

        case 'd': // direita
            if (py + 1 < 20 && ehPassavel(atual[px][py + 1])) {
                rerun = atual[px][py + 1];
                atual[px][py] = 0;
                py = py + 1;
                atual[px][py - 1] = rerun1;
                atual[px][py] = 9;
                rerun1 = rerun;
            }
            break;

        // -------------------------------------------------------------
        // GIRO DO MAPA (q = horário, e = anti-horário)
        // Só funciona se o jogador estiver sobre a alavanca (rerun1 == 3).
        // Depois de girar:
        //   1) reencontra o jogador na matriz (posição mudou com o giro)
        //   2) alterna terreno oculto/visível (4<->6 e 8<->7), invertendo
        //      o sentido dependendo do estado atual de "giro"
        //   3) aplica gravidade nos blocos vermelhos (2)
        // NOTA PARA MODDERS: os passos 2 e 3 estão DUPLICADOS entre os
        // casos 'q' e 'e' (só muda a condição inicial de "giro"). Se for
        // alterar essa lógica, lembre-se de replicar a mudança nos dois
        // lugares, ou melhor: extraia para uma função própria, como foi
        // feito na versão final do jogo (AlternarTerrenoOculto() e
        // AplicarGravidadeBlocos()).
        // -------------------------------------------------------------
        case 'q':
            if (rerun1 == 3) {
                GirarMatrizQ(m);

                // 1) reencontra o jogador após o giro
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        if (atual[i][j] == 9) {
                            px = i;
                            py = j;
                        }
                    }
                }

                // 2) alterna terreno oculto/visível
                if (giro == false) {
                    for (int i = 0; i < 20; i++) {
                        for (int j = 0; j < 20; j++) {
                            if (m[i][j] == 4) {
                                m[i][j] = 6;
                            } else if (m[i][j] == 6) {
                                m[i][j] = 4;
                            }

                            if (m[i][j] == 8) {
                                m[i][j] = 7;
                            } else if (m[i][j] == 7) {
                                m[i][j] = 8;
                            }
                        }
                    }
                    giro = true;
                } else {
                    for (int i = 0; i < 20; i++) {
                        for (int j = 0; j < 20; j++) {
                            if (m[i][j] == 4) {
                                m[i][j] = 6;
                            } else if (m[i][j] == 6) {
                                m[i][j] = 4;
                            }

                            if (m[i][j] == 8) {
                                m[i][j] = 7;
                            } else if (m[i][j] == 7) {
                                m[i][j] = 8;
                            }
                        }
                    }
                    giro = false;
                }

                // 3) aplica gravidade nos blocos vermelhos (2), fazendo-os
                //    "cair" enquanto houver espaço andável abaixo
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        if (m[i][j] == 2) {
                            int aux1 = 0, aux = 0, x = i, y = j;
                            while (x + 1 < 20 && ehPassavel(m[x + 1][y])) {
                                aux = m[x + 1][y];
                                m[x][y] = 0;
                                x = x + 1;
                                m[x - 1][y] = aux1;
                                m[x][y] = 2;
                                aux1 = aux;
                            }
                        }
                    }
                }
            }
            break;

        case 'e':
            if (rerun1 == 3) {
                GirarMatrizE(m);

                // 1) reencontra o jogador após o giro
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        if (atual[i][j] == 9) {
                            px = i;
                            py = j;
                        }
                    }
                }

                // 2) alterna terreno oculto/visível
                //    (aqui a ordem das checagens 4/6 e 8/7 é invertida em
                //    relação ao case 'q' — resultado é o mesmo, mas repare
                //    nisso se for refatorar para uma função única)
                if (giro == true) {
                    for (int i = 0; i < 20; i++) {
                        for (int j = 0; j < 20; j++) {
                            if (m[i][j] == 4) {
                                m[i][j] = 6;
                            } else if (m[i][j] == 6) {
                                m[i][j] = 4;
                            }

                            if (m[i][j] == 8) {
                                m[i][j] = 7;
                            } else if (m[i][j] == 7) {
                                m[i][j] = 8;
                            }
                        }
                    }
                    giro = false;
                } else {
                    for (int i = 0; i < 20; i++) {
                        for (int j = 0; j < 20; j++) {
                            if (m[i][j] == 8) {
                                m[i][j] = 7;
                            } else if (m[i][j] == 7) {
                                m[i][j] = 8;
                            }

                            if (m[i][j] == 4) {
                                m[i][j] = 6;
                            } else if (m[i][j] == 6) {
                                m[i][j] = 4;
                            }
                        }
                    }
                    giro = true;
                }

                // 3) aplica gravidade nos blocos vermelhos (2)
                for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 20; j++) {
                        if (m[i][j] == 2) {
                            int aux1 = 0, aux = 0, x = i, y = j;
                            while (x + 1 < 20 && ehPassavel(m[x + 1][y])) {
                                aux = m[x + 1][y];
                                m[x][y] = 0;
                                x = x + 1;
                                m[x - 1][y] = aux1;
                                m[x][y] = 2;
                                aux1 = aux;
                            }
                        }
                    }
                }
            }
            break;

        // -------------------------------------------------------------
        // VITÓRIA (Enter sobre a saída, tile 5)
        // Para mudar a condição de vitória (ex: exigir todos os blocos
        // vermelhos em certas posições), é aqui que a checagem entraria.
        // -------------------------------------------------------------
        case '\n':
            if (rerun1 == 5) {
                F = 0; // encerra o loop principal
            }
            break;
        }

        SetCursorPos(0, 0); // redesenha sempre por cima, sem rolar a tela
    }

    system("clear");
    printf("\033[33m");
    cout << "\n\n       Parabéns! Você ganhou!\n\n";

    return 0;
}
