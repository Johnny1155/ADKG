#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "types.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    ui->Canvas->changePolygon();
}

void Widget::on_pushButton_2_clicked()
{
    //Get polygons
    std::vector<QPointFB> polA = ui->Canvas->getA();
    std::vector<QPointFB> polB = ui->Canvas->getB();

    //Perform Boolean operation
    TBooleanOperation oper = (TBooleanOperation)(ui->comboBox->currentIndex());
    std::vector<Edge> res = Algorithms::booleanOperations(polA, polB, oper);

    //Set importatnt intersections
    if(oper == Union)
    {
        ui->Canvas->clearInterA();
        ui->Canvas->clearInterB();
        ui->Canvas->clearInterRes();
    }

    else if(oper == Intersect)
    {
        std::vector<QPointF> inter_res;
        inter_res = Algorithms::solve0DProblems(polA, polB, inter_res);
        inter_res = Algorithms::solve0DProblems(polB, polA, inter_res);
        ui->Canvas->clearInterA();
        ui->Canvas->clearInterB();
        ui->Canvas->setIntersectsAloneRes(inter_res);
    }

    else if(oper == DifferenceAB)
    {
        std::vector<QPointF> inter_b;
        inter_b = Algorithms::solve0DProblems(polA, polB, inter_b);
        inter_b = Algorithms::solve0DProblems(polB, polA, inter_b);
        ui->Canvas->clearInterA();
        ui->Canvas->clearInterRes();
        ui->Canvas->setIntersectsAloneB(inter_b);
    }

    else if(oper == DifferenceBA)
    {
        std::vector<QPointF> inter_a;
        inter_a = Algorithms::solve0DProblems(polA, polB, inter_a);
        inter_a = Algorithms::solve0DProblems(polB, polA, inter_a);
        ui->Canvas->clearInterB();
        ui->Canvas->clearInterRes();
        ui->Canvas->setIntersectsAloneA(inter_a);
    }

    //Set results and update
    ui->Canvas->setRes(res);
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearResults();
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();
    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    //Load Polygon
    QString filePath=QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");
    std::string path_utf8=filePath.toUtf8().constData();

    std::vector<QPointFB> polA, polB;

    ui->Canvas->loadTxtFile(polA, polB, path_utf8);

    ui->Canvas->setA(polA);
    ui->Canvas->setB(polB);

    //Clear old results
    ui->Canvas->clearResults();

    repaint();
}









