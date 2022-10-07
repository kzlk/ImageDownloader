#ifndef CPHOTOPAGE_H
#define CPHOTOPAGE_H

#include "cphoto.h"
#include "cresultpage.h"
#include <QList>

class CPhotoPage : public CResultPage
{
  public:
    CPhotoPage() = default;

    explicit CPhotoPage(QObject *parent);

    bool parseJson(const QJsonObject &jsonObject) override;

    QVector<CPhoto *> photos() const;

  private:
    QVector<CPhoto *> _photos;
};

#endif // CPHOTOPAGE_H
