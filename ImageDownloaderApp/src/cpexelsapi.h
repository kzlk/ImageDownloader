#ifndef CPEXELSAPI_H
#define CPEXELSAPI_H

#include "restclient.h"
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtCore>

#include "cphotopage.h"

class CPexelsApi : public QObject
{
    Q_OBJECT
  private:
    const QString m_apiKey;
    bool m_totalPagesSet;
    QString m_searchKey;

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

    QString ORIGINAL_IMAGE_URL =
        "https://images.pexels.com/photos/%1/pexels-photo-%2.%3";

    // for Authorization
    const QString HEADER_KEY = "Authorization";
    // my API key
    const QString API =
        "563492ad6f917000010000012f6388fbf5064c56baad8491e5de9bad";

    const qint16 itemPerPage = 30;

    QMap<QString, QString> headerKeyValue;

  public:
    explicit CPexelsApi(QString apiKey, QObject *parent = nullptr);

    QString getApiKey() const;

    bool getimageSearchJson(QString searchKey, int pageNumber = 1,
                            QString apiKey = "");

    pexels::CRestClient *restClient{};

    QList<QString> *listImageUrl{};

    int m_totalPages{};

    const QString &getSearcedQuery() const;

    const QString &getHeaderKey() const;

    const QString &getHeaderValue() const;

    void setHeaderValue(const QString &newHeaderValue);

    CPhotoPage *photoPage{};

  signals:
    void gotImagesUrlList();

  public slots:
    void jsonInfoReceived();
};

#endif // CPEXELSAPI_H
