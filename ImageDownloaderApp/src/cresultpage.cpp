#include "cresultpage.h"


CResultPage::CResultPage() noexcept
{
     _pageIndex = 1;
     _itemsPerPage = 20;
     _totalItems = 0;
     _totalPages = 0;
     _hasNext = false;
}

 uint CResultPage::pageIndex() const
 {
     return _pageIndex;
 }

 uint CResultPage::itemsPerPage() const
 {
     return _itemsPerPage;
 }

 uint CResultPage::totalItems() const
 {
     return _totalItems;
 }

 uint CResultPage::totalPages() const
 {
     return _totalPages;
 }

 bool CResultPage::hasNext() const
 {
     return _hasNext;
 }

 QString CResultPage::lastError() const
 {
     return _lastError;
 }

 bool CResultPage::parseJson(const QJsonObject& jsonObject)
 {
     _pageIndex = jsonObject["page"].toInt();
     _totalItems = jsonObject["total_results"].toInt();
     _itemsPerPage = jsonObject["per_page"].toInt();
     _hasNext = jsonObject.find("next_page") != jsonObject.end();

     if (_totalItems > 0)
         _totalPages = _totalItems / _itemsPerPage + 1;
     else
         _totalPages = 0;

     return true;
 }
