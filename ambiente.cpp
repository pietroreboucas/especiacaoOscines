#include "ambiente.h"
#include <math.h>
#include <algorithm>

ambiente::ambiente()
{
    // inicializamos as variaveis do ambiente;

    this->tamanhoDoMundo=1000;
    this->capacidadeDeSuporte=10000;
    this->contadorDeGeracoes=0;

    // vamos definir a capacidade do vetor de agentes como o triplo da capacidade de suporte

    this->bando.reserve(this->capacidadeDeSuporte*3);

    // agora vamos criar nosso agentes

    for (int i=0; i<(this->capacidadeDeSuporte/2); i++)
    {
        this->passaro.push_back(i);
        agente piupiu(true);
        this->bando.push_back(piupiu);
    }
    for (int i=0; i<(this->capacidadeDeSuporte/2); i++)
    {
        this->passaro.push_back(i+(capacidadeDeSuporte/2));
        agente piupiu(false);
        this->bando.push_back(piupiu);
    }
}

void ambiente::rodaGeracao()
{
    // cada geracao corresponde a 1 ano
    // o 12 mes do ano corresponde a época de acasalamento

    for(int mes=1;mes<=12;mes++)
    {
        if (mes==12)
        {
            this->rodaAcasalamento();
            this->acabandoRelacionamentos();
    //        this->atualizaPercepcao();
    //        this->aulaDeCanto();
            this->genocidio();
        }
        else
        {
            // primeiramente embaralhamos o vetor de sorteio

            shuffle(passaro.begin(),passaro.end(),std::default_random_engine(1));

            // vamos então cada agente vai agir na ordem do vetor de sorteio

            for (int j=0; j<this->bando.size(); j++)
            {
                this->atualizaPercepcao(this->passaro[j]);   // percepcao
                this->bando[this->passaro[j]].rodaModelo();  // modelo de mundo
                this->bando[this->passaro[j]].atuacao();     // atuacao
            }
        }
    }
}

posicao ambiente::getLocal(int i)
{
    posicao local=this->bando[i].getLocal();
    local.setX(local.getX()*this->tamanhoDoMundo);
    local.setY(local.getY()*this->tamanhoDoMundo);
    return local;
}

int ambiente::getNumeroPassaros()
{
    return this->bando.size();
}

double ambiente::getTamanhoDoMundo() const
{
    return tamanhoDoMundo;
}

bool ambiente::getEhMacho(int i)
{
    return this->bando[i].getEhMacho();
}

bool ambiente::getRelacionamentoSerio(int i)
{
    return this->bando[i].getRelacionamentoSerio();
}

double ambiente::getGeneCanto(int i)
{
    return this->bando[i].getGeneCanto();
}

int ambiente::getContadorDeGeracoes() const
{
    return contadorDeGeracoes;
}

void ambiente::atualizaPercepcao(int i)
{
    double distancia;   // distancia entre os agentes i e j

    this->bando[i].limparVizinhanca();                // limpamos a vizinhança

    for (int j=0; j<this->bando.size(); j++)          // então para cada outro agente
    {

        if (this->bando[j].getEhMacho()==true)        // se for macho
        {
            distancia=this->calcularDistancia(i,j);            // calculamos a distância
            if (distancia<this->bando[i].getRaioVizinhanca())  // caso menor que o raioVizinhanca
            {
                this->bando[i].adicionarVizinhanca(&this->bando[j]);   // adiciona a vizinhança
            }
        }
    }

}

void ambiente::rodaAcasalamento()
{
    // vamos fazer com que cada fêmea em um relacionamentoSerio gere dois passaros, um macho e uma fêmea
    
    for (int i=0; i<this->bando.size(); i++)      // pegaremos um a um os agentes
    {
        if (this->bando[i].getEhMacho()==false)   // que são fêmeas
        {
            if(this->bando[i].getRelacionamentoSerio()==true)   // que estão em um relacionamentoSerio
            {
                // agora precisaremos do construtor de agente que criamos para reprodução sexuada

                // primeiramente pegamos os valores do geneMae, genePai, o localMae e a vizinhança da mãe

                double geneMae=this->bando[i].getGeneCanto();
                double genePai=this->bando[i].getParceiro()->getGeneCanto();
                posicao localMae=this->bando[i].getLocal();

                // vamos então criar um macho e uma fêmea e acrescentar ao vetor bando

                // essa parte comentada abaixo faz com que o agente surja na vizinhança da mãe, ao invés de surgir
                // no mesmo sítio que ela
                /*double dado=(double)rand()/RAND_MAX;
                localMae.acumulaX(-0.1+dado*0.2);
                dado=(double)rand()/RAND_MAX;
                localMae.acumulaY(-0.1+dado*0.2);*/

                agente winnie(false, geneMae, genePai, localMae);     // criando a fêmea
                this->bando.push_back(winnie);                        // acrescentando ao vetor

                /*localMae=this->bando[i].getLocal();
                dado=(double)rand()/RAND_MAX;
                localMae.acumulaX(-0.1+dado*0.2);
                dado=(double)rand()/RAND_MAX;
                localMae.acumulaY(-0.1+dado*0.2);*/

                agente picapau(true, geneMae, genePai, localMae);      // criando o macho
                this->bando.push_back(picapau);                        // acrescentando ao vetor
            }
        }
    }     
}

void ambiente::genocidio()
{
    // primeiramente vamos misturar as posições dos agentes no vetor de sorteio para que nenhum deles seja privilegiado

    this->passaro.clear();   // limpamos o vetor de sorteio

    for (int i=0; i<this->bando.size(); i++)   // criamos um novo com o número certo de agentes
    {
        this->passaro.push_back(i);
    }

    shuffle(passaro.begin(),passaro.end(),std::default_random_engine(1));

    // quando o tamanho do bando é maior que a capacidade de suporte do ambiente
    // os agentes acima da capacidade de suporte são deletados

    if (this->bando.size()>=capacidadeDeSuporte)
    {
        int mortes=(this->bando.size()-capacidadeDeSuporte);

        for (int i=0; i<mortes; i++)
        {
            this->bando.erase(this->bando.begin()+this->passaro[i]);
        }
    }

    // vamos sortear novamente o vetor de sorteio por via das dúvidas

    this->passaro.clear();   // limpamos o vetor de sorteio

    for (int i=0; i<this->bando.size(); i++)   // criamos um novo com o número certo de agentes
    {
        this->passaro.push_back(i);
    }
}

void ambiente::acabandoRelacionamentos()
{
    for (int i=0; i<this->bando.size(); i++)
    {
        this->bando[i].fimDeRelacionamento();
    }
}

void ambiente::aulaDeCanto()
{
    int primeiroFilhote=this->capacidadeDeSuporte;
    int numeroFilhotes=this->bando.size()-this->capacidadeDeSuporte;

    for (int i=primeiroFilhote; i<(primeiroFilhote+numeroFilhotes); i++)
    {
        if (this->bando[i].getEhMacho()==true)
        {
            this->bando[i].aprenderCanto();
        }
    }
}

double ambiente::calcularDistancia(int i, int j)
{
    double distancia=0;      // váriavel que usaremos para armazenar a distância entre dois agentes

    posicao localI,localJ;   // objetos para armazenar temporariamente os objetos "local" de ambos os agentes

    localI=this->bando[i].getLocal();   // armazenando as localizações de cada agente em um dos objetos
    localJ=this->bando[j].getLocal();

    // vamos agora usar essas informações que armazenamos para calcular a distância entre eles usando pitagoras

    double deltaX,deltaY;    // váriaveis auxíliares para o calculo da distância

    deltaX=localI.getX()-localJ.getX();
    deltaY=localI.getY()-localJ.getY();

    distancia=sqrt(deltaX*deltaX+deltaY*deltaY);  // calculo da distância usando pitagoras

    return distancia;
}
