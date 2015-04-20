#include "Cylinder.h"

Cylinder::Cylinder(){

}

Cylinder::Cylinder(int divisions,int stacks, std::vector<Vertex_Normal> &  vec){
	DrawStacks(divisions,stacks,vec);
}

void Cylinder::DrawStacks(int divisions,int stack_h, std::vector<Vertex_Normal> &  stacks){
	Matrix4 base = Matrix4();
	double d_theta = double(double(360) / double(divisions));
	double d_h = double(1)/double(stack_h);
	double y_pos = d_h + -.5;
	Matrix4 rot = base.yrotation(DegreeToRad(d_theta));
	//NEED to draw four points then rotate them around the axis and store them into a vector
	//Then go up in the height until you have reached the top of the division

	//Starting vector
	Vector4 v0 = Vector4(0.5, -.5, 0, 1);
	double v2_x = double(.5)*cos(DegreeToRad(d_theta));
	double v2_z = double(.5)*sin(DegreeToRad(d_theta));
	Vector4 v1 = Vector4(v2_x, -.5, v2_z, 1);
	Vector4 v2 = Vector4(v2_x, y_pos, v2_z, 1);
	//Create first triangle
	AddTriangleWithVertexNormal(Vector3(v2[0], v2[1], v2[2]), Vector3(v1[0], v1[1], v1[2]), Vector3(v0[0], v0[1], v0[2]), stacks, 1);

	Vector4 v3 = Vector4(0.5, y_pos, 0, 1);
	//Create second triangle
	AddTriangleWithVertexNormal(Vector3(v2[0], v2[1], v2[2]), Vector3(v0[0], v0[1], v0[2]), Vector3(v3[0], v3[1], v3[2]), stacks, 1);

	int count = 1;
	int h_count = 0;
	Vector4 prev0 = v0;
	Vector4 prev1 = v1;
	Vector4 prev2 = v2;
	Vector4 prev3 = v3;
	Matrix4 height;
	while (h_count < stack_h){
		while ( count < divisions){
			Vector4 t0 = rot*prev0;
			Vector4 t1 = rot*prev1;
			Vector4 t2 = rot*prev2;
			Vector4 t3 = rot*prev3;
			AddTriangleWithVertexNormal(Vector3(t2[0], t2[1], t2[2]), Vector3(t1[0], t1[1], t1[2]), Vector3(t0[0], t0[1], t0[2]), stacks,1);
			AddTriangleWithVertexNormal(Vector3(t2[0], t2[1], t2[2]), Vector3(t0[0], t0[1], t0[2]), Vector3(t3[0], t3[1], t3[2]), stacks,1);

			count++;
			prev0 = t0;
			prev1 = t1;
			prev2 = t2;
			prev3 = t3;
		}
		count = 0;
		height = base.translation(Vector3(0,d_h,0));
		prev0 = height*prev0;
		prev1 = height*prev1;
		prev2 = height*prev2;
		prev3 = height*prev3;
		h_count++;
	}
}

void Cylinder::Intersect(HitRecord & hr, Point3 P, Vector3 d){
	double A = d[0]*d[0] + d[2]*d[2];
	double B = double(2)*(P[0]*d[0] + P[2]*d[2]);
	double C = P[0]*P[0] + P[2]*P[2] - double(.25);
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
			goto Check_Caps;
		}
		if (closest < 0){
			std::swap(closest,furthest);
			std::swap(test_pt_1,test_pt_2);
		}

		/*std::cout << "Closest: " << closest << std::endl;
		std::cout << "Furthest: " << furthest << std::endl;
		std::cout << "Test Point 1: ";
		test_pt_1.print();
		std::cout << "Test Point 2: ";
		test_pt_2.print();*/
		//Way to High
		if (test_pt_1[1] > .5 && test_pt_2[1] > .5){
			return;
		}
		//Way to Low
		if (test_pt_1[1] < -.5 && test_pt_2[1] < -.5){
			return;
		}
		//Perfect
		if (test_pt_1[1] < .5 && test_pt_1[1] > -.5){
			if (closest >= 0 ){
				Vector3 vec = Vector3(P[0], P[1], P[2]) + closest*d;
				Vector3 norm = CalcNormal(vec, 1);
				norm.normalize();
				hr.addHit(closest, 0, 0, Point3(vec[0], vec[1], vec[2]), norm);
			}
		}
		//Perfect
		if (test_pt_2[1] < .5 && test_pt_2[1] > -.5){
			if (furthest >= 0 ){
				Vector3 vec = Vector3(P[0], P[1], P[2]) + furthest*d;
				Vector3 norm = CalcNormal(vec, 1);
				norm.normalize();
				hr.addHit(furthest, 0, 0, Point3(vec[0], vec[1], vec[2]), norm);
			}
		
		}

	}

	Check_Caps:
	//Top
	double t = -1;
	double t_top = .5 - P[1];
	double t_bot = d[1];
	if (!isZero(t_bot)){
		t = t_top / t_bot;
		Vector3 vec = Vector3(P[0], P[1], P[2]) + t*d;
		double test_pt = vec[0] * vec[0] + vec[2] * vec[2];
		//std::cout << "t_top: " << t << std::endl;
		//vec.print();
		if (t >= 0  && test_pt <= .25){
			hr.addHit(t, 0, 0, Point3(vec[0], vec[1], vec[2]), Vector3(0, 1, 0));
		}
	}
	//Bot
	t_top = .5 + P[1];
	t_bot = -d[1];
	if (!isZero(t_bot)){
		t = t_top / t_bot;
		Vector3 vec = Vector3(P[0], P[1], P[2]) + t*d;
		double test_pt = vec[0] * vec[0] + vec[2] * vec[2];
		//std::cout << "t_bot: " << t << std::endl;
		//vec.print();
		if (t >= 0  && test_pt <= .25){
			hr.addHit(t, 0, 0, Point3(vec[0], vec[1], vec[2]), Vector3(0, -1, 0));
		}
	}
	return;
}