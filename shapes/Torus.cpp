#include "Torus.h"

Torus::Torus(int param1, int param2, std::vector<Vertex_Normal> &  torus, std::vector<Line_Points> &  torus_circles){
	std::vector<std::vector<Line_Points>> coordinates;
	double big_theta = double(double(double(90) / double(param2)));
	if (param1 < 3){
		param1 = 3;
	}
	double cur_big_theta = double(0);
	int iter = param2 * 4;
	for (int i = 0; i < iter; i++){
		Circle(DegreeToRad(cur_big_theta), param1, torus_circles);
		cur_big_theta += big_theta;
	}
	cur_big_theta = double(0);
	int inner_count = 0;
	for (int i = 0; i < int(torus_circles.size()); i++){
		inner_count++;
		Vector3 v0 = torus_circles[i].points[0];
		Vector3 v1 = torus_circles[i].points[1];
		double prev_theta = cur_big_theta;
		cur_big_theta += big_theta;
		Vector3 v2 = Vector3(Calc_X(torus_circles[i].angles[0], DegreeToRad(cur_big_theta)), Calc_Y(torus_circles[i].angles[0]), Calc_Z(torus_circles[i].angles[0], DegreeToRad(cur_big_theta)));
		Vector3 v3 = Vector3(Calc_X(torus_circles[i].angles[1], DegreeToRad(cur_big_theta)), Calc_Y(torus_circles[i].angles[1]), Calc_Z(torus_circles[i].angles[1], DegreeToRad(cur_big_theta)));
		AddTriangleWithVertexNormal(v0, v3, v2, torus, torus_circles[i].angles[0], torus_circles[i].angles[1], torus_circles[i].angles[0], DegreeToRad(prev_theta), DegreeToRad(cur_big_theta), DegreeToRad(cur_big_theta));
		AddTriangleWithVertexNormal(v1, v3, v0, torus, torus_circles[i].angles[1], torus_circles[i].angles[1], torus_circles[i].angles[0], DegreeToRad(prev_theta), DegreeToRad(cur_big_theta), DegreeToRad(prev_theta));
		if (inner_count < param1){
			cur_big_theta = prev_theta;
		}
		else{
			inner_count = 0;
			cur_big_theta = cur_big_theta;
		}
	}
}

void Torus::Circle(double current_theta, int num_divisions, std::vector<Line_Points> &  t_circles){
	double d_theta = DegreeToRad(double(double(360) / double(num_divisions)));
	Vector3 v1 = Vector3(Calc_X(0, current_theta), Calc_Y(0), Calc_Z(0, current_theta));
	Vector3 v2 = Vector3(Calc_X(d_theta, current_theta), Calc_Y(d_theta), Calc_Z(d_theta, current_theta));

	AddPoints(v1, v2, 0, d_theta, current_theta, t_circles);
	int count = 1;
	double delta_degrees = double(double(360) / double(num_divisions));
	double cur_degrees = 0.0 + delta_degrees;
	while (count < num_divisions){
		double a_temp = DegreeToRad(cur_degrees);
		v1 = Vector3(Calc_X(a_temp, current_theta), Calc_Y(a_temp), Calc_Z(a_temp, current_theta));
		cur_degrees += delta_degrees;
		double rad_angle = DegreeToRad(cur_degrees);
		v2 = Vector3(Calc_X(rad_angle, current_theta), Calc_Y(rad_angle), Calc_Z(rad_angle, current_theta));
		AddPoints(v1, v2, a_temp, rad_angle, current_theta, t_circles);
		count++;
	}
}


void Torus::AddPoints(Vector3 v1, Vector3 v2, double v1_angle, double v2_angle, double current_theta, std::vector<Line_Points> &  t_circles){
	Line_Points pt = Line_Points(v1, v2, Torus_CalcNormal(v1_angle, current_theta), Torus_CalcNormal(v2_angle, current_theta), v1_angle, v2_angle);
	//pt.print();
	t_circles.push_back(pt);
}

double Torus::Calc_X(double little_circle, double big_circle){
	double lhs = (radius_b + radius*cos(little_circle));
	double rhs = cos(big_circle);
	return lhs*rhs;
}

double Torus::Calc_Y(double little_circle){
	return radius*sin(little_circle);
}

double Torus::Calc_Z(double little_circle, double big_circle){
	double lhs = (radius_b + radius*cos(little_circle));
	double rhs = sin(big_circle);
	return (lhs*rhs);
}

