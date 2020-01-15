#include "commandwindow.h"
#include "ui_commandwindow.h"
#include <QTimer>
#include <QMessageBox>

CommandWindow::CommandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandWindow)
{
    ui->setupUi(this);

    QTimer* timerList = new QTimer(this);
    timerList->start();
    timerList->setInterval(3000);
    connect(timerList, SIGNAL(timeout()), this, SLOT(populateList()));

    ui->listPages->setSelectionMode(QAbstractItemView::SingleSelection); // nu poate selecta mai multe fisiere din lista
    populateList();
}
void CommandWindow::populateList()
{
    ui->listPages->clear();
    char msg[256];
    write(MyClient::cd, CMD_LIST_ALL, 256);

    printf("%s\n", CMD_LIST_ALL);
    fflush(stdout);
    read(MyClient::cd, msg, 256);
    for (; 0 != strcmp(msg, CMD_END); read(MyClient::cd, msg, 256))
        ui->listPages->addItem(new QListWidgetItem(QString(msg)));
}

CommandWindow::~CommandWindow()
{
    delete ui;
}

void CommandWindow::on_btnGetPage_clicked()
{
    char fileSelected[256];
    char line[256];
    memset(fileSelected, 0, 256);
    memset(line, 0, 256);
    strcpy(fileSelected, ui->listPages->currentItem()->text().toStdString().c_str());

    write(MyClient::cd, CMD_GET_PAGE, 256);
    write(MyClient::cd, fileSelected, 256);
    FILE* fileNew = fopen(fileSelected, "w");
    QString newInput("");
    read(MyClient::cd, line, 256);
    for (; 0 != strcmp(line, CMD_END); read(MyClient::cd, line, 256))
    {
        fprintf(fileNew, "%s", line);
        newInput.append(line);
    }
    ui->inputViewPage->setText(newInput);
    fflush(stdout);
    fclose(fileNew);
    if (newInput.isEmpty())
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setText("The received file is empty!");
        msgBox->show();
    }

}

void CommandWindow::on_btnUploadPage_clicked()
{
    int fileSize = ui->inputWritePage->toPlainText().size() + 1;
    char msg[256], pageName[256];
    char text[fileSize];
    memset(text, 0, fileSize);
    strcpy(pageName, ui->lePageName->text().toStdString().c_str());
    strcpy(text, ui->inputWritePage->toPlainText().toStdString().c_str());
    if (0 == strlen(text) || 0 == strlen(pageName))
    {
        QMessageBox*  msgBox = new QMessageBox();
        msgBox->setText("Fill all the fields!");
        msgBox->show();
        return;
    }
    write(MyClient::cd, CMD_ADD_PAGE, 256);
    write(MyClient::cd, pageName, 256);
    write(MyClient::cd, &fileSize, 4);
    write(MyClient::cd, text, fileSize);
    printf("Page uploaded!\n");
    fflush(stdout);
}

void CommandWindow::on_btnRemovePage_clicked()
{
    write(MyClient::cd, CMD_DELETE_PAGE, 256);
    write(MyClient::cd, ui->listPages->currentItem()->text().toStdString().c_str(), 256);
    printf("Page %s is removed!\n", ui->listPages->currentItem()->text().toStdString().c_str());
    fflush(stdout);
}
