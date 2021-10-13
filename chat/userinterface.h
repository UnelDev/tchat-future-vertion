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

class userinterface : public QWidget
{
    Q_OBJECT

public:
    userinterface(QWidget *parent = nullptr);
    ~userinterface();
    void startTrayIcon();
    void displayMessagelist(QString message);
    void displayconnectlabel(QString label);
    void changestateconnectbuton(bool state);
    void addItemOfClientList(QString item);
    int remouveItemsOfClientList(QString item, int nbClient);
    QString returnpsedo();

private slots:
    void on_conectbuton_clicked();

private:
    clients *client;
    Ui::userinterface *ui;
    QMenu *stmenu;
    QSystemTrayIcon* sticon;
};
#endif // USERINTERFACE_H
