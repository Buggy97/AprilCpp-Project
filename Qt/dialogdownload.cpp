#include "dialogdownload.h"
#include "ui_dialogdownload.h"
#include <QTextCodec>
#include <QByteArray>

DialogDownload::DialogDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDownload),
    file1Ready(false),
    file2Ready(false),
    file1("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt"),
    file2("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt"),
    fd1(nullptr),
    fd2(nullptr)
{
    fd1 = new FileDownloader(QUrl(file1));
    fd2 = new FileDownloader(QUrl(file2));
    connect(fd1, SIGNAL(downloaded()),
            this, SLOT(downloadedFile1()));
    connect(fd2, SIGNAL(downloaded()),
            this, SLOT(downloadedFile2()));
    connect(fd1, SIGNAL(progressUpdate(qint64, qint64)),
            this, SLOT(progressUpdate1(qint64, qint64)));
    connect(fd2, SIGNAL(progressUpdate(qint64, qint64)),
            this, SLOT(progressUpdate2(qint64, qint64)));
    connect(fd1, SIGNAL(gotError(QString,QString)),
            this, SLOT(gotError(QString,QString)));
    connect(fd1, SIGNAL(gotSslErrors(QString,const QList<QSslError> &)),
            this, SLOT(gotSslErrors(QString,const QList<QSslError>&)));
    connect(fd2, SIGNAL(gotError(QString,QString)),
            this, SLOT(gotError(QString, QString)));
    connect(fd2, SIGNAL(gotSslErrors(QString, const QList<QSslError> &)),
            this, SLOT(gotSslErrors(QString, const QList<QSslError>&)));
    ui->setupUi(this);
}

DialogDownload::~DialogDownload()
{
    delete fd1;
    delete fd2;
    delete ui;
}

void DialogDownload::progressUpdate1(qint64 ist, qint64 max)
{
    ui->firstFileProgress->setMaximum(max);
    ui->firstFileProgress->setValue(ist);
    //qDebug() << "1MAX:"<< max << "VAL:" << ist;

}
void DialogDownload::progressUpdate2(qint64 ist, qint64 max)
{
    ui->secondFileProgress->setMaximum(max);
    ui->secondFileProgress->setValue(ist);
}
void DialogDownload::downloadedFile1()
{
    this->ui->textBrowser->setTextColor(QColor("GREEN"));
    this->ui->textBrowser->append("File  " + file1 + " downloaded!");
    this->ui->textBrowser->setTextColor(QColor("BLACK"));
    file1Ready = true;
    if(file2Ready)
    {
        this->ui->textBrowser->setTextColor(QColor("GREEN"));
        this->ui->textBrowser->append("\nAll file are downloaded!");
        this->ui->textBrowser->setTextColor(QColor("BLACK"));
        emit finished(this->fd1->downloadedData(), this->fd2->downloadedData());
    }
}
void DialogDownload::downloadedFile2()
{
    this->ui->textBrowser->setTextColor(QColor("GREEN"));
    this->ui->textBrowser->append("File  " + file2 + " downloaded!");
    this->ui->textBrowser->setTextColor(QColor("BLACK"));
    file2Ready = true;
    if(file1Ready)
    {
        this->ui->textBrowser->setTextColor(QColor("GREEN"));
        this->ui->textBrowser->append("\nAll file are downloaded!");
        this->ui->textBrowser->setTextColor(QColor("BLACK"));
        emit finished(this->fd1->downloadedData(), this->fd2->downloadedData());
    }
}

void DialogDownload::gotError(QString file, QString e)
{
    this->ui->textBrowser->setTextColor(QColor("RED"));
    this->ui->textBrowser->append("File  " + file + ":");
    this->ui->textBrowser->append("Network error: " + e + "");
    this->ui->textBrowser->setTextColor(QColor("BLACK"));
}
void DialogDownload::gotSslErrors(QString file, const QList<QSslError>& e)
{
    this->ui->textBrowser->setTextColor(QColor("RED"));
    this->ui->textBrowser->append("File  " + file + ":");
    QList<QSslError>::const_iterator it = e.begin();
    while(it!=e.end())
    {
        this->ui->textBrowser->append(it->errorString()+"\n");
        it++;
    }
    this->ui->textBrowser->setTextColor(QColor("BLACK"));
}

void DialogDownload::showEvent( QShowEvent* event )
{
    QDialog::showEvent( event );
    on_retryBtn_clicked();
}

void DialogDownload::on_abortBtn_clicked()
{
    if(this->file1Ready && this->file2Ready)
        this->done(42);
    else
        this->done(40);
    this->close();
}

void DialogDownload::on_retryBtn_clicked()
{
    this->file1Ready = false;
    this->file2Ready = false;
    this->ui->firstFileProgress->reset();
    this->ui->secondFileProgress->reset();
    this->ui->textBrowser->append("\n\nFetching...");
    this->ui->textBrowser->append("Downloading file " + file1 + " ...");
    this->ui->textBrowser->append("Downloading file " + file2 + " ...");
    fd1->doDownload();
    fd2->doDownload();
}
