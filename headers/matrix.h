#ifndef MATRIX_H
#define MATRIX_H


#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <QDebug>
using namespace std;

typedef double Item;


class Matrix
{
private:
    int row;
    int col;
    Item **matrix;

public:
    Matrix(int, int);
    Matrix(){};
    ~Matrix();

    int getRow();
    int getCol();
    double getItem(int, int);

    int setItem(int, int, double);


    void reset();
    Matrix transparent();
    Item determinant();
    Item minor(int, int);
    Item algebroich_dop(int, int);
    Matrix minors_matrix();
    Matrix obrashenie_matrix();
    Matrix algebroich_dop_matrix();
    Matrix obrez(int, int);

    Matrix operator*(const Matrix &);
    Matrix operator-(const Matrix &);
    Matrix operator*(const Item &);
    Matrix operator+(const Matrix &);
    Matrix & operator=(const Matrix &);
    friend ostream & operator<<(ostream & os, const Matrix & m);

};
#endif // MATRIX_H
