#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class userinterface; }
QT_END_NAMESPACE
class userinterface;
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
    int valueOFServeurPort();
    QString returnpsedo();

private slots:
    void on_conectbuton_clicked();

    void on_sentbuton_clicked();

private:
    Ui::userinterface *ui;
    QMenu *stmenu;
    QSystemTrayIcon* sticon;
};
#endif // USERINTERFACE_H
