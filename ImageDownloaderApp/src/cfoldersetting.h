#ifndef CFOLDERSETTING_H
#define CFOLDERSETTING_H
#define FOLDER_INFO "FILE_FOLDER_INFO.txt"

#include <QDebug>
#include <QFile>

class CFolderSetting
{
  public:
    CFolderSetting();

    QString getFolderPath();
    void writeFolderPath(QString &path);

  private:
    QFile *folder{};
    QString folderPath{};
};

#endif // CFOLDERSETTING_H
