#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QChart>
#include "dialogdownload.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_fetchBtn_clicked();
    void gotError(QString, QString);
    void gotSslErrors(QString, const QList<QSslError> &);
    void progressUpdate1(qint64 ist, qint64 max);
    void progressUpdate2(qint64 ist, qint64 max);
    void downloadedFile1();
    void downloadedFile2();
    void table1Clicked(const QModelIndex&);
    void table2Clicked(const QModelIndex&);
    void on_abortBtn_clicked();


private:
    Ui::MainWindow *ui;
    bool file1Ready;
    bool file2Ready;
    const QString file1;
    const QString file2;
    FileDownloader* fd1;
    FileDownloader* fd2;
    void fetch_files();
    void displayData();
    void enableTabWidgets();
    QtCharts::QChart* getChart(const QString name, const QMap<QString, int>& map) const;
    QtCharts::QChart* getSecondChart(const int quantity1, const int quantity2) const;
    void processEntry(QString entry, QTableWidget* table, QMap<QString, int>& map);
};
#endif // MAINWINDOW_H
