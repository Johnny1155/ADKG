#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QDebug>


class Draw : public QWidget
{
	Q_OBJECT

private:
	std::vector<QPoint> points;
	QPolygon ch;
	QPolygonF minBoundary;

public:
	explicit Draw(QWidget *parent = nullptr);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *e);
	void clearCH(){ch.clear(); repaint();}
	void clearPoints(){points.clear(); repaint();}
	void clearMinBoundary(){minBoundary.clear(); repaint();}
	std::vector<QPoint> getPoints(){return points;}
	void setCH(QPolygon &hull) {ch = hull;}
	void setPoints(std::vector<QPoint> &pts) {points = pts;}
	void setMinBoundary(QPolygonF &bound){minBoundary = bound;}
	QPolygon getCh(){return ch;}

signals:

public slots:
};

#endif // DRAW_H
