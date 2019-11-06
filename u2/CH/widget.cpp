#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "generatepoints.h"

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
	//Create convex hull
	QPolygon ch;
	std::vector<QPoint> points = ui->Canvas->getPoints();

	if (points.size() > 2)
	{
		//Time started
		clock_t t_start = std::clock();

		//Create CH
		if (ui->comboBox->currentIndex() == 0)
			ch = Algorithms::jarvisScan(points);
		else if(ui->comboBox->currentIndex() == 1)
			ch = Algorithms::qHull(points);
		else if (ui->comboBox->currentIndex() == 2)
			ch = Algorithms::sweepLine(points);
		else
			ch=Algorithms::grahamScan(points);

		//Strict convex hull
		if (ui->checkBox->checkState())
			ch = Algorithms::strictCH(ch);

		//Time ended
		clock_t t_end = std::clock();
		clock_t t_final = t_end-t_start;

		//Set time into label and convert it from number to QString
		if (t_final == 0)
			ui->label_setTime->setText("<1 ms");
		else
			ui->label_setTime->setText(QString::number(t_final)+" ms");

		//Clear widget for warning message if amount of points is correct
		ui->label_4->clear();

		//Draw
		ui->Canvas->setCH(ch);
		repaint();
		}
	//Warning message
	else
		ui->label_4->setText("Less than 3 points!");
}

void Widget::on_pushButton_2_clicked()
{
	//Clear points
	ui->Canvas->clearPoints();
}

void Widget::on_pushButton_3_clicked()
{
	//Clear convex hull
	ui->Canvas->clearCH();
}

void Widget::on_pushButton_genPoints_clicked()
{
	//Generating points
	std::vector<QPoint> points;
	int n = ui->lineEdit_genPoints->text().toInt();

	if(n > 0)
	{
		//Clear widget for warning message if amount of points is correct
		ui->label_4->clear();

		if (ui->comboBox_genPoints->currentIndex() == 0)
		{
			points=GeneratePoints::generatingRandomField(n);
		}
		else if (ui->comboBox_genPoints->currentIndex() == 1)
		{
			points=GeneratePoints::generatingCircle(n);
		}
		else if (ui->comboBox_genPoints->currentIndex() == 2)
		{
			points=GeneratePoints::generatingEllipse(n);
		}
		else if (ui->comboBox_genPoints->currentIndex() == 3)
		{
			points=GeneratePoints::generatingSquare(n);
		}
		else if (ui->comboBox_genPoints->currentIndex() == 4)
		{
			points=GeneratePoints::generatingStar(n);
		}
	}
	//Warning message
	else
		ui->label_4->setText("Set at least 1 point");

	ui->Canvas->setPoints(points);
	repaint();
}

void Widget::on_pushButton_minBox_clicked()
{
	//Create minimum area enclosing box
	QPolygon ch=ui->Canvas->getCh();

	if (ch.size() > 2)
	{
		QPolygonF ch_new=Algorithms::minimumRectangle(ch);

		ui->Canvas->setMinBoundary(ch_new);
		repaint();
	}
}

void Widget::on_pushButton_4_clicked()
{
	//Clear minimum area enclosing box
	ui->Canvas->clearMinBoundary();
}

void Widget::on_pushButton_5_clicked()
{
	//Clear all
	ui->Canvas->clearCH();
	ui->Canvas->clearMinBoundary();
	ui->Canvas->clearPoints();
}
