#ifndef RLS_H
#define RLS_H

#include <QMainWindow>
#include <QDebug>
#include <QRandomGenerator>
#include <QTime>
#include <QFile>
#include "taircraft.h"
namespace Ui {
class Rls;
}

class Rls : public QMainWindow
{
    Q_OBJECT

public:
    explicit Rls(QWidget *parent = nullptr);
    ~Rls();
    QStringList GetInfo();
    void Hider();
    void Random();
signals:
    void SendData();
private:
    QStringList FormOutput();

    bool Check();
    Ui::Rls *ui;
    QStringList InfoForm;
};

class RlsLogic
{
public:
    RlsLogic() {};
    RlsLogic(double X, double Y, double Range) :X(X), Y(Y), Range(Range) { Created = 1; };
    ~RlsLogic() { Created = 0; };
    void Peleng(unsigned NewTime, QList<TAircraft*>* ListObjects);
    int Created = 0;
private:
    unsigned Time=0;
    double X;
    double Y;
    double Range;//Дальность поиска
};

#endif // RLS_H
