#include "cphoto.h"

CPhoto::CPhoto(QObject* parent){};

QString CPhoto::srcUrl(PhotoSize photoSize) const
{
    return _srcUrls[(int)photoSize];
}

bool CPhoto::parseJson(const QJsonObject &jsonObject)
{


    _width = jsonObject["width"].toInt();
    _height = jsonObject["height"].toInt();

    static QMap<QString, int> photoSizesMap
    {
         { "original", PhotoSize::ORIGINAL },
         { "tiny", PhotoSize::TINY }
    };

    const auto& keys = photoSizesMap.keys();
    std::for_each(keys.begin(), keys.end(), [=](const QString& key) {
        _srcUrls[photoSizesMap[key]] = jsonObject["src"].toObject()[key].toString();
    });

    return true;
}

int CPhoto::width() const
{
    return _width;
}

int CPhoto::height() const
{
    return _height;
}


