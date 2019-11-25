#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QtGui>
#include <vector>
#include <QDebug>
#include <iostream>
#include <fstream>


using namespace std;

class draw : public QWidget
{
	Q_OBJECT

private:
	QPoint q;
	vector<QPolygon> polygons;
	vector<bool> color_result;
	string path;

public:
	explicit draw(QWidget *parent = 0);
	void mousePressEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);

	// Method returning point "q"
	QPoint getPoint() {return q;}

	// Method returning vector of loaded polygons
	vector<QPolygon> getPolygon(){return polygons;}

	void loadTxtFile(vector<QPolygon> &polygons, string &path);
	void setColorResult(vector<bool> colRes) {color_result = colRes;}
	void setTxtPath(string path_x) {path=path_x;
									loadTxtFile(polygons, path);}
signals:

public slots:
};

#endif // DRAW_H
