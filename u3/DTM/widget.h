#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <iostream>
#include <QFileDialog>
#include <QtGui>

#include "sortbyz.h"
#include "orcuntours.h"
#include "qcontdescr.h"
#include "edge.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    double z_min, z_max, dz, dz_main;
    int x_max, y_max;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_cl_polygon_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_load_file_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_slope_clicked();

    void on_pushButton_aspect_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
