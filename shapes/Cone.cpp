#include "Cone.h"

Cone::Cone(int divisions, int stacks, std::vector<Vertex_Normal> &  vec){
	DrawStacks(divisions, stacks, vec);
}

void Cone::DrawStacks(int divisions, int stack_h, std::vector<Vertex_Normal> &  stacks){
	Matrix4 base = Matrix4();
	double d_theta = double(double(360) / double(divisions));
	double d_h = double(1) / double(stack_h);
	double y_pos = d_h + -.5;
	Matrix4 rot = base.yrotation(DegreeToRad(d_theta));


	//Starting vector
	Vector4 v0 = Vector4(0.5, -.5, 0, 1);
	double v2_x = CalcG(0)*cos(DegreeToRad(d_theta));
	double v2_z = CalcG(0)*sin(DegreeToRad(d_theta));
	Vector4 v1 = Vector4(v2_x, -.5, v2_z, 1);

	//Calc vector after moving up on cone 
	v2_x = CalcG(Cur_y(y_pos))*cos(DegreeToRad(d_theta));
	v2_z = CalcG(Cur_y(y_pos))*sin(DegreeToRad(d_theta));
	Vector4 v2 = Vector4(v2_x, y_pos, v2_z, 1);
	//Create first triangle
	AddTriangleWithVertexNormal(Vector3(v2[0], v2[1], v2[2]), Vector3(v1[0], v1[1], v1[2]), Vector3(v0[0], v0[1], v0[2]), stacks,2);
	v2_x = CalcG(Cur_y(y_pos))*cos(DegreeToRad(0));
	v2_z = CalcG(Cur_y(y_pos))*sin(DegreeToRad(0));
	Vector4 v3 = Vector4(v2_x, y_pos, v2_z, 1);
	//Create second triangle
	AddTriangleWithVertexNormal(Vector3(v2[0], v2[1], v2[2]), Vector3(v0[0], v0[1], v0[2]), Vector3(v3[0], v3[1], v3[2]), stacks,2);

	//So v3 becomes new v0 and then v2 becomes new v1


	int count = 1;
	int h_count = 0;
	Vector4 prev0 = v0;
	Vector4 prev1 = v1;
	Vector4 prev2 = v2;
	Vector4 prev3 = v3;
	Matrix4 height;
	while (h_count < stack_h){
		while (count < divisions){
			Vector4 t0 = rot*prev0;
			Vector4 t1 = rot*prev1;
			Vector4 t2 = rot*prev2;
			Vector4 t3 = rot*prev3;
			if (h_count == stack_h - 1){
				AddTriangleWithVertexNormal(Vector3(t2[0], t2[1], t2[2]), Vector3(t1[0], t1[1], t1[2]), Vector3(t0[0], t0[1], t0[2]), stacks, 5);
			}
			if (h_count != stack_h-1){
				AddTriangleWithVertexNormal(Vector3(t2[0], t2[1], t2[2]), Vector3(t1[0], t1[1], t1[2]), Vector3(t0[0], t0[1], t0[2]), stacks, 2);
				AddTriangleWithVertexNormal(Vector3(t2[0], t2[1], t2[2]), Vector3(t0[0], t0[1], t0[2]), Vector3(t3[0], t3[1], t3[2]), stacks, 2);
			}
			count++;
			prev0 = t0;
			prev1 = t1;
			prev2 = t2;
			prev3 = t3;
		}
		prev0 = prev3;
		prev1 = prev2;
		count = 0;
		y_pos += d_h;
		v2_x = CalcG(Cur_y(y_pos))*cos(DegreeToRad(d_theta));
		v2_z = CalcG(Cur_y(y_pos))*sin(DegreeToRad(d_theta));
		prev2 = Vector4(v2_x, y_pos, v2_z, 1);
		v2_x = CalcG(Cur_y(y_pos))*cos(DegreeToRad(0));
		v2_z = CalcG(Cur_y(y_pos))*sin(DegreeToRad(0));
		prev3 = Vector4(v2_x, y_pos, v2_z, 1);
		h_count++;	
	}
}

double Cone::CalcG(double d){
	double top = double(double(1.0) - double(d));
	double t_r = double(.5)*top;
	return t_r;
}

double Cone::Cur_y(double num){
	return double(1.0) - (double(.5) - num);
}