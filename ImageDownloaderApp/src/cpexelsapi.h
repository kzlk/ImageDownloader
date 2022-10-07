#ifndef CPEXELSAPI_H
#define CPEXELSAPI_H

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtCore>

#include "restclient.h"
#include "cphotopage.h"

class CPexelsApi : public QObject
{
    Q_OBJECT
  private:
    const QString m_apiKey{};
    QString m_searchKey{};

    // Api urls
    const QString PEXELS_URL = "https://api.pexels.com/v1";

    // Photos search
    const QString PEXELS_SEARCH_PHOTOS_URL =
        QString("%1/search").arg(PEXELS_URL);

    // Photo URL
    //"page default" 1 - current page
    //"per page" default 50 - num photo per request
    //"query" - searched keyword
    QString FULL_SEARCHED_URL = QString("%1?query=%2&page=%3&per_page=%4");

    // for Authorization
    const QString HEADER_KEY = "Authorization";

    //photo item per page
    const qint16 itemPerPage = 30;

    //header - api key
    QMap<QString, QString> headerKeyValue{};

  public:
    explicit CPexelsApi(QString apiKey, QObject *parent = nullptr);

    QString getApiKey() const;

    bool getimageSearchJson(QString searchKey, int pageNumber = 1,
                            QString apiKey = "");

    pexels::CRestClient *restClient{};

    CPhotoPage *photoPage{};

  signals:
    void gotImagesUrlList();

  public slots:
    void jsonInfoReceived();
};

#endif // CPEXELSAPI_H
