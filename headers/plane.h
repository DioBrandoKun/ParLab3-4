#ifndef PLANE_H
#define PLANE_H

#include <QMainWindow>
#include "mainwindow.h"
#include "qcustomplot.h"
#include <QAbstractSlider>
#include <QScrollBar>
#include "rls.h"
#include "taircraft.h"
#include <thread>
#include <QTimer>
#include <QMutex>
#include <QColor>
#include <QDate>
namespace Ui {
class Plane;
}
class Timer
{
public:
   static unsigned Time;
   static bool Start;
   static void UpTime()
   {
        Time++;
   }
   static void Stop(){Start=false;};
   static void Run(){Start=true;}
};
class Plane : public QMainWindow
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);
    ~Plane();
    Ui::Plane *ui;
private slots:
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
private slots:
    void Resize();
private:
    void CreateRls(double=0,double=0,bool=false);
    void CreateRocket(double x=0,double y=0,bool Full=false);
    void GraphCreator();
    QList<QColor> Colors{QColor( 146 , 5 , 87 ,255),
                QColor( 36 , 184 , 83 ,255),
                QColor( 116 , 76 , 49 ,255),
                QColor( 39 , 185 , 47 ,255),
                QColor( 205 , 97 , 182 ,255),
                QColor( 92 , 181 , 123 ,255),
                QColor( 169 , 35 , 46 ,255),
                QColor( 206 , 5 , 125 ,255),
                QColor( 254 , 193 , 236 ,255),
                QColor( 171 , 116 , 200 ,255),
                QColor( 61 , 73 , 17 ,255),
                QColor( 130 , 110 , 188 ,255),
                QColor( 149 , 254 , 68 ,255),
                QColor( 10 , 126 , 214 ,255),
                QColor( 134 , 92 , 54 ,255),
                QColor( 153 , 99 , 1 ,255),
                QColor( 61 , 162 , 181 ,255),
                QColor( 149 , 140 , 214 ,255),
                QColor( 65 , 185 , 4 ,255),
                QColor( 183 , 214 , 230 ,255),
                QColor( 98 , 209 , 50 ,255),
                QColor( 81 , 44 , 145 ,255),
                QColor( 210 , 188 , 176 ,255),
                QColor( 254 , 59 , 63 ,255),
                QColor( 252 , 73 , 238 ,255),
                QColor( 68 , 167 , 93 ,255),
                QColor( 199 , 51 , 190 ,255),
                QColor( 207 , 79 , 161 ,255),
                QColor( 89 , 44 , 64 ,255),
                QColor( 253 , 159 , 92 ,255),
                QColor( 115 , 228 , 174 ,255),
                QColor( 144 , 147 , 69 ,255),
                QColor( 101 , 50 , 5 ,255),
                QColor( 189 , 233 , 194 ,255),
                QColor( 245 , 157 , 73 ,255),
                QColor( 119 , 59 , 73 ,255),
                QColor( 88 , 82 , 219 ,255),
                QColor( 61 , 136 , 125 ,255),
                QColor( 215 , 105 , 186 ,255),
                QColor( 50 , 153 , 109 ,255),
                QColor( 38 , 5 , 48 ,255),
                QColor( 113 , 136 , 20 ,255),
                QColor( 100 , 31 , 91 ,255),
                QColor( 247 , 69 , 84 ,255),
                QColor( 229 , 244 , 33 ,255),
                QColor( 79 , 129 , 88 ,255),
                QColor( 177 , 114 , 251 ,255),
                QColor( 42 , 113 , 137 ,255),
                QColor( 182 , 7 , 27 ,255),
                QColor( 149 , 91 , 99 ,255),
                QColor( 66 , 94 , 35 ,255),
                QColor( 212 , 249 , 205 ,255),
                QColor( 233 , 165 , 56 ,255),
                QColor( 53 , 255 , 34 ,255),
                QColor( 176 , 167 , 175 ,255),
                QColor( 152 , 32 , 1 ,255),
                QColor( 234 , 179 , 255 ,255),
                QColor( 86 , 37 , 106 ,255),
                QColor( 162 , 82 , 168 ,255),
                QColor( 91 , 214 , 167 ,255),
                QColor( 155 , 54 , 54 ,255),
                QColor( 73 , 102 , 203 ,255),
                QColor( 2 , 240 , 237 ,255),
                QColor( 56 , 135 , 216 ,255),
                QColor( 192 , 85 , 73 ,255),
                QColor( 49 , 81 , 153 ,255),
                QColor( 164 , 116 , 193 ,255),
                QColor( 244 , 228 , 76 ,255),
                QColor( 247 , 133 , 236 ,255),
                QColor( 57 , 32 , 214 ,255),
                QColor( 216 , 139 , 110 ,255),
                QColor( 89 , 57 , 89 ,255),
                QColor( 151 , 5 , 39 ,255),
                QColor( 34 , 209 , 155 ,255),
                QColor( 29 , 240 , 68 ,255),
                QColor( 180 , 221 , 250 ,255),
                QColor( 231 , 135 , 37 ,255),
                QColor( 172 , 190 , 145 ,255),
                QColor( 118 , 237 , 10 ,255),
                QColor( 62 , 88 , 221 ,255),
                QColor( 161 , 157 , 190 ,255),
                QColor( 82 , 131 , 191 ,255),
                QColor( 12 , 252 , 31 ,255),
                QColor( 202 , 101 , 104 ,255),
                QColor( 116 , 24 , 75 ,255),
                QColor( 255 , 200 , 102 ,255),
                QColor( 244 , 116 , 201 ,255),
                QColor( 207 , 194 , 11 ,255),
                QColor( 71 , 66 , 130 ,255),
                QColor( 142 , 132 , 211 ,255),
                QColor( 168 , 237 , 115 ,255),
                QColor( 115 , 10 , 5 ,255),
                QColor( 110 , 213 , 144 ,255),
                QColor( 134 , 153 , 116 ,255),
                QColor( 156 , 0 , 230 ,255),
                QColor( 62 , 147 , 138 ,255),
                QColor( 117 , 227 , 213 ,255),
                QColor( 179 , 65 , 16 ,255),
                QColor( 227 , 201 , 216 ,255),
                QColor( 177 , 235 , 61 ,255),
                QColor( 109 , 56 , 7 ,255),
                QColor( 165 , 34 , 125 ,255),
                QColor( 191 , 56 , 243 ,255),
                QColor( 173 , 51 , 68 ,255),
                QColor( 211 , 143 , 131 ,255),
                QColor( 194 , 117 , 44 ,255),
                QColor( 224 , 96 , 231 ,255),
                QColor( 242 , 154 , 174 ,255),
                QColor( 21 , 190 , 216 ,255),
                QColor( 157 , 206 , 132 ,255),
                QColor( 175 , 210 , 233 ,255),
                QColor( 213 , 103 , 183 ,255),
                QColor( 254 , 43 , 1 ,255),
                QColor( 73 , 214 , 68 ,255),
                QColor( 155 , 33 , 178 ,255),
                QColor( 249 , 211 , 75 ,255),
                QColor( 35 , 242 , 232 ,255),
                QColor( 67 , 206 , 104 ,255),
                QColor( 14 , 22 , 210 ,255),
                QColor( 15 , 245 , 84 ,255),
                QColor( 64 , 159 , 153 ,255),
                QColor( 92 , 127 , 173 ,255),
                QColor( 238 , 153 , 7 ,255),
                QColor( 53 , 4 , 128 ,255),
                QColor( 94 , 166 , 250 ,255),
                QColor( 255 , 148 , 82 ,255),
                QColor( 198 , 178 , 244 ,255),
                QColor( 223 , 240 , 20 ,255),
                QColor( 23 , 170 , 178 ,255),
                QColor( 98 , 87 , 116 ,255),
                QColor( 254 , 141 , 41 ,255),
                QColor( 74 , 82 , 189 ,255),
                QColor( 156 , 190 , 96 ,255),
                QColor( 163 , 32 , 6 ,255),
                QColor( 185 , 95 , 140 ,255),
                QColor( 163 , 235 , 10 ,255),
                QColor( 222 , 3 , 50 ,255),
                QColor( 132 , 43 , 88 ,255),
                QColor( 94 , 163 , 149 ,255),
                QColor( 213 , 198 , 64 ,255),
                QColor( 120 , 205 , 84 ,255),
                QColor( 31 , 231 , 24 ,255),
                QColor( 3 , 51 , 223 ,255),
                QColor( 162 , 163 , 180 ,255),
                QColor( 16 , 241 , 79 ,255),
                QColor( 123 , 229 , 229 ,255),
                QColor( 7 , 153 , 50 ,255),
                QColor( 102 , 251 , 232 ,255),
                QColor( 124 , 39 , 111 ,255),
                QColor( 219 , 244 , 99 ,255),QColor( 140 , 219 , 122 ,255),
                QColor( 175 , 245 , 45 ,255),
                QColor( 93 , 60 , 41 ,255),
                QColor( 61 , 212 , 104 ,255),
                QColor( 223 , 232 , 165 ,255),
                QColor( 116 , 154 , 202 ,255),
                QColor( 60 , 96 , 139 ,255),
                QColor( 83 , 51 , 237 ,255),
                QColor( 24 , 50 , 53 ,255),
                QColor( 42 , 157 , 131 ,255),
                QColor( 17 , 213 , 107 ,255),
                QColor( 244 , 22 , 24 ,255),
                QColor( 65 , 240 , 88 ,255),
                QColor( 50 , 167 , 207 ,255),
                QColor( 55 , 58 , 53 ,255),
                QColor( 188 , 186 , 141 ,255),
                QColor( 9 , 55 , 135 ,255),
                QColor( 116 , 152 , 29 ,255),
                QColor( 36 , 46 , 10 ,255),
                QColor( 200 , 182 , 167 ,255),
                QColor( 48 , 191 , 244 ,255),
                QColor( 54 , 195 , 89 ,255),
                QColor( 157 , 158 , 239 ,255),
                QColor( 52 , 25 , 181 ,255),
                QColor( 44 , 14 , 53 ,255),
                QColor( 126 , 46 , 155 ,255),
                QColor( 14 , 234 , 16 ,255),
                QColor( 164 , 153 , 192 ,255),
                QColor( 68 , 223 , 81 ,255),
                QColor( 73 , 50 , 178 ,255),
                QColor( 220 , 116 , 69 ,255),
                QColor( 147 , 127 , 108 ,255),
                QColor( 124 , 77 , 111 ,255),
                QColor( 17 , 50 , 199 ,255),
                QColor( 47 , 56 , 162 ,255),
                QColor( 234 , 60 , 225 ,255),
                QColor( 113 , 11 , 25 ,255),
                QColor( 104 , 107 , 51 ,255),
                QColor( 118 , 2 , 103 ,255),
                QColor( 184 , 88 , 191 ,255),
                QColor( 239 , 49 , 131 ,255),
                QColor( 130 , 12 , 74 ,255)};
    bool RlsCreated=false;
    bool Opened=false;
    bool MoveTime=false;
    bool NeedToRestart=false;
    bool FirstRun=true;
    std::thread* ThrTime;
    std::thread* ThrRls;
public:   static QTime tm1;
    QList<std::thread*>* ThrObj;
    int ObjNum=0;
    RlsLogic* RlsLog;
    QList<double> RlsPos;
    QList<TAircraft*>* AirObj;

};

void Draw(double x,double y,double RangeRls,QCustomPlot* Plot);
void Dwig(TAircraft* Obj);
void Pel(RlsLogic*,QList<TAircraft*>*);
void Start();
#endif // PLANE_H
