#include "calcserver.h"
#include "calc.h"
#include <QTextStream>
#include <QDebug>

//QTextStream out(stdout);

CalcServer::CalcServer(QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newClient()));
    if (!tcpServer->listen(QHostAddress::LocalHost, CALC_PORT) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        qDebug() << QString::fromUtf8("Server started");
    }
}

CalcServer::~CalcServer()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i]);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i]->close();
            SClients.remove(i);
        }
        tcpServer->close();
        qDebug() << QString::fromUtf8("Server stopped");
        server_status=0;
    }
}

void CalcServer::newClient()
{
    if(server_status==1){
        QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
        int clientId = clientSocket->socketDescriptor();
        qDebug() << QString::fromUtf8("New connection ") << clientId;
        SClients[clientId] = clientSocket;
        connect(SClients[clientId],SIGNAL(readyRead()),this, SLOT(calcRequest()));
        connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    }
}

void CalcServer::calcRequest()
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    QByteArray arr =  socket->readAll();
    QString formula = QString(arr);
    QString res = formula + "= " + calc_formula(formula);
    qDebug() <<  res;
    socket->write(res.toLatin1());
}

void CalcServer::stateChanged(QAbstractSocket::SocketState state)
{
    QObject * object = QObject::sender();
    if (!object)
        return;
    QTcpSocket * socket = static_cast<QTcpSocket *>(object);
    qDebug() << __func__ << " " << state;
}
