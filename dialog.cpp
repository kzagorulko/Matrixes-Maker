#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // валидация вводимых значений
    ui->matrixSize->setValidator( new QIntValidator);
    ui->userValue->setValidator(new QDoubleValidator);
}

int Dialog::getMatrixSize()
{
    return ui->matrixSize->text().toInt();
}

QString Dialog::getMatrixName()
{
    return ui->matrixName->text();
}

float Dialog::getDefinedValue()
{
    // Здесь и далее запятая заменяется на точку, чтобы функция toFloat() работала кореектно
    return ui->userValue->text().replace(",",".").toFloat();
}

Dialog::~Dialog()
{
    delete ui;
}

filType Dialog::getFilType()
{
    // Из енума понятно что есть что
    if(ui->nullKeep->isChecked())
        return NULLIFIED;
    else if(ui->randomMake->isChecked())
        return RANDOM;
    else
        return DEFINED;
}
