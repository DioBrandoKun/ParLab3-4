#include "plane.h"
#include "ui_plane.h"
#include "qcustomplot.h"
unsigned Timer::Time=0;
bool Timer::Start=true;
QTime Plane::tm1;
QMutex m1;
Plane::Plane(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Plane)
{

    /*QRandomGenerator Noise(QTime::currentTime().msec()); //Генератор цветов RGB
    QList<int> ListR;
    QList<int> ListG;
    QList<int> ListB;
    int j=0;
    while(j<150)
    {
        int YesR=1;
        int YesG=1;
        int YesB=1;
        int R=Noise.bounded(0,256);
        int G=Noise.bounded(0,256);
        int B=Noise.bounded(0,256);
        foreach(int Item,ListR)
            if(abs(Item-R)<45)
            {
                YesR=-1;
                break;
            }
        foreach(int Item,ListG)
            if(abs(Item-G)<45)
            {
                YesG=-1;
                break;
            }
        foreach(int Item,ListB)
            if(abs(Item-B)<45)
            {
                YesB=-1;
                break;
            }
        if((YesR+YesG+YesB)>0)
        {

            j++;
            qDebug()<<"QColor("<<R<<","<<G<<","<<B<<",255),";
        }
    }*/

    ThrObj=new QList<std::thread*>();
    ui->setupUi(this);
    horzScrollBarChanged(0);
    vertScrollBarChanged(0);
    ui->verticalScrollBar->hide();
    ui->horizontalScrollBar->hide();
    AirObj=new QList<TAircraft*>();
    ui->Plot->setInteraction(QCP::iRangeZoom, true);
    ui->Plot->setInteraction(QCP::iRangeDrag, true);
    ui->CreateAir->hide();
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, &QTimer::timeout, this,[this]() {ui->Plot->replot();});
    //connect(ui->Plot,&QCustomPlot::mouseWheel,this,&Plane::Resize);
    //connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    //connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    //connect(ui->Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->Stop,&QPushButton::clicked,this,[this](){
        Timer::Stop();
    });
    connect(ui->CreateRls,&QPushButton::clicked,[this]()//Кнопка для создания полного окна Rls
            {
                CreateRls(0,0,true);
            });
    /*connect(ui->Start,&QPushButton::clicked,this,[this]()
    {
        tm1=QTime::currentTime();
        qDebug()<<tm1.toString("hh:mm:ss.zzz");
        if(FirstRun){
           foreach(TAircraft* Obj,*AirObj)
            {
                std::thread* New=new std::thread(&Dwig,Obj);
                AirObj->removeOne(Obj);
                ThrObj->append(New);
            }
           this->ThrTime=new std::thread(&Start);
           RlsLogic* RlsForThr=this->RlsLog;
           QList<TAircraft*>* ObjForThr=this->AirObj;
           this->ThrRls=new std::thread(&Pel,RlsForThr,ObjForThr);
           FirstRun=false;
          }
        else if(NeedToRestart)
        {
            foreach(TAircraft* Obj,*AirObj)
             {
                 std::thread* New=new std::thread(&Dwig,Obj);
                 AirObj->removeOne(Obj);
                 ThrObj->append(New);
             }
        }
        Timer::Run();
    });*/
    //connect(ui->Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    connect(ui->Start,&QPushButton::clicked,this,[this]()
        {
            tm1=QTime::currentTime();
            qDebug()<<tm1.toString("hh:mm:ss.zzz");
            qDebug()<<Timer::Time;
            while(Timer::Time<500)
            {
            Timer::UpTime();
            this->RlsLog->Peleng(Timer::Time,this->AirObj);
            foreach(TAircraft* Obj,*AirObj)
                 {
                     Obj->Move(Timer::Time);
                 }
            }
           qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz");
        });
    connect(ui->checkBox,&QCheckBox::clicked,[this]()//Рандом генерация
    {
        QRandomGenerator AirObjGen(QTime::currentTime().msec());
        if(!RlsCreated)
        {
            RlsCreated=true;
            ui->CreateRls->hide();
            ui->CreateAir->show();
            Rls* RlsRand=new Rls();
            RlsRand->Random();
            QStringList RlsData=RlsRand->GetInfo();
            Draw(RlsData[0].toDouble(),RlsData[1].toDouble(),RlsData[2].toDouble(),ui->Plot);
            RlsPos={RlsData[0].toDouble(),RlsData[1].toDouble()};
            RlsLog = new RlsLogic(RlsData[0].toDouble(),RlsData[1].toDouble(),RlsData[2].toDouble());
        }
        int Objs=AirObjGen.bounded(5,20);
        for(int i=0;i<1;i++)
        {
            GraphCreator();
        }
    });
    connect(ui->CreateAir,&QPushButton::clicked,[this]()//Окно для создания вне графика
    {
        CreateRocket(0,0,true);
    });
    connect(ui->Plot,&QCustomPlot::mouseDoubleClick,[this](QMouseEvent *event){//дабл клик на экран создание RLS или ракеты
        double x=ui->Plot->xAxis->pixelToCoord(event->pos().x());
        double y=ui->Plot->yAxis->pixelToCoord(event->pos().y());
        if(!RlsCreated)
        {
            CreateRls(x,y);
        }
        else if(!Opened)
        {
           CreateRocket(x,y);
        }
    });
}

Plane::~Plane()
{
    delete ui;
}
void Plane::horzScrollBarChanged(int value)
{
  if (qAbs(ui->Plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->Plot->xAxis->setRange(value/100.0, ui->Plot->xAxis->range().size(), Qt::AlignCenter);
    ui->Plot->replot();
  }
}
void Plane::vertScrollBarChanged(int value)
{
  if (qAbs(ui->Plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->Plot->yAxis->setRange(-value/100.0, ui->Plot->yAxis->range().size(), Qt::AlignCenter);
    ui->Plot->replot();
  }
}
void Plane::xAxisChanged(QCPRange range)
{
  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
}

void Plane::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
}
void Draw(double x,double y,double RangeRls,QCustomPlot* Plot)
{
    QCPItemEllipse* Circle=new QCPItemEllipse(Plot);
    Circle->setPen(QPen(Qt::SolidLine));
    Circle->setBrush(QBrush(Qt::red));
    Circle->topLeft->setCoords(x-4,y+4);
    Circle->bottomRight->setCoords(x+4,y-4);
    QCPItemEllipse* Range=new QCPItemEllipse(Plot);
    Range->setPen(QPen(Qt::SolidLine));
    Range->topLeft->setCoords(x-RangeRls,y+RangeRls);
    Range->bottomRight->setCoords(x+RangeRls,y-RangeRls);
    Plot->replot();
}
void Plane::Resize()
{
  QVector<double> RangeY=ui->Plot->yAxis->tickVector();
  QVector<double> RangeX=ui->Plot->yAxis->tickVector();
  ui->verticalScrollBar->setRange(-RangeY.last()*1000,RangeY.last()*1000);
  ui->horizontalScrollBar->setRange(-RangeX.last()*1000,RangeX.last()*1000);
}
void Plane::CreateRls(double x,double y,bool Full)
{
    qDebug()<<Timer::Time;
    Rls* RlsWindow=new Rls(this);
    if(!Full) RlsWindow->Hider();
    RlsWindow->setWindowTitle("Rls");
    RlsWindow->show();
    connect(RlsWindow,&Rls::SendData,[this,x,y,RlsWindow,Full]()
    {
       ui->CreateRls->hide();
       ui->CreateAir->show();
       RlsCreated=true;
       QStringList RlsData=RlsWindow->GetInfo();
       double x_used;
       double y_used;
       if(Full)
       {
           x_used=RlsData[0].toDouble();
           y_used=RlsData[1].toDouble();
       }
       else
       {
           x_used=x;
           y_used=y;
       }
       Draw(x_used,y_used,RlsData[2].toDouble(),ui->Plot);
       RlsPos={x,y};
       RlsWindow->close();
       RlsLog = new RlsLogic(x_used,y_used,RlsData[2].toDouble());
       ui->Plot->replot();
    });
}
void Plane::GraphCreator()
{
    QCPGraph* Graph=ui->Plot->addGraph();
    QCPGraph* PNoise=ui->Plot->addGraph();
    QCPGraph* Noise=ui->Plot->addGraph();
    QCPGraph* Clear=ui->Plot->addGraph();
    int ColorNum=ObjNum%(Colors.size()-1);
    Graph->setPen(QPen(Colors[ColorNum],2,Qt::DashLine));
    PNoise->setPen(QPen(Colors[ColorNum].darker(25),2,Qt::SolidLine));
    Noise->setPen(QPen(Colors[ColorNum].darker(50),2,Qt::DashDotLine));
    Clear->setPen(QPen(Colors[ColorNum].darker(75),2,Qt::DashDotDotLine));
    MainWindow* Obj=new MainWindow;
    Obj->Random();
    QStringList AirData=Obj->GetInfo();
    QString Type=Obj->GetAir();
    Graph->addData(AirData[0].toDouble(),AirData[1].toDouble());
    Graph->addData(AirData[0].toDouble()+0.001,AirData[1].toDouble()+0.001);
    TAircraft* AirObj=new TAircraft(Timer::Time,0,AirData[0].toDouble(),AirData[1].toDouble(),
                AirData[2].toDouble(),AirData[3].toDouble(),AirData[4].toDouble(),AirData[5].toDouble(),
                this->RlsPos[0],this->RlsPos[1],ObjNum,Graph,PNoise,Noise,Clear);
    this->AirObj->append(AirObj);
    ObjNum++;
    ui->Plot->replot();
    NeedToRestart=true;
}
void Plane::CreateRocket(double x,double y,bool Full)
{
    Opened=!Opened;
    MainWindow* Data=new MainWindow(this);
    if(!Full) Data->Hider();
    Data->setWindowTitle("Air");
    Data->show();
    connect(Data,&MainWindow::Closed,[this]
    {
        Opened=false;
    });
    connect(Data,&MainWindow::SendData,[Data,this,x,y,Full](){
        Opened=!Opened;
        QStringList AirData=Data->GetInfo();
        QString Type=Data->GetAir();
        double x_used;
        double y_used;
        if(Full)
        {
            x_used=AirData[0].toDouble();
            y_used=AirData[1].toDouble();
        }
        else
        {
            x_used=x;
            y_used=y;
        }
        QCPGraph* Graph=ui->Plot->addGraph();
        QCPGraph* PNoise=ui->Plot->addGraph();
        QCPGraph* Noise=ui->Plot->addGraph();
        QCPGraph* Clear=ui->Plot->addGraph();
        int ColorNum=ObjNum%(Colors.size()-1);
        Graph->setPen(QPen(Colors[ColorNum],2,Qt::DashLine));
        PNoise->setPen(QPen(Colors[ColorNum].darker(25),2,Qt::SolidLine));
        Noise->setPen(QPen(Colors[ColorNum].darker(50),2,Qt::DashDotLine));
        Clear->setPen(QPen(Colors[ColorNum].darker(75),2,Qt::DashDotDotLine));
        Graph->addData(x_used,y_used);
        Graph->addData(x_used+0.001,y_used+0.001);
        TAircraft* AirObj=new TAircraft(Timer::Time,0,x_used,y_used,
                    AirData[2].toDouble(),AirData[3].toDouble(),AirData[4].toDouble(),AirData[5].toDouble(),
                    this->RlsPos[0],this->RlsPos[1],ObjNum,Graph,PNoise,Noise,Clear);
        this->AirObj->append(AirObj);
        //connect(AirObj,&TAircraft::Moved,this,[this](){ui->Plot->replot();});
        ObjNum++;
        foreach(QString Str,AirData) qDebug()<<Str<<" ";
        Data->close();
        ui->Plot->replot();
        NeedToRestart=true;
    });
}
void Start()
{
while(true)
    {
        if(Timer::Start)
        {
         m1.lock();
         Timer::UpTime();
         if(Timer::Time>500)
         {
             qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz");
         }
         m1.unlock();
         //qDebug()<<Timer::Time<<" ";
        }
    }
}
void Dwig(TAircraft* Obj)
{
    forever
    {
        m1.lock();
        Obj->Move(Timer::Time);
        m1.unlock();
    }
}
void Pel(RlsLogic* Rls,QList<TAircraft*>* Obj)
{
    forever
    {
        m1.lock();
        Rls->Peleng(Timer::Time,Obj);
        m1.unlock();
    }
}
