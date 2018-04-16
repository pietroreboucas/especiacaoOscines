#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

// essa classe foi criada com o objetivo de encapsular as regras de movimentação do agente
// ela dará ao agente números (dx e dy) para somar a sua atual posição para que ele mude de lugar

class movimentacao
{
public:
    movimentacao();
    void movimento();         // método que calcula o novo dx e o novo dy
    void parado();            // método que impõe dx=0 e dy=0

    double getDx() const;     // métodos para que os objetos da classe agente consigam acessar dx e dy
    double getDy() const;

    // métodos para setar os valores dos parâmetros de movimento

    void setDirecao(double value);
    void setAnguloDeVisao(double value);
    void setPasso(double value);
    double getPasso() const;

private:
    double dx,dy;              // essas são as variaveis que serão somadas ao posicionamento do agente
                               // creio que elas existirão independente da estratégia de movimentaçãoa adotada

    // abaixo estão as variaveis usadas para implementar uma movimentação simples
    // com ângulo de visão e passo de tamanho fixo

    double direcao;            // a direção para qual o passo deve ser dado
    double anguloDeVisao;      // o ângulo de visão do agente
    double passo;              // o tamanho do passo
};

#endif // MOVIMENTACAO_H
