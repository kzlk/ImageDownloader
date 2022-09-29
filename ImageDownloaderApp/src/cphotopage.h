#ifndef CPHOTOPAGE_H
#define CPHOTOPAGE_H

#include "cphoto.h"
#include "cresultpage.h"
#include <QList>


class CPhotoPage : public CResultPage
{
public:
    CPhotoPage() = default;

    explicit CPhotoPage(QObject* parent);

    bool parseJson(const QJsonObject& jsonObject) override;

    QList<CPhoto*> photos() const;

   private:

    QList<CPhoto*> _photos;

};

#endif // CPHOTOPAGE_H
