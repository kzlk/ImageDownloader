#ifndef CRESULTPAGE_H
#define CRESULTPAGE_H
#include <QObject>
#include "IParser.h"


class CResultPage : IParser
{
public:
    CResultPage() noexcept;


    uint pageIndex() const;
    uint itemsPerPage() const;
    uint totalItems() const;
    uint totalPages() const;
    bool hasNext() const;
    QString lastError() const;

    bool parseJson(const QJsonObject& jsonObject) override;

private:
    uint _pageIndex;
    bool _hasNext;
    uint _itemsPerPage;
    uint _totalItems;
    uint _totalPages;
    QString _lastError;

};

#endif // CRESULTPAGE_H
