#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mimes->addItems(mimes.mimeTypes());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mimes_currentTextChanged(const QString &currentText)
{
    ui->apps->clear();

    QList<MimeTypeHandler*> apps = mimes.appsFor(currentText);
    foreach (MimeTypeHandler *app, apps) {
        ui->apps->addItem(QString("%1 - %2 [%3]").arg(app->name, app->exec, app->icon));
    }
}
