#ifndef CPEXELSMAINWINDOW_H
#define CPEXELSMAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QProgressBar>

#include "cfoldersetting.h"
#include "chistory.h"
#include "cpexelsapi.h"
#include "cphotodownloader.h"
#include "imageviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class CPexelsMainWindow;
}
QT_END_NAMESPACE

class CPexelsMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    CPexelsMainWindow(QWidget *parent = nullptr);
    virtual ~CPexelsMainWindow();

  private:
    void keyPressEvent(QKeyEvent *event) override;
    void init();

    QLabel *statusImgUrlLabel;
    QString folderPath{};
    int m_currentPage = 1;
    int m_lastPage;
    QString m_searchKey;
    QVector<int> downloadPrepare{};
    QMap<QListWidgetItem *, QImage *> m_mpImageList{};
    QVector<CPhoto *> readyForDownload{};
    QVector<QProgressBar *> progressBar{};
    ImageViewer *m_imgViewer{};
    CFolderSetting folderSetting{};
    CPexelsApi pexelsApi;
    FileDownloader imgDownloader;
    CHistory searchHistory;

    QVector<int> myIndex{};
    QVector<QListWidgetItem *> selectedI{};

    static int counter;

  public:
    Ui::CPexelsMainWindow *ui;

  public slots:
    void doSearch(int pageNumber = 1);
    void loadImage();
    void displayImage(const QString &imgUrl, QByteArray *image,
                      const QSize &target_size);
    void downloadSetImage(QString imageUrl, QSize sz);

    void viewImage(QListWidgetItem *);

    void goToNextPage();
    void goToPrevPage();
    void goToLastPage();
    void goToFirstPage();

    void prepareImage(QListWidgetItem *);

    void updPBar(int pValue, int indexP);

    bool checkFolder(QString &path);

    void updateHistoryFile(QListWidget * item);

  signals:
    void setImage(QString imageUrl, QSize sz);
    void updateHistFile(QListWidget* item);

  private slots:
    void on_actionSearch_triggered();
    void on_actionDownload_triggered();
    void on_actionChangeFolder_triggered();
    void on_btn_changeFolder_clicked();
    void on_pushButton_download_clicked();
    void on_pushButton_clicked();
    void on_actionHistory_triggered();
    void on_clearch_history_clicked();
};
#endif // CPEXELSMAINWINDOW_H
