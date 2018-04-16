#include "posicao.h"

posicao::posicao()
{

}

void posicao::acumulaX(double dx)
{
    this->x+=dx;
}

void posicao::acumulaY(double dy)
{
    this->y+=dy;
}

double posicao::getX() const
{
    return x;
}

double posicao::getY() const
{
    return y;
}

void posicao::setX(double value)
{
    x = value;
}

void posicao::setY(double value)
{
    y = value;
}
