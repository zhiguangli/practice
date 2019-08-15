#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <vector>
#include <string>
#include <memory>
#include <QObject>
#include "include/QtPromise"

using namespace QtPromise;

class QNetworkAccessManager;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);

    void start();

    void downloadTask(int page);

    QPromise<QByteArray> doHttpRequest(const QUrl &url);

    QPromise<std::vector<std::string>> parseUrls(const QByteArray& data,
                                                 const std::string& pattern);

    QPromise<std::vector<std::string>> concatUrls(const std::string &prefix,
                                                  const std::vector<std::string> &urls);

    bool save(const std::string& filePath, const QByteArray &data);

private:
    //QNetworkAccessManager *networkAccessManager;
    int sn = 0;
};


#endif // DOWNLOADER_H
