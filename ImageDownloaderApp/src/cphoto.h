#ifndef CPHOTO_H
#define CPHOTO_H
#include "IParser.h"
#include <QMap>

class CPhoto : public IParser
{
public:

    CPhoto() = default;

    enum PhotoSize
    {
        ORIGINAL, // original image quality
        TINY,     // tiny image quality
        _COUNT
    };

    int id() const;
    int width() const;
    int height() const;

    // get photo description
    QString alt() const;

    explicit CPhoto(QObject* parent);

    bool parseJson(const QJsonObject& jsonObject) override;

    //get image url
    QString srcUrl(PhotoSize photoSize) const;

private:
    QString _srcUrls[_COUNT];
    QString _alt;
    int _width, _height, _id;
};

#endif // CPHOTO_H
