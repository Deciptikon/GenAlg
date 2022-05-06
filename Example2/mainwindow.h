#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QList>
#include <QRandomGenerator64>
#include <QDebug>
#include "customtype.h"


#include "../GenAlg/GenAlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QThread *threadGA;
    GenAlg< CustomType > *ga;
};
#endif // MAINWINDOW_H
