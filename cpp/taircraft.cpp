#include "taircraft.h"
TAircraft& TAircraft::operator=(const TAircraft& Copy)//копирующее присваивание
{
    this->CurrTime = Copy.CurrTime;
    for (int i = 0; i < 4; i++)this->StateVec[i] = Copy.StateVec[i];
    this->MassFuel = Copy.MassFuel;
    return *this;
}
void TAircraft::Move(unsigned Real)
{
    /*QFile File((QString::number(Num)+".txt"));
    File.open(QIODevice::WriteOnly| QIODevice::Text|QIODevice::Append);
    QTextStream Stream(&File);*/
    if(CurrTime!=Real)
    {
        double Sign[2]={-1.0,1.0};
        QRandomGenerator Noise(QTime::currentTime().msec());
    //Stream<<QString::number(StateVec[2])<<"\n";
        SetP(Real);//P

        Points->append(QPair<double,double>{StateVec[0], StateVec[1]});
        CurrTime = Real;
        QList<double> NewClear=Kutta(this);

        /*StateVec[2] = New[2];//+Normal(Noise.bounded(22.5)*Sign[Noise.bounded(0,2)]);
        StateVec[1] = New[1];//+Normal(Noise.bounded(22.5)*Sign[Noise.bounded(0,2)]);
        StateVec[0] = New[0];//+Normal(Noise.bounded(22.5)*Sign[Noise.bounded(0,2)]);*/
        //Plots[0]->addData(NewClear[0],NewClear[1]);
        SetPNoise();
        QList<double> Graph=Kutta(this);
        StateVec[2] = NewClear[2];
        StateVec[1] = NewClear[1];
        StateVec[0] = NewClear[0];
        //Plots[1]->addData(Graph[0],Graph[1]);
        //Plots[2]->addData(Graph[0]+Normal(Noise.bounded(100.0)*Sign[Noise.bounded(0,2)]),Graph[1]+Normal(Noise.bounded(100.0)*Sign[Noise.bounded(0,2)]));
        Calmen(this);
        SetP(Real);

        //+Normal(Noise.bounded(22.5)*Sign[Noise.bounded(0,2)]);
        //+Normal(Noise.bounded(22.5)*Sign[Noise.bounded(0,2)]);

        //File.write( (+"\n").to);

    }
    //Plot->re
}

