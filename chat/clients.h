#ifndef CLIENTS_H
#define CLIENTS_H
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <QSettings>
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
    QSettings* settings;
    QTcpSocket *socket;
    quint16 messagesize;
    QString version;
    //interne
    void processthemessage(QMap<QString,QString> message);
    void processcomand(QMap<QString, QString> commend);
    QString generatemesage(QMap<QString, QString> message);
    int nbuser;
};

#endif // CLIENTS_H
