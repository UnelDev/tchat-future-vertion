#include "clients.h"
#include "userinterface.h"
clients::clients(QWidget *parent)
    : QWidget(parent)
{
    settings = new QSettings("ananta system","tchat",nullptr);
    if(!settings->contains("succes/succes")){
        settings->setValue("succes/succes",true);
    }if(!settings->contains("succes/nbmessage")){
        settings->setValue("succes/nbmessage",0);
    }if(!settings->contains("settings/SaveMessage")){
        settings->setValue("settings/SaveMessage",true);
    }if(!settings->contains("settings/visualNotification")){
        settings->setValue("settings/visualNotification",true);
    }if(!settings->contains("settings/SoundNotification")){
        settings->setValue("settings/SoundNotification",true);
    }if(!settings->contains("settings/transparencyIsActived")){
        settings->setValue("settings/transparencyIsActived", false);
    }if(!settings->contains("settings/color")){
        settings->setValue("settings/color","white");
    }if(!settings->contains("settings/path")){
        settings->setValue("settings/path",":/sound/notifdefault.wav");
    }if(!settings->contains("settings/transparency")){
        settings->setValue("settings/transparency","0.5");
    }if(!settings->contains("succes/10userSimultaneously")){
        settings->setValue("succes/10userSimultaneously", false);
    }if(!settings->contains("succes/30userSimultaneously")){
        settings->setValue("succes/30userSimultaneously", false);
    }if(!settings->contains("succes/100userSimultaneously")){
        settings->setValue("succes/100userSimultaneously", false);
    }if(!settings->contains("succes/server/nbserveur")){
        settings->setValue("succes/server/nbserveur", 0);
    }if(!settings->contains("succes/server/nbserveur")){
        settings->setValue("succes/server/nbserveur", 0);
    }
    version="5.0";
    nbuser=0;
    socket = new QTcpSocket; //serveur
    //connect(socket, &QTcpSocket::readyRead, this ,&clients::datareceived);
    //connect(socket, &QTcpSocket::connected,this,&clients::connected);
    //connect(socket, &QTcpSocket::disconnected,this,&clients::desconnect);
    //connect(socket, &QTcpSocket::errorOccurred, this, &clients::socketerror);
    messagesize = 0;
    //conexion
    //connectto("127.0.0.1", valueOFServeurPort());
    fonctionQuiFaitAppelleAUserinterface();
}
clients::~clients(){
    delete socket;
}
void clients::fonctionQuiFaitAppelleAUserinterface()
{
  m_ptrToUserinterface->displayMessagelist("hello word");
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






void clients::desconnect()
{
    QString textmessage = generatemesage(tr("d??conecter du serveur"),tr("chat bot"));
    //ui->displayconnectlabel(tr("<font color=\"#ff0000\">D??connect??</font>"));
    //ui->displayMessagelist(textmessage);
    //ui->changestateconnectbuton(true);
}
void clients::connectto(QString ip, int port)
{
    //ui->displayMessagelist(generatemesage(tr("tentative de connexion en cour"),tr("chat bot")));
    //ui->changestateconnectbuton(false);
    socket->abort();
    socket->connectToHost(ip, port);
    //ui->displayconnectlabel(tr("<font color=\"#894B23\">tenetative lanc??</font>"));
}
void clients::connected()
{
    QString textmessage = generatemesage(tr("conexion reusi"), tr("chat bot"));
    sentdatamap("connection","Serveur Tchat Bot");
    //ui->displayMessagelist(textmessage);
    //ui->changestateconnectbuton(true);
    //ui->displayconnectlabel(tr("<font color=\"#70AD47\">connect??</font>"));
    for (int compteur {settings->value("succes/server/nbserveur").toInt()}; compteur > 0; --compteur)
    {
        if(socket->peerAddress().Any==settings->value("succes/server/"+QString::number(settings->value("succes/server/nbserveur").toInt()))){
            return;
        }
    }
    settings->setValue("succes/server/"+QString::number(settings->value("succes/server/nbserveur").toInt()),socket->peerAddress().Any);
    settings->setValue("succes/server/nbserveur", settings->value("succes/server/nbserveur").toInt()+1);
    if(settings->value("succes/server/nbserveur").toInt()==20){
        settings->setValue("succes/20server", true);
    }
}
QString clients::generatedate()
{
    QString heurs = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString Date = QDateTime::currentDateTime().toString(" dd MM yyyy");
    QDateTime::fromString(heurs, "hh:mm:ss");
    return(tr("<span style=\"font-size: 10px\"> Le ")+Date+tr("</span> <span style=\"font-size: 10px\">?? ")+heurs+tr(" </span><br/>"));

}
QString clients::generatedate(QMap<QString, QString> date)
{
    return(tr("<span style=\"font-size: 10px\"> Le ")+date["shippingday"]+" "+date["sendingdate"]+"/"+date["shippingmonth"]+"/"+date["shippingyears"] +tr("</span> <span style=\"font-size: 10px\"> ?? ")+date["sendingtime"]+":"+date["minuteofsending"]+tr(" </span><br/>"));

}




//fonction interne

void clients::sentdatamap(const QMap<QString,QString> sendmap)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << sendmap;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));
    socket->write(paquet); // On envoie le paquet
}
void clients::sentdatamap(const QString type, QString message, QString pseudo, QDateTime seconde, QDateTime minute, QDateTime heures, QDateTime NoJour, QDate jour){
    QMap<QString,QString> sendmap;
    sendmap["type"]=type;
    sendmap["message"]=message;
    sendmap["pseudo"]=pseudo;
    sendmap["version"]=version;
    sendmap["secondofsending"]=seconde.toString();
    sendmap["minuteofsending"]=minute.toString();
    sendmap["sendingtime"]=heures.toString();
    sendmap["sendingdate"]=NoJour.toString();
    sendmap["shippingday"]=jour.toString();
    sentdatamap(sendmap);
}
void clients::sentdatamap(const QString type, QString message, QString pseudo){
    QMap<QString,QString> sendmap;
    sendmap["type"]=type;
    sendmap["message"]=message;
    sendmap["pseudo"]=pseudo;
    sendmap["version"]=version;
    sendmap["secondofsending"]=QDateTime::currentDateTime().toString("ss");;
    sendmap["minuteofsending"]=QDateTime::currentDateTime().toString("mm");;
    sendmap["sendingtime"]=QDateTime::currentDateTime().toString("hh");
    sendmap["sendingdate"]=QDateTime::currentDateTime().toString("d");
    sendmap["shippingday"]=QDateTime::currentDateTime().toString("dddd");
    sendmap["shippingmonth"]=QDateTime::currentDateTime().toString("MMMM");
    sendmap["shippingyears"]=QDateTime::currentDateTime().toString("yyyy");
    sentdatamap(sendmap);
}
void clients::sentdatamap(const QString type, QString message){
    QMap<QString,QString> sendmap;
    sendmap["type"]=type;
    sendmap["message"]=message;
    sendmap["pseudo"]=//ui->returnpsedo();
    sendmap["version"]=version;
    sendmap["secondofsending"]=QDateTime::currentDateTime().toString("ss");;
    sendmap["minuteofsending"]=QDateTime::currentDateTime().toString("mm");;
    sendmap["sendingtime"]=QDateTime::currentDateTime().toString("hh");
    sendmap["sendingdate"]=QDateTime::currentDateTime().toString("d");
    sendmap["shippingday"]=QDateTime::currentDateTime().toString("dddd");
    sendmap["shippingmonth"]=QDateTime::currentDateTime().toString("MMMM");
    sendmap["shippingyears"]=QDateTime::currentDateTime().toString("yyyy");
    sentdatamap(sendmap);
}
void clients::sentcommende(const QString commande){
    QMap<QString,QString> sendmap;
    sendmap["type"]="cmd";
    sendmap["message"]=commande;
    sendmap["pseudo"]=//ui->returnpsedo();
    sendmap["version"]=version;
    sendmap["secondofsending"]=QDateTime::currentDateTime().toString("ss");;
    sendmap["minuteofsending"]=QDateTime::currentDateTime().toString("mm");;
    sendmap["sendingtime"]=QDateTime::currentDateTime().toString("hh");
    sendmap["sendingdate"]=QDateTime::currentDateTime().toString("d");
    sendmap["shippingday"]=QDateTime::currentDateTime().toString("dddd");
    sendmap["shippingmonth"]=QDateTime::currentDateTime().toString("MMMM");
    sendmap["shippingyears"]=QDateTime::currentDateTime().toString("yyyy");
    sentdatamap(sendmap);
}
void clients::sentcommende(const QString commande, QString arg){
    QMap<QString,QString> sendmap;
    sendmap["type"]="cmd";
    sendmap["message"]=commande;
    sendmap["arg"]=arg;
    sendmap["pseudo"]=//ui->returnpsedo();
    sendmap["version"]=version;
    sendmap["secondofsending"]=QDateTime::currentDateTime().toString("ss");;
    sendmap["minuteofsending"]=QDateTime::currentDateTime().toString("m");;
    sendmap["sendingtime"]=QDateTime::currentDateTime().toString("hh");
    sendmap["sendingdate"]=QDateTime::currentDateTime().toString("d");
    sendmap["shippingday"]=QDateTime::currentDateTime().toString("ddd");
    sendmap["shippingmonth"]=QDateTime::currentDateTime().toString("MMMM");
    sendmap["shippingyears"]=QDateTime::currentDateTime().toString("yyyy");;
    sentdatamap(sendmap);
}

//fonctioninterne deriv??e
void clients::processthemessage(QMap<QString,QString> message)
{
    if(message["type"]=="cmd"){
        processcomand(message);
    }else if(message["type"]=="msg"){
        //ui->displayMessagelist(generatemesage(message));
    }else if(message["type"]=="connection"){
       //ui->addItemOfClientList(message["psedo"]);
        ++nbuser;
        if(nbuser==10){
            settings->setValue("succes/10userSimultaneously", true);
        }else if(nbuser==30){
            settings->setValue("succes/30userSimultaneously", true);
        }else if(nbuser==100){
            settings->setValue("succes/100userSimultaneously", true);
        }
    }else{
        QMessageBox::critical(this, tr("erreur"), tr("un packet a ??t?? recu mais l'indantificateur : ") + message["type"] + tr(" est inconu."));
    }
}
void clients::processcomand(QMap<QString, QString> commend)
{
    if (commend["message"] == "psedo?"){
        //sentcommende("psedo_", //ui->returnpsedo());
    }else if (commend["message"]=="vertion?"){
        sentcommende("version",version);
    }else if (commend["message"]=="pesdoAnonimousinvalid"){
        QMessageBox::critical(this, tr("erreur"), tr("il faut un autre psedo que anonimous ou rien pour se conecter"));
    }else if(commend["message"]=="psedoalreadyuse"){
        QMessageBox::critical(this, tr("erreur"), tr("un autre client porte deja ce psedo changer de psedo pour vous connecter"));
    }else if(commend["message"]=="pseudoresembling"){
        QMessageBox::critical(this, tr("erreur"), tr("un autre client porte deja un psedo resemblant changer de psedo pour vous connecter"));
    }else if (commend["message"]=="update_") {
        //ui->displayMessagelist(commend["arg"]);
    }else if (commend["message"]=="isconnected"){
        //ui->addItemOfClientList(commend["arg"]);
        ++nbuser;
    }else if(commend["message"]=="desconnected"){
        //ui->remouveItemsOfClientList(commend["arg"],nbuser);
    }else{
        QMessageBox::critical(this, tr("erreur"), tr("un packet de comande a ??t?? recu mais la comande est incomprise."));
    }
}
QString clients::generatemesage(QMap<QString, QString> message){
    if(message["psedo"] == "" ||message["psedo"] == " "){
        message["psedo"] = "anonymous";
    }
    return(tr("<span style=\"font-size: 12px; font-weight: bold;\">")+message["psedo"]+tr("</span>")+generatedate(message)+tr("<span style=\"font-size: 14px; \">")+message["message"]+tr("</span><br/><br/>"));
}
QString clients::generatemesage(QString message, QString psedo)
{
    if(psedo == "" ||psedo == " "){
        psedo = "anonymous";
    }
    return(tr("<span style=\"font-size: 12px; font-weight: bold;\">")+psedo+tr("</span>")+generatedate()+tr("<span style=\"font-size: 14px; \">")+message+tr("</span><br/><br/>"));
}



//ilisible
void clients::socketerror(QAbstractSocket::SocketError erreur)
{
    //ui->displayconnectlabel(tr("<font color=\"#ff0000\">D??connect??</font>"));
    switch(erreur) // On affiche un message diff?rent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            //ui->displayMessagelist(generatemesage(QObject::tr("ERREUR : le serveur primaire n'a pas pu ??tre trouv??. V??rifiez le nom du serveur et le mot de passe."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ConnectionRefusedError:
            //ui->displayMessagelist(generatemesage(QObject::tr("ERREUR : le serveur primaire a refus?? la connexion. V??rifiez si le programme \"serveur\" a bien ??t?? lanc??. V??rifiez aussi l'IP et le port."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::RemoteHostClosedError:
            //ui->displayMessagelist(generatemesage(QObject::tr("ERREUR : le serveur primaire a coup?? la connexion."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::SocketAccessError:
            //ui->displayMessagelist(generatemesage(QObject::tr("L'op??ration a ??chou?? car l'application ne dispose pas des privil??ges req//uis."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;
        case QAbstractSocket::SocketResourceError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Le syst??me local a manqu?? de ressources (par exemple, trop de sockets)."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::SocketTimeoutError:
            //ui->displayMessagelist(generatemesage(QObject::tr("loperation a expir??e"),tr("chat bot")));
           //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::DatagramTooLargeError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Le datagramme ??tait plus grand que la limite du syst??me d'exploitation (q//ui peut ??tre aussi basse que 8192 octets)"),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;
        case QAbstractSocket::NetworkError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Une erreur s'est prod//uite avec le r??seau (par exemple, le c??ble r??seau a ??t?? \"accidentellement\" d??branch??)."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::UnsupportedSocketOperationError:
            //ui->displayMessagelist(generatemesage(QObject::tr("L'op??ration de socket demand??e n'est pas prise en charge par le syst??me d'exploitation local (par exemple, absence de prise en charge d'IPv6)."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyAuthenticationRequiredError:
            //ui->displayMessagelist(generatemesage(QObject::tr(" le proxy req//uiert une authentification."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyConnectionRefusedError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Impossible de contacter le serveur proxy car la connexion ?? ce serveur a ??t?? refus??e"),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyConnectionClosedError:
            //ui->displayMessagelist(generatemesage(QObject::tr("La connexion au serveur proxy a ??t?? ferm??e de mani??re inattendue (avant que la connexion au pair final ne soit ??tablie)"),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyConnectionTimeoutError:
            //ui->displayMessagelist(generatemesage(QObject::tr("La connexion au serveur proxy a expir?? ou le serveur proxy a cess?? de r??pondre lors de la phase d'authentification."),tr("chat bot")));
           //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyNotFoundError:
            //ui->displayMessagelist(generatemesage(QObject::tr("le proxi est inrouvable"),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::ProxyProtocolError:
            //ui->displayMessagelist(generatemesage(QObject::tr("La n??gociation de connexion avec le serveur proxy a ??chou??, car la r??ponse du serveur proxy n'a pas pu ??tre comprise."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::OperationError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Une op??ration a ??t?? tent??e alors que le socket ??tait dans un ??tat q//ui ne l'autorisait pas."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::TemporaryError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Une erreur temporaire s'est prod//uite (par exemple, l'op??ration bloquerait et le socket n'est pas bloquant)."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

        case QAbstractSocket::UnknownSocketError:
            //ui->displayMessagelist(generatemesage(QObject::tr("Une erreur non identifi??e s'est prod//uite."),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;

    default:
            //ui->displayMessagelist(generatemesage(QObject::tr("ERREUR : ") + socket->errorString(),tr("chat bot")));
            //ui->changestateconnectbuton(true);
        break;
    }
}
