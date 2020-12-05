#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette Pallet;
    Pallet.setColor(QPalette::Base,Qt::gray);
    Pallet.setColor(QPalette::Text,Qt::white);
    Q_FOREACH(QLineEdit* Line,this->findChildren<QLineEdit*>())
    {
      if(Line->isReadOnly())
        Line->setPalette(Pallet);
    }
    connect(ui->GetInfo,&QPushButton::clicked,[this]()
    {
        if(!this->Check())
        {
            qDebug()<<"Wrong";
            return;
        }
        this->InfoForm=this->FormOutput();
        if(ui->checkBox->isChecked())Air="Bomb";
        else Air="Air";
        emit SendData();
        this->hide();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
QStringList MainWindow::FormOutput()
{
  QStringList Output;
  Q_FOREACH(QLineEdit* Line,this->findChildren<QLineEdit*>())
  {
    if(!Line->isReadOnly())
    Output.append(Line->text());
  }
  return Output;
}
QStringList MainWindow::GetInfo()
{
    return InfoForm;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    emit Closed();
    event->accept();
}
void MainWindow::Hider()
{

        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->PozitionX->hide();
        ui->PozitionY->hide();
        ui->PozitionX->setText("0");
        ui->PozitionY->setText("0");
}
bool MainWindow::Check()
{
  QRegExp Reg("^[0-9]*[.,]?[0-9]+$");
  QRegExp RegXY("^[-]?[0-9]*[.,]?[0-9]+$");
  int K=2;
  Q_FOREACH(QLineEdit* Line,this->findChildren<QLineEdit*>())
  {
    if(!Line->isReadOnly())
       if(K)
       {
           if(!RegXY.exactMatch(Line->text()))
                return 0;
           K--;
       }
       else if(!Reg.exactMatch(Line->text()))
                return 0;
  }
  if(!ui->checkBox->isChecked() &&!ui->checkBox_2->isChecked() )
      return 0;
  return 1;
}
void MainWindow::Random()
{
    double Sign[2]={-1.0,1.0};
    QStringList Typer={"Air","Bomb"};
    QRandomGenerator Rand(QTime::currentTime().msec());
    ui->PozitionX->setText(QString::number(Rand.bounded(5000.0)*Sign[Rand.bounded(0,2)]));
    ui->PozitionY->setText(QString::number(Rand.bounded(5000.0)*Sign[Rand.bounded(0,2)]));
    ui->Speed->setText(QString::number(Rand.bounded(25.0)+5));
    ui->Mass->setText(QString::number(Rand.bounded(1000.0)+500));
    ui->MassFuel->setText(QString::number(Rand.bounded(1000.0)+700));
    ui->P0->setText(QString::number(Rand.bounded(50.0)+200));
    this->Air=Typer[Rand.bounded(0,2)];
    this->InfoForm=this->FormOutput();
}

