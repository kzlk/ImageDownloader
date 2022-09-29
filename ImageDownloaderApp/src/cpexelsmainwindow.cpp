#include "cpexelsmainwindow.h"
#include "ui_pexelsmainwindow.h"

#include <QSplitter>
#include <QPainter>
#include <QFileDialog>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <qregularexpression.h>

#define MY_API_KEY "563492ad6f917000010000012f6388fbf5064c56baad8491e5de9bad"
int CPexelsMainWindow::counter = 0;

CPexelsMainWindow::CPexelsMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , pexelsApi(CPexelsApi(MY_API_KEY)),
      ui(new Ui::CPexelsMainWindow)
{
    ui->setupUi(this);

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
    //setCentralWidget(main_spliter);

    ui->stackedWidget->addWidget(main_spliter);
    ui->stackedWidget->setCurrentIndex(3);

    statusImgUrlLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusImgUrlLabel);
    statusImgUrlLabel->setText("Image Url: None");
    //connect button search
    connect(ui->btnSearch, &QPushButton::clicked, this, &CPexelsMainWindow::doSearch);
    connect(&pexelsApi, &CPexelsApi::gotImagesUrlList, this, &CPexelsMainWindow::loadImage);
    connect(&imgDownloader, &FileDownloader::downloaded, this, &CPexelsMainWindow::displayImage);
    connect(this, &CPexelsMainWindow::setImage, this, &CPexelsMainWindow::downloadSetImage);
    ui->listWidget->setViewMode(QListWidget::IconMode);

    //click on item on QListWidget
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &CPexelsMainWindow::viewImage);

    //
    connect(ui->listWidget,  &QListWidget::itemClicked,this, &CPexelsMainWindow::prepareImage);


    init();

    ui->grpPageTrack->setHidden(true);

    connect(ui->btnNextPage, &QPushButton::clicked, this, &CPexelsMainWindow::goToNextPage);
    connect(ui->btnPreviousPage, &QPushButton::clicked, this, &CPexelsMainWindow::goToPrevPage);
    connect(ui->btnLastPage, &QPushButton::clicked, this, &CPexelsMainWindow::goToLastPage);
    connect(ui->btnFirstPage, &QPushButton::clicked, this, &CPexelsMainWindow::goToFirstPage);

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

//TODO: add item to history
void CPexelsMainWindow :: doSearch(int pageNumber)
{
    if(ui->edtSearch->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "No search input provided");
        return;
    }

    ui->grpPageTrack->setHidden(false);
    m_imgViewer->init();
    m_imgViewer->setHidden(true);
    counter = 0;
    auto searchKey = ui->edtSearch->text();
    if(m_searchKey != searchKey)
    {
       init();
    }
    else
    {
        pageNumber = this->m_currentPage;
    }
    m_searchKey = searchKey;
    pexelsApi.getimageSearchJson(searchKey, pageNumber);
    ui->listWidget->clear();
    m_mpImageList.clear();
}

void CPexelsMainWindow::loadImage()
{
    //load image min size
    m_lastPage = pexelsApi.photoPage->totalPages();
    ui->edtTotalPages->setText(QString::number(m_lastPage));

    auto imageUrl = pexelsApi.photoPage->photos().at(counter)->srcUrl(pexelsApi.photoPage->photos().at(counter)->TINY);
    counter++;
    qDebug() << imageUrl << " | num" << counter;
    emit setImage(imageUrl, QSize(190, 190));
}

void CPexelsMainWindow::displayImage(const QString& imgUrl,
                                          QImage* image,
                                          const QSize &target_size)
{
    if (image)
    {
        const QImage img = *image;
        auto scaledImg = img.scaled(target_size);
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
        m_mpImageList.insert(item, image);
    }

    if(counter < pexelsApi.photoPage->itemsPerPage())
        loadImage();
}

void CPexelsMainWindow::downloadSetImage(QString imageUrl, QSize sz)
{
    imgDownloader.setFile(imageUrl, sz);
}

//TODO: view original image
void CPexelsMainWindow::viewImage(QListWidgetItem* item)
{
    m_imgViewer->setHidden(false);

    auto imgPtr = m_mpImageList[item];

    if(imgPtr){
        m_imgViewer->attachImagePtr(imgPtr);

        auto imgUrl = item->data(Qt::UserRole).toString();

        for(int i = 0; i < pexelsApi.photoPage->itemsPerPage(); i++)
        {
            auto a = pexelsApi.photoPage->photos()[i]->srcUrl(pexelsApi.photoPage->photos()[i]->TINY);
            if(a == imgUrl)
            {
                  statusImgUrlLabel->setText(QString("Image Url: %1").arg(pexelsApi.photoPage->photos()[i]->srcUrl(pexelsApi.photoPage->photos()[i]->ORIGINAL)));
            }else
            {
                qDebug() << imgUrl << " Original image not found";
            }
        }
    }

}

void CPexelsMainWindow::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
        doSearch();
    else
        QMainWindow::keyPressEvent(event);
}

void CPexelsMainWindow::goToNextPage()
{
    auto currentPage = ui->edtCurrentPage->text().toInt();

    if(currentPage < m_lastPage){
        ui->edtCurrentPage->setText(QString::number(++m_currentPage));
        ui->edtPreviousPage->setText(QString::number(m_currentPage - 1));
        ui->edtNextPage->setText(QString::number((m_currentPage + 1) > m_lastPage ? m_lastPage:(m_currentPage + 1)));
        ui->listWidget->clear();
        doSearch(m_currentPage);
    }

}
void CPexelsMainWindow::goToPrevPage()
{
    auto currentPage = ui->edtCurrentPage->text().toInt();
    if(currentPage > 1){
        ui->edtCurrentPage->setText(QString::number(--m_currentPage));
        ui->edtPreviousPage->setText(QString::number((m_currentPage - 1) == 0 ? 1 : (m_currentPage - 1)));
        ui->edtNextPage->setText(QString::number(m_currentPage + 1));
         ui->listWidget->clear();
        doSearch(m_currentPage);
    }
}
void CPexelsMainWindow::goToLastPage()
{
    m_currentPage = m_lastPage;
    ui->edtCurrentPage->setText(QString::number(m_lastPage));
    ui->edtPreviousPage->setText(QString::number(m_lastPage - 1));
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
//add photo ID
void CPexelsMainWindow::prepareImage(QListWidgetItem *item)
{

       static QRegularExpression re("\\d+");
       static QRegularExpressionMatch match;
               match = re.match(item->data(Qt::UserRole).toString());
       int matched{};

       if (match.hasMatch())
       {

            matched = match.captured(0).toInt();
            qDebug() << "From match " << matched;
            if(downloadPrepare.begin() == downloadPrepare.end())
            {
                downloadPrepare.push_back(matched);
                qDebug() << "Item " << matched << " added to Vector from else";
                return;
            }

            for(QVector<int>::ConstIterator iter = downloadPrepare.begin(); iter != downloadPrepare.end(); ++iter )
            {
                if(*iter == matched)
                {
                    qDebug() << "Item " << matched << " removed from Vector";
                    downloadPrepare.erase(iter);
                    return;
                }
            }
            }

       downloadPrepare.push_back(matched);
       qDebug() << "Item " << matched << " added to Vector";
       return;

       }


void CPexelsMainWindow::on_actionSearch_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void CPexelsMainWindow::on_actionDownload_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void CPexelsMainWindow::on_actionChangeFolder_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void CPexelsMainWindow::on_btn_changeFolder_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Folder choosing",
                                                     QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    ui->lineEdit_select_folder->setText(path);
}




void CPexelsMainWindow::on_pushButton_download_clicked()
{
    // delete all progress bar
    // add status bar on 2 page on stacked widget
    // function which append to queue url TINY to download and select from Image list ORIGINAL url
    // if image is select? then delete it
    // function download image with selected image
    // class download driver/engine
    //  method to get data from selected url
}



