#ifndef TAIRCRAFT_H
#define TAIRCRAFT_H
#include "target.h"
#include <QFile>
#include <QDebug>

class TAircraft :public TTarget//Скорее всего надо будет добавить пару переменных не до конца понял Move
{
public:
    TAircraft() {};
    TAircraft(unsigned Time, int Type, double X, double Y, double Speed, double Mass, double P0, double MassFuel,double RlsX,double RlsY,int Num,QCPGraph* Graph,QCPGraph* Bort,QCPGraph* PNoise,QCPGraph* Clear)
        :TTarget(Time, Type, X, Y, Speed, Mass, P0, MassFuel,RlsX,RlsY,Num,Graph,Bort,PNoise,Clear) {
    };
    ~TAircraft() {};
    virtual void Move(unsigned Real) override;
    void Signal();
    TAircraft& operator=(const TAircraft& Copy);

};
#endif // TAIRCRAFT_H
