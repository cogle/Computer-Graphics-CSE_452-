#ifndef TORUS_H
#define TORUS_H

#include "Shapes.h"

class Torus : public Shape{
public:
	Torus();
	Torus(int param1, int param2, std::vector<Vertex_Normal> &  torus, std::vector<Line_Points> &  torus_circles);
private:
	void Circle(double current_theta, int num_divisions, std::vector<Line_Points> &  t_circles);
	void AddPoints(Vector3 v1, Vector3 v2, double v1_angle, double v2_angle, double current_theta, std::vector<Line_Points> &  t_circles);
	double Calc_X(double little_circle, double big_circle);
	double Calc_Y(double little_circle);
	double Calc_Z(double little_circle, double big_circle);
	const double radius = .5;
	const double radius_b = 1.0;
};

#endif