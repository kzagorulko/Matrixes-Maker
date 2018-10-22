#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // валидация вводимых значений
    ui->matrixSize->setValidator( new QIntValidator);
    // Из-за особенностей ДаблВалидатора поставил сетлокал на Англию, там используется ".", а не ","
    QLocale locale(QLocale::English);
    QDoubleValidator * validator = new QDoubleValidator;
    validator->setLocale(locale);
    ui->userValue->setValidator(validator);
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
    return ui->userValue->text().toFloat();
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