#include "userinterface.h"
#include "ui_userinterface.h"

userinterface::userinterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::userinterface)
{
    ui->setupUi(this);
    startTrayIcon();
    client = new clients();
}

userinterface::~userinterface()
{
    delete ui;
    delete client;
    delete sticon;
}
void userinterface::startTrayIcon(){
    sticon = new QSystemTrayIcon(this); // on construit notre icône de notification
    // Création du menu contextuel de notre icône
    QMenu* stmenu = new QMenu(this);
    QAction* actTexte1 = new QAction(tr("quiter"),this);
    QAction* condense = new QAction(tr("condenser la fenetre"), this);
    //connect(qApp, &QGuiApplication::applicationStateChanged, this,  &Widget::changetransparency);
    stmenu->addAction(actTexte1);
    stmenu->addAction(condense);
    condense->setCheckable(true);
    sticon->setContextMenu(stmenu); // On assigne le menu contextuel à l'icône de notification
    QIcon icon(":/images/Ananta.png");
    sticon->setIcon(icon); // On assigne une image à notre icône
    sticon->show(); // On affiche l'icône
    connect(actTexte1, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(condense, SIGNAL(triggered()), this, SLOT(condesed()));
}
void userinterface::on_conectbuton_clicked()
{
    client->conect();
}
void userinterface::displayMessagelist(QString message){
    ui->messagelist->append(message);
}
void userinterface::displayconnectlabel(QString label){
    ui->conectstatelabel->setText(label);
}
void userinterface::changestateconnectbuton(bool state){
    ui->conectbuton->setEnabled(state);
}
void userinterface::addItemOfClientList(QString item){
    ui->clientlist->addItem(item);
}
void remouveItemsOfClientList(QString item){
    if(ui->clientlist->findItems(item,Qt::MatchCaseSensitive).size()==1){
        ui->clientlist->removeItemWidget(ui->clientlist->findItems(item,Qt::MatchCaseSensitive)[1]); //on suprime le nom specifier
        QMessageBox::critical(this, tr("supression de client"), tr("le client vien d'etre suprimer"));
    }else{
        for (int compteur {ui->clientlist->findItems(item,Qt::MatchCaseSensitive).size()-1}; compteur > 0; --compteur) //tan que des utilistateur porte le nom specifier
        {
            ui->clientlist->removeItemWidget(ui->clientlist->findItems(item,Qt::MatchCaseSensitive)[1]); //on suprime le nom specifier
            QMessageBox::critical(this, tr("supression de client"), tr("le client vien d'etre suprimer"));
            --nbuser;
        }
    }
}
QString userinterface::returnpsedo(){
    return ui->pseudo->text();
}


