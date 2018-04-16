#ifndef POSICAO_H
#define POSICAO_H

// os objetos criados por essa classe servem apenas para armazenar informações relativas a posição do agente

class posicao
{
public:
    posicao();

    void acumulaX (double dx);        // método para adicionar valores dx e dy respectivamente a x e y
    void acumulaY (double dy);

    double getX() const;
    double getY() const;

    void setX(double value);
    void setY(double value);

private:
    double x,y;     // coordenadas x e y do posicionamento do agente

};

#endif // POSICAO_H
