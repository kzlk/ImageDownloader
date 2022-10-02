#include "cfoldersetting.h"

CFolderSetting::CFolderSetting()
{

    folder = new QFile(FOLDER_INFO);

    if(folder->size() != 0) return;

    if(!folder->open(QIODevice::WriteOnly))
    {
        qDebug() << "Problem to open file \n" << FOLDER_INFO;
        return;
    }

#ifdef __linux__
        folderPath = "home/" + QString(getlogin()) + "/downloads/";
#elif _WIN32
        folderPath = "C:/Users/" + QString(getenv("USERNAME")) + "/Downloads/";
#endif
        folder->write(folderPath.toUtf8());
        qDebug() << "Default Path" << folderPath << " wrote to" << FOLDER_INFO;

       folder->close();
}

QString CFolderSetting::getFolderPath()
{
    if (!folder->open(QIODevice::ReadOnly))
    {
         qDebug() << "Problem opening file when reading from " << FOLDER_INFO;
         return "";
    }

    folderPath = QString(folder->readAll());
    qDebug() << "Folder path " << folderPath << FOLDER_INFO;
    folder->close();
    return folderPath;

}

void CFolderSetting::writeFolderPath(QString &path)
{
    if(!folder->open(QIODevice::WriteOnly))
    {
        qDebug() << "Problem to open file \n" << FOLDER_INFO;
        return;
    }

    folder->write(path.toUtf8());
    qDebug() << "Path" << path << " wrote to" << FOLDER_INFO;
    folder->close();
}
