#include "cphotodownloader.h"
#include "src/cpexelsmainwindow.h"

#include <QListWidgetItem>
#include <QPainter>

FileDownloader::FileDownloader(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager;

    // cache
    diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory("cacheDir");
    manager->setCache(diskCache);

    connect(manager, &QNetworkAccessManager::finished, this,
            &FileDownloader::onFinished);


}

FileDownloader::FileDownloader(CPhoto *photoObj, QString &folderPath,
                               int &pBarIndex, QObject *parent)
    : QObject(parent), downloadPath(folderPath), progressBarIndex(pBarIndex),
      photo(photoObj)
{
}

FileDownloader::~FileDownloader()
{
    manager->deleteLater();
    delete diskCache;
}

void FileDownloader::run()
{
    manager = new QNetworkAccessManager;
    setFile((photo->srcUrl(photo->ORIGINAL)));

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()),
            Qt::QueuedConnection);
    connect(manager, &QNetworkAccessManager::finished, this,
            &FileDownloader::saveFileToDrive, Qt::DirectConnection);
    connect(reply, &QNetworkReply::downloadProgress, this,
            &FileDownloader::updateProgress, Qt::DirectConnection);
    loop.exec();
}

void FileDownloader::saveFileToDrive(QNetworkReply *reply)
{
    switch (reply->error())
    {
    case QNetworkReply::NoError: {
        qDebug("file is downloaded successfully.");
        QFile file(downloadPath + photo->alt() + url.fileName());
        qDebug() << "Thread # "
                 << Concurrency::details::platform::GetCurrentThreadId()
                 << "download in "
                 << downloadPath + photo->alt() + url.fileName();
        reply->deleteLater();
        m_DownloadedData.append(reply->readAll());
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(m_DownloadedData);
        }

        file.close();
    }
    break;
    default: {
        qDebug(reply->errorString().toLatin1());
    };
    }
}

void FileDownloader::setFile(QString fileURL, const QSize &target_size)
{
    m_DownloadedData.clear();
    m_target_size = target_size;

    m_fileUrl = fileURL;

    QNetworkRequest request;
    url = m_fileUrl;
    request.setUrl(url);

    request.setRawHeader("User-Agent", userAgent);

    QSslConfiguration sslConfiguration(
        QSslConfiguration::defaultConfiguration());
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    request.setSslConfiguration(sslConfiguration);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                         QNetworkRequest::PreferCache);
    reply = manager->get(request);
}

void FileDownloader::onFinished(QNetworkReply *reply)
{
    //image from cache? or no?
    QVariant fromCache =
        reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute);
    qDebug() << "Page from cache?" << fromCache.toBool();

    switch (reply->error())
    {
    case QNetworkReply::NoError: {
        qDebug("file is downloaded successfully.");
        reply->deleteLater();
        m_DownloadedData.append(reply->readAll());
        emit downloaded(m_fileUrl, &m_DownloadedData, m_target_size);
    }
    break;
    default: {
        qDebug(reply->errorString().toLatin1());
    };
    }
}

// update progress bar
// TODO: make progress in byte/mb per sec -> (1.2MB/3.6MB)
void FileDownloader::updateProgress(qint64 now, qint64 total)
{
    emit updateProgressBar(100 * now / total, progressBarIndex);
}
