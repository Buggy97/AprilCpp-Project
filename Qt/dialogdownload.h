#ifndef DIALOGDOWNLOAD_H
#define DIALOGDOWNLOAD_H

#include <QDialog>
#include "filedownloader.h"

namespace Ui {
class DialogDownload;
}

class DialogDownload : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDownload(QWidget *parent = nullptr);
    void showEvent( QShowEvent* event );
    ~DialogDownload();

private:
    Ui::DialogDownload *ui;
    QByteArray file1Data;
    QByteArray file2Data;
    bool file1Ready;
    bool file2Ready;
    const QString file1;
    const QString file2;
    FileDownloader* fd1;
    FileDownloader* fd2;
    void fetch_files();

signals:
    void userAborted();
    void finished(QByteArray file1, QByteArray file2);

private slots:
    void gotError(QString, QString);
    void gotSslErrors(QString, const QList<QSslError> &);
    void progressUpdate1(qint64 ist, qint64 max);
    void progressUpdate2(qint64 ist, qint64 max);
    void downloadedFile1();
    void downloadedFile2();
    void on_abortBtn_clicked();
    void on_retryBtn_clicked();
};

#endif // DIALOGDOWNLOAD_H
