#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "ambiente.h"          // incluindo a classe ambiente ao dialog

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    ambiente caatinga;                        // usando o construtor da classe ambiente para criar o objeto caatinga

protected:
    void paintEvent(QPaintEvent *e);          // necessário para usar o Qpainter para printar os agentes na tela
private slots:
    void on_pushButton_rodaMundo_clicked();

};

#endif // DIALOG_H
