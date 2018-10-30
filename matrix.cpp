#include "matrix.h"


void Matrix::setSize(int size)
{
    /* формула вычисления количества элементов в массиве: размер матрицы в кувардрате - размер *
     * матрицы, таким образом получается матрица без главной диагонали, поэтому смело можно    *
     * поделить на два, а затем, прибвать эту диагональ. Упрощаем. Получаем данную формулу:    *
     * количество элементов в динамическом массиве = (size*size + size)/2                      */
    this->oldValues = new float[(sizeOfMatrix*sizeOfMatrix+sizeOfMatrix)/2];
    for(int i = 0; i < (sizeOfMatrix*sizeOfMatrix+sizeOfMatrix)/2;i++)
        this->oldValues[i] = this->values[i];
    this->sizeOfMatrix = size;
    this->values = new float[(size*size + size)/2];
}

int Matrix::getSize()
{
    return this->sizeOfMatrix;
}

void Matrix::setValue(int i, int j, float value)
{
    /* выражение в квадратных скобках: часть ((i)*(i)+(i)) пересчитывает координату по i(строки) в*
     * рамки одномерного массива, а затем добавляется координата по х (столбец.                   */
    this->values[(i*i+i)/2+j] = value;
}

void Matrix::setValue(int i, float value)
{
    this->values[i] = value;
}

float Matrix::getValue(int i, int j)
{
    // аналогично с Matrix::setValue
    return values[(i*i+i)/2+j];
}

float Matrix::getValue(int i)
{
    /* Перегруженная функция добавлена, чтобы легче было получать все значения *
     * обычным перебором.                                                      */
    return values[i];
}

void Matrix::setName(QString name)
{
    this->name = name;
}

bool Matrix::thisWidget(QWidget * otherWidget)
{
    return this->widget == otherWidget;
}

void Matrix::setWidget(QWidget * widget)
{
    this->widget = widget;
}

QString Matrix::getName()
{
    return this->name;
}

QString Matrix::getStringMatrix()
{
    /* Текстовой вывод всей наеобходимой информации о матрице, изначально предназначен для *
     * выгрузки информации в текстовой файл                                                */
    QString result = "Matrix " + getName() + ": ";
    for(int i = 0; i < (getSize()*getSize()+getSize())/2; i++) {
        result += QString::number(getValue(i)) + " ";
    }
    return result;
}

Matrix::Matrix()
{
    this->sizeOfMatrix = 0;
}

