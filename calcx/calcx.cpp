#include "calcx.h"
#include "ui_calcx.h"
#include "assert.h"

#include <QObject>
#include <QWidget>
#include <QTime>

Calcx::Calcx(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Calcx),
    tcpSocket(new QTcpSocket(this)),
    networkSession(NULL)
{
    ui->setupUi(this);

    stream.setDevice(tcpSocket);
    stream.setVersion(QDataStream::Qt_4_0);
    ui->hostLineEdit->setText("localhost");
    ui->portLineEdit->setText("12345");

    create_button(QString("7"),ui->horizontalLayout_1);
    create_button(QString("8"),ui->horizontalLayout_1);
    create_button(QString("9"),ui->horizontalLayout_1);
    create_button(QString("/"),ui->horizontalLayout_1);

    create_button(QString("4"),ui->horizontalLayout_2);
    create_button(QString("5"),ui->horizontalLayout_2);
    create_button(QString("6"),ui->horizontalLayout_2);
    create_button(QString("*"),ui->horizontalLayout_2);

    create_button(QString("1"),ui->horizontalLayout_3);
    create_button(QString("2"),ui->horizontalLayout_3);
    create_button(QString("3"),ui->horizontalLayout_3);
    create_button(QString("-"),ui->horizontalLayout_3);

    create_button(QString("0"),ui->horizontalLayout_4);
    create_button(QString("."),ui->horizontalLayout_4);
    QPushButton * butt_res =
    create_button(QString("="),ui->horizontalLayout_4);
    create_button(QString("+"),ui->horizontalLayout_4);

    connect( butt_res, SIGNAL(clicked()), this, SLOT(on_lineEdit_returnPressed()) );

    tcpSocket->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readResult()));
}

Calcx::~Calcx()
{
    foreach (QObject *obj, button_prints.keys())
    {
        obj->deleteLater();
    }

    delete ui;
}

QPushButton * Calcx::create_button(QString str, QLayout *parent)
{
    QPushButton *butt = new QPushButton();
    parent->addWidget(butt);
    butt->setText(str);
    butt->setObjectName(QString("butt_") + str);
    button_prints.insert(butt,str);
    connect (butt, SIGNAL(clicked()), this, SLOT(on_print_clicked()));
    return butt;
}

void Calcx::on_print_clicked()
{
    QString str = ui->lineEdit->text();
    str.remove(QRegExp("=.*"));
    str += button_prints[sender()];

    ui->lineEdit->setText(str);
}

void Calcx::on_lineEdit_returnPressed()
{
    QString exp = QString("([-+*/]?[0-9]+.[0-9]+)+");
    QValidator *valid = new QRegExpValidator(QRegExp(exp), this);
    QString formula = ui->lineEdit->text();
    formula.remove(" ");
    formula.remove(QRegExp("=.*"));
    ui->lineEdit->setText(formula);
    int pos = 0;
    if (valid->validate(formula,pos) !=  QValidator::Acceptable)
        return;
    QTextStream toserv(tcpSocket);
    toserv << formula;
}

void Calcx::readResult() {
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QByteArray res =  socket->readAll();
    ui->lineEdit->setText(QString(res));
    QString status = res + " " + QTime::currentTime().toString();
    ui->statusBar->showMessage(status);
}

void Calcx::on_setButton_clicked()
{

    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());

}
