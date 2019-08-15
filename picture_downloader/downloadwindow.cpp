#include "downloadwindow.h"
#include "ui_downloadwindow.h"
#include "downloader.h"

DownloadWindow::DownloadWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadWindow)
{
    ui->setupUi(this);

    downloader = new Downloader(this);
}

DownloadWindow::~DownloadWindow()
{
    delete ui;
}


void DownloadWindow::on_pushButton_download_clicked()
{
    //downloader->download();
    downloader->start();
}
