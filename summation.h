#ifndef SUMMATION_H
#define SUMMATION_H

#include <QDialog>

namespace Ui {
class Summation;
}

class Summation : public QDialog
{
    Q_OBJECT

public:
    explicit Summation(QWidget *parent = 0);
    ~Summation();

private:
    Ui::Summation *ui;
};

#endif // SUMMATION_H
