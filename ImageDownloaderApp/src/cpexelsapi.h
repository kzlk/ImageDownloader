#ifndef CPEXELSAPI_H
#define CPEXELSAPI_H

#include <QtCore>
#include "restclient.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>

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
    const QString PEXELS_SEARCH_PHOTOS_URL = QString("%1/search").arg(PEXELS_URL);
    // Photo URL
    //"page default" 1 - current page
    //"per page" default 50 - num photo per request
    //"query" - searched keyword
     QString FULL_SEARCHED_URL = QString("%1?query=%2&page=%3&per_page=%4");

    //for Authorization
    QString HEADER_KEY = "Authorization";
    //my API key
    QString API = "563492ad6f917000010000012f6388fbf5064c56baad8491e5de9bad";

    qint16 itemPerPage = 60;

    QMap<QString, QString> headerKeyValue;

public:
    explicit CPexelsApi(QString apiKey, QObject *parent = nullptr);

    QString getApiKey() const;

    bool getimageSearchJson(QString searchKey,
                            int pageNumber = 1,
                            QString apiKey = "");

    pexels::CRestClient *restClient{};

    QList<QString> *listImageUrl{};

    int m_totalPages{};

    const QString &getSearcedQuery() const;

    const QString &getHeaderKey() const;

    const QString &getHeaderValue() const;

    void setHeaderValue(const QString &newHeaderValue);

    CPhotoPage* photoPage{};

signals:
    void gotImagesUrlList();

public slots:
    void jsonInfoReceived();
};

#endif // CPEXELSAPI_H
