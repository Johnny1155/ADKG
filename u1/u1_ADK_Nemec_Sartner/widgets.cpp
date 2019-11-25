#include "widgets.h"
#include "ui_widgets.h"
#include "algorithms.h"
#include "draw.h"

Widgets::Widgets(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Widgets)
{
	ui->setupUi(this);

	//Name of main window
	this->setWindowTitle("U1_Nemec_Sartner");
}

Widgets::~Widgets()
{
	delete ui;
}

// Pushbutton for analyzing
void Widgets::on_pushButton_analyze_clicked()
{

	// Getting of needed variables from class Draw
	QPoint q = ui->Canvas->getPoint();
	vector<QPolygon> polygons = ui->Canvas->getPolygon();

	// Declaration of needed variables

	// Point of actual polygon
	QPoint p;

	// Actual polygon
	QPolygon act_polygon;

	// Polygon in vector format for methods which use it
	vector<QPoint> pol_i;

	// Variable for marking painted polygons
	vector<bool> color_result;

	// Browsing all loaded polygons
	for (unsigned int i = 0; i < polygons.size(); i++ )
	{

		// Separation of actual polygon
		act_polygon = polygons[i];

		for (int j = 0; j < act_polygon.size(); j++)
		{
			p = act_polygon[j];
			pol_i.push_back(p);
		}

		// Variable for resuts of analyzing
		int res = 0;

		// Widing method
		if (ui->comboBox->currentIndex() == 0)
		{
			res = Algorithms::positionPointPolygonWinding(q, pol_i);

			// Decision if the polygon has to be painted or not
			if (res == 0)
			{
				color_result.push_back(false);
			}
			else
			{
				color_result.push_back(true);
			}
		}

		//Ray Crossing method
		else
		{
			res = Algorithms::positionPointPolygonRayCrossing(q, pol_i);

			// Decision if the polygon has to be painted or not
			if (res == 0)
			{

				// Polygon is also painted in case, when point "q" lies on its vertex
				if (count(pol_i.begin(), pol_i.end(), q))
				{
					color_result.push_back(true);
				}
				else
				{
					color_result.push_back(false);
				}
			}
			else
			{
				color_result.push_back(true);
			}
		}

		// Clear vector for analyzed polygon
		pol_i.clear();
	}

	// Sending vector with informations, which polygons have to be painted
	ui->Canvas->setColorResult(color_result);
	ui->Canvas->repaint();
}

// Loads text file with polygons
void Widgets::on_pushButton_loadTxtFile_clicked()
{
	QString filePath=QFileDialog::getOpenFileName(
				this,
				tr("Select file"),
				"/",
				"Text file (*.txt);;All files (*.*)");
	string path_utf8=filePath.toUtf8().constData();
	ui->Canvas->setTxtPath(path_utf8);
}
