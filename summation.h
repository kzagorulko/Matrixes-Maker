#ifndef SUMMATION_H
#define SUMMATION_H

#include <QDialog>
#include <QMessageBox>
#include "matrix.h"

namespace Ui {
class Summation;
}

class Summation : public QDialog
{
    Q_OBJECT

public:
    explicit Summation(Matrix * matrixes, int matrixSize, QWidget *parent = 0);
    bool canBeFolded();
    QString getName();
    int getIndex(bool nummer = false);
    ~Summation();

private slots:
    void on_change();
    void on_buttonBox_accepted();

private:
    Ui::Summation *ui;
    Matrix *matrixes;
    bool result; // служит для определения возможности сложения двух матриц
};

#endif // SUMMATION_H
