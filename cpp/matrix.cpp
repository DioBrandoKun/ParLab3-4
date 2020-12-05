#include "matrix.h"

Matrix::Matrix(int row, int col)
{
    this->row = row;
    this->col = col;
    this->matrix = new Item * [row];
    for (int i = 0; i < row; ++i)
        matrix[i] = new Item[col];
};

Matrix::~Matrix()
{
    for (int i = 0; i < row; ++i)
        delete [] matrix[i];
    delete [] matrix;
};

int Matrix::getRow()
{
    return row;
};

int Matrix::getCol()
{
    return col;
};

double Matrix::getItem(int row, int col)
{
    return matrix[row][col];
}

int Matrix::setItem(int row, int col, double item)
{
    if ((this->row > row) && (this->col > col))
    {
        matrix[row][col] = item;
        return 1;
    }
    return 0;
};

void Matrix::reset()
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = 0;
};

Matrix Matrix::transparent()
{
    Matrix res(row,col);
    if (row == col)
    {
    Item buffer;

    for (int i = 0; i < row; i++)
        for (int j = i; j < col; j++)
        {
            res.setItem(j,i,matrix[i][j]);
            res.setItem(i,j,matrix[j][i]);
        }
    }
    return res;
};

Matrix Matrix::obrez(int row, int col)
{
    Matrix buffer(this->row-1, this->col-1);

    buffer.reset();

    int buf_row = 0;

    for (int i = 0; i < this->row; i++)
    {
        int buf_col = 0;
        if (i != row)
        {
            for (int j = 0; j < this->col; j++)
            {
                if (j != col)
                {
                    buffer.setItem(buf_row, buf_col, matrix[i][j]);
                    buf_col++;
                }
            }
            buf_row++;
        }
    }

    return buffer;
};

Item Matrix::determinant()
{
    if (this->row == this->col)
    {
        if (this->row != 1)
        {
            Item determ = 0;

            for (int i = 0;i < this->col; i++)
            {
                Matrix buffering(this->row, this->col);

                for (int ii = 0; ii < this->row;ii++)
                    for (int jj = 0; jj < this->col;jj++)
                        buffering.setItem(ii, jj, matrix[ii][jj]);

                buffering = buffering.obrez(0,i);

                if (i%2 != 1)
                    determ += matrix[0][i] * buffering.determinant();
                else
                    determ -= matrix[0][i] * buffering.determinant();
            }
            return determ;
        }
        else
            return matrix[0][0];
    }
    else
    {
        return -2;
    }
};

Item Matrix::minor(int row, int col)
{
    Matrix buffer(this->row, this->col);

    for (int ii = 0; ii < this->row;ii++)
        for (int jj = 0; jj < this->col;jj++)
            buffer.setItem(ii, jj, matrix[ii][jj]);

    buffer = buffer.obrez(row, col);

    return buffer.determinant();
};

Matrix Matrix::minors_matrix()
{
    Matrix buffer(this->row, this->col);
    Matrix out_buffer(this->row, this->col);

    for (int ii = 0; ii < this->row;ii++)
        for (int jj = 0; jj < this->col;jj++)
            buffer.setItem(ii, jj, matrix[ii][jj]);

    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            out_buffer.setItem(i, j, buffer.minor(i, j));

    return out_buffer;
};

Item Matrix::algebroich_dop(int row, int col)
{
    if ((row + col) % 2)
        return -matrix[row][col];
    else
        return matrix[row][col];
};

Matrix Matrix::algebroich_dop_matrix()
{
    Matrix buffer(this->row, this->col);

    for (int ii = 0; ii < this->row;ii++)
        for (int jj = 0; jj < this->col;jj++)
            buffer.setItem(ii, jj, matrix[ii][jj]);

    for (int i = 0; i < this->row; i++)
        for (int j = 0; j < this->col; j++)
            buffer.setItem(i, j, buffer.algebroich_dop(i, j));

    return buffer;
};

Matrix Matrix::obrashenie_matrix()
{
    Matrix buffer(this->row, this->col);
    Matrix minor_matrix(this->row, this->col);

    for (int ii = 0; ii < this->row;ii++)
        for (int jj = 0; jj < this->col;jj++)
        {
            buffer.setItem(ii, jj, matrix[ii][jj]);
            minor_matrix.setItem(ii, jj, matrix[ii][jj]);
        }

    minor_matrix = minor_matrix.minors_matrix();
    minor_matrix = minor_matrix.algebroich_dop_matrix();
    minor_matrix.transparent();

    return ((minor_matrix) * (1/buffer.determinant()));
};

Matrix Matrix::operator*(const Matrix & a)
{
    Matrix res(row, a.col);

    res.reset();

    if (col == a.row)
    {
        for (int index_row = 0; index_row < res.row; index_row++)
            for (int index_col = 0; index_col < res.col; index_col++)
                for (int index_mulriplying = 0; index_mulriplying < a.row; index_mulriplying++)
                    res.matrix[index_row][index_col] += matrix[index_row][index_mulriplying] * a.matrix[index_mulriplying][index_col];
    }
    else
        cout << "\nIncorrect arguments in matrices multiplying.\n";

    return res;
};

Matrix Matrix::operator*(const Item & a)
{
    Matrix* res=new Matrix(row, col);

    res->reset();

    for (int index_row = 0; index_row < res->row; index_row++)
        for (int index_col = 0; index_col < res->col; index_col++)
            res->setItem(index_row, index_col, matrix[index_row][index_col] * a);


    return *res;
};

Matrix Matrix::operator+(const Matrix & a)
{
    Matrix* res=new Matrix(row, a.col);

    res->reset();

    if ((col == a.col)&&(row == a.row))
    {
        for (int index_row = 0; index_row < res->row; index_row++)
            for (int index_col = 0; index_col < res->col; index_col++)
                res->matrix[index_row][index_col] = matrix[index_row][index_col] + a.matrix[index_row][index_col];
    }
    else
        cout << "\nIncorrect arguments in matrices concotinating.\n";

    return *res;
};

Matrix & Matrix::operator=(const Matrix & a)
{
    if (this != &a)
    {
        //for (int index_deliting = 0; index_deliting < row; index_deliting++)
        //    delete [] matrix[index_deliting];
        //delete [] matrix;

        row = a.row;
        col = a.col;
        //matrix = new Item * [row];
        //for (int index_creating = 0; index_creating < row; index_creating++)
        //    matrix[index_creating] = new Item[col];
        for (int index_filling = 0; index_filling < row; index_filling++)
            for (int j = 0; j < col; j++)
            {
                //qDebug()<<index_filling<<j;
                //qDebug()<<this->matrix[index_filling][j];// = a.matrix[index_filling][j];
                qDebug()<<a.matrix[index_filling][j];
            }
    }
    return *this;
};

ostream & operator<<(ostream & os, const Matrix & m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
            os << m.matrix[i][j] << "\t";
        os << "\n";
    }

    return os;
};
Matrix Matrix::operator-(const Matrix & a)
{
    Matrix res(row,col);

    reset();

    if ((col == a.col)&&(row == a.row))
    {
        for (int index_row = 0; index_row < res.row; index_row++)
            for (int index_col = 0; index_col < res.col; index_col++)
            {

                matrix[index_row][index_col] = matrix[index_row][index_col] - a.matrix[index_row][index_col];
                qDebug()<<matrix[index_row][index_col];
            }
    }
    else
        cout << "\nIncorrect arguments in matrices concotinating.\n";

    return res;
}
