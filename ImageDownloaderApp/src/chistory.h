#ifndef CHISTORY_H
#define CHISTORY_H

#include <QString>
#include <QFile>
#include <QTextStream>
#define HISTORY_FILE "SearchHistory.txt"

class CHistory
{
public:
    CHistory();
    static void updateHistory(QStringList& list);
    static void addHistoryItem(const QString&item);
    static QStringList get();
};

#endif // CHISTORY_H
