#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mimetypes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mimes_currentTextChanged(const QString &currentText);

private:
    Ui::MainWindow *ui;
    MimeTypes mimes;
};

#endif // MAINWINDOW_H
