#ifndef AGENTE_H
#define AGENTE_H
#include <vector>           // necessario para criar a vizinhança
#include <stdlib.h>         // necessario para usar o rand
#include "posicao.h"        // cria objetos para armazenar os posicionamentos dos agentes
#include "movimentacao.h"   // cria objetos com as regras de regras de movimentação

using namespace std;

class agente
{
public:
    agente(bool macho);          // variavel booleana para determinar se o agente é macho

    // temos abaixo um segundo construtor para quando os agentes sugirem pela reprodução sexuada

    agente(bool macho, double geneMaeI, double geneMaeII,
           double genePaiI, double genePaiII, posicao localMae);

    /*#### metodos chamados pelo ambiente ####*/

    void rodaModelo();                  // método que vai rodar o modelo de mundo do nosso agente
    void atuacao();                     // método responsavel pela atuação do agente
    void fimDeAcasalamento();         // seta o relacionamentoSerio como false

    /*#### métodos que lidam com a vizinhança de um agente ####*/

    void limparVizinhanca();
    void adicionarVizinhanca(agente *vizinho);

    /*#### métodos get end set ####*/

    posicao getLocal() const;
    double getRaioVizinhanca() const;
    bool getEhMacho() const;
    agente *getParceiro() const;
    double getGeneCantoI() const;
    double getGeneCantoII() const;
    double getValorCanto() const;
    vector <agente *> getVizinhanca() const;
    bool getAdulto() const;
    bool getFlertando() const;
    void setFlertando(bool value);
    int getIdade() const;

private:
    /*#### variaveis de estado ####*/

    bool ehMacho;                  // determina se temos um agente macho ou femea
    bool adulto;                   // para diferenciar adultos e filhotes
    bool flertando;                // passaro em epoca de acasalamento
    int contadorDeMeses;           // conta os meses do ano
    int tempoDeMaturacao;          // *meses para que um filhote atinja a maturacao sexual
    int idade;                     // tempo de vida em anos do passaro

    /*#### variaveis espaciais e de interacao ####*/

    posicao local;                 // objeto que possui a posicao do agente
    movimentacao movimento;        // objeto que possui as regras de movimentação
    vector <agente *> vizinhanca;  // vetor de ponteiros com vizinhos de um determinado agente
    agente *parceiro;              // ponteiro que aponta para o parceiro de uma fêmea
    double raioVizinhanca;         // *tamanho da vizinhanca de um agente

        // o conceito de vizinho usado nesse código é:
            // um pássaro de qualquer sexo é vizinho ao outro
            //caso ambos fossem machos um pudesse ouvir o outro

    /*#### variaveis relacionadas aos cantos ####*/

    double geneCantoI;               // gene de canto numero 1
    double geneCantoII;              // gene de canto numero 2
    double inicioRangeCanto;         // media aritmetica entre os genes 1 e 2
    double rangeCanto;               // *tamanho da faixa de canto reconhecível
    double valorCanto;               // valor referente ao canto que o macho canta
    double passoMutacao;             // *tamanho do passo de mutação dado
    double probabilidadeMutacao;     // *probabilidade de ocorrer mutação
    double probabilidadeAcasalamento;// *probabilidade de a fêmea acasalar com um macho

    /*#### alguns métodos auxiliares privados ####*/

    void aprenderCanto();                 // método chamado para que o agente aprenda seu canto
    void condicoesDeContorno();           // aplica as condições de contorno do agente
    void namoraComigo();                  // torna um passaro macho 'parceiro' de um passaro femea
    void selecionarParceiro();            // método que identifica o parceiro de determinado agente
};

#endif // AGENTE_H
