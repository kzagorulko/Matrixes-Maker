#ifndef MATRIX_H
#define MATRIX_H
#include <QString>
#include <QWidget>

class Matrix
{
private:
    int sizeOfMatrix;
    float *values, *oldValues;
    QString name;
    QWidget * widget;

public:
    void setSize(int size);
    int getSize();
    void setValue(int i, int j, float value);
    void setValue(int i, float value);
    float getValue(int i, int j);
    float getValue(int i);
    void setName(QString name);
    bool thisWidget(QWidget * otherWidget);
    void setWidget(QWidget * widget);
    QString getName();
    QString getStringMatrix();
    Matrix(); 
};

#endif // MATRIX_H
