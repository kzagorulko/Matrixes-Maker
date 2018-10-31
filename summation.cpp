#include "summation.h"
#include "ui_summation.h"

Summation::Summation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Summation)
{
    ui->setupUi(this);
}

Summation::~Summation()
{
    delete ui;
}
