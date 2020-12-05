#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QStringList>
#include <QPalette>
#include <QRegExp>
#include <QDebug>
#include <QCloseEvent>
#include <QRandomGenerator>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList GetInfo();
    void CloseIt();
    void Hider();
    QString GetAir(){return Air;}
    void Random();
signals:
    void SendData();
    void Closed();
protected:
    virtual void closeEvent(QCloseEvent *event);
private:
    bool Check();
    QString Air;
    QStringList FormOutput();
    QStringList InfoForm;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
