#include "cphoto.h"

CPhoto::CPhoto(QObject* parent){}

int CPhoto::id() const
{
    return _id;
};

QString CPhoto::srcUrl(PhotoSize photoSize) const
{
    return _srcUrls[(int)photoSize];
}

bool CPhoto::parseJson(const QJsonObject &jsonObject)
{

    _id = jsonObject["id"].toInt();
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

    _alt = jsonObject["alt"].toString();

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

QString CPhoto::alt() const
{
    return _alt;
}


