#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChart>
#include <QTextCodec>
#include <QDesktopServices>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      file1Ready(false),
      file2Ready(false),
      file1("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt"),
      file2("http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt"),
      fd1(nullptr),
      fd2(nullptr)
{
      ui->setupUi(this);
      fd1 = new FileDownloader(file1);
      fd2 = new FileDownloader(file2);
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
      connect(ui->tableUniversal, SIGNAL(doubleClicked(const QModelIndex&)),
              this, SLOT(table1Clicked(const QModelIndex&)));
      connect(ui->tableEMI, SIGNAL(doubleClicked(const QModelIndex&)),
              this, SLOT(table2Clicked(const QModelIndex&)));
      ui->tableUniversal->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->tableEMI->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->firstFileProgress->reset();
      ui->secondFileProgress->reset();
      ui->textBrowser->append("Sto recuperando i file...");
      ui->textBrowser->append("Sto scaricando il file " + file1 + " ...");
      ui->textBrowser->append("Sto scaricando il file " + file2 + " ...");
      fd1->doDownload();
      fd2->doDownload();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete fd1;
    delete fd2;
}

void MainWindow::on_fetchBtn_clicked()
{
    file1Ready = false;
    file2Ready = false;
    ui->firstFileProgress->reset();
    ui->secondFileProgress->reset();
    ui->textBrowser->append("\n\nSto recuperando i file...");
    ui->textBrowser->append("Sto scaricando il file " + file1 + " ...");
    ui->textBrowser->append("Sto scaricando il file " + file2 + " ...");
    fd1->doDownload();
    fd2->doDownload();
}

void MainWindow::progressUpdate1(qint64 ist, qint64 max)
{
    ui->firstFileProgress->setMaximum(max);
    ui->firstFileProgress->setValue(ist);

}
void MainWindow::progressUpdate2(qint64 ist, qint64 max)
{
    ui->secondFileProgress->setMaximum(max);
    ui->secondFileProgress->setValue(ist);
}
void MainWindow::downloadedFile1()
{
    ui->textBrowser->setTextColor(QColor("GREEN"));
    ui->textBrowser->append("File  " + file1 + " scaricato!");
    ui->textBrowser->setTextColor(QColor("BLACK"));
    file1Ready = true;
    if(file2Ready)
    {
        ui->textBrowser->setTextColor(QColor("GREEN"));
        ui->textBrowser->append("\nTutti i file sono stati scaricati!");
        ui->textBrowser->setTextColor(QColor("BLACK"));
        enableTabWidgets();
        displayData();
    }
}
void MainWindow::downloadedFile2()
{
    ui->textBrowser->setTextColor(QColor("GREEN"));
    ui->textBrowser->append("File  " + file2 + " scaricato!");
    ui->textBrowser->setTextColor(QColor("BLACK"));
    file2Ready = true;
    if(file1Ready)
    {
        ui->textBrowser->setTextColor(QColor("GREEN"));
        ui->textBrowser->append("\nTutti i file sono stati scaricati!");
        ui->textBrowser->setTextColor(QColor("BLACK"));
        enableTabWidgets();
        displayData();
    }
}

void MainWindow::gotError(QString file, QString e)
{
    ui->tabWidget->setCurrentIndex(5);
    ui->textBrowser->setTextColor(QColor("RED"));
    ui->textBrowser->append("File  " + file + ":");
    ui->textBrowser->append("Errore di rete: " + e + "");
    ui->textBrowser->setTextColor(QColor("BLACK"));
}
void MainWindow::gotSslErrors(QString file, const QList<QSslError>& e)
{
    ui->tabWidget->setCurrentIndex(5);
    ui->textBrowser->setTextColor(QColor("RED"));
    ui->textBrowser->append("File  " + file + ":");
    ui->textBrowser->append("Errore SSL: ");
    QList<QSslError>::const_iterator it = e.begin();
    while(it!=e.end())
    {
        ui->textBrowser->append(it->errorString()+"\n");
        it++;
    }
    ui->textBrowser->setTextColor(QColor("BLACK"));
}

void MainWindow::table1Clicked(const QModelIndex& i)
{
    QString url = ui->tableUniversal->item(i.row(), 1)->text();
    if(url.indexOf("https")!=-1)
        QDesktopServices::openUrl(url);
}

void MainWindow::table2Clicked(const QModelIndex& i)
{
    QString url = ui->tableEMI->item(i.row(), 1)->text();
    if(url.indexOf("https")!=-1)
        QDesktopServices::openUrl(url);
}

void MainWindow::processEntry(QString entry, QTableWidget* table, QMap<QString, int>& map)
{
    QString rawLink = entry.split(" ")[0];
    QString sublink;
    if((rawLink.length()-rawLink.lastIndexOf("wiki"))<rawLink.length())
        sublink = rawLink.right(rawLink.length()-rawLink.lastIndexOf("wiki"));
    if(sublink.size()>0)
        sublink.chop(1);
    QString wikiLink;
    if(sublink.length()<=5)
        wikiLink = "Nessun link disponibile";
    else
        wikiLink = "https://www.wikipedia.org/" + sublink;
    int namePos =  entry.length()-entry.lastIndexOf("\"")-1;
    QString artistName = entry.right(namePos).trimmed();
    artistName.replace("_", " ");
    artistName = artistName.trimmed();
    if(artistName.size()!=0)
        artistName[0] = artistName[0].toUpper();
    QString firstArtistLetter = "\0";
    if(artistName.size()!=0)
        firstArtistLetter = QString(artistName[0]);
    if(namePos!=0 && map.contains(firstArtistLetter))
        map[firstArtistLetter]++;
    else if(namePos!=0)
        map[firstArtistLetter]=1;
    table->insertRow(table->rowCount());
    table->setItem(table->rowCount()-1,
                   0,
                   new QTableWidgetItem(artistName));
    table->setItem(table->rowCount()-1,
                   1,
                   new QTableWidgetItem(wikiLink));
}

QtCharts::QChart* MainWindow::getChart(const QString name, const QMap<QString, int>& map) const
{
    int maxArtistName = 0;
    QStringList uniCategories;
    QtCharts::QBarSet* set = new QtCharts::QBarSet(name);
    QtCharts::QBarSeries* series = new QtCharts::QBarSeries();
    QtCharts::QChart* chart = new QtCharts::QChart();
    QtCharts::QBarCategoryAxis* axisX = new QtCharts::QBarCategoryAxis();
    QtCharts::QValueAxis* axisY = new QtCharts::QValueAxis();
    foreach(QString letter, map.keys())
    {
        int artistsPerLetter = map[letter];
        if(artistsPerLetter > maxArtistName)
            maxArtistName = artistsPerLetter;
        *set << artistsPerLetter;
        uniCategories << letter;
    }
    maxArtistName += maxArtistName/10;
    series->append(set);
    chart->addSeries(series);
    axisX->append(uniCategories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisY->setRange(0, maxArtistName);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

QtCharts::QChart* MainWindow::getSecondChart(const int quantity1,
                                             const int quantity2) const
{
    int maxArtistsPerLabel = 0;
    QStringList categories;
    QtCharts::QBarSet* set = new QtCharts::QBarSet("Artisti totali");
    QtCharts::QBarSeries* series = new QtCharts::QBarSeries();
    QtCharts::QChart* chart = new QtCharts::QChart();
    QtCharts::QBarCategoryAxis* axisX = new QtCharts::QBarCategoryAxis();
    QtCharts::QValueAxis* axisY = new QtCharts::QValueAxis();
    categories << "Universal" << "EMI";
    *set << quantity1 << quantity2;
    series->append(set);
    chart->addSeries(series);
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    maxArtistsPerLabel = quantity1>quantity2?quantity1:quantity2;
    maxArtistsPerLabel += maxArtistsPerLabel/10;
    axisY->setRange(0, maxArtistsPerLabel); //In questo modo sono bilanciati
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return chart;
}

void MainWindow::enableTabWidgets()
{
    ui->tableUniversal->setEnabled(true);
    ui->tableEMI->setEnabled(true);
    ui->chartUniversal->setEnabled(true);
    ui->chartEMI->setEnabled(true);
    ui->chartCompare->setEnabled(true);
}

void MainWindow::displayData()
{
    ui->tableUniversal->setRowCount(0);
    ui->tableEMI->setRowCount(0);
    QString* tmp1 = new QString(fd1->downloadedData());
    QString* tmp2 = new QString(fd2->downloadedData());
    QStringList* f1Data = new QStringList(tmp1->split("\n"));
    QStringList* f2Data = new QStringList(tmp2->split("\n"));
    QMap<QString, int> uniArtistPerLetter;
    QMap<QString, int> emiArtistPerLetter;
    foreach(QString entry, *f1Data)
        processEntry(entry, ui->tableUniversal, uniArtistPerLetter);
    foreach(QString entry, *f2Data)
        processEntry(entry, ui->tableEMI, emiArtistPerLetter);
    ui->chartUniversal->setChart(getChart("Artisti universal", uniArtistPerLetter));
    ui->chartEMI->setChart(getChart("Artisti EMI", emiArtistPerLetter));
    ui->chartCompare->setChart(getSecondChart(f1Data->length(), f2Data->length()));
    delete f2Data;
    delete f1Data;
    delete tmp2;
    delete tmp1;
}

