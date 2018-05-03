#include "ambiente.h"
#include <math.h>
#include <algorithm>

ambiente::ambiente()
{
    // inicializamos as variaveis do ambiente;

    this->tamanhoDoMundo=1000;
    this->capacidadeDeSuporte=10000;
    this->contadorDeGeracoes=0;
    this->epocaDeAcasalamento=false;
    this->idadeMaxima=3;

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
        if (mes==12) this->epocaDeAcasalamento=true;

        // embaralhamos o vetor de sorteio

        shuffle(passaro.begin(),passaro.end(),std::default_random_engine(1));

        // vamos então cada agente vai agir na ordem do vetor de sorteio

        for (unsigned int j=0; j<this->bando.size(); j++)
        {
            this->atualizaPercepcao(this->passaro[j]);        // percepcao
            this->bando[this->passaro[j]].rodaModelo();       // modelo de mundo
            this->bando[this->passaro[j]].atuacao();          // atuacao
        }

        if (epocaDeAcasalamento==true)
        {
            this->rodaAcasalamento();
            this->mortesPorIdade();
            this->mortesAleatorias();
            this->acabandoAcasalamento();      // revisar
            this->epocaDeAcasalamento=false;
            this->contadorDeGeracoes++;
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
    return this->bando[i].getFlertando();
}

double ambiente::getGeneCanto(int i)
{
    return this->bando[i].getGeneCantoI();
}

int ambiente::getContadorDeGeracoes() const
{
    return contadorDeGeracoes;
}

void ambiente::atualizaPercepcao(int i)
{
    double distancia;   // distancia entre os agentes i e j

    this->bando[i].limparVizinhanca();                    // limpamos a vizinhança

    if (this->epocaDeAcasalamento==true)
    {
        this->bando[i].setFlertando(true);
    }

    if (((this->bando[i].getEhMacho()==true) & (this->bando[i].getAdulto()==false))^
       ((this->bando[i].getEhMacho()==false) & (this->bando[i].getFlertando()==true)))
    {
        for (unsigned int j=0; j<this->bando.size(); j++) // então para cada outro agente
        {

            if (this->bando[j].getEhMacho()==true)        // se for macho
            {
                distancia=this->calcularDistancia(i,j);            // calculamos a distância
                if (distancia<this->bando[i].getRaioVizinhanca())  // se menor que raioVizinhanca
                {
                    this->bando[i].adicionarVizinhanca(&this->bando[j]); // adiciona a vizinhança
                }
            }
        }
    }
}

void ambiente::rodaAcasalamento()
{
    // vamos fazer com que cada fêmea em um relacionamento gere dois passaros um de cada sexo
    
    for (unsigned int i=0; i<this->bando.size(); i++)      // pegaremos um a um os agentes
    {
        if (this->bando[i].getEhMacho()==false)   // que são fêmeas
        {
            if(this->bando[i].getFlertando()==true)   // que estão em um relacionamento
            {
                // agora precisaremos do construtor de agente que criamos para reprodução sexuada

                // primeiramente pegamos os valores necessarios para criacao do filhote

                double geneMaeI=this->bando[i].getGeneCantoI();
                double geneMaeII=this->bando[i].getGeneCantoII();
                double genePaiI=this->bando[i].getParceiro()->getGeneCantoI();
                double genePaiII=this->bando[i].getParceiro()->getGeneCantoII();
                posicao localMae=this->bando[i].getLocal();

                // vamos então criar um macho e uma fêmea e acrescentar ao vetor bando

                // criando a fêmea:

                agente winnie(false, geneMaeI, geneMaeII, genePaiI, genePaiII, localMae);
                this->bando.push_back(winnie);                  // acrescentando ao vetor

                /*localMae=this->bando[i].getLocal();
                dado=(double)rand()/RAND_MAX;
                localMae.acumulaX(-0.1+dado*0.2);
                dado=(double)rand()/RAND_MAX;
                localMae.acumulaY(-0.1+dado*0.2);*/

                // criando o macho

                agente picapau(true, geneMaeI, geneMaeII, genePaiI, genePaiII, localMae);
                this->bando.push_back(picapau);                  // acrescentando ao vetor
            }
        }
    }
}

void ambiente::mortesPorIdade()
{
    for (int i=(this->bando.size()-1);i>=0;i--)            // essa contagem é decrescente
    {
        if (this->bando[i].getIdade()>=this->idadeMaxima)  // se a idade for maior/igual a maxima
        {
            this->bando.erase(bando.begin()+i);            // apaga o passaro
        }
    }
}

void ambiente::mortesAleatorias()
{
    // CHECAR ESSE ALGORITIMO DEPOIS (otimizar)

    // primeiramente vamos misturar as posições dos agentes no vetor de sorteio

    this->passaro.clear();   // limpamos o vetor de sorteio

    for (unsigned int i=0; i<this->bando.size(); i++) // criamos um com o número certo de agentes
    {
        this->passaro.push_back(i);
    }

    shuffle(passaro.begin(),passaro.end(),std::default_random_engine(1));

    // quando o tamanho do bando é maior que a capacidade de suporte do ambiente
    // os agentes acima da capacidade de suporte são deletados

    if (this->bando.size()>=capacidadeDeSuporte)
    {
        int mortes=(this->bando.size()-capacidadeDeSuporte);

        for (int j=0; j<mortes; j++)
        {
            int l=j;

            while (this->passaro[j]>=this->bando.size())
            {
                l++;
            }

            this->bando.erase(bando.begin()+this->passaro[l]);
        }
    }

    // vamos sortear novamente o vetor de sorteio por via das dúvidas

    this->passaro.clear();   // limpamos o vetor de sorteio

    for (unsigned int m=0; m<this->bando.size(); m++) // criamos um com o número certo de agentes
    {
        this->passaro.push_back(m);
    }
}

void ambiente::acabandoAcasalamento()
{
    for (unsigned int i=0; i<this->bando.size(); i++)
    {
        this->bando[i].fimDeAcasalamento();
    }
}

double ambiente::calcularDistancia(int i, int j)
{
    double distancia=0;      // distância entre dois agentes

    posicao localI,localJ;   // objetos "local" de ambos os agentes

    localI=this->bando[i].getLocal();
    localJ=this->bando[j].getLocal();

    // vamos calcular a distância entre eles usando pitagoras

    double deltaX,deltaY;    // váriaveis auxíliares para o calculo da distância

    deltaX=localI.getX()-localJ.getX();
    deltaY=localI.getY()-localJ.getY();

    distancia=sqrt(deltaX*deltaX+deltaY*deltaY);  // calculo da distância usando pitagoras

    return distancia;
}
