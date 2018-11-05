#include "summation.h"
#include "ui_summation.h"

Summation::Summation(Matrix *matrixes, int matrixSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Summation)
{
    this->matrixes = matrixes;
    QStringList list;
    for(int i = 0; i < matrixSize; i++) {
        list+=matrixes[i].getName();
    }

    ui->setupUi(this);
    setWindowTitle("Суммирование");
    ui->comboBox->addItems(list);
    ui->comboBox_2->addItems(list);

    connect(ui->comboBox,   SIGNAL(currentIndexChanged(QString)), this, SLOT(on_change()));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_change()));

    ui->lineEdit->setText(ui->comboBox->currentText() + "+" + ui->comboBox_2->currentText());
}

bool Summation::canBeFolded()
{
    return result;
}

QString Summation::getName()
{
    return ui->lineEdit->text();
}

int Summation::getIndex(bool nummer)
{
    return nummer ? ui->comboBox_2->currentIndex(): ui->comboBox->currentIndex();
}

Summation::~Summation()
{
    delete ui;
}

void Summation::on_change()
{
    // При изменении любого комбо бокса меняется название
    ui->lineEdit->setText(ui->comboBox->currentText() + "+" + ui->comboBox_2->currentText());
}

void Summation::on_buttonBox_accepted()
{
    //TODO сделать проверку на пустые выбраные значения

    result = true;
    /* при невозможности сложения матриц, необходимо уведомить пользователя и запретить сложение *
     * матриц, изменя значение result на false.                                                  */
    if(matrixes[ui->comboBox->currentIndex()].getSize() !=
            matrixes[ui->comboBox_2->currentIndex()].getSize()) {
        qDebug("sizes do not match: %i != %i", matrixes[ui->comboBox->currentIndex()].getSize(),
                matrixes[ui->comboBox_2->currentIndex()].getSize());
        QMessageBox msg;
        msg.setText("Ошибка! Вы пытаетесь сложить матрицы разного размера!"
                    "Выберите другие матрицы");
        msg.exec();
        result = false;
    }
}
