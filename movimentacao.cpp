#include "movimentacao.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.1416             // o valor de PI é usado no método "movimento"

movimentacao::movimentacao()
{
    // inicializando todas as váriaveis

    this->anguloDeVisao=0;
    this->direcao=0;
    this->passo=0;
    this->dx=0;
    this->dy=0;
}

void movimentacao::movimento()
{
    // calcula um novo dx e um novo dy considerando que os agentes se movem com um ângulo limite de manobra

    double variacaoDeDirecao=(double)rand()/RAND_MAX;
    this->direcao+=(variacaoDeDirecao-0.5)*this->anguloDeVisao;
    this->dx=this->passo*cos(this->direcao*PI/180);
    this->dy=this->passo*sin(this->direcao*PI/180);
}

void movimentacao::parado()
{
    // como o nome sugere, escolhe dx e dy iguais a zero para que o agente não se movimente ao atuar

    this->dx=0;
    this->dy=0;
}

double movimentacao::getDx() const
{
    return dx;
}

double movimentacao::getDy() const
{
    return dy;
}

void movimentacao::setDirecao(double value)
{
    direcao = value;
}

void movimentacao::setAnguloDeVisao(double value)
{
    anguloDeVisao = value;
}

void movimentacao::setPasso(double value)
{
    passo = value;
}

double movimentacao::getPasso() const
{
    return passo;
}
