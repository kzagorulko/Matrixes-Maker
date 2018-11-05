#include "assignment.h"
#include "ui_assignment.h"

Assignment::Assignment(Matrix *matrixes, int matrixSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Assignment)
{
    ui->setupUi(this);
    setWindowTitle("Присваивание");
    this->matrixes = matrixes;
    QStringList list;
    for(int i = 0; i < matrixSize; i++) {
        list+=matrixes[i].getName();
    }

    ui->comboBox->addItems(list);
    ui->comboBox_2->addItems(list);


}

Assignment::~Assignment()
{
    delete ui;
}

int Assignment::getIndexOfMatrix(bool image)
{
    return image ? ui->comboBox_2->currentIndex() : ui->comboBox->currentIndex();
}

bool Assignment::isSizeOk()
{
    return result;
}

void Assignment::on_buttonBox_accepted()
{
    result = true;
    /* при невозможности сложения матриц, необходимо уведомить пользователя и запретить сложение *
     * матриц, изменя значение result на false.                                                  */
    if(matrixes[ui->comboBox->currentIndex()].getSize() !=
            matrixes[ui->comboBox_2->currentIndex()].getSize()) {
        qDebug("sizes do not match: %i != %i", matrixes[ui->comboBox->currentIndex()].getSize(),
                matrixes[ui->comboBox_2->currentIndex()].getSize());
        QMessageBox msg;
        msg.setText("Ошибка! Вы пытаетесь присвоить матрицу другого размера!"
                    "Выберите другие матрицы");
        msg.exec();
        result = false;
    }
}
