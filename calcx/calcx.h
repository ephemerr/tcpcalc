#ifndef CALCX_H
#define CALCX_H

#include <QMainWindow>
#include <QtCore>
#include <QPushButton>
#include <QtNetwork>

namespace Ui {
class Calcx;
}

class Calcx : public QMainWindow
{
    Q_OBJECT
    QMap<QObject*, QString> button_prints;
    QPushButton* create_button(QString str, QLayout *parent);
//    double calc(QList<Lexem> &rpn, int &i);
public:
    explicit Calcx(QWidget *parent = 0);
    ~Calcx();

private slots:
    void on_print_clicked();
    void on_lineEdit_returnPressed();
    void readResult();

    void on_setButton_clicked();

private:
    Ui::Calcx *ui;
    QTcpSocket *tcpSocket;
    QDataStream stream;
    QNetworkSession *networkSession;
};

#endif // CALCX_H
