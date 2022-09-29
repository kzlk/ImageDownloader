#ifndef IPARSER_H
#define IPARSER_H

#include <QJsonObject>


class IParser
{
protected:
    virtual bool parseJson(const QJsonObject& jsonObject) = 0;
};
#endif // IPARSER_H
