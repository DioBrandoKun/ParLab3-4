#ifndef TARGET_H
#define TARGET_H

#include <QtMath>
#include <QVector>
#include <QList>
#include <QPair>
#include <QRandomGenerator>
#include "qcustomplot.h"
#include <math.h>
#include <matrix.h>
#include <armadillo>

class TTarget
{    
public:
    TTarget() {};
    TTarget(unsigned Time, int Type, double X, double Y, double Speed, double Mass, double P0, double MassFuel,double RlsX,double RlsY,int Num,QCPGraph* Graph,QCPGraph* Bort,QCPGraph* PNoise,QCPGraph* Clear)//конструктор
        :CurrTime(Time), TypeTarget(Type), StateVec{ X,Y,Speed }, Mass(Mass), P0(P0), MassFuel(MassFuel),Num(Num),Plots({Graph,Bort,PNoise,Clear})
    {
        QRandomGenerator Gener(QTime::currentTime().msec());
        double X_pos = fabs(X - RlsX);
        double Y_pos = fabs(Y - RlsY);
        StateVec[3] = acos(X_pos / (sqrt(X_pos * X_pos + Y_pos * Y_pos)));
        if (X < RlsX && Y> RlsY) StateVec[3] = M_PI - StateVec[3];//4
        else if (X < RlsX && Y < RlsY) StateVec[3] = M_PI + StateVec[3];
        else if (X > RlsX && Y < RlsY) StateVec[3] = 2 * M_PI - StateVec[3];
        Points=new QList<QPair<double,double>>();
        P0Star.zeros();
        X0Star.zeros();
        double Massiv[]{X,Y,Speed,MassUse};
        for(int i=0;i<3;i++)
        {
            P0Star(i,i)=Gener.bounded(10.0)+10;
        }
         P0Star(3,3)=Gener.bounded(1.0);
        //P0Star.reset();
        //X0Star.reset();
    };
    ~TTarget() {};
    virtual void Move(unsigned  Real) = 0;
    double FunctionV(double V,unsigned time)
    {
        double TimeUmpact=GetP(time);
        double VUmpact=GetX(V);
        return (TimeUmpact-VUmpact)/(Mass+MassFuel);
    };
    double GetX();
    double GetX(double);
    double GetPNoise(unsigned);
    double GetP(unsigned);
    void SetPNoise();
    void SetP(unsigned Time);
    unsigned CurrTime;//Время
    short int TypeTarget;//Тип
    double StateVec[4];//вектор состояния 0-X 1-Y 2-V 3-Угол курса
    const double SMidel = 1; //Площадь Миделя
    const double AtmPo = 1.255;//Плотность атмосферы
    const double Cx = 1; //Лобовое сопротивление
    const double MassUse = 25;//Массовый расход в секунду
    const double MassCrit = 150;//Критическая масса топлива
    double Mass;//Масса самолета без топлива
    double P0;
    double P;
    arma::mat P0Star=arma::Mat<double>(4,4);//Бортовое устройство
    arma::mat X0Star=arma::Mat<double>(4,1);//тоже самое
    //Matrix P0Star=Matrix(4,4);
    //Matrix X0Star=Matrix(4,1);
    QList<QCPGraph*> Plots={};//отрисовка движений самолета чистая
    //QCPGraph* PlotNoise;//шум по P
    //QCPGraph* PNoise;//общий шум
    //QCPGraph* Clear;//после калмыка
    double MassFuel;//Масс топлива
    int Num;
    QList<QPair<double,double>>* Points;
};
template<class AirObj>
QList<double> Kutta(AirObj* Air)
{
    double h=1.0;
    double x=Air->StateVec[2],y=Air->CurrTime;
    double k1=Air->FunctionV(x,y);
    double k2=Air->FunctionV(x+h/2,y+h/2*k1);
    double k3=Air->FunctionV(x+h/2,y+h/2*k2);
    double k4=Air->FunctionV(x+h,y+h*k3);
    double NewV=x+h/6*(k1+2*k2+2*k3+k4);
    if(NewV<0) NewV=0;
    double NewX=Air->StateVec[0]-NewV*cos(Air->StateVec[3]);
    double NewY=Air->StateVec[1]-NewV*sin(Air->StateVec[3]);
    QList<double> Out={NewX,NewY,NewV};
    return Out;
}
double Normal(double x);
Matrix Calman(TTarget*);
void Calmen(TTarget* Air);
#endif // TARGET_H
