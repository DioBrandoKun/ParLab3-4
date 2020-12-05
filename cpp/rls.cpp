#include "rls.h"
#include "ui_rls.h"

Rls::Rls(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Rls)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,[this]()
    {
        if(!this->Check())
        {
            qDebug()<<"Wrong";
            return;
        }
        this->InfoForm=this->FormOutput();
        emit SendData();
        this->hide();
    });
}
QStringList Rls::GetInfo()
{
    return InfoForm;
}
Rls::~Rls()
{
    delete ui;
}
bool Rls::Check()
{
  QRegExp Reg("^[0-9]*[.,]?[0-9]+$");
  QRegExp RegXY("^[-]?[0-9]*[.,]?[0-9]+$");

  if(!RegXY.exactMatch(ui->lineEdit->text())) return 0;
  if(!RegXY.exactMatch(ui->lineEdit_2->text())) return 0;
  if(!Reg.exactMatch(ui->lineEdit_6->text())) return 0;
  return 1;
}
void Rls::Hider()
{
        ui->lineX->hide();
        ui->lineY->hide();
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit->setText("0");
        ui->lineEdit_2->setText("0");
}
QStringList Rls::FormOutput()
{
  QStringList Output;
  Q_FOREACH(QLineEdit* Line,this->findChildren<QLineEdit*>())
  {
    if(!Line->isReadOnly())
    Output.append(Line->text());
  }
  return Output;
}
void Rls::Random()
{
    double Sign[2]={-1.0,1.0};
    QRandomGenerator Rand(QTime::currentTime().msec());
    ui->lineEdit->setText(QString::number(Rand.bounded(1000.0)*Sign[Rand.bounded(0,2)]));
    ui->lineEdit_2->setText(QString::number(Rand.bounded(1000.0)*Sign[Rand.bounded(0,2)]));
    ui->lineEdit_6->setText(QString::number(Rand.bounded(1000.0)+100));
    this->InfoForm=this->FormOutput();
}
void RlsLogic::Peleng(unsigned NewTime,QList<TAircraft*>* ListObjects)//Каждую секунду смотрим где каждый объект из списка и попадает ли он в область
{
    if(Time==NewTime) return;
    Time=NewTime;
    QFile File("Rls.txt");
    File.open(QIODevice::WriteOnly| QIODevice::Text|QIODevice::Append);
    QTextStream Stream(&File);
    foreach (TAircraft* ListObject,*ListObjects)//рассматриваем каждый объект
    {
        unsigned Index=ListObject->Num;
        unsigned ObjTime= ListObject->CurrTime;
        double ObjX = ListObject->StateVec[0];
        double ObjY = ListObject->StateVec[1];
        double Length = sqrt(pow(ObjX - X, 2) + pow(ObjY - Y, 2));//Формула из файла
        if (Length < Range)
        {
            double Azimut;
            if((ObjX - X)==0) Azimut = atan((ObjY - Y) / (0.01));
            else Azimut = atan((ObjY - Y) / (ObjX - X));
            Stream  << "\t" << Index << "\t" << ObjTime << "\t" << Length << "\t" << Azimut << endl;
        }
    }
    File.close();
}
