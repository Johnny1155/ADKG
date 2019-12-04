#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0;
    z_max = 500;
    dz = 1;
    dz_main = 5;

    //Set initial values
    ui->lineEdit->setText(QString::number(z_min));
    ui->lineEdit_2->setText(QString::number(z_max));
    ui->lineEdit_3->setText(QString::number(dz));
    x_max=ui->Canvas->height();
    y_max=ui->Canvas->width();


    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("");

    ui->comboBox_2->setItemData(0, QBrush(Qt::red), Qt::BackgroundColorRole);
    ui->comboBox_2->setItemData(1, QBrush(Qt::green), Qt::BackgroundColorRole);
    ui->comboBox_2->setItemData(2, QBrush(Qt::blue), Qt::BackgroundColorRole);

    ui->comboBox_2->setStyleSheet("QComboBox { background-color: red; }");

    ui->comboBox_3->addItem("");
    ui->comboBox_3->addItem("");
    ui->comboBox_3->addItem("");

    ui->comboBox_3->setItemData(0, QBrush(Qt::red), Qt::BackgroundColorRole);
    ui->comboBox_3->setItemData(1, QBrush(Qt::green), Qt::BackgroundColorRole);
    ui->comboBox_3->setItemData(2, QBrush(Qt::blue), Qt::BackgroundColorRole);

    ui->comboBox_3->setStyleSheet("QComboBox { background-color: red; }");


    ui->pushButton_hyps->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->clearDT();

    //Construct DT
    std::vector<QPoint3D> points=ui->Canvas->getPoints();
    std::vector<Edge> dt = Algorithms::DT(points);
    ui->Canvas->setDt(dt);

    repaint();
}

void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearPoints();
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearPoints();
    ui->Canvas->clearDT();
    ui->Canvas->clearAngles();
    ui->Canvas->clearContours();
    ui->Canvas->clearMainContours();
    ui->Canvas->clearPolygon();
    ui->Canvas->clearContDesc();
    ui->Canvas->clearDtm();
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearDT();
    ui->Canvas->clearAngles();
    ui->Canvas->clearMainContours();
    ui->Canvas->clearContours();
    ui->Canvas->clearContDesc();
    ui->Canvas->clearDtm();


    std::vector<QPoint3D> points=ui->Canvas->getPoints();
    std::vector<QPoint3D> points_;

    if (points.size()>2)
    {

        //Construct DT
        QPolygonF pol=ui->Canvas->getPolygon();
        if (ui->checkBox_2->isChecked())
        {
            points_=Algorithms::calculatePointsInsidePolygon(points, pol);
            points=points_;
        }


        std::vector<Edge> dt = Algorithms::DT(points);
        std::vector<double> angles;
        ui->Canvas->setDt(dt);

        //Create contour lines
        std::vector<Edge> contours = Algorithms::createContourLines(dt,  z_min, z_max, dz);
        std::vector<Edge> main_cont = Algorithms::createContourLines(dt, z_min, z_max, dz*dz_main);

        if (ui->checkBox_2->isChecked())
        {
            contours=Algorithms::calculateContoursInsidePolygon(contours, pol);
            main_cont=Algorithms::calculateContoursInsidePolygon(main_cont, pol);
        }

        ui->Canvas->setContours(contours);
        ui->Canvas->setMainContours(main_cont);

        repaint();
    }
}

void Widget::on_lineEdit_editingFinished()
{
    //Set z_min
    z_min = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_2_editingFinished()
{
    //Set z_max
    z_max = ui->lineEdit_2->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished()
{
    //Set dz
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_lineEdit_4_editingFinished()
{
    dz_main = ui->lineEdit_4->text().toInt();
}

void Widget::on_checkBox_clicked(bool checked)
{
    if (checked)
        ui->Canvas->setPolygonDraw(true);
    else
        ui->Canvas->setPolygonDraw(false);
}

void Widget::on_pushButton_cl_polygon_clicked()
{
    ui->Canvas->clearPolygon();
    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    ui->Canvas->clearPoints();
    ui->Canvas->clearContDesc();
    ui->Canvas->clearDT();
    ui->Canvas->clearContours();
    ui->Canvas->clearMainContours();
    ui->Canvas->clearDtm();

    x_max=ui->Canvas->height();
    y_max=ui->Canvas->width();

    std::vector<QPoint3D> points;

    if (ui->comboBox->currentIndex()==0)
        points=Algorithms::generateValley(x_max, y_max);
    else if (ui->comboBox->currentIndex()==1)
        points=Algorithms::generateHill(x_max, y_max);
    else if (ui->comboBox->currentIndex()==2)
        points=Algorithms::generateRidge(x_max, y_max);
    else if (ui->comboBox->currentIndex()==3)
        points=Algorithms::generateSaddle(x_max, y_max);
    else if (ui->comboBox->currentIndex()==4)
        points=Algorithms::generateVolcano(x_max, y_max);

    ui->Canvas->setPoints(points);
    repaint();
}

void Widget::on_pushButton_6_clicked()
{
    ui->Canvas->clearDT();
    repaint();
}

void Widget::on_pushButton_load_file_clicked()
{
    QString filePath=QFileDialog::getOpenFileName(
                this,
                tr("Select file"),
                "/",
                "Text file (*.txt);;All files (*.*)");
    std::string path_utf8=filePath.toUtf8().constData();


    x_max=ui->Canvas->height();
    y_max=ui->Canvas->width();

    std::vector<QPoint3D> points=Algorithms::loadFile(path_utf8, x_max, y_max);

    ui->Canvas->setPoints(points);

    repaint();
}

void Widget::on_pushButton_7_clicked()
{
    std::vector<Edge> m_cont=ui->Canvas->getMainContours();

    if (m_cont.size()>0)
    {
        std::vector<OrCuntours> oc=Algorithms::orientateContours(m_cont);
        std::vector<QContDescr> ds=Algorithms::describeContours(oc);

        ui->Canvas->setContoursDesc(ds);

        repaint();
    }
}

void Widget::on_pushButton_slope_clicked()
{
    ui->Canvas->setSlope(true);
    ui->Canvas->setAspect(false);

    ui->Canvas->clearDtm();

    std::vector<Edge> dt;

    //Create triangulation
    if (ui->Canvas->getDtSize() == 0)
    {
        std::vector<QPoint3D> points=ui->Canvas->getPoints();
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
    }

    //Triangulation has been created
    else
    {
        dt = ui->Canvas->getDt();
    }

    //Analyze DTM
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);
    ui->Canvas->setDTM(dtm);


    repaint();
}

void Widget::on_pushButton_aspect_clicked()
{
    ui->Canvas->setSlope(false);
    ui->Canvas->setAspect(true);

    ui->Canvas->clearDtm();

    std::vector<Edge> dt;

    //Create triangulation
    if (ui->Canvas->getDtSize() == 0)
    {
        std::vector<QPoint3D> points=ui->Canvas->getPoints();
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
    }

    //Triangulation has been created
    else
    {
        dt = ui->Canvas->getDt();
    }

    //Analyze DTM
    std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);
    ui->Canvas->setDTM(dtm);

    repaint();
}


void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    if (index==0)
        ui->comboBox_2->setStyleSheet("QComboBox { background-color: red; }");
    else if (index==1)
        ui->comboBox_2->setStyleSheet("QComboBox { background-color: green; }");
    else if (index==2)
        ui->comboBox_2->setStyleSheet("QComboBox { background-color: blue; }");

    ui->Canvas->setColor(index);
}

void Widget::on_comboBox_3_currentIndexChanged(int index)
{
    if (index==0)
        ui->comboBox_3->setStyleSheet("QComboBox { background-color: red; }");
    else if (index==1)
        ui->comboBox_3->setStyleSheet("QComboBox { background-color: green; }");
    else if (index==2)
        ui->comboBox_3->setStyleSheet("QComboBox { background-color: blue; }");

    ui->Canvas->setColor2(index);
}














