#include <math.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec)
{
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec);

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#endif

float anguloA, anguloB, anguloC;

float larguraCubo = 20;
int larguraTela = 160, alturaTela = 44;
float bufferProfundidade[160 * 44];
char bufferCaracteres[160 * 44];
int codigoASCIIFundo = '.';
int distanciaDaCamera = 100;
float deslocamentoHorizontal;
float fatorDeAmpliacao = 40;

float incrementoVelocidade = 0.6;

float x, y, z;
float inversoZ;
int xProjetado, yProjetado;
int indice;

float calcularX(int i, int j, int k) {
  return j * sin(anguloA) * sin(anguloB) * cos(anguloC) - k * cos(anguloA) * sin(anguloB) * cos(anguloC) +
         j * cos(anguloA) * sin(anguloC) + k * sin(anguloA) * sin(anguloC) + i * cos(anguloB) * cos(anguloC);
}

float calcularY(int i, int j, int k) {
  return j * cos(anguloA) * cos(anguloC) + k * sin(anguloA) * cos(anguloC) -
         j * sin(anguloA) * sin(anguloB) * sin(anguloC) + k * cos(anguloA) * sin(anguloB) * sin(anguloC) -
         i * cos(anguloB) * sin(anguloC);
}

float calcularZ(int i, int j, int k) {
  return k * cos(anguloA) * cos(anguloB) - j * sin(anguloA) * cos(anguloB) + i * sin(anguloB);
}

void calcularParaSuperficie(float cuboX, float cuboY, float cuboZ, int ch) {
  x = calcularX(cuboX, cuboY, cuboZ);
  y = calcularY(cuboX, cuboY, cuboZ);
  z = calcularZ(cuboX, cuboY, cuboZ) + distanciaDaCamera;

  inversoZ = 1 / z;

  xProjetado = (int)(larguraTela / 2 + deslocamentoHorizontal + fatorDeAmpliacao * inversoZ * x * 2);
  yProjetado = (int)(alturaTela / 2 + fatorDeAmpliacao * inversoZ * y);

  indice = xProjetado + yProjetado * larguraTela;
  if (indice >= 0 && indice < larguraTela * alturaTela) {
    if (inversoZ > bufferProfundidade[indice]) {
      bufferProfundidade[indice] = inversoZ;
      bufferCaracteres[indice] = ch;
    }
  }
}

int main() {
  float cuboX, cuboY;
  int k;

  printf("\x1b[2J");
  while (1) {
    memset(bufferCaracteres, codigoASCIIFundo, larguraTela * alturaTela);
    memset(bufferProfundidade, 0, larguraTela * alturaTela * 4);
    larguraCubo = 20;
    deslocamentoHorizontal = -2 * larguraCubo;
    // primeiro cubo
    for (cuboX = -larguraCubo; cuboX < larguraCubo; cuboX += incrementoVelocidade) {
      for (cuboY = -larguraCubo; cuboY < larguraCubo;
           cuboY += incrementoVelocidade) {
        calcularParaSuperficie(cuboX, cuboY, -larguraCubo, '@');
        calcularParaSuperficie(larguraCubo, cuboY, cuboX, '$');
        calcularParaSuperficie(-larguraCubo, cuboY, -cuboX, '~');
        calcularParaSuperficie(-cuboX, cuboY, larguraCubo, '#');
        calcularParaSuperficie(cuboX, -larguraCubo, -cuboY, ';');
        calcularParaSuperficie(cuboX, larguraCubo, cuboY, '+');
      }
    }
    larguraCubo = 10;
    deslocamentoHorizontal = 1 * larguraCubo;
    // segundo cubo
    for (cuboX = -larguraCubo; cuboX < larguraCubo; cuboX += incrementoVelocidade) {
      for (cuboY = -larguraCubo; cuboY < larguraCubo;
           cuboY += incrementoVelocidade) {
        calcularParaSuperficie(cuboX, cuboY, -larguraCubo, '@');
        calcularParaSuperficie(larguraCubo, cuboY, cuboX, '$');
        calcularParaSuperficie(-larguraCubo, cuboY, -cuboX, '~');
        calcularParaSuperficie(-cuboX, cuboY, larguraCubo, '#');
        calcularParaSuperficie(cuboX, -larguraCubo, -cuboY, ';');
        calcularParaSuperficie(cuboX, larguraCubo, cuboY, '+');
      }
    }
    larguraCubo = 5;
    deslocamentoHorizontal = 8 * larguraCubo;
    // terceiro cubo
    for (cuboX = -larguraCubo; cuboX < larguraCubo; cuboX += incrementoVelocidade) {
      for (cuboY = -larguraCubo; cuboY < larguraCubo;
           cuboY += incrementoVelocidade) {
        calcularParaSuperficie(cuboX, cuboY, -larguraCubo, '@');
        calcularParaSuperficie(larguraCubo, cuboY, cuboX, '$');
        calcularParaSuperficie(-larguraCubo, cuboY, -cuboX, '~');
        calcularParaSuperficie(-cuboX, cuboY, larguraCubo, '#');
        calcularParaSuperficie(cuboX, -larguraCubo, -cuboY, ';');
        calcularParaSuperficie(cuboX, larguraCubo, cuboY, '+');
      }
    }
    printf("\x1b[H");
    for (k = 0; k < larguraTela * alturaTela; k++) {
      putchar(k % larguraTela ? bufferCaracteres[k] : 10);
    }

    anguloA += 0.05;
    anguloB += 0.05;
    anguloC += 0.01;
    usleep(8000 * 2);
  }
  return 0;
}
