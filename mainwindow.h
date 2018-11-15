#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QProgressBar>
#include <QTabWidget>
#include <QTimer>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include <ctime>

// локальные файлы
#include "dialog.h"
#include "matrix.h"
#include "summation.h"
#include "assignment.h"

namespace Ui {
class MainWindow;
}

namespace Di {
class Dialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_menuCreate_triggered();
    void step(); // Метод для регулярной обработки условий
    void on_lineEdit_changed();
    void resizeEvent(QResizeEvent * event);
    void on_saveFile_triggered();
    void on_openFile_triggered();
    void on_summation_triggered();

    void on_assignment_triggered();

private:
    Ui::MainWindow* ui;
    QProgressBar* progressBar; // Прогресс бар в строке состояния
    QTabWidget* mainTabWidget; // создание нескольких вкладок для работы с несколькими матрицами
    QTimer* actionTimer; // Таймер для регулярной обработки условий
    Matrix* matrixes; // Массив матриц
    QWidget * currentTab;
    Matrix * currentMatrix; // Текущая матрица

    // Переменные настроек
    bool displayControlSetting;
    bool incorrectTabWidgetDisplay; // Если таб виджет накладывается на меню бар (было обнаружено в
                                    // kubuntu), то следует изменить координату "у" таб виджета

    int tabWidgetY; // переменная, которая нулевая, когда c отображением всё впорядке, иначе выстой
                    // в меню бар

    QString get_lineEdits_values(QWidget *widget);
    QString get_matrix_values(QWidget *widget);

    void setMenusEnabled(bool value);

    int currentNumberOfTabs;

};

#endif // MAINWINDOW_H
