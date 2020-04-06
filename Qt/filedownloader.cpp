#include "filedownloader.h"
#include <QTextCodec>

FileDownloader::FileDownloader(QUrl fileUrl, QObject *parent) :
 QObject(parent),
 file(fileUrl),
 downloading(false),
 reply(nullptr)
{
    connect(
            &accMan, SIGNAL (finished(QNetworkReply*)),
            this, SLOT (fileDownloaded(QNetworkReply*)) );
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::doDownload()
{
    if(downloading)
        abort();
    QNetworkRequest request(file);
    downloading = true;
    reply = accMan.get(request);
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(downloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),
            this, SLOT(sslErrors(const QList<QSslError>&)));
}

void FileDownloader::abort()
{
    if(downloading)
    {
        disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(error(QNetworkReply::NetworkError)));
        disconnect(reply, SIGNAL(sslErrors(const QList<QSslError>&)),
                this, SLOT(sslErrors(const QList<QSslError>&)));
        reply->close();
        delete reply;
        reply = nullptr;
    }
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    dData = pReply->readAll();
    pReply->deleteLater();
    if (pReply->error()==QNetworkReply::NetworkError::NoError)
    {
        downloading = false;
        emit downloaded();
    }
}

void FileDownloader::downloadProgress(qint64 ist, qint64 max)
{
    emit progressUpdate(ist, max);
}

void FileDownloader::error(QNetworkReply::NetworkError e)
{
    downloading = false;
    emit gotError(this->file.url(), reply->errorString());
    reply->deleteLater();
}
void FileDownloader::sslErrors(const QList<QSslError>& e)
{
    downloading = false;
    emit gotSslErrors(this->file.url(), e);
    reply->deleteLater();
}

QByteArray FileDownloader::downloadedData() const
{
    return dData;
}
