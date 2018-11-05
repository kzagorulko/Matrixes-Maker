#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <QDialog>
#include <QMessageBox>
#include "matrix.h"

namespace Ui {
class Assignment;
}

class Assignment : public QDialog
{
    Q_OBJECT

public:
    explicit Assignment(Matrix *matrixes, int matrixSize, QWidget *parent = 0);
    ~Assignment();
    int getIndexOfMatrix(bool image = false); /* Когда image == true возращается индекс матрицы, *
                                               * которая будет присваиваться, иначе индекс       *
                                               * исходной.                                       */
    bool isSizeOk();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Assignment *ui;
    Matrix * matrixes;
    bool result;
};

#endif // ASSIGNMENT_H
