#include "chistory.h"
#include "qdebug.h"

#include <QDateTime>

CHistory::CHistory()
{
}

void CHistory::updateHistory(QStringList &list)
{
    QFile outputFile(HISTORY_FILE);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream edit(&outputFile);
        for (int i = 0; i < list.size(); i++)
            edit << list[i] << Qt::endl;
    }
    outputFile.close();
}

void CHistory::addHistoryItem(const QString &item)
{
    QFile history(HISTORY_FILE);
    if (!history.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Problem opening file " << HISTORY_FILE
                 << " when writing history";
        return;
    }
    QTextStream stream(&history);
    stream << QDateTime::currentDateTime().toString() << '\t' << item << "\n";
    history.close();
}

QStringList CHistory::get()
{
    QFile history(HISTORY_FILE);
    QStringList historyList;
    if (!history.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        historyList
            << "Error! Can open file \"SearchHistory\" with serarch history\n";
        return historyList;
    }
    else
    {
        QTextStream stream(&history);
        while (!stream.atEnd())
            historyList << stream.readLine();
        return historyList;
    }
    history.close();
}
