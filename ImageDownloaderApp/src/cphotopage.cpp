#include "cphotopage.h"
#include "qjsonarray.h"


QVector<CPhoto*> CPhotoPage::photos() const
{
    return _photos;
}

bool CPhotoPage::parseJson(const QJsonObject& jsonObject)
{
    if (!CResultPage::parseJson(jsonObject))
        return false;

    // parse photos
    const QJsonArray& jsonArray = jsonObject["photos"].toArray();

    std::for_each(jsonArray.begin(), jsonArray.end(), [=](const QJsonValue& jsonValue) {
        auto photo = new CPhoto();
        photo->parseJson(jsonValue.toObject());
        _photos << photo;
    });

    return true;
}
