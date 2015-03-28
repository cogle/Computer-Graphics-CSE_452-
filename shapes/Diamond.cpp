#include "Diamond.h"

Diamond::Diamond(){
	
}

void Diamond::Intersect(HitRecord & hr, Point3 P, Vector3 d){
	double h_2 = .25;

	double A = d[0] * d[0] * h_2 + d[2] * d[2] * h_2 - double(.25)*d[1] * d[1];
	double B_left = double(2)*P[0] * d[0] * h_2 + double(2)* P[2] * d[2] * h_2;
	double B_right_1 = double(.5)*d[1];
	double B_right_2 = (double(.5) - P[1]);
	double B_right = B_right_1*B_right_2;
	double B = B_left + B_right;


	double C_left = P[0] * P[0] * h_2 + P[2] * P[2] * h_2;
	double C_inner = double(.5) - P[1];
	double C_inner_2 = C_inner*C_inner;
	double C_right = double(.25)*C_inner_2;
	double C = C_left - C_right;

	double test = B*B - double(4)*A*C;

	double t_1 = -1;
	double t_2 = -1;

	double closest;
	double furthest;


	if (test >= 0){
		double top_lhs = -B;
		double top_rhs = sqrt(test);
		double top_1 = top_lhs + top_rhs;
		double top_2 = top_lhs - top_rhs;



		double bot = double(2) * A;

		double t_1 = top_1 / bot;
		double t_2 = top_2 / bot;

		closest = t_1 < t_2 ? t_1 : t_2;
		furthest = t_1 < t_2 ? t_2 : t_1;

		Point3 test_pt_1 = P + closest*d;
		Point3 test_pt_2 = P + furthest*d;


		if (closest < 0 && furthest < 0){
			goto Bot_Check;
		}
		if (closest < 0){
			std::swap(closest, furthest);
			std::swap(test_pt_1, test_pt_2);
		}


		//Way to High
		if (test_pt_1[1] > .5 && test_pt_2[1] > .5){
			return;
		}
		//Way to Low
		if (test_pt_1[1] < -.5 && test_pt_2[1] < -.5){
			return;
		}
		//Top
		if (test_pt_1[1] == .5){
			Vector3 vec = Vector3(P[0], P[1], P[2]) + closest*d;
			hr.addHit(closest, 0, 0, Point3(vec[0], vec[1], vec[2]), Vector3(0, 1, 0));
		}
		//Perfect
		if (test_pt_1[1] < .5 && test_pt_1[1] > 0){
			if (closest >= 0 && closest <= 4){
				Vector3 vec = Vector3(P[0], P[1], P[2]) + closest*d;
				Vector3 norm = CalcNormal(vec, 2);
				norm.normalize();
				hr.addHit(closest, 0, 0, Point3(vec[0], vec[1], vec[2]), norm);
			}
		}
		//Perfect
		if (test_pt_2[1] < .5 && test_pt_2[1] > 0){
			if (furthest >= 0 && furthest <= 4){
				Vector3 vec = Vector3(P[0], P[1], P[2]) + furthest*d;
				Vector3 norm = CalcNormal(vec, 2);
				norm.normalize();
				hr.addHit(furthest, 0, 0, Point3(vec[0], vec[1], vec[2]), norm);
			}
		}
	}


	//Bot
Bot_Check:
	double t = -1;
	double t_top = .5 + P[1];
	double t_bot = -d[1];
	if (!isZero(t_bot)){
		t = t_top / t_bot;
		Vector3 vec = Vector3(P[0], P[1], P[2]) + t*d;
		double test_pt = vec[0] * vec[0] + vec[2] * vec[2];
		if (t >= 0 && t <= 4 && test_pt <= .25){
			hr.addHit(t, 0, 0, Point3(vec[0], vec[1], vec[2]), Vector3(0, -1, 0));
		}
	}

	return;
}