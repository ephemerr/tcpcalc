#ifndef CALCSERVER_H
#define CALCSERVER_H

#include <QtCore>
#include <QtNetwork>

#define CALC_PORT 12345


class CalcServer : public QObject
{
    Q_OBJECT
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;public:
    explicit CalcServer(QObject *parent = 0);
    ~CalcServer();
    
signals:
    
public slots:
    void newClient();
    void calcRequest(); // обработчик входящих данных
    void stateChanged(QAbstractSocket::SocketState stat); // обработчик изменения состояния
    
};

#endif // CALCSERVER_H
