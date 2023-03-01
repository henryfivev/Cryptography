#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
class Matrix
{
private:
    vector<vector<int>> mat;
    unsigned rows;
    unsigned cols;

public:
    Matrix(unsigned _rows, unsigned _cols, const int &_initial)
    {
        mat.resize(_rows);
        for (unsigned i = 0; i < mat.size(); i++)
        {
            mat[i].resize(_cols);
        }
        rows = _rows;
        cols = _cols;
    }
    Matrix(const Matrix &rhs)
    {
        mat = rhs.mat;
        rows = rhs.get_rows();
        cols = rhs.get_cols();
    }
    Matrix &operator=(const Matrix &rhs)
    {
        if (&rhs == this)
            return *this;
        unsigned new_rows = rhs.get_rows();
        unsigned new_cols = rhs.get_cols();
        mat.resize(new_rows);
        for (unsigned i = 0; i < mat.size(); i++)
        {
            mat[i].resize(new_cols);
        }
        for (unsigned i = 0; i < new_rows; i++)
        {

            for (unsigned j = 0; j < new_cols; j++)
            {
                mat[i][j] = rhs(i, j);
            }
        }
        rows = new_rows;
        cols = new_cols;
        return *this;
    }
    Matrix operator+(const Matrix &rhs)
    {
        Matrix result(rows, cols, 0.0);
        for (unsigned i = 0; i < rows; i++)
        {
            for (unsigned j = 0; j < cols; j++)
            {
                result(i, j) = (this->mat[i][j] + rhs(i, j)) %
                               26;
            }
        }
        return result;
    }
    Matrix operator-(const Matrix &rhs)
    {
        Matrix result(rows, cols, 0.0);
        for (unsigned i = 0; i < rows; i++)
        {
            for (unsigned j = 0; j < cols; j++)
            {
                result(i, j) = (this->mat[i][j] - rhs(i, j)) %
                               26;
            }
        }
        return result;
    }
    Matrix operator*(const Matrix &rhs)
    {
        unsigned rows = rhs.get_rows();
        unsigned cols = rhs.get_cols();
        Matrix result(rows, cols, 0.0);
        for (unsigned i = 0; i < rows; i++)
        {
            for (unsigned j = 0; j < cols; j++)
            {
                for (unsigned k = 0; k < rows; k++)
                {
                    result(i, j) += (this->mat[i][k] * rhs(k,
                                                           j)) %
                                    26;
                }
            }
        }
        for (unsigned i = 0; i < rows; i++)
        {
            for (unsigned j = 0; j < cols; j++)
            {
                result(i, j) = result(i, j) % 26;
            }
        }
        return result;
    }
    int &operator()(const unsigned &row, const unsigned &col)
    {
        return this->mat[row][col];
    }
    const int &operator()(const unsigned &row, const unsigned
                                                   &col) const
    {
        return this->mat[row][col];
    }
    unsigned get_rows() const
    {
        return this->rows;
    }
    unsigned get_cols() const
    {
        return this->cols;
    }
};
Matrix getInverse(const Matrix &m)
{
    Matrix result(m.get_rows(), m.get_cols(), 0.0);
    int det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
              m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
              m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2,
                                                         0));
    det = det % 26;
    int invdet = 0;
    for (int i = 1; i <= 26; i++)
    {
        if (i * det % 26 == 1)
        {
            invdet = i;
        }
    }
    result(0, 0) = ((m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) *
                    invdet) %
                   26;
    result(0, 1) = ((m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) *
                    invdet) %
                   26;
    result(0, 2) = ((m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) *
                    invdet) %
                   26;
    result(1, 0) = ((m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) *
                    invdet) %
                   26;
    result(1, 1) = ((m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) *
                    invdet) %
                   26;
    result(1, 2) = ((m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) *
                    invdet) %
                   26;
    result(2, 0) = ((m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) *
                    invdet) %
                   26;
    result(2, 1) = ((m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) *
                    invdet) %
                   26;
    result(2, 2) = ((m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) *
                    invdet) %
                   26;
    return result;
}
void modulo(Matrix &m)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (m(i, j) < 0)
            {
                m(i, j) = 26 + m(i, j);
            }
        }
    }
}
void show(Matrix m, unsigned rows = 3, unsigned cols = 3)
{
    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            cout << m(i, j) << " ";
        }
        cout << endl;
    }
}
int main()
{
    int x1[] = {0, 3, 8, 18, 15, 11, 0, 24, 4};
    int x2[] = {3, 4, 16, 20, 0, 19, 8, 14, 13};
    int y1[] = {3, 18, 17, 12, 18, 8, 14, 15, 11};
    int y2[] = {23, 11, 9, 1, 25, 20, 11, 11, 12};
    Matrix m1(3, 3, 0.0), m2(3, 3, 0.0), m3(3, 3, 0.0), m4(3, 3, 0.0);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m1(i, j) = x1[3 * i + j];
            m2(i, j) = x2[3 * i + j];
            m3(i, j) = y1[3 * i + j];
            m4(i, j) = y2[3 * i + j];
        }
    }
    Matrix X = m2 - m1;
    Matrix Y = m4 - m3;
    cout << "\nX is \n";
    show(X);
    cout << "\nY is \n";
    show(Y);
    Matrix invX = getInverse(X);
    modulo(invX);
    cout << "\ninverse of X is \n";
    show(invX);
    Matrix L = invX * Y;
    modulo(L);
    cout << "\nthe key is \n";
    show(L);
    Matrix b = m3 - m1 * L;
    modulo(b);
    cout << "\nb is \n";
    show(b, 1, 3);
}