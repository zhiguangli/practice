#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <QWidget>

namespace Ui {
class DownloadWindow;
}

class Downloader;

class DownloadWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWindow(QWidget *parent = nullptr);
    ~DownloadWindow();


private slots:
    void on_pushButton_download_clicked();

private:
    Ui::DownloadWindow *ui;
    Downloader *downloader;
};

#endif // DOWNLOADWINDOW_H
