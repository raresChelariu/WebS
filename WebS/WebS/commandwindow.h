#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include <QMainWindow>
#include "myclient.h"

namespace Ui {
class CommandWindow;
}

class CommandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandWindow(QWidget *parent = 0);
    ~CommandWindow();

private slots:
    void on_btnGetPage_clicked();
    void populateList();
    void on_btnUploadPage_clicked();

    void on_btnRemovePage_clicked();

private:
    Ui::CommandWindow *ui;
};

#endif // COMMANDWINDOW_H
