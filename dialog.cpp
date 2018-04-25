#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include "posicao.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *e) // esse método aqui é pra desenhar as paradas diferenciadas
{
    QPainter romeroBritto;
    romeroBritto.begin(this);

    int tamanhoJanelaX=this->geometry().width();
    int tamanhoJanelaY=this->geometry().height();

    double escalaX=(double)tamanhoJanelaX/this->caatinga.getTamanhoDoMundo();
    double escalaY=(double)tamanhoJanelaY/this->caatinga.getTamanhoDoMundo();

    posicao pincel;
    romeroBritto.setPen(QColor(0,0,0,0));

    for (int i=0; i<this->caatinga.getNumeroPassaros(); i++)
    {
        if (this->caatinga.getEhMacho(i)==true)
        {
            pincel=this->caatinga.getLocal(i);

            if (this->caatinga.getRelacionamentoSerio(i))
            {
                romeroBritto.setBrush(QColor(0,255,0));
            }
            else
            {
                romeroBritto.setBrush(QColor(0,0,255));
            }


            romeroBritto.drawEllipse(QPointF(pincel.getX()*escalaX,pincel.getY()*escalaY),2,2);
        }
        else
        {
            pincel=this->caatinga.getLocal(i);

            if (this->caatinga.getRelacionamentoSerio(i))
            {
                romeroBritto.setBrush(QColor(0,255,0));
            }
            else
            {
                romeroBritto.setBrush(QColor(255,0,0));
            }


            romeroBritto.drawEllipse(QPointF(pincel.getX()*escalaX,pincel.getY()*escalaY),2,2);
        }
    }
    romeroBritto.end();
}

void Dialog::on_pushButton_rodaMundo_clicked()
{
    this->caatinga.rodaGeracao();
    this->repaint();
}
