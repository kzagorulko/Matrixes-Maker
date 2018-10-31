#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QMessageBox>

#define MINUS_W 100 // Временные константы, т.к. до конца не построен графический интерфейс.
#define MINUS_H 100 // Подразумевают собой расстояние от правого и нижнего края
#define HEIGHT_OF_TAB 30 // Высота служебной панели в TabWidget


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

    // Установление настроек
    displayControlSetting = true; /* когда контроль включён - устанавливается минимальная высота *
                                   * и ширина для lineEdit'ов.                                   */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menuCreate_triggered()  // triggered = нажание
{

    Dialog dialog;


    // Открывается окно создания матрицы, выполняется при нажатии кнопки ОК
    if(dialog.exec()) {
        /* Проверка на то, что высота TextEdit будет больше 27 пксл., а длинна больше 50 - иначе *
         * ухудшается читаемость. Контроль отображения, когда влючен не должен влиять на         *
         * поведение алгоритма.                                                                  */
        if(((dialog.getMatrixSize()*27 + 3 < height() - statusBar()->height() - MINUS_H -
                HEIGHT_OF_TAB) && ( dialog.getMatrixSize()*50 + 3 < width() - MINUS_W)) ||
                !displayControlSetting) {

            if(currentNumberOfTabs == 0)  {
                // Создаётся таб виджет при первом создании вкладки
                mainTabWidget->setVisible(true);
                mainTabWidget->setGeometry(0, 0, width() - MINUS_W , height() -
                                           statusBar()->height() - MINUS_H );
                matrixes = new Matrix[currentNumberOfTabs+1];
            } else {
                // Изменяем размер массива матриц
                Matrix * temp = new Matrix[currentNumberOfTabs];
                for(int i = 0; i < currentNumberOfTabs; i++)
                    temp[i] = matrixes[i];

                matrixes = new Matrix[currentNumberOfTabs+1];
                for(int i = 0; i < currentNumberOfTabs; i++)
                    matrixes[i] = temp[i];
            }

            progressBar->setValue(0);
            matrixes[currentNumberOfTabs].setSize(dialog.getMatrixSize());
            matrixes[currentNumberOfTabs].setName(dialog.getMatrixName());

            filType type = dialog.getFilType();
            srand(time(0)); /* если понадобится рандом, то автоматически рандомизируем до *
                             * вхожденияв цикл.                                           */

            // непосредственое создание самой матрицы на экране
            QWidget * newTab = new QWidget();
            QGridLayout * layout = new QGridLayout;
            layout->setVerticalSpacing(0);
            layout->setHorizontalSpacing(0);
            matrixes[currentNumberOfTabs].setWidget(newTab);

            // Заполнение layout'a полями
            for(int i = 0; i < matrixes[currentNumberOfTabs].getSize(); i++) {
                for(int j = 0; j < matrixes[currentNumberOfTabs].getSize(); j++) {
                    if(i < j) {
                        QLabel * label = new QLabel;
                        label->setText(" 0");
                        layout->addWidget(label, i, j);
                    }else {
                        QLineEdit * line = new QLineEdit;
                        line->setValidator(new QDoubleValidator);
                        connect(line, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_changed()));
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
                        line->setText(QString::number(value).replace(".",","));
                        matrixes[currentNumberOfTabs].setValue(i, j, value);
                    }
                }
            }

            newTab->setLayout(layout);
            mainTabWidget->addTab(currentTab = newTab, matrixes[currentNumberOfTabs].getName());
            currentNumberOfTabs++;
            progressBar->setValue(100);
        } else {
            QMessageBox msg;
            msg.setText("Ошибка! Корректное отображение матрицы невозможно. Увеличте размер окна или "
                        "воспользуйтесь другими функциями программы.");
            msg.exec();
        }
    }
}

QString MainWindow::get_lineEdits_values(QWidget * widget)
{
    // Функция, которая используется для получения значений едитов выбранной вкладки
    QLayout * layout = widget->layout();
    QString result = QString::number(int(sqrt(layout->count())));
    for(int i = 0; i < layout->count(); i++) {
        /* Для того, чтобы не "залезть" в лейблы, расчитывается условие неперехода в верхний *
         * треугольник                                                                       */
        int j = i%(int)sqrt(layout->count()), i_i = i/(int)sqrt(layout->count());
        if(i_i >= j) {
            QLineEdit *line = qobject_cast<QLineEdit*>(layout->itemAt(i)->widget());
            result+= " " + line->text();
        }
    }

    result+= " " + mainTabWidget->tabText(mainTabWidget->currentIndex());
    return result;
}

QString MainWindow::get_matrix_values(QWidget * widget)
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
            currentMatrix = &matrixes[i];
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

void MainWindow::on_lineEdit_changed()
{
    /* слот, вызываемый при завершении редактирования */

    // т.к. работа производится над вкладкой, отображённой на экране, задаём текущую вкладку
    currentTab = mainTabWidget->currentWidget();
    // поэлементно разделяем строки значений
    QStringList edits  = get_lineEdits_values(currentTab).split(" ");
    QStringList matrix = get_matrix_values(currentTab).split(" ");
    /* цикл начинается с единицы, т.к. обе функции нулевым элементом выдают размер матрицы       *
     * а заканчивается на один раньше, потому что функции выдают последним значением имя матрицы */
    for(int i = 1; i < edits.length()-1; i++) {
        if(edits[i] != matrix[i]) {
            /* i-1 по той же причине, по которой цикл начинается с единицы - функция нулевым *
             * элементом выдаёт размер, а функция класса Matrix - setValue, задаёт именно    *
             * значение, которые хранятся, начиная с первого элемента                        */
            currentMatrix->setValue(i-1, edits[i].replace(",", ".").toFloat());
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    // убрать кнопку
    currentTab = mainTabWidget->currentWidget();
    qDebug("In edits, these values:  %s\nIn matrix, these values: %s\n",
           get_lineEdits_values(currentTab).toLocal8Bit().data(),
           get_matrix_values(currentTab).toLocal8Bit().data());
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
        //QLayout * layout = mainTabWidget->currentWidget()->layout();
        QByteArray fileText =  this->get_matrix_values(currentTab).toLocal8Bit();

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
        // Чтение информации из файла
        QString matrix = file.readLine();
        QStringList splitMatrix= matrix.split(' ');
        qDebug("Opened matrix: %s", matrix.toLocal8Bit().data());
        int sizeOfMatrix = splitMatrix[0].toInt();
        /* Проверка на то, что высота TextEdit будет больше 27 пксл., а длинна больше 50 - иначе *
         * ухудшается читаемость. Контроль отображения, когда влючен не должен влиять на         *
         * поведение алгоритма.                                                                  */
        if(((sizeOfMatrix*27 + 3 < height() - statusBar()->height() - MINUS_H - HEIGHT_OF_TAB)
            && ( sizeOfMatrix*50 + 3 < width() - MINUS_W) ) || !displayControlSetting) {

            QString nameOfMatrix = splitMatrix[splitMatrix.count()-1];
            qDebug("Name of opened matrix: %s", nameOfMatrix.toLocal8Bit().data());

            // Генерация вкладки
            if(currentNumberOfTabs == 0)
            {
                // Создаётся таб виджет при первом создании вкладки
                mainTabWidget->setVisible(true);
                mainTabWidget->setGeometry(0, 0, width() - MINUS_W , height() -
                                           statusBar()->height() - MINUS_H );
                matrixes = new Matrix[currentNumberOfTabs+1];
            } else {
                // Изменяем размер массива матриц
                Matrix * temp = new Matrix[currentNumberOfTabs];
                for(int i = 0; i < currentNumberOfTabs; i++)
                    temp[i] = matrixes[i];

                matrixes = new Matrix[currentNumberOfTabs+1];
                for(int i = 0; i < currentNumberOfTabs; i++)
                    matrixes[i] = temp[i];
            }

            progressBar->setValue(0);
            matrixes[currentNumberOfTabs].setSize(sizeOfMatrix);
            matrixes[currentNumberOfTabs].setName(nameOfMatrix);

            // непосредственое создание самой матрицы на экране
            QWidget * newTab = new QWidget();
            QGridLayout * layout = new QGridLayout;
            matrixes[currentNumberOfTabs].setWidget(newTab);

            // Заполнение layout'a полями
            for(int i = 0, k = 1; i < matrixes[currentNumberOfTabs].getSize(); i++) {
                for(int j = 0; j < matrixes[currentNumberOfTabs].getSize(); j++) {
                    if(i < j) {
                        QLabel * label = new QLabel;
                        label->setText("0");
                        layout->addWidget(label, i, j);
                    }else {
                        QLineEdit * line = new QLineEdit;
                        line->setValidator(new QDoubleValidator);
                        connect(line, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_changed()));
                        layout->addWidget(line, i, j);
                        float value = splitMatrix[k++].toFloat();
                        line->setText(QString::number(value).replace(".",","));
                        matrixes[currentNumberOfTabs].setValue(i, j, value);
                    }
                }
            }
            newTab->setLayout(layout);
            mainTabWidget->addTab(currentTab = newTab, matrixes[currentNumberOfTabs].getName());
            currentNumberOfTabs++;
            progressBar->setValue(100);
        } else {
            QMessageBox msg;
            msg.setText("Ошибка! Корректное отображение матрицы невозможно. Увеличте размер окна "
                        "или воспользуйтесь другими функциями программы.");
            msg.exec();
        }
    }
    file.close();
    qDebug("File is closed.");
}

void MainWindow::on_summation_triggered()
{

}
