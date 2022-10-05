#include "cpexelsmainwindow.h"
#include "ui_pexelsmainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QQueue>
#include <QRegularExpressionMatch>
#include <QSplitter>
#include <qregularexpression.h>

#define MY_API_KEY "563492ad6f917000010000012f6388fbf5064c56baad8491e5de9bad"
int CPexelsMainWindow::counter = 0;

CPexelsMainWindow::CPexelsMainWindow(QWidget *parent)
    : QMainWindow(parent), pexelsApi(CPexelsApi(MY_API_KEY)),
      ui(new Ui::CPexelsMainWindow)
{
    ui->setupUi(this);
    //
    folderPath = folderSetting.getFolderPath();

    ui->lineEdit_select_folder->setText(this->folderPath);

    auto image_spliter = new QSplitter(Qt::Horizontal);
    image_spliter->addWidget(ui->listWidget);
    image_spliter->addWidget(m_imgViewer = new ImageViewer());

    auto main_spliter = new QSplitter(Qt::Vertical);
    main_spliter->addWidget(ui->grpPexelsSearch);
    main_spliter->addWidget(image_spliter);
    main_spliter->addWidget(ui->grpPageTrack);

    m_imgViewer->setHidden(true);

    int w = main_spliter->width();
    main_spliter->setSizes({w / 7, 10 * w / 7, w / 10});

    ui->stackedWidget->addWidget(main_spliter);
    ui->stackedWidget->setCurrentIndex(4);

    statusImgUrlLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusImgUrlLabel);
    statusImgUrlLabel->setText("Image Url: None");

    // connect button search
    connect(ui->btnSearch, &QPushButton::clicked, this,
            &CPexelsMainWindow::doSearch);
    connect(&pexelsApi, &CPexelsApi::gotImagesUrlList, this,
            &CPexelsMainWindow::loadImage);

    connect(&imgDownloader, &FileDownloader::downloaded, this,
            &CPexelsMainWindow::displayImage);

    connect(this, &CPexelsMainWindow::setImage, this,
            &CPexelsMainWindow::downloadSetImage);
    ui->listWidget->setViewMode(QListWidget::IconMode);

    // click on item on QListWidget
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this,
            &CPexelsMainWindow::viewImage);

    //
    connect(ui->listWidget, &QListWidget::itemClicked, this,
            &CPexelsMainWindow::prepareImage);

    init();

    ui->grpPageTrack->setHidden(true);

    connect(ui->btnNextPage, &QPushButton::clicked, this,
            &CPexelsMainWindow::goToNextPage);
    connect(ui->btnPreviousPage, &QPushButton::clicked, this,
            &CPexelsMainWindow::goToPrevPage);
    connect(ui->btnLastPage, &QPushButton::clicked, this,
            &CPexelsMainWindow::goToLastPage);
    connect(ui->btnFirstPage, &QPushButton::clicked, this,
            &CPexelsMainWindow::goToFirstPage);

    connect(this, &CPexelsMainWindow::updateHistFile, this,
            &CPexelsMainWindow::updateHistoryFile);

    this->showMaximized();
}

CPexelsMainWindow::~CPexelsMainWindow()
{
    delete ui;
}

void CPexelsMainWindow::init()
{
    m_currentPage = 1;
    ui->edtCurrentPage->setText("1");
    ui->edtPreviousPage->setText("1");
    ui->edtNextPage->setText("2");
}

void CPexelsMainWindow ::doSearch(int pageNumber)
{
    if (ui->edtSearch->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "No search input provided");
        return;
    }

    ui->grpPageTrack->setHidden(false);
    m_imgViewer->init();
    m_imgViewer->setHidden(true);
    counter = 0;
    auto searchKey = ui->edtSearch->text();
    if (m_searchKey != searchKey)
    {
        init();
    }
    else
    {
        pageNumber = this->m_currentPage;
    }
    m_searchKey = searchKey;
    CHistory::addHistoryItem(searchKey);
    pexelsApi.getimageSearchJson(searchKey, pageNumber);
    ui->listWidget->clear();
    m_mpImageList.clear();
}

void CPexelsMainWindow::loadImage()
{
    // load image min size
    m_lastPage = pexelsApi.photoPage->totalPages();
    ui->edtTotalPages->setText(QString::number(m_lastPage));

    // get TINY image url for faster preview
    auto imageUrl = pexelsApi.photoPage->photos().at(counter)->srcUrl(
        pexelsApi.photoPage->photos().at(counter)->TINY);
    counter++;

    qDebug() << imageUrl << " | num" << counter;
    emit setImage(imageUrl, QSize(190, 190));
}

// display scaled image && add image to QListWidget
void CPexelsMainWindow::displayImage(const QString &imgUrl, QByteArray *image,
                                     const QSize &target_size)
{
    QImage *img = new QImage();
    img->loadFromData(*image);

    if (img)
    {
        auto scaledImg = img->scaled(target_size);
        QPixmap *pixmap = new QPixmap(target_size);
        pixmap->fill(Qt::transparent);

        QPainter painter(pixmap);
        int x = (target_size.width() - scaledImg.width()) / 2;
        int y = (target_size.height() - scaledImg.height()) / 2;
        painter.drawImage(QRect{{x, y}, scaledImg.size()}, scaledImg);
        painter.end();

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::DecorationRole, *pixmap);
        item->setData(Qt::UserRole, imgUrl);
        ui->listWidget->addItem(item);
        m_mpImageList.insert(item, img);
    }

    if (counter < pexelsApi.photoPage->itemsPerPage())
        loadImage();
}

void CPexelsMainWindow::downloadSetImage(QString imageUrl, QSize sz)
{
    imgDownloader.setFile(imageUrl, sz);
}

// Double click for image
// TODO: view original image when double click
void CPexelsMainWindow::viewImage(QListWidgetItem *item)
{
    m_imgViewer->setHidden(false);

    auto imgPtr = m_mpImageList[item];

    if (imgPtr)
    {
        m_imgViewer->attachImagePtr(imgPtr);

        auto imgUrl = item->data(Qt::UserRole).toString();

        statusImgUrlLabel->setText(QString("Image Url: %1").arg(imgUrl));
    }
}

void CPexelsMainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
        doSearch();
    else
        QMainWindow::keyPressEvent(event);
}

void CPexelsMainWindow::goToNextPage()
{
    auto currentPage = ui->edtCurrentPage->text().toInt();

    if (currentPage < m_lastPage)
    {
        ui->edtCurrentPage->setText(QString::number(++m_currentPage));
        ui->edtPreviousPage->setText(QString::number(m_currentPage - 1));
        ui->edtNextPage->setText(QString::number(
            (m_currentPage + 1) > m_lastPage ? m_lastPage
                                             : (m_currentPage + 1)));
        ui->listWidget->clear();
        doSearch(m_currentPage);
    }
}
void CPexelsMainWindow::goToPrevPage()
{
    auto currentPage = ui->edtCurrentPage->text().toInt();
    if (currentPage > 1)
    {
        ui->edtCurrentPage->setText(QString::number(--m_currentPage));
        ui->edtPreviousPage->setText(QString::number(
            (m_currentPage - 1) == 0 ? 1 : (m_currentPage - 1)));
        ui->edtNextPage->setText(QString::number(m_currentPage + 1));
        ui->listWidget->clear();
        doSearch(m_currentPage);
    }
}
void CPexelsMainWindow::goToLastPage()
{
    m_currentPage = m_lastPage;
    ui->edtCurrentPage->setText(QString::number(m_lastPage));
    ui->edtPreviousPage->setText(QString::number(m_lastPage));
    ui->edtNextPage->setText(QString::number(m_lastPage));
    ui->listWidget->clear();
    doSearch(m_lastPage);
}

void CPexelsMainWindow::goToFirstPage()
{
    m_currentPage = 1;
    ui->edtCurrentPage->setText(QString::number(m_currentPage));
    ui->edtPreviousPage->setText(QString::number(m_currentPage));
    ui->edtNextPage->setText(QString::number(m_currentPage + 1));
    ui->listWidget->clear();
    doSearch(m_currentPage);
}

// add photo ID
void CPexelsMainWindow::prepareImage(QListWidgetItem *item)
{
    static QRegularExpression re("(\\d+)");
    re.setPatternOptions(QRegularExpression::MultilineOption);
    static QRegularExpressionMatch match;
    match = re.match(item->data(Qt::UserRole).toString());
    qDebug() << "All match -> " << match << '\n';

    // photo id
    int matched{};

    if (match.hasMatch())
    {
        matched = match.captured(0).toInt();

        // display photo name

        for (auto &i : pexelsApi.photoPage->photos())
        {
            if (matched == i->id())
            {
                statusImgUrlLabel->setText(
                    QString("Description: " + i->alt() + " | ID %1")
                        .arg(matched));

                if (readyForDownload.begin() == readyForDownload.end())
                {
                    readyForDownload.push_back(i);
                    qDebug() << "Item " << matched << " added to Vector";
                    return;
                }

                for (QVector<CPhoto *>::Iterator iter =
                         readyForDownload.begin();
                     iter != readyForDownload.end(); ++iter)
                {
                    auto photo = *iter;
                    if (photo->id() == i->id())
                    {
                        qDebug()
                            << "Item " << matched << " removed from Vector";
                        readyForDownload.erase(iter);
                        statusImgUrlLabel->setText(
                            QString("Description: None | ID: None"));
                        return;
                    }
                    else
                    {
                        readyForDownload.push_back(i);
                        qDebug() << "Item " << matched << " added to Vector";
                        return;
                    }
                }
            }
        }
    }
}

void CPexelsMainWindow::updPBar(int pValue, int indexP)
{
    progressBar.at(indexP)->setValue(pValue);
}

bool CPexelsMainWindow::checkFolder(QString &path)
{
    QFileInfo folder(path);
    if (!folder.exists())
    {
        QMessageBox::warning(this, "Error! ", "Folder doesn't exists");
        return false;
    }

    if (!folder.isDir())
    {
        QMessageBox::warning(this, "Error! ", "This is not a directory!");
        return false;
    }

    return true;
}

void CPexelsMainWindow::updateHistoryFile(QListWidget *item)
{
    QStringList pickedItem{};
    for (int i = 0; i < item->count(); ++i)
        pickedItem << item->item(i)->text();

    CHistory::updateHistory(pickedItem);
}

void CPexelsMainWindow::on_actionSearch_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void CPexelsMainWindow::on_actionDownload_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void CPexelsMainWindow::on_actionChangeFolder_triggered()
{
    qDebug() << "Folder << " << folderPath;

    ui->stackedWidget->setCurrentIndex(1);

    checkFolder(folderPath);
}

void CPexelsMainWindow::on_btn_changeFolder_clicked()
{
    QString path = QFileDialog::getExistingDirectory(
        this, "Folder choosing",
        QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    path += "/";
    if (!checkFolder(path))
        return;
    ui->lineEdit_select_folder->setText(path);
    folderPath = path;
    folderSetting.writeFolderPath(path);
}

void CPexelsMainWindow::on_pushButton_download_clicked()
{
    if (!checkFolder(folderPath))
        return;
    for (auto &pBar : progressBar)
        delete pBar;
    progressBar.clear();

    QQueue<CPhoto *> photo;
    std::move(readyForDownload.begin(), readyForDownload.end(),
              std::inserter(photo, photo.end()));
    readyForDownload.clear();

    auto rowCount = photo.size();
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(rowCount);

    for (int i = 0; i < rowCount; i++)
    {
        QProgressBar *bar = new QProgressBar();
        bar->setStyleSheet(
            "QProgressBar {"
            "background-color:rgb(200,200,200);"
            "color:rgb(170,85,127);"
            "border-style:solid;"
            "border-radius: 10px;"
            "text-align: center;}"
            "QProgressBar::chunk{"
            "border-radius:10px;"
            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, "
            "y2:1, "
            "stop:0 rgba(185, 105, 254, 255), stop:1 rgba(106,211,255,255));}");

        progressBar.push_back(bar);
        ui->tableWidget->setCellWidget(i, 0, bar);
        FileDownloader *downloader =
            new FileDownloader(photo.dequeue(), folderPath, i);
        QThreadPool::globalInstance()->start(downloader);
        connect(downloader, &FileDownloader::updateProgressBar, this,
                &CPexelsMainWindow::updPBar);
    }
}
// clear all item selection on listView and clear all image in prepare for
// downloading
void CPexelsMainWindow::on_pushButton_clicked()
{
    readyForDownload.clear();
    ui->listWidget->clearSelection();
}

void CPexelsMainWindow::on_actionHistory_triggered()
{
    ui->listWidget_history->clear();
    ui->listWidget_history->setStyleSheet("font: 20pt Comic Sans MS");
    ui->listWidget_history->sortItems(Qt::DescendingOrder);
    ui->listWidget_history->addItems(CHistory::get());

    if (ui->listWidget_history->count() == 0)
        ui->listWidget_history->addItem("There are no history record now");

    auto res = ui->listWidget_history->sizeHintForColumn(0) +
               15 * ui->listWidget_history->frameWidth();
    ui->listWidget_history->setFixedWidth(res);
    ui->stackedWidget->setCurrentIndex(2);
}

void CPexelsMainWindow::on_clearch_history_clicked()
{
    qDeleteAll(ui->listWidget_history->selectedItems());
    emit updateHistFile(ui->listWidget_history);
}
