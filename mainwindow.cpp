#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QMessageBox>

#define MINUS_W 100 // Временные константы, т.к. до конца не построен графический интерфейс.
#define MINUS_H 100 // Подразумевают собой расстояние от правого и нижнего края


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Создание ui формы
    ui->setupUi(this);

    currentNumberOfTabs = 0; // Количество вкладок обнуляется

    // Создание progress bar для наглядного отображения загрузки
    progressBar = new QProgressBar(this);
    progressBar->setLayoutDirection(Qt::LeftToRight);
    statusBar()->addWidget(progressBar);
    progressBar->setValue(100);

    // Создания таймера для проверки регулярных условий
    actionTimer = new QTimer(this);
    actionTimer->setInterval(1000);
    connect(actionTimer, SIGNAL(timeout()), this, SLOT(step()));
    actionTimer->start();

    // Объявление виджета вкладок для дальнейшего использования
    mainTabWidget = new QTabWidget(this);
    mainTabWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menuCreate_triggered()  // triggered = нажание
{

    Dialog dialog;


    // Открывается окно создания матрицы, выполняется при нажатии кнопки ОК
    if(dialog.exec())
    {
        if(currentNumberOfTabs > 0)
        {
            // Изменяем размер массива матриц
            Matrix * temp = new Matrix[currentNumberOfTabs];
            for(int i = 0; i < currentNumberOfTabs; i++)
                temp[i] = matrixes[i];

            matrixes = new Matrix[currentNumberOfTabs+1];
            for(int i = 0; i < currentNumberOfTabs; i++)
                matrixes[i] = temp[i];

            matrixes[currentNumberOfTabs].setSize(dialog.getMatrixSize());
            matrixes[currentNumberOfTabs].setName(dialog.getMatrixName());

            filType type = dialog.getFilType();
            srand(time(0)); /* если понадобится рандом, то автоматически рандомизируем до *
                             * вхожденияв цикл.                                           */

            // непосредственое создание самой матрицы на экране
            QWidget * newTab = new QWidget();
            matrixes[currentNumberOfTabs].setWidget(newTab);
            QGridLayout * layout = new QGridLayout;
            // Заполнение layout'a полями
            for(int i = 0; i < matrixes[currentNumberOfTabs].getSize(); i++) {
                for(int j = 0; j < matrixes[currentNumberOfTabs].getSize(); j++) {
                    if(i < j) {
                        QLabel * label = new QLabel;
                        label->setText("0");
                        layout->addWidget(label, i, j);
                    }else {
                        QDynamicLineEdit * line = new QDynamicLineEdit;
                        layout->addWidget(line, i, j);
                        float value;
                        switch (type) {
                            case RANDOM:
                                value = rand()%10;
                                break;
                            case NULLIFIED:
                                value = 0;
                                break;
                            case DEFINED:
                                value = dialog.getDefinedValue();
                                break;
                        }
                        line->setText(QString::number(value));
                        matrixes[currentNumberOfTabs].setValue(i, j, value);
                    }
                }
            }
            newTab->setLayout(layout);
            mainTabWidget->addTab(currentTab = newTab, matrixes[currentNumberOfTabs].getName());
            currentNumberOfTabs++;
            progressBar->setValue(100);
        }
        else
        {
            currentNumberOfTabs++;

            mainTabWidget->setVisible(true);
            mainTabWidget->setGeometry(0, 0, width() - MINUS_W , height() -
                                       statusBar()->height() - MINUS_H );

            progressBar->setValue(0);
            matrixes = new Matrix[currentNumberOfTabs];
            matrixes[0].setSize(dialog.getMatrixSize());
            matrixes[0].setName(dialog.getMatrixName());
            filType type = dialog.getFilType();
            srand(time(0)); /* если понадобится рандом, то автоматически рандо мизируем до  *
                             * вхождения в цикл.                                            */

            // Генеразция вкладки
            QWidget * newTab = new QWidget();
            QGridLayout * layout = new QGridLayout;
            matrixes[0].setWidget(newTab);

            // Заполнение layout'a полями
            for(int i = 0; i < matrixes[0].getSize(); i++) {
                for(int j = 0; j < matrixes[0].getSize(); j++) {
                    if(i < j) {
                        QLabel * label = new QLabel;
                        label->setText("0");
                        layout->addWidget(label, i, j);
                    }else {
                        /*если элемент находится на главное диагонали или ниже *
                         *её, создаётся поле, которое можно редактировать.     */
                        QDynamicLineEdit * line = new QDynamicLineEdit;
                        layout->addWidget(line, i, j);
                        float value;
                        // выбирается тип, указанный при создании вкладки
                        switch (type) {
                            case RANDOM:
                                value = rand()%10;
                                break;
                            case NULLIFIED:
                                value = 0;
                                break;
                            case DEFINED:
                                value = dialog.getDefinedValue();
                                break;
                        }
                        matrixes[0].setValue(i, j, value);
                        line->setText(QString::number(value));
                    }
                }
            }

            newTab->setLayout(layout);
            mainTabWidget->addTab(currentTab = newTab, matrixes[0].getName());

            progressBar->setValue(100);
        }
    }

}

QString MainWindow::get_values(QLayout *layout)
{
    // Мэсдж бокс для просмотра некоторой информации -- удалить
    QMessageBox msg;
    msg.setText(QString::number(layout->count()));
    //msg.exec();    

    QString result = QString::number(int(sqrt(layout->count())));
    for(int i = 0; i < layout->count(); i++) {
        int j = i%(int)sqrt(layout->count()), i_i = i/(int)sqrt(layout->count());
        if(i_i >= j) {
            QDynamicLineEdit *line = qobject_cast<QDynamicLineEdit*>(layout->itemAt(i)->widget());
            result+= " " + line->text();
        }
    }

    result+= " " + mainTabWidget->tabText(mainTabWidget->currentIndex());
    return result;
}

QString MainWindow::get_values(QWidget * widget)
{
    /* Перегруженная функция получения значений матрицы. Удобнее в использовании, ведь требует *
     * только ссылку на матрицу(виджет) и включает в себя использование матрицы в динамическом *
     * массиве                                                                                 */
    QString result = "";
    // В цикле просматриваются все матрицы, использовающиеся в данный момент и находится необходимая
    for(int i = 0; (i < currentNumberOfTabs) && (result == ""); i++) {
        if(matrixes[i].thisWidget(widget)) {
            // Первая цифра -- это размер квадратной матрицы
            int size;
            result+= QString::number(size = matrixes[i].getSize());
            for(int j = 0; j < (size*size + size)/2; j++) {
                // Далее прибавляются все значения
                result+= " " + QString::number(matrixes[i].getValue(j));
            }
        }
    }
    // И в самом конце прибавляется имя.
    result += " " + mainTabWidget->tabText(mainTabWidget->currentIndex());
    return result;
}

void MainWindow::step()
{
    // Полный прогресс бар можно не демонстрировать
    if(progressBar->value() == 100)
        progressBar->setVisible(false);
    else if (!progressBar->isVisible())
        progressBar->setVisible(true);

}

void MainWindow::on_pushButton_clicked()
{
    // Эта кнопка необходима для проверки значений матриц. В скором времени откажусь.
    this->currentTab = mainTabWidget->currentWidget();
    QLayout * layout = mainTabWidget->currentWidget()->layout();
    ui->label->setText(this->get_values(layout));
    QByteArray res = this->get_values(currentTab).toLocal8Bit();
    const char * debugInfo = res.data();
    qDebug("In the matrix, these values: %s", debugInfo);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if(mainTabWidget->isVisible()) {
        mainTabWidget->setGeometry(0, 0, width() - MINUS_W , height() -
                                   statusBar()->height() - MINUS_H );
    }
}

void MainWindow::on_saveFile_triggered()
{
    // Вызов диалога файлов. Вывод сообщения в дебаг с путём файла.
    QFileDialog * dialog = new QFileDialog;
    QString path = dialog->getSaveFileName(0, "Matrix save", "", "*.txt");
    QByteArray ba = path.toLocal8Bit() + " file is open";
    const char *inDebug = ba.data();
    qDebug("\nThe file %s is opened.",inDebug);
    QFile file(path);

    // Выгрузка элементов из матрицы, выбранной на экране.
    if(mainTabWidget->count() > 0) {
        this->currentTab = mainTabWidget->currentWidget();
        QLayout * layout = mainTabWidget->currentWidget()->layout();
        QByteArray fileText =  this->get_values(layout).toLocal8Bit();

        //Непосредственная запись в файл
        if(file.open(QIODevice::WriteOnly)) {
            file.write(fileText);
        }
    } else {
        // проверка на пустоту вкладки
        QMessageBox msg;
        msg.setText("Вкладка с матрицей не выбрана! Создайте или откройте матрицу и повторите "
                    "действие");
        msg.exec();
    }


    file.close();
    qDebug("File is closed.");

}

void MainWindow::on_openFile_triggered()
{
    // Вызов диалога файлов. Вывод сообщения в дебаг с путём файла.
    QFileDialog * dialog = new QFileDialog;
    QString path = dialog->getOpenFileName(0, "Matrix open", "", "*.txt");
    QByteArray ba = path.toLocal8Bit() + " file is open";
    const char *inDebug = ba.data();
    qDebug("\nThe file %s is opend.",inDebug);
    QFile file(path);

    if(file.open(QIODevice::ReadOnly)) {
        // создание новой вкладки и отрисовка элементов


    }

    file.close();
    qDebug("File is closed.");
}
