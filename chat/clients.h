#ifndef CLIENTS_H
#define CLIENTS_H
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
class clients : public QWidget
{
    Q_OBJECT
public:
    clients();
    ~clients();
    void conect();
    void connectto(QString ip, int port);
    void datareceived();

    QString generatemesage(QString message, QString psedo);
    QString generatedate();
private:
    QTcpSocket *socket;
    quint16 messagesize;

    //interne
    void processthemessage(QMap<QString,QString> message);
    void processcomand(QMap<QString, QString> commend);
    QString generatemesage(QMap<QString, QString> message);
};

#endif // CLIENTS_H
