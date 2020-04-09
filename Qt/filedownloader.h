#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
 Q_OBJECT
 public:
  explicit FileDownloader(QUrl imageUrl, QObject *parent = 0);
  virtual ~FileDownloader();
  void doDownload();
  void abort();
  QByteArray downloadedData() const;

 signals:
  void downloaded();
  void progressUpdate(qint64, qint64);
  void gotError(QString, QString);
  void gotSslErrors(QString, const QList<QSslError> &);

 private slots:
  void fileDownloaded(QNetworkReply* pReply);
  void downloadProgress(qint64, qint64);
  void error();
  void sslErrors(const QList<QSslError> &);

 private:
  QUrl file;
  bool downloading;
  QNetworkAccessManager accMan;
  QByteArray dData;
  QNetworkReply* reply;
};

#endif // FILEDOWNLOADER_H
