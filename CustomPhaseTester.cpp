#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
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

void Fase1(int (&mat)[20][20]) {
  int temp[20][20]{{13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 1, 1, 1, 1, 1, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 0, 0, 9, 0, 0, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 0, 0, 0, 0, 0, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 0, 0, 5, 0, 0, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13, 1, 1, 1, 1, 1, 1, 1,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
                  {13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13}};
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 20; j++)
      mat[i][j] = temp[i][j];
}

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
    return valor == 6 || valor == 0 || valor == 3 || valor == 5 || valor == 8;
}

int main() {
  printf("\033[?25l");

  int m[20][20];
  Fase1(m);

  int (*atual)[20] = m;

  int px = 0, py = 0, rerun = 0, rerun1 = 0;
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 20; j++) {
      if (atual[i][j] == 9) {
        px = i;
        py = j;
      }
    }
  }

  int F = 1;
  bool giro = true;
  while (F == 1) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 20; j++) {
        switch (m[i][j]) {
        case 0: cout << " "; break;
        case 1: printf("\033[42m"); cout << " "; printf("\033[0m"); break;
        case 2: printf("\033[41m"); cout << " "; printf("\033[0m"); break;
        case 3: printf("\033[35m"); cout << "/"; break;
        case 4: printf("\033[31m"); cout << "="; break;
        case 5: printf("\033[33m"); cout << "S"; break;
        case 6: printf("\033[37m"); cout << ":"; break;
        case 7: printf("\033[31m"); cout << "|"; break;
        case 8: printf("\033[37m"); cout << ";"; break;
        case 9: printf("\033[36m"); cout << "@"; break;
        default: cout << " ";
        }
      }
      cout << endl;
    }
    char x;
    x = getch();
    switch (x) {
    case 'w':
      if (px - 1 >= 0 && ehPassavel(atual[px-1][py])) {
          rerun = atual[px-1][py];
          atual[px][py] = 0;
          px = px - 1;
          atual[px+1][py] = rerun1;
          atual[px][py] = 9;
          rerun1 = rerun;
      }
      break;
    case 's':
      if (px + 1 < 20 && ehPassavel(atual[px+1][py])) {
          rerun = atual[px+1][py];
          atual[px][py] = 0;
          px = px + 1;
          atual[px-1][py] = rerun1;
          atual[px][py] = 9;
          rerun1 = rerun;
      }
      break;
    case 'a':
      if (py - 1 >= 0 && ehPassavel(atual[px][py-1])) {
          rerun = atual[px][py-1];
          atual[px][py] = 0;
          py = py - 1;
          atual[px][py+1] = rerun1;
          atual[px][py] = 9;
          rerun1 = rerun;
      }
      break;
    case 'd':
      if (py + 1 < 20 && ehPassavel(atual[px][py+1])) {
          rerun = atual[px][py+1];
          atual[px][py] = 0;
          py = py + 1;
          atual[px][py-1] = rerun1;
          atual[px][py] = 9;
          rerun1 = rerun;
      }
      break;
      case 'q':
        if(rerun1==3){
          GirarMatrizQ(m);
          for (int i = 0; i < 20; i++) {
              for (int j = 0; j < 20; j++) {
                  if (atual[i][j] == 9) {
                      px = i;
                      py = j;
                    }
                }
            }
            if(giro==false){
                for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                      if (m[i][j]==4) {
                          m[i][j]=6;
                        }else if (m[i][j]==6) {
                          m[i][j]=4;
                        }
                       
                        if (m[i][j]==8) {
                            m[i][j]=7;
                        }else if (m[i][j]==7) {
                            m[i][j]=8;
                        }
                  }
                }
                giro=true;
            }else{
                for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                      if (m[i][j]==4) {
                          m[i][j]=6;
                        }else if (m[i][j]==6) {
                          m[i][j]=4;
                        }
                       
                        if (m[i][j]==8) {
                            m[i][j]=7;
                        }else if (m[i][j]==7) {
                            m[i][j]=8;
                        }
                  }
                }
                giro=false;
            }
            
            for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                      if(m[i][j]==2){
                          int aux1 = 0, aux=0, x=i, y=j;
                          while (x + 1 < 20 && ehPassavel(m[x+1][y])) {
                              aux = m[x+1][y];
                              m[x][y] = 0;
                              x = x + 1;
                              m[x-1][y] = aux1;
                              m[x][y] = 2;
                              aux1 = aux;
                          }
                      }
                  }
                }
        }
      break;
      case 'e':
        if(rerun1==3){
          GirarMatrizE(m);
          for (int i = 0; i < 20; i++) {
              for (int j = 0; j < 20; j++) {
                  if (atual[i][j] == 9) {
                      px = i;
                      py = j;
                    }
                }
            }
            if(giro==true){
                for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                        if (m[i][j]==4) {
                          m[i][j]=6;
                        }else if (m[i][j]==6) {
                          m[i][j]=4;
                        }
                       
                        if (m[i][j]==8) {
                            m[i][j]=7;
                        }else if (m[i][j]==7) {
                            m[i][j]=8;
                        }
                  }
                }
                giro=false;
            }else{
                for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                      if (m[i][j]==8) {
                        m[i][j]=7;
                      }else if (m[i][j]==7) {
                        m[i][j]=8;
                      }
                     
                      if (m[i][j]==4) {
                          m[i][j]=6;
                        }else if (m[i][j]==6) {
                          m[i][j]=4;
                        }
                  }
                }
                giro=true;
            }
            
            for (int i = 0; i < 20; i++) {
                  for (int j = 0; j < 20; j++) {
                      if(m[i][j]==2){
                          int aux1 = 0, aux=0, x=i, y=j;
                          while (x + 1 < 20 && ehPassavel(m[x+1][y])) {
                              aux = m[x+1][y];
                              m[x][y] = 0;
                              x = x + 1;
                              m[x-1][y] = aux1;
                              m[x][y] = 2;
                              aux1 = aux;
                          }
                      }
                  }
                }
        }
      break;
      case '\n':
        if(rerun1==5){
            F=0;
        }
      break;
    }
    SetCursorPos(0, 0);
  }
  system("clear");
  printf("\033[33m"); cout<<"\n\n       Parabéns! Você ganhou!\n\n";

  return 0;
}
