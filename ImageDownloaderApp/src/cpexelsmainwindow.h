#ifndef CPEXELSMAINWINDOW_H
#define CPEXELSMAINWINDOW_H

#include "cpexelsapi.h"
#include "imageviewer.h"
#include "cphotodownloader.h"
#include "cfoldersetting.h"

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class CPexelsMainWindow; }
QT_END_NAMESPACE

class CPexelsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CPexelsMainWindow(QWidget *parent = nullptr);
    ~CPexelsMainWindow();
    CPexelsApi pexelsApi;
    FileDownloader imgDownloader;
    static int counter;

    QMap<QListWidgetItem*, QImage*> m_mpImageList;

    ImageViewer *m_imgViewer;

    CFolderSetting folderSetting{};

private:
    QLabel *statusImgUrlLabel;

    void keyPressEvent(QKeyEvent *event) override;

    QString folderPath{};
    int m_currentPage = 1;
    int m_lastPage;
    QString m_searchKey;
    void init();
    QVector<int> downloadPrepare{};
    //
    QThreadPool *pool{};

public:
    Ui::CPexelsMainWindow *ui;

public slots:
    void doSearch(int pageNumber = 1);
    void loadImage();
    void displayImage(const QString& imgUrl,
                      QImage* image,
                      const QSize &target_size);
    void downloadSetImage(QString imageUrl, QSize sz);

    void viewImage(QListWidgetItem*);

    void goToNextPage();
    void goToPrevPage();
    void goToLastPage();
    void goToFirstPage();

    void prepareImage(QListWidgetItem*);


signals:
    void setImage(QString imageUrl, QSize sz);

private slots:

    void on_actionSearch_triggered();
    void on_actionDownload_triggered();
    void on_actionChangeFolder_triggered();
    void on_btn_changeFolder_clicked();
    void on_pushButton_download_clicked();

};
#endif // CPEXELSMAINWINDOW_H
