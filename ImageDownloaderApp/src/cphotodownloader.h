#ifndef CPHOTODOWNLOADER_H
#define CPHOTODOWNLOADER_H

#include "qimagereader.h"
#include "qmutex.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QStringList>
#include <QListWidget>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = 0);
    virtual ~FileDownloader();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    const QByteArray userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36";
    QString m_fileUrl;
    QByteArray m_DownloadedData;
    QSize m_target_size;

private slots:
    void onFinished(QNetworkReply*);

public slots:
    void setFile(QString fileURL,
                 const QSize target_size);


signals:
   void downloaded(const QString& imgUrl,
                   QImage*,
                   const QSize);

};

#endif // CPHOTODOWNLOADER_H
