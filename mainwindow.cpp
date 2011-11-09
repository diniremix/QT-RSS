#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{

    connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(readData(const QHttpResponseHeader &)));

    ui->setupUi(this);

    ui->treeWidget->setColumnHidden(1, true);
    ui->treeWidget->setColumnHidden(2, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetch()
{
        xml.clear();

        QUrl url(ui->lineEdit->text());

        http.setHost(url.host());
        connectionId = http.get(url.path());
}

 void MainWindow::readData(const QHttpResponseHeader &resp)
 {
     if (resp.statusCode() != 200)
         http.abort();
     else {
         xml.addData(http.readAll());
         parseXml();
     }

 }

 void MainWindow::parseXml()
 {

     while (!xml.atEnd()) {
         xml.readNext();
         if (xml.isStartElement()) {

             if (xml.name() == "item"){

                 if (titleString!=""){
                    feed = new QTreeWidgetItem;
                    feed->setText(0, titleString);
                    feed->setText(2, linkString);
                    ui->treeWidget->addTopLevelItem(feed);

                 }

                 linkString.clear();
                 titleString.clear();
                 dateString.clear();
             }

             currentTag = xml.name().toString();
         } else if (xml.isEndElement()) {
              if (xml.name() == "item") {

                 QTreeWidgetItem *item = new QTreeWidgetItem(feed);
                 item->setText(0, titleString);
                 item->setText(1, dateString);
                 item->setText(2, linkString);
                 ui->treeWidget->addTopLevelItem(item);

                 titleString.clear();
                 linkString.clear();
                 dateString.clear();
             }

         } else if (xml.isCharacters() && !xml.isWhitespace()) {
             if (currentTag == "title")
                 titleString += xml.text().toString();
             else if (currentTag == "link")
                 linkString += xml.text().toString();
             else if (currentTag == "pubDate")
                 dateString += xml.text().toString();
         }
     }
     if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
         qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
         http.abort();
     }
 }

 void MainWindow::itemActivated(QTreeWidgetItem * item)
 {
     ui->webView->load(QUrl(item->text(2)));
     ui->webView->show();

 }

