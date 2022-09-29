#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

namespace pexels
{
class CRestClient : public QObject
{
    Q_OBJECT
private:
    QString url{};
    QMap<QString, QString> header{};
public:
    explicit CRestClient(QString& url, QMap<QString, QString>&, QObject *parent = nullptr);
    QNetworkAccessManager *netManager{};
    QNetworkReply *netReply{};
    QByteArray dataBuffer{};
    QPixmap *img{};

    //GET
    void get();

    QJsonObject *userJsonInfo{};

signals:
    void jsonInfoReceived();

public slots:
    void readData();
    void finishReading();
};
}
#endif // RESTCLIENT_H
