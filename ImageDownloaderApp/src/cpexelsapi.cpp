#include "cpexelsapi.h"

#include <QString>

CPexelsApi::CPexelsApi(QString apiKey, QObject *parent)
    : QObject{parent}, m_apiKey(apiKey)
{
}

QString CPexelsApi::getApiKey() const
{
    return m_apiKey;
}

bool CPexelsApi::getimageSearchJson(QString searchKey, int pageNumber,
                                    QString apiKey)
{
    m_searchKey = searchKey;

    if (apiKey == "")
        apiKey = getApiKey();
    if (apiKey.isEmpty())
        return false;

    // url for photo search
    QString url = FULL_SEARCHED_URL.arg(PEXELS_SEARCH_PHOTOS_URL)
                      .arg(searchKey)
                      .arg(pageNumber)
                      .arg(itemPerPage);

    qDebug() << "Requested URL: " << url;

    headerKeyValue.insert(HEADER_KEY, apiKey);
    restClient = new pexels::CRestClient(url, headerKeyValue);
    restClient->get();

    connect(restClient, SIGNAL(jsonInfoReceived()), this,
            SLOT(jsonInfoReceived()));
    return true;
}

void CPexelsApi::jsonInfoReceived()
{
    QJsonObject *jsonList = restClient->userJsonInfo;

    photoPage = new CPhotoPage();
    photoPage->parseJson(*jsonList);

    emit gotImagesUrlList();
}
