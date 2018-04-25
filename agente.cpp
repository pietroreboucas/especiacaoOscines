#include "agente.h"
#include <vector>
#include <algorithm>

agente::agente(bool macho)
{
    // esse construtor só servirá para criar os agentes que serão dispostos aleatóriamente no início da simulação

    static int contador=0;                                       // adicionando um valor ao RG para o pássaro
    this->RG=contador++;

    this->local.setX((double)rand()/RAND_MAX);                   // inicializando as variáveis do objeto "local"
    this->local.setY((double)rand()/RAND_MAX);                   // da classe "posição"

    this->movimento.setPasso(0.001);                             // inicializando as varáveis do objeto "caminhada"
    this->movimento.setDirecao((double)rand()/RAND_MAX*360);     // da classe "movimentação"
    this->movimento.setAnguloDeVisao(40);

    this->passoMutacao=0.02;
    this->probabilidadeMutacao=0.01;

    // vamos inicializar os valores do gene e o valor do canto inicial de cada agente da simulação
    // o valor de geneCanto é o valor inicial do range de cantos reconhecíveis

    this->rangeGeneCanto=1;
    this->geneCanto=2.4+0.2*((double)rand()/RAND_MAX);
    this->valorCanto=(this->geneCanto)+(this->rangeGeneCanto)*((double)rand()/RAND_MAX);

    // incicializando as demais variaveis

    this->ehMacho=macho;                                 // assinalando o agente como macho (ou fêmea)
    this->relacionamentoSerio=false;                     // definindo o status de relacionamento do novo pássaro
    this->raioVizinhanca=10*this->movimento.getPasso();  // inicializando raio da vizinhança
}

agente::agente(bool macho, double geneMae, double genePai, posicao localMae)
{
    // esse construtor criará os agentes que surgirão por reprodução sexuada

    static int contador=0;                                       // adicionando um valor ao RG para o pássaro
    this->RG=contador++;

    this->local=localMae;   // inicializando as variáveis do objeto "local" será iniciada com o posicionamento da mãe

    this->movimento.setPasso(0.01);                              // inicializando as varáveis do objeto "caminhada"
    this->movimento.setDirecao((double)rand()/RAND_MAX*360);     // da classe "movimentação"
    this->movimento.setAnguloDeVisao(40);                        // não é diferente do outro construtor

    this->passoMutacao=0.02;
    this->probabilidadeMutacao=0.01;

    // vamos inicializar os valores do gene e o valor do canto inicial de cada agente da simulação

    this->rangeGeneCanto=1;

    // temos que jogar um dado para decidir se o filhote terá o gene da mãe ou do pai

    double dado=(double)rand()/RAND_MAX;

    if (dado<0.5)
    {
        this->geneCanto=geneMae;
    }
    else
    {
        this->geneCanto=genePai;
    }

    // tambem queremos acrescentar uma chance de mutação em relação ao gene escolhido

    dado=(double)rand()/RAND_MAX;    // rolando o dado para determinar mutação genética

    if (dado<this->probabilidadeMutacao)
    {
        //mutação que pode ser 'positiva' (aumentando o valor do gene) ou 'negativa' (diminuindo o valor do gene)

        dado=(double)rand()/RAND_MAX;

        if  (dado<0.5)
        {
            this->geneCanto+=this->passoMutacao;   // passo genético positivo
        }
        else
        {
            this->geneCanto-=this->passoMutacao;   // passo genético negativo
        }
    }

    // valor de canto aleatório dentro do range, depois faremos com que os machos aprendam seus cantos no ambiente

    this->valorCanto=(this->geneCanto)+(this->rangeGeneCanto)*((double)rand()/RAND_MAX);;

    this->ehMacho=macho;                                 // assinalando o agente como macho (ou fêmea)
    this->relacionamentoSerio=false;                     // definindo o status de relacionamento do novo pássaro
    this->raioVizinhanca=0.1;                            // inicializando raio da vizinhança
}

void agente::rodaModelo()
{
    // método que descreve o modelo de mundo do agente
    // movimento ou parado são métodos da classe movimentação e estão descritos lá

    if (this->ehMacho==false)                  // caso seja fêmea tenta encontrar um parceiro
    {
        this->selecionarParceiro();
    }

    if (relacionamentoSerio==true)             // pássaro em um relacionamento sério
    {
        this->movimento.parado();              // fica esperando a época de acasalamento
    }
    else                                       // pássaro solteiro
    {
        this->movimento.movimento();           // vai em busca de um parceiro
    }
}

void agente::atuacao()
{
    // primeiramente nos deslocamos a quantidade definida no 'rodaModelo'

    this->local.acumulaX(this->movimento.getDx());
    this->local.acumulaY(this->movimento.getDy());

    // depois do deslocamento aplicam-se as condições de contorno

    this->condicoesDeContorno();
}

void agente::aprenderCanto()
{
    // é chamado para escolher o valor do canto do agente filhote

    // definindo algumas variaveis auxiliares

    double cantoTotal=0;                    // onde vamos somar todos os cantos de uma vizinhança
    double cantoAuxiliar=0;                 // usaremos para armazenar os cantos e testar se estão no range do filhote
    int numeroDeCantos=0;                   // número de cantos armazenados, que usaremos para fazer a média

    for (int i=0; i<(this->vizinhanca.size()); i++)            // para cada canto na vizinhança
    {
        // armazenamos o valor do vizinho no cantoAuxiliar

        cantoAuxiliar=this->vizinhanca[i]->getValorCanto();

        // testamos se o canto Auxiliar está dentro do range do filhote

        if (cantoAuxiliar>=(this->geneCanto) && cantoAuxiliar<=((this->geneCanto)+(this->rangeGeneCanto)))
        {
            // caso esteja, somamos mais 1 canto no numeroDeCantos e somamos o canto no cantoTotal

            numeroDeCantos++;
            cantoTotal+=cantoAuxiliar;
        }
    }

    if (numeroDeCantos>0)
    {
        double canto=(cantoTotal/numeroDeCantos);     // calculamos a média e setamos o valor do canto

        this->valorCanto=canto;
    }

    // da maneira como está definido ele não vai funcionar caso tente aprender e não tenha ninguem para ensinar
}


void agente::limparVizinhanca()
{
    // método que limpa o vetor de ponteiros da vizinhança de um agente

    this->vizinhanca.clear();
}

void agente::adicionarVizinhanca(agente *vizinho)
{
    // método que adiciona um vizinho ao fim do vetor de ponteiros da vizinhança de uma agente

    this->vizinhanca.push_back(vizinho);
}

posicao agente::getLocal() const
{
    return local;
}

double agente::getRaioVizinhanca() const
{
    return raioVizinhanca;
}

bool agente::getRelacionamentoSerio() const
{
    return relacionamentoSerio;
}

bool agente::getEhMacho() const
{
    return ehMacho;
}

agente *agente::getParceiro() const
{
    return parceiro;
}

double agente::getGeneCanto() const
{
    return geneCanto;
}

double agente::getValorCanto() const
{
    return valorCanto;
}

vector<agente *> agente::getVizinhanca() const
{
    return vizinhanca;
}

void agente::condicoesDeContorno()
{
    // aplica as condições de contorno

    if(this->local.getX()>1) this->local.acumulaX(-1);
    if(this->local.getX()<0) this->local.acumulaX(1);
    if(this->local.getY()>1) this->local.acumulaY(-1);
    if(this->local.getY()<0) this->local.acumulaY(1);
}

void agente::namoraComigo()
{
    // é chamado para o macho quando a fêmea quer estabelecer um relacionamento sério com ele

    this->relacionamentoSerio=true;     // coloca seu relacionamentoSerio como true
}

void agente::selecionarParceiro()
{
    // é um método que só será usado pelas fêmeas que determina se elas irão acasalar com um macho (e qual deles)
    
    // a probabilidade da fêmea escolher um determinado macho não foi colocada como variavel (sorry)

    if (this->relacionamentoSerio==false)     // essa decisão só precisa ser tomada se ela não está em um relacionamento
    {
        // o pedaço abaixo cria um vetor para sortear qual macho será avaliado primeiro
        
        vector <int> sorteio;
        for (int i=0; i<this->vizinhanca.size(); i++)
        {
            sorteio.push_back(i);
        }
        shuffle(sorteio.begin(),sorteio.end(),std::default_random_engine(1));

        // uma vez feito um o embaralhamento do vetor de sorteio a fêmea irá avaliar cada um separadamente
        // e haverá uma probabilidade de que ela escollha acasalar com cada macho

        for (int j=0; j<this->vizinhanca.size(); j++)
        {
            if (this->vizinhanca[sorteio[j]]->getRelacionamentoSerio()==false)   // elas estão procurando solteiros
            {
                double cantoAuxiliar=this->vizinhanca[sorteio[j]]->getValorCanto();

                if (cantoAuxiliar>=(this->geneCanto) && cantoAuxiliar<=((this->geneCanto)+(this->rangeGeneCanto)))
                {
                    double dado=(double)rand()/RAND_MAX;

                    if (dado<0.01)      // aqui se define uma probabilidade da fêmea acasalar com o macho
                    {
                        this->relacionamentoSerio=true;

                        // colocamos o ponteiro 'parceiro' apontando para o vizinho que a fêmea escolheu acasalar

                        this->parceiro=this->vizinhanca[sorteio[j]];
                        this->parceiro->namoraComigo();     // coloca o parceiro em relacionamento sério
                        break;                                           // tendo como parceiro a fêmea
                    }
                }
            }
        }
    }
}

void agente::fimDeRelacionamento()
{
    this->relacionamentoSerio=false;
}
