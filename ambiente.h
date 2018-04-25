#ifndef AMBIENTE_H
#define AMBIENTE_H
#include "agente.h"       // classe do agente
#include <vector>         // colocamos os agentes em um vetor para trabalhar com eles

using namespace std;


class ambiente
{  
public:
    ambiente();                 // construtor do ambiente

    void rodaGeracao();         // faz uma iteração em nossa simulação

    // métodos para transferir informações sobre os agentes para o dialog

    posicao getLocal(int i);
    int getNumeroPassaros();
    double getTamanhoDoMundo() const;
    bool getEhMacho(int i);
    bool getRelacionamentoSerio(int i);
    double getGeneCanto(int i);
    int getContadorDeGeracoes() const;

private:
    vector <agente> bando;      // vetor onde serão colocados os agentes
    vector <int> passaro;        // vetor usado para fazer sorteios
    double tamanhoDoMundo;      // tamanho do mundo
    int capacidadeDeSuporte;    // número máximo de pássaros na simulação
    bool especiacao;            // se o critério de especiação foi atingido retorna true
    int contadorDeGeracoes;     // ... um contador de gerações

    // métodos definidos para gerenciar partes diferentes de "rodaGeracao"

    void atualizaPercepcao(int i);  // atualiza quem está na vizinhança do passaro i
    void rodaAcasalamento();        // faz com pássaros em relacionamento sério tenham filhos
    void genocidio();               // mata pássaros aleatóriamente até K
    void acabandoRelacionamentos(); // seta todos os agente como relacionamenteoSerio=false
    void aulaDeCanto();             // faz com que os pássaros machos aprendam seus cantos

    // alguns outros métodos privados

    double calcularDistancia(int i, int j);// método para calcular a distância entre dois agentes
};

#endif // AMBIENTE_H
