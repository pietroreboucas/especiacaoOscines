#include "agente.h"
#include <vector>
#include <algorithm>

agente::agente(bool macho)
{
    // cria agentes que serão dispostos aleatóriamente no início da simulação

    this->idade=0;
    this->contadorDeMeses=0;
    this->tempoDeMaturacao=6;

    this->adulto=true;
    this->ehMacho=macho;                         // assinalando o agente como macho (ou fêmea)
    this->flertando=false;                // definindo os passaros como solteiros
    this->probabilidadeAcasalamento=0.05;

    this->local.setX((double)rand()/RAND_MAX);                // variáveis de "local"
    this->local.setY((double)rand()/RAND_MAX);                // da classe "posição"

    this->movimento.setPasso(0.001);                          // varáveis de "caminhada"
    this->movimento.setDirecao((double)rand()/RAND_MAX*360);  // da classe "movimentação"
    this->movimento.setAnguloDeVisao(40);

    this->passoMutacao=0.02;
    this->probabilidadeMutacao=0.01;

    // vamos inicializar os valores do gene e o valor do canto inicial
    // o valor de geneCanto é o valor inicial do range de cantos reconhecíveis

    this->rangeCanto=1;
    this->geneCantoI=2.4+0.2*((double)rand()/RAND_MAX);
    this->geneCantoII=2.4+0.2*((double)rand()/RAND_MAX);
    this->inicioRangeCanto=(geneCantoI+geneCantoII)/2;
    this->valorCanto=(this->inicioRangeCanto)+(this->rangeCanto)*((double)rand()/RAND_MAX);

    // incicializando as demais variaveis

    this->raioVizinhanca=10*this->movimento.getPasso();  // inicializando raio da vizinhança
    this->limparVizinhanca();
    this->parceiro=nullptr;
}

agente::agente(bool macho, double geneMaeI, double geneMaeII,
               double genePaiI, double genePaiII, posicao localMae)
{
    // esse construtor criará os agentes que surgirão por reprodução sexuada

    this->idade=0;
    this->contadorDeMeses=0;
    this->tempoDeMaturacao=6;

    this->adulto=false;
    this->ehMacho=macho;              // assinalando o agente como macho (ou fêmea)
    this->flertando=false;     // definindo os passaros como solteiros
    this->probabilidadeAcasalamento=0.05;

    this->local=localMae;             // "local" será iniciada com o posicionamento da mãe

    this->movimento.setPasso(0.001);                          // varáveis de "caminhada"
    this->movimento.setDirecao((double)rand()/RAND_MAX*360);  // da classe "movimentação"
    this->movimento.setAnguloDeVisao(40);

    this->passoMutacao=0.02;
    this->probabilidadeMutacao=0.01;

    // vamos inicializar os valores do gene e o valor do canto inicial

    this->rangeCanto=1;

    // temos que jogar um dado duas vezes para decidir os genes I e II do filhote
    // primeiro para o geneI:

    double dado=(double)rand()/RAND_MAX;

    if (dado<0.5)
    {
        this->geneCantoI=geneMaeI;
    }
    else
    {
        this->geneCantoI=genePaiI;
    }

    // chance de mutação em relação ao gene escolhido

    dado=(double)rand()/RAND_MAX;    // rolando o dado para determinar mutação genética

    if (dado<this->probabilidadeMutacao)
    {
        //mutação que pode ser 'positiva' ou 'negativa'

        dado=(double)rand()/RAND_MAX;

        if  (dado<0.5)
        {
            this->geneCantoI+=this->passoMutacao;   // passo genético positivo
        }
        else
        {
            this->geneCantoI-=this->passoMutacao;   // passo genético negativo
        }
    }

    // agora para o gene II:

    dado=(double)rand()/RAND_MAX;

    if (dado<0.5)
    {
        this->geneCantoII=geneMaeII;
    }
    else
    {
        this->geneCantoII=genePaiII;
    }

    // chance de mutação em relação ao gene escolhido

    dado=(double)rand()/RAND_MAX;    // rolando o dado para determinar mutação genética

    if (dado<this->probabilidadeMutacao)
    {
        //mutação que pode ser 'positiva' ou 'negativa'

        dado=(double)rand()/RAND_MAX;

        if  (dado<0.5)
        {
            this->geneCantoII+=this->passoMutacao;   // passo genético positivo
        }
        else
        {
            this->geneCantoII-=this->passoMutacao;   // passo genético negativo
        }
    }

    // usamos os genes para determinar o inicio do range

    this->inicioRangeCanto=(geneCantoI+geneCantoII)/2;

    // colocamos um valor aleatório inicial no canto dos machos;

    this->valorCanto=(this->inicioRangeCanto)+(this->rangeCanto)*((double)rand()/RAND_MAX);

    // incicializando as demais variaveis

    this->raioVizinhanca=10*this->movimento.getPasso();  // inicializando raio da vizinhança
    this->limparVizinhanca();
    this->parceiro=nullptr;
}

void agente::rodaModelo()
{
    this->contadorDeMeses++;

    if (this->ehMacho==true)    // se for macho
    {
        if (this->contadorDeMeses>this->tempoDeMaturacao)   // checa se ja ficou adulto
        {
            this->adulto=true;  // se ficou seta a variavel adulto como true
        }

        if (this->adulto==true)  // se for adulto
        {
            if (this->flertando==true)          // checa se esta em epoca de acasalamento
            {
                this->movimento.parado();       // se sim fica parado
            }
            else
            {
                this->movimento.movimento();    // caso não se move normalmente
            }
        }
        else                   // caso seja filhote
        {
            this->aprenderCanto();              // modifica o canto
            this->movimento.movimento();        // se move normalmente
        }
    }
    else                       // se for fêmea
    {
        if (this->flertando==true)              // e estiver na epoca de acasalamento
        {
            this->movimento.parado();           // fica parada e
            this->selecionarParceiro();         // seleciona um parceiro
        }
        else                                        // caso não seja epoca de acasalamento
        {
            this->movimento.movimento();        // se move normalmente
        }
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
    // sera chamado a cada iteracao de um filhote macho para modificar seu canto

    // definindo algumas variaveis auxiliares

    double cantoTotal=0;         // onde vamos somar todos os cantos de uma vizinhança
    int numeroDeCantos=0;        // número de cantos armazenados, que usaremos para fazer a média

    if (this->vizinhanca.size()>0)
    {
        for (int i=0; i<(this->vizinhanca.size()); i++)   // para cada canto na vizinhança
        {
            // somamos os cantos da vizinhanca na variavel canto total

            cantoTotal+=this->vizinhanca[i]->getValorCanto();

            numeroDeCantos++;     // somamos mais um ao valor do numero de cantos
        }

        double cantoMedio=(cantoTotal/numeroDeCantos);    // calculamos a média

        this->valorCanto=(valorCanto+cantoMedio)/2;       // fazemos a media com o que ele ja sabe
    }

    // da maneira como está definido ele so vai aprender se tiver quem ouvir
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

bool agente::getEhMacho() const
{
    return ehMacho;
}

agente *agente::getParceiro() const
{
    return parceiro;
}

double agente::getGeneCantoI() const
{
    return geneCantoI;
}

double agente::getValorCanto() const
{
    return valorCanto;
}

vector<agente *> agente::getVizinhanca() const
{
    return vizinhanca;
}

bool agente::getAdulto() const
{
    return adulto;
}

void agente::setFlertando(bool value)
{
    flertando = value;
}

int agente::getIdade() const
{
    return idade;
}

double agente::getGeneCantoII() const
{
    return geneCantoII;
}

bool agente::getFlertando() const
{
    return flertando;
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

    this->flertando=true;     // coloca seu relacionamentoSerio como true
}

void agente::selecionarParceiro()
{
    // usado pelas fêmeas para selecionar um macho

    if (this->flertando==false)     // se ela não está em um relacionamento
    {
        // o pedaço abaixo cria um vetor para sortear qual macho será avaliado primeiro
        
        vector <int> sorteio;
        for (int i=0; i<this->vizinhanca.size(); i++)
        {
            sorteio.push_back(i);
        }
        shuffle(sorteio.begin(),sorteio.end(),std::default_random_engine(1));

        // uma vez feito um o sorteio a fêmea irá avaliar cada um separadamente na ordem

        for (int j=0; j<this->vizinhanca.size(); j++)
        {
            if (this->vizinhanca[sorteio[j]]->getFlertando()==false)   // apenas solteiros
            {
                double cantoAuxiliar=this->vizinhanca[sorteio[j]]->getValorCanto();

                if (cantoAuxiliar>=(this->inicioRangeCanto) &&  // se está dentro do range
                    cantoAuxiliar<=((this->inicioRangeCanto)+(this->rangeCanto)))
                {
                    double dado=(double)rand()/RAND_MAX;

                    if (dado<this->probabilidadeAcasalamento)   // avaliamos a probabilidade
                    {
                        this->flertando=true;

                        // colocamos o ponteiro 'parceiro' apontando o macho

                        this->parceiro=this->vizinhanca[sorteio[j]];
                        this->parceiro->namoraComigo();  // coloca o parceiro em
                        break;                           // relacionamento sério com a fêmea
                    }
                }
            }
        }
    }
}

void agente::fimDeAcasalamento()
{
    this->flertando=false;
    this->idade++;
}
