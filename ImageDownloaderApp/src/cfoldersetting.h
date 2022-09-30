#ifndef CFOLDERSETTING_H
#define CFOLDERSETTING_H
#include <QFile>
#include <QDebug>
#define FOLDER_INFO "FILE_FOLDER_INFO.txt"

class CFolderSetting
{
public:
    CFolderSetting();

   QString getFolderPath();
   void writeFolderPath(QString& path);

private:
    QFile* folder{};
    QString folderPath{};
};

#endif // CFOLDERSETTING_H
