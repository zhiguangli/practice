#include "downloader.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <string>
#include <regex>
#include <fstream>
#include <QEventLoop>
#include <QDebug>
#include <QtConcurrent>

Downloader::Downloader(QObject *parent) : QObject(parent)
{
}

void Downloader::start()
{
    QThreadPool::globalInstance()->setMaxThreadCount(8);
    for(int page = 1; page <= 3; ++page)
        QtConcurrent::run([=]{ downloadTask(page); });
}

void Downloader::downloadTask(int page)
{
    std::string baseUrl = "http://www.51ztzj.com";
    std::string url =  baseUrl + "/dbizhi/category_27_" + std::to_string(page) + ".htm";

    //1.发起当前页面请求，获取当前页面返回数据
    doHttpRequest(QUrl(url.data()))
            .then([=](const QByteArray &data){

                std::string pattern = "/desk/[0-9]+.htm";
                //2. 根据页面返回数据，解析符合"/desk/[0-9]+.htm"这种形式的链接
                return parseUrls(data, pattern);
            })
            .then([=](const std::vector<std::string> &pageUrls){
                //3. 把上一步的符合"/desk/[0-9]+.htm"这种形式的链接拼接成合法的url.
                return concatUrls(baseUrl, pageUrls);
            })
            .each([=](const std::string &pageUrl, int index){

                    //4. 发起图片页面请求 ("http://www.51ztzj.com/desk/[0-9]+.htm")
                    doHttpRequest(QUrl(pageUrl.data()))
                            .then([=](const QByteArray &data){

                                std::string pattern = "<img src=\"http://img.51ztzj.com//upload/image/.+/.+_670x419.jpg";
                                //5. 根据页面返回数据，
                                //   解析符合"<img src=\"http://img.51ztzj.com//upload/image/.+/.+_670x419.jpg"
                                //   这种形式的字符串
                                return parseUrls(data, pattern);
                            })
                            .then([=](const std::vector<std::string> &urls) {

                                //6. 对这种"<img src=\"http://img.51ztzj.com//upload/image/.+/.+_670x419.jpg"这种形式的字符串，
                                //   进行去掉前10个字符,得到这种"http://img.51ztzj.com//upload/image/.+/.+_670x419.jpg"形式的字符串
                                if(!urls.empty()) {
                                    //下载图片
                                    std::string url = urls[0];
                                    return std::string(url.erase(0, 10));
                                } else {
                                    return std::string();
                                }
                            })
                            .then([=](const std::string &imageUrl){

                                if(imageUrl.empty())
                                    return;
                                //7. 发起下载图片请求
                                doHttpRequest(QUrl(imageUrl.data()))
                                        .then([&](const QByteArray &data){

                                            std::string filePath = qApp->applicationDirPath().toStdString()
                                                                    + "/download/"
                                                                    + std::to_string(this->sn++)
                                                                    + ".jpg";
                                            //8. 把图片数据写入文件
                                            save(filePath, data);
                                        });
                            }).wait();
            }).wait();
}

QPromise<QByteArray> Downloader::doHttpRequest(const QUrl &url)
{
    return QPromise<QByteArray>([&](
            const QPromiseResolve<QByteArray>& resolve,
            const QPromiseReject<QByteArray>& reject) {

            QEventLoop eventloop;
            QNetworkAccessManager networkAccessManager;
            QNetworkReply* reply = networkAccessManager.get(QNetworkRequest(url));

            connect(reply, &QNetworkReply::finished, [=, &eventloop]() {
                if (reply->error() == QNetworkReply::NoError)
                    resolve(reply->readAll());
                 else
                    reject(reply->error());

                eventloop.quit();
                reply->deleteLater();
            });

            eventloop.exec();
        });
}

QPromise<std::vector<std::string>> Downloader::parseUrls(const QByteArray& data, const std::string &pattern)
{
    return QtPromise::resolve(QtConcurrent::run([=](const QByteArray& data, const std::string &pattern) {
        std::vector<std::string> entries;

        std::string replyData = data.toStdString();
        //使用c++11提供的正则表达库
        std::regex r(pattern);
        auto begin = std::sregex_iterator(replyData.begin(), replyData.end(), r);
        auto end   = std::sregex_iterator();

        for(std::sregex_iterator iter = begin; iter != end; ++iter)
            entries.emplace_back(iter->str());

        return entries;
    }, data, pattern));
}

QPromise<std::vector<std::string>> Downloader::concatUrls(const std::string &prefix, const std::vector<std::string> &urls)
{
    return QtPromise::resolve(QtConcurrent::run([](const std::string &prefix, const std::vector<std::string> &urls) {
        std::vector<std::string> entries;

        for (size_t i = 0; i < urls.size(); ++i)
            entries.emplace_back(prefix + urls[i]);

        return entries;
    }, prefix, urls));
}

bool Downloader::save(const std::string &filePath, const QByteArray &data)
{
    try
    {
        std::fstream outStream(filePath, std::fstream::binary
                              | std::fstream::trunc
                              | std::fstream::out);

        if(!outStream.is_open())
            return false;

        std::string content = data.toStdString();

        outStream.write(content.data(), static_cast<unsigned>(content.size()));
        outStream.flush();
    }
    catch (std::exception &e)
    {
        qDebug() << "exception: " << e.what();
        return false;
    }
    return true;
}






