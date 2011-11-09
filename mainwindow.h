#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QtNetwork>
#include <QDebug>
#include <QList>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
     void fetch();
     void itemActivated(QTreeWidgetItem * item);
     void readData(const QHttpResponseHeader &);

private:
    Ui::MainWindowClass *ui;

    void parseXml();

    QString currentTag;
    QString linkString;
    QString titleString;
    QString dateString;

    QTreeWidgetItem *feed;

    int connectionId;
    QHttp http;
    QXmlStreamReader xml;

};

#endif // MAINWINDOW_H
