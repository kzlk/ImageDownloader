#include "restclient.h"
using namespace pexels;

CRestClient::CRestClient(QString &url, QMap<QString, QString> &header,
                         QObject *parent)
    : QObject{parent}
{
    this->header = header;
    this->url = url;
}

void CRestClient::readData()
{
    dataBuffer.append(netReply->readAll());
}

void CRestClient::get()
{
    QNetworkRequest req{url};
    req.setRawHeader(this->header.firstKey().toUtf8(),
                     header.value(header.firstKey()).toUtf8());
    netManager = new QNetworkAccessManager(this);
    netReply = netManager->get(req);
    qDebug() << netReply->readBufferSize();

    connect(netReply, &QNetworkReply::readyRead, this, &CRestClient::readData);
    connect(netReply, &QNetworkReply::finished, this,
            &CRestClient::finishReading);
}

void CRestClient::finishReading()
{
    if (netReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error : " << netReply->errorString();
        QMessageBox::warning(
            NULL, "Error",
            QString("Request[Error] : %1").arg(netReply->errorString()));
    }
    else
    {
        // CONVERT THE DATA FROM A JSON DOC TO A JSON OBJECT
        userJsonInfo =
            new QJsonObject(QJsonDocument::fromJson(dataBuffer).object());
        emit jsonInfoReceived();
    }
}
