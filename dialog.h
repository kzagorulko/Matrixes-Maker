#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

// перечисляемый тип выгляди здесь удобнее численной или буквенной переменной.
enum filType {
    NULLIFIED,
    RANDOM,
    DEFINED
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    int getMatrixSize();
    QString getMatrixName();
    float getDefinedValue();
    ~Dialog();
    filType getFilType();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
