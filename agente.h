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

    agente(bool macho, double geneMae, double genePai, posicao localMae);

    void rodaModelo();                  // método que vai rodar o modelo de mundo do nosso agente
    void atuacao();                     // método responsavel pela atuação do agente
    void aprenderCanto();               // método chamado para que o agente aprenda seu canto
    void fimDeRelacionamento();         // seta o relacionamentoSerio como false

    // métodos que lidam com a vizinhança de um agente

    void limparVizinhanca();
    void adicionarVizinhanca(agente *vizinho);

    // métodos 'get' usados para transmitir a informação para as demais camadas do modelo

    posicao getLocal() const;
    double getRaioVizinhanca() const;
    bool getRelacionamentoSerio() const;
    bool getEhMacho() const;
    agente *getParceiro() const;
    double getGeneCanto() const;
    double getValorCanto() const;
    vector <agente *> getVizinhanca() const;

    int inutil=0;

private:
    int RG;                        // ESSA VARIAVEL NÃO FOI CORRETAMENTE UTILIZADAD
    bool ehMacho;                  // "ehMacho=true" o agente é macho, "ehMacho=false" o agente é fêmea
    posicao local;                 // objeto da classe posicao chamado local (posição do agente)
    movimentacao caminhada;        // objeto  da classe movimentacao chamado caminhada (possui as regras de movimentação)
    vector <agente *> vizinhanca;  // vetor de ponteiros com vizinhos de um determinado agente
    agente *parceiro;              // ponteiro que aponta para o parceiro de uma fêmea
    double raioVizinhanca;         // o tamanho da vizinhança no qual consideraremos agentes como vizinhos

    // o conceito de vizinho usado nesse código é:
    // um pássaro de qualquer sexo é vizinho ao outro se caso ambos fossem machos um pudesse ouvir o outro

    // abaixo temos váriaveis relacionadas aos cantos que são reconhecidos

    double geneCanto;           // uma única variavel que determina onde começa genéticamente o range de cantos reconhecidos
    double valorCanto;          // valor referente ao canto que o macho canta (pode ser ou não determinado por aprendizagem)
    double rangeGeneCanto;      // tamanho da faixa de canto reconhecível
    double passoMutacao;        // tamanho do passo de mutação dado
    double probabilidadeMutacao;// probabilidade de ocorrer mutação

    // será necessário uma variavel booleana para definir "status de relacionamento" de um pássaro

    bool relacionamentoSerio;     // o pássaro já encontrou um par e aguarda a temporada de acasalamento

    // alguns métodos auxiliares privados

    void condicoesDeContorno();           // aplica as condições de contorno do agente (DEVERIA ESTAR EM MOVIMENTACAO)
    void namoraComigo();                  // torna um passaro macho 'parceiro' de um passaro femea
    void selecionarParceiro();            // método que identifica o parceiro de determinado agente
};

#endif // AGENTE_H
