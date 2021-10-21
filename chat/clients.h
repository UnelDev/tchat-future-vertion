#ifndef CLIENTS_H
#define CLIENTS_H
#include <QTcpSocket>
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <QSettings>
#include <QNetworkInterface>
#include "userinterface.h"
class userinterface;
class clients : public QWidget
{
    Q_OBJECT
public:
        void setB(userinterface* ptr) { m_ptrToUserinterface = ptr;}
        void fonctionQuiFaitAppelleAUserinterface();
    clients(QWidget *parent = nullptr);
    ~clients();
    void desconnect();
    void connectto(QString ip, int port);
    void connected();
    void datareceived();

    void sentdatamap(const QMap<QString,QString> sendmap);
    void sentdatamap(const QString type, QString message, QString psedo, QDateTime seconde, QDateTime minute, QDateTime heurs, QDateTime NoJour, QDate jour);
    void sentdatamap(const QString type, QString message, QString pseudo);
    void sentdatamap(const QString type, QString message);
    void sentcommende(const QString commende);
    void sentcommende(const QString commende, QString arg);

    QString generatemesage(QString message, QString psedo);
    QString generatedate();
    QString generatedate(QMap<QString, QString> date);


    //ilisible
    void socketerror(QAbstractSocket::SocketError erreur);
private:
    userinterface* m_ptrToUserinterface = nullptr;
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
