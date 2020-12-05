#include "target.h"

double TTarget::GetX()
{
    return Cx * AtmPo * StateVec[2] * StateVec[2] * SMidel / 2;
}
double TTarget::GetX(double V)
{
    return Cx * AtmPo * V * V * SMidel / 2;
}
void TTarget::SetP(unsigned Time)
{
    QRandomGenerator Noise(QTime::currentTime().msec());
    double MassFuelGet = MassFuel - (Time - CurrTime) * MassUse;
    if (MassFuelGet > MassCrit) P = P0;
    else P = P0 * MassFuelGet / MassCrit;
}
void TTarget::SetPNoise()
{
    QRandomGenerator Noise(QTime::currentTime().msec());
    P=P+Normal(Noise.bounded(10.0));
}
double TTarget::GetPNoise(unsigned Time)
{
    QRandomGenerator Noise(QTime::currentTime().msec());
    double MassFuelGet = MassFuel - (Time - CurrTime) * MassUse;
    if (MassFuelGet<0) MassFuelGet=0;
    if (MassFuelGet > MassCrit) return P0+Normal(Noise.bounded(10.0));
    else return P0 * MassFuelGet / MassCrit+Normal(Noise.bounded(10.0));
}
double TTarget::GetP(unsigned Time)
{
    QRandomGenerator Noise(QTime::currentTime().msec());
    double MassFuelGet = MassFuel - (Time - CurrTime) * MassUse;
    if (MassFuelGet<0) MassFuelGet=0;
    if (MassFuelGet > MassCrit) return P0;
    else return P0 * MassFuelGet / MassCrit;
}
double Normal(double x)
{
    return 0.5+0.5*erf(x/(1*sqrt(2.0)));
}

void Calmen(TTarget* Air)
{
    double Sign[2]={-1.0,1.0};
    QRandomGenerator Gener(QTime::currentTime().msec());
    double VectorNoise[4];
    for (int i=0;i<4;i++)
        VectorNoise[i]=(Normal(Gener.bounded(1.0))+Gener.bounded(5.0))*Sign[Gener.bounded(0,2)];
    //qDebug()<<Normal(Gener.bounded(100.0));
    double MassUse=Air->MassUse;
    double Vcos=Air->StateVec[2]*cos(Air->StateVec[3]);
    double Vsin=Air->StateVec[2]*sin(Air->StateVec[3]);
    double V=Air->StateVec[2];
    double Mass=Air->Mass+Air->MassFuel;
    double MassX[]{Vcos,Vsin,V,MassUse};
    arma::mat XOpor(4,1);//X(t) опорная точка
    arma::mat Dx(4,1);//X(t)
    for(int i=0;i<4;i++)
    {
        XOpor(i,0)=MassX[i];
        Dx(i,0)=MassX[i]+VectorNoise[i];
    }
    double X=Air->StateVec[0];
    double Y=Air->StateVec[1];

    double MassY[]{X,Y,V};
    arma::mat  YOpor(3,1);//y
    arma::mat  YMatr(3,1);//y опорная
    arma::mat  DeltaX(4,1);
    arma::mat  DeltaY(3,1);
    for(int i=0;i<3;i++)
    {
        YOpor(i,0)=MassY[i];
        YMatr(i,0)=MassY[i]+VectorNoise[i];
    }
    DeltaX=Dx-XOpor;
    DeltaY=YMatr-YOpor;
    Air->Plots[2]->addData(YMatr(0,0),YMatr(1,0));
    arma::mat F(4,4);
    arma::mat  Sin(4,4);
    Sin.eye();//матрица H
    arma::mat  Fdx(4,4);//матрица F' просто посчитай
    Fdx.zeros();
    Fdx(0,2)=cos(Air->StateVec[3]);
    Fdx(1,2)=sin(Air->StateVec[3]);
    Fdx(3,2)=-(2*Air->GetX())/(V*Mass);
    Fdx(3,3)=(-Air->P0
                +
                Air->GetX())/pow(Mass,2);
    double DeltaTime=1.0;
    F=Sin+Fdx*DeltaTime;
    arma::mat  H(3,4);
    H.zeros();
    for(int i=0;i<3;i++)
        H(i,i)=1;
    arma::mat  XApper(4,1);
    XApper=F*DeltaX;//(Air->X0Star);
    arma::mat  PApper(4,4);
    arma::mat  SuperNoise(4,4);
    arma::mat  ThreeNoise(3,3);
    SuperNoise.zeros();
    ThreeNoise.zeros();
    double MatrixElem[]{5,5,7,0.1};
    for(int i=0;i<3;i++)
            ThreeNoise(i,i)=MatrixElem[i];//3 3 5 1
    for(int i=0;i<4;i++)
            SuperNoise(i,i)=MatrixElem[i];
    PApper=F*(Air->P0Star)*trans(F)+SuperNoise;//F - 4x4 P0-4x4
    //qDebug()<<Air->P0Star(0,0)<<"\t"<<Air->P0Star(1,1)<<"\t"<<Air->P0Star(2,2)<<"\t"<<Air->P0Star(3,3);
    // qDebug()<<'\n';
    Air->P0Star=inv((inv(PApper)+trans(H)*inv(ThreeNoise)*H));
    DeltaX=Air->P0Star*(inv(PApper)*XApper+trans(H)*inv(ThreeNoise)*DeltaY);
    Air->Plots[3]->addData(YMatr(0,0)-DeltaX(0,0),YMatr(1,0)-DeltaX(1,0));
    qDebug()<<DeltaX(0,0)<<"\t"<<DeltaX(1,0)<<"\t"<<DeltaX(2,0)<<"\t"<<DeltaX(3,0);
    //Air->Plots[3]->addData(Air->X0Star(0,0),Air->X0Star(1,0));
    //return Dx;
}
/*
Matrix Calmen(TTarget* Air,unsigned NewTime)
{
    QRandomGenerator Gener(QTime::currentTime().msec());
    double MassUse=Air->MassUse;
    double Vcos=Air->StateVec[2]*cos(Air->StateVec[3]);
    double Vsin=Air->StateVec[2]*sin(Air->StateVec[3]);
    double V=Air->StateVec[2];
    double Mass=Air->Mass+Air->MassFuel;
    double MassX[]{Vcos,Vsin,V,MassUse};
    Matrix XOpor(4,1);//X(t) опорная точка
    Matrix Dx(4,1);//X(t)
    for(int i=0;i<4;i++)
    {
        XOpor.setItem(0,i,MassX[i]);
        Dx.setItem(0,i,MassX[i]+Normal(Gener.bounded(10.0)));
    }
    double X=Air->StateVec[0];
    double Y=Air->StateVec[1];


    double MassY[]{X,Y,V};
    Matrix YOpor(3,1);//y
    Matrix YMatr(3,1);//y опорная
    Matrix DeltaY(3,1);
    for(int i=0;i<3;i++)
    {
        YOpor.setItem(0,i,MassY[i]+Normal(Gener.bounded(10.0)));
        YMatr.setItem(0,i,MassY[i]+Normal(Gener.bounded(10.0))+Normal(Gener.bounded(10.0)));
    }
    DeltaY=(YMatr-YOpor);

    Matrix F(4,4);
    Matrix Sin(4,4);
    Sin.reset();
    for(int i=0;i<4;i++)
        Sin.setItem(i,i,1);//матрица H
    Matrix Fdx(4,4);//матрица F' просто посчитай
    Fdx.reset();
    Fdx.setItem(0,2,cos(Air->StateVec[3]));
    Fdx.setItem(1,2,sin(Air->StateVec[3]));
    Fdx.setItem(3,2,-(2*Air->GetX())/(V*Mass));
    Fdx.setItem(3,3,(-Air->P0
                +
                Air->GetX())/pow(Mass,2));
    //unsigned DeltaTime=NewTime-Air->CurrTime;
    double DeltaTime=1.0;
    F=Sin+Fdx*DeltaTime;
    Matrix H(3,4);
    H.reset();
    for(int i=0;i<3;i++)
        H.setItem(i,i,1);

    Matrix XApper(4,1);
    XApper=F*(Air->X0Star);

    Matrix PApper(4,4);
    Matrix SuperNoise(4,4);
    Matrix ThreeNoise(3,3);
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            ThreeNoise.setItem(i,j,Normal(Gener.bounded(10.0)));
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            SuperNoise.setItem(i,j,Normal(Gener.bounded(10.0)));
    PApper=F*(Air->P0Star)*F.obrashenie_matrix()+SuperNoise;
    Air->P0Star=(PApper.obrashenie_matrix()+H.transparent()*ThreeNoise.obrashenie_matrix()*H).obrashenie_matrix();
    Air->X0Star=Air->P0Star*(PApper.obrashenie_matrix()*XApper+H.transparent()*ThreeNoise.obrashenie_matrix()*DeltaY);
    return Dx;
}*/
