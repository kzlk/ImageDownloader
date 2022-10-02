#ifndef CPHOTO_H
#define CPHOTO_H
#include "IParser.h"

class CPhoto : public IParser
{
public:

    CPhoto() = default;

    enum PhotoSize
    {
        ORIGINAL,
        TINY,
        _COUNT
    };

    int id() const;
    int width() const;
    int height() const;

    //photo description
    QString alt() const;

    explicit CPhoto(QObject* parent);

    bool parseJson(const QJsonObject& jsonObject) override;

    QString srcUrl(PhotoSize photoSize) const;

private:
    QString _srcUrls[_COUNT];
    QString _alt;
    int _width, _height, _id;
};

#endif // CPHOTO_H
