#ifndef CPHOTODOWNLOADER_H
#define CPHOTODOWNLOADER_H
#include "cphoto.h"
#include <QFile>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QRunnable>
#include <QStringList>

class FileDownloader : public QObject, public QRunnable
{
    Q_OBJECT

  public:
    explicit FileDownloader(QObject *parent = 0);
    FileDownloader(CPhoto *photoObj, QString& filePath, int &pBarIndex, QObject *parent = 0);
    ~FileDownloader();

  private:
    QNetworkAccessManager *manager{};
    QNetworkReply *reply{};
    QUrl url{};

    //"USER - AGENT"
    const QByteArray userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                                 "AppleWebKit/537.36 (KHTML, like Gecko)"
                                 " Chrome/105.0.0.0 Safari/537.36";

    QString m_fileUrl{};
    QByteArray m_DownloadedData{};
    QSize m_target_size{};

    QString downloadPath{};
    int progressBarIndex{};

    CPhoto *photo{};

  protected:
    void run() override;

  private slots:
    void onFinished(QNetworkReply *);
    // for progress bar
    void updateProgress(qint64 now, qint64 total);
    void saveFileToDrive(QNetworkReply *reply);

  public slots:
    void setFile(QString fileURL, const QSize &target_size = {});

  signals:

    // return byte array of downloaded photo
    void downloaded(const QString &imgUrl, QByteArray *, const QSize = {});

    // update progress bar in main window
    void updateProgressBar(int receivedPersent, int pBarIndex);
};

#endif // CPHOTODOWNLOADER_H
