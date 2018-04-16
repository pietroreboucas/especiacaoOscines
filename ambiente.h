#ifndef AMBIENTE_H
#define AMBIENTE_H
#include "agente.h"       // classe do agente
#include <vector>         // colocamos os agentes em um vetor para trabalhar com eles

using namespace std;


class ambiente
{  
public:
    ambiente();                 // construtor do ambiente

    void rodaMundo();           // faz uma iteração em nossa simulação

    // métodos para transferir informações sobre os agentes para o dialog

    posicao getLocal(int i);
    int getNumeroPassaros();
    double getTamanhoDoMundo() const;
    bool getEhMacho(int i);
    bool getRelacionamentoSerio(int i);
    double getGeneCanto(int i);

    // métodos para usar no modo tela preta

    int getNumeroGeracoes();
    bool getEspeciacao() const;

    // métodos para usar na versão mais nova do dialog

    int getContadorDeIteracoes() const;
    int getDistanciaGene() const;

private:
    vector <agente> bando;      // vetor onde serão colocados os agentes
    vector <int> sorteioBando;  // vetor usado para fazer sorteios
    double tamanhoDoMundo;      // tamanho do mundo
    int capacidadeDeSuporte;    // número máximo de pássaros na simulação
    bool epocaDeAcasalamento;   // se "epocaDeAcasalamento = true" então pássaros com parceiros terão filhos
    bool especiacao;            // se o critério de especiação foi atingido retorna true
    int contadorDeGeracoes;
    int contadorDeIteracoes;
    int distanciaGene;

    // métodos definidos para gerenciar partes diferentes de "roda mundo"

    void atualizaPercepcao();       // atualiza quem está nas suas vizinhanças
    void rodaModelo();              // roda o modelo de mundo de cada agente
    void realizaAtuacao();          // realiza a atuação de cada agente
    void rodaAcasalamento();        // faz com pássaros em relacionamento sério tenham filhos
    void derivaGenica();            // mata aleatóriamente passáros para que a população não seja maior que K
    void acabandoRelacionamentos(); // seta todos os agente como relacionamenteoSerio=false
    void aulaDeCanto();             // faz com que os pássaros machos aprendam seus cantos

    // alguns outros métodos privados

    double calcularDistancia(int i, int j);// método para calcular a distância entre dois agentes
    bool parceirosSuficientes();           // contará quantos agentes possuem parceiros e dirá se é época de acasalameto
    bool criterioDeEspeciacao();
};

#endif // AMBIENTE_H
