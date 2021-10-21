#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
#include "clients.h"
QT_BEGIN_NAMESPACE
namespace Ui { class userinterface; }
QT_END_NAMESPACE
class clients;
class userinterface : public QWidget
{
    Q_OBJECT

public:
    void setA(clients* ptr) { m_ptrToClients = ptr;}
    void fonctionQuiFaitAppelleAClients();
    userinterface(QWidget *parent = nullptr);
    ~userinterface();
    void startTrayIcon();
    void displayMessagelist(QString message);
    void displayconnectlabel(QString label);
    void changestateconnectbuton(bool state);
    void addItemOfClientList(QString item);
    int remouveItemsOfClientList(QString item, int nbClient);
    int valueOFServeurPort();
    QString returnpsedo();

private slots:
    void on_conectbuton_clicked();

    void on_sentbuton_clicked();

private:
    clients* m_ptrToClients = nullptr;
    Ui::userinterface *ui;
    QMenu *stmenu;
    QSystemTrayIcon* sticon;
};
#endif // USERINTERFACE_H
