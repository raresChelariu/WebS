#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commandwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyClient* currClient = new MyClient();
    if (0 == currClient->connectToServer())
    {
        this->setEnabled(false); // nu mai poate folosi fereastra
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setText("The attempt to connect to the server has failed!");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec(); // este fortat sa dea ok pt a merge mai departe
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnSignIn_clicked()
{
    char username[256], password[256], message[256];
    memset(username, 0, 256);
    memset(password, 0, 256);
    strcpy(username, ui->inputUser->text().toStdString().c_str());
    strcpy(password, ui->inputPass->text().toStdString().c_str());

    if (0 == strlen(username) || 0 == strlen(password))
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setText("Fill all the fields!\n");
        msgBox->show();
        return;
    }
    sprintf(message, "%s;%s", username, password);
    printf("%s", message); fflush(stdout);

    write(MyClient::cd, message, 256);
    read(MyClient::cd, message, 256);
    printf("Message from server : @%s@", message); fflush(stdout);

    if (!strcmp(message, CMD_LOGIN_GOOD))
    {
        CommandWindow* window = new CommandWindow();
        window->show();
        this->hide();
        return;
    }
    if (!strcmp(message, CMD_LOGIN_BAD))
    {
        this->setEnabled(false);
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setText("Credentials are not valid");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
        this->setEnabled(true);
        ui->inputUser->setText(""); ui->inputPass->setText("");
        return;
    }
    printf("Bad message %s", message);
    fflush(stdout);
}
