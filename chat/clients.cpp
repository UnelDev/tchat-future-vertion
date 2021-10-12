#include "clients.h"
#include "userinterface.h"
userinterface *ui;
clients::clients()
{
    socket = new QTcpSocket; //serveur
    connect(socket, &QTcpSocket::readyRead, this ,&clients::datareceived);
    connect(socket, &QTcpSocket::connected,this,&clients::connected);
    connect(socket, &QTcpSocket::disconnected,this,&clients::desconnect);
    connect(socket, &QTcpSocket::errorOccurred, this, &clients::socketerror);
    messagesize = 0;
    //conexion
    client_connectto("127.0.0.1", ui->serveurport->value());
}
clients::~clients(){
    delete socket;
}
void clients::datareceived()
{
    QDataStream in(socket);
    while (1) {
        if (messagesize == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                 return;
            in >> messagesize;
        }
        if (socket->bytesAvailable() < messagesize)
            return;
        // Si on arrive jusqu'? cette ligne, on peut r?cup?rer le message entier
        QMap<QString,QString> messageRecu;
        in >> messageRecu;
        processthemessage(messageRecu);
        // On remet la taille du message ? 0 pour pouvoir recevoir de futurs messages
        messagesize = 0;
    }
}









void clients::conect(){
    ui->startTrayIcon();
    QMessageBox::critical(nullptr,tr("psedo invalid"),tr("vous ne pouvez avoir anonymous ou aucun psedo"));
}
void clients::connectto(QString ip, int port)
{
    ui->displayMessagelist(generatemesage(tr("tentative de connexion en cour"),tr("chat bot")));
    ui->changestateconnectbuton(false);
    socket->abort();
    socket->connectToHost(ip, port);
    ui->displayconnectlabel(tr("<font color=\"#894B23\">tenetative lancé</font>"));
}
QString clients::generatemesage(QString message, QString psedo)
{
    if(psedo == "" ||psedo == " "){
        psedo = "anonymous";
    }
    return(tr("<span style=\"font-size: 12px; font-weight: bold;\">")+psedo+tr("</span>")+generatedate()+tr("<span style=\"font-size: 14px; \">")+message+tr("</span><br/><br/>"));
}
QString clients::generatedate()
{
    QString heurs = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString Date = QDateTime::currentDateTime().toString(" dd MM yyyy");
    QDateTime::fromString(heurs, "hh:mm:ss");
    return(tr("<span style=\"font-size: 10px\"> Le ")+Date+tr("</span> <span style=\"font-size: 10px\">à ")+heurs+tr(" </span><br/>"));

}




//fonction interne
void clients::processthemessage(QMap<QString,QString> message)
{
    if(message["type"]=="cmd"){
        processcomand(message);
    }else if(message["type"]=="msg"){
        ui->displayMessagelist(generatemesage(message));
    }else if(message["type"]=="connection"){
        ui->clientlist->addItem(message["psedo"]);
        ++nbuser;
        if(nbuser==10){
            settings->setValue("succes/10userSimultaneously", true);
        }else if(nbuser==30){
            settings->setValue("succes/30userSimultaneously", true);
        }else if(nbuser==100){
            settings->setValue("succes/100userSimultaneously", true);
        }
    }else{
        QMessageBox::critical(this, tr("erreur"), tr("un packet a été recu mais l'indantificateur : ") + message["type"] + tr(" est inconu."));
    }
}
void clients::processcomand(QMap<QString, QString> commend)
{
    if (commend["message"] == "psedo?"){
        client_sentcommende("psedo_", ui->returnpsedo());
    }else if (commend["message"]=="vertion?"){
        client_sentcommende("version",version);
    }else if (commend["message"]=="pesdoAnonimousinvalid"){
        QMessageBox::critical(this, tr("erreur"), tr("il faut un autre psedo que anonimous ou rien pour se conecter"));
    }else if(commend["message"]=="psedoalreadyuse"){
        QMessageBox::critical(this, tr("erreur"), tr("un autre client porte deja ce psedo changer de psedo pour vous connecter"));
    }else if(commend["message"]=="pseudoresembling"){
        QMessageBox::critical(this, tr("erreur"), tr("un autre client porte deja un psedo resemblant changer de psedo pour vous connecter"));
    }else if (commend["message"]=="update_") {
        ui->displayMessagelist(commend["arg"]);
    }else if (commend["message"]=="isconnected"){
        ui->addItemOfClientList(commend["arg"]);
        ++nbuser;
    }else if(commend["message"]=="desconnected"){
        if(ui->clientlist->findItems(commend["arg"],Qt::MatchCaseSensitive).size()==1){
            ui->clientlist->removeItemWidget(ui->clientlist->findItems(commend["arg"],Qt::MatchCaseSensitive)[1]); //on suprime le nom specifier
            QMessageBox::critical(this, tr("supression de client"), tr("le client vien d'etre suprimer"));
        }else{
            for (int compteur {ui->clientlist->findItems(commend["arg"],Qt::MatchCaseSensitive).size()-1}; compteur > 0; --compteur) //tan que des utilistateur porte le nom specifier
            {
                ui->clientlist->removeItemWidget(ui->clientlist->findItems(commend["arg"],Qt::MatchCaseSensitive)[1]); //on suprime le nom specifier
                QMessageBox::critical(this, tr("supression de client"), tr("le client vien d'etre suprimer"));
                --nbuser;
            }
        }
    }else{
        QMessageBox::critical(this, tr("erreur"), tr("un packet de comande a été recu mais la comande est incomprise."));
    }
}
QString clients::generatemesage(QMap<QString, QString> message){
    if(message["psedo"] == "" ||message["psedo"] == " "){
        message["psedo"] = "anonymous";
    }
    return(tr("<span style=\"font-size: 12px; font-weight: bold;\">")+message["psedo"]+tr("</span>")+generatedate(message)+tr("<span style=\"font-size: 14px; \">")+message["message"]+tr("</span><br/><br/>"));
}