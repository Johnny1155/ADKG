#ifndef WIDGETS_H
#define WIDGETS_H

#include <QMainWindow>
#include <QtGui>
#include <vector>
#include <QFileDialog>
#include <iostream>

namespace Ui {
class Widgets;
}

class Widgets : public QMainWindow
{
    Q_OBJECT

public:
    explicit Widgets(QWidget *parent = 0);
    ~Widgets();

private slots:
    void on_pushButton_analyze_clicked();

    void on_pushButton_loadTxtFile_clicked();

private:
    Ui::Widgets *ui;
};

#endif // WIDGETS_H
