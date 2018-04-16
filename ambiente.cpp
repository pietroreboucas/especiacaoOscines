#include "ambiente.h"
#include <math.h>
#include <algorithm>

ambiente::ambiente()
{
    // inicializamos as variaveis do ambiente;

    this->tamanhoDoMundo=1000;
    this->capacidadeDeSuporte=10000;
    this->epocaDeAcasalamento=false;
    this->contadorDeGeracoes=0;
    this->contadorDeIteracoes=0;
    this->especiacao=false;

    // vamos definir a capacidade do vetor de agentes como o dobro da capacidade de suporte

    this->bando.reserve(this->capacidadeDeSuporte*2);

    // agora vamos criar nosso agentes

    for (int i=0; i<(this->capacidadeDeSuporte/2); i++)
    {
        this->sorteioBando.push_back(i);
        agente piupiu(true);
        this->bando.push_back(piupiu);
    }
    for (int i=0; i<(this->capacidadeDeSuporte/2); i++)
    {
        this->sorteioBando.push_back(i+(capacidadeDeSuporte/2));
        agente piupiu(false);
        this->bando.push_back(piupiu);
    }

    this->distanciaGene=0;
}

void ambiente::rodaMundo()
{
    // primeiramente checaremos se nosso bando possui parceiros o suficiente para uma época de acasalamento

    this->epocaDeAcasalamento=this->parceirosSuficientes();

    if (this->epocaDeAcasalamento==true)
    {
        this->rodaAcasalamento();
        this->acabandoRelacionamentos();
//        this->atualizaPercepcao();
//        this->aulaDeCanto();
        this->derivaGenica();       
        this->epocaDeAcasalamento=false;
        this->contadorDeIteracoes=0;
        this->contadorDeGeracoes++;
        this->especiacao=this->criterioDeEspeciacao();
    }
    else
    {
        this->atualizaPercepcao();
        this->rodaModelo();
        this->realizaAtuacao();

        this->contadorDeIteracoes++;
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

int ambiente::getNumeroGeracoes()
{
    return this->contadorDeGeracoes;
}

bool ambiente::getEspeciacao() const
{
    return especiacao;
}

int ambiente::getContadorDeIteracoes() const
{
    return contadorDeIteracoes;
}

int ambiente::getDistanciaGene() const
{
    return distanciaGene;
}

void ambiente::atualizaPercepcao()
{
    double distancia;   // variavel auxiliar na qual armazenaremos a distancia entre os agentes a cada interação

    for (int i=0; i<this->bando.size(); i++)                  // para cada agente
    {
        if (this->bando[i].getRelacionamentoSerio()==false)   // que não está em um relacionamento sério
        {
            this->bando[i].limparVizinhanca();                // limpamos a vizinhança
            for (int j=0; j<this->bando.size(); j++)          // vamos então para cada outro agente
            {

                if (this->bando[j].getEhMacho()==true)        // se for macho
                {
                    distancia=this->calcularDistancia(i,j);            // calculamos a distância
                    if (distancia<this->bando[i].getRaioVizinhanca())  // se essa for menor que o raioVizinhanca
                    {
                        this->bando[i].adicionarVizinhanca(&this->bando[j]);   // adiciona ele a vizinhança
                    }
                }
            }
        }
    }


}

void ambiente::rodaModelo()
{
    // primeiramente vamos embaralhar o vetor de sorteio

    shuffle(sorteioBando.begin(),sorteioBando.end(),std::default_random_engine(1));

    // agora vamos rodar o modelo de mundo de cada agente na ordem sorteada pelo vetor

    for (int i=0; i<this->bando.size(); i++)
    {
        this->bando[sorteioBando[i]].rodaModelo();
    }
}

void ambiente::realizaAtuacao()
{
    // em nossa atuação a unica coisa que acontece é movimento (ou não), então realmente não importa quem vai primeiro

    for (int i=0; i<this->bando.size(); i++)
    {
        this->bando[i].atuacao();
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

void ambiente::derivaGenica()
{
    // primeiramente vamos misturar as posições dos agentes no vetor de sorteio para que nenhum deles seja privilegiado

    this->sorteioBando.clear();   // limpamos o vetor de sorteio

    for (int i=0; i<this->bando.size(); i++)   // criamos um novo com o número certo de agentes
    {
        this->sorteioBando.push_back(i);
    }

    shuffle(sorteioBando.begin(),sorteioBando.end(),std::default_random_engine(1));

    // quando o tamanho do bando é maior que a capacidade de suporte do ambiente
    // os agentes acima da capacidade de suporte são deletados

    if (this->bando.size()>=capacidadeDeSuporte)
    {
        int mortes=(this->bando.size()-capacidadeDeSuporte);
        for (int i=0; i<mortes; i++)
            this->bando.erase(this->bando.begin()+this->sorteioBando[i]);
    }

    // vamos sortear novamente o vetor de sorteio por via das dúvidas

    this->sorteioBando.clear();   // limpamos o vetor de sorteio

    for (int i=0; i<this->bando.size(); i++)   // criamos um novo com o número certo de agentes
    {
        this->sorteioBando.push_back(i);
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

bool ambiente::parceirosSuficientes()
{
    int numeroPassarosEmCasal=0;                // contador do número de pássaros em um casal

    // checaremos um a um se os pássaros estão em um casal e acrescentaremos +1 ao contador sempre que sim

    for (int i=0; i<this->bando.size(); i++)
    {
        if (this->bando[i].getRelacionamentoSerio()==true)
        {
            numeroPassarosEmCasal++;
        }
    }
    // queremos que quando o número de passáros em casais seja maior que metade da capacidade de suporte
    // se diga que há parceiros o suficiente para uma época de acasalamento
    // essa checagem será feita no inicio de cada rodaMundo
    
    if (numeroPassarosEmCasal>=(this->capacidadeDeSuporte/2))
    {
        return true;     // há parceiros o suficiente para uma época de acasalamento o/
    }
    else
    {
        return false;    // não há parceiros o suficiente =(
    }
}

bool ambiente::criterioDeEspeciacao()
{
    double geneMenor=this->bando[0].getGeneCanto();
    double geneMaior=this->bando[0].getGeneCanto();

    for (int i=0; i<this->bando.size(); i++)
    {
        if(this->bando[i].getGeneCanto()<geneMenor)
        {
            geneMenor=this->bando[i].getGeneCanto();
        }
        if(this->bando[i].getGeneCanto()>geneMaior)
        {
            geneMaior=this->bando[i].getGeneCanto();
        }
    }

    this->distanciaGene=(int)(geneMaior-geneMenor)*100;

    if ((geneMaior-geneMenor)<1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

