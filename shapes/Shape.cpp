#include "Shapes.h"
#include "Square.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Torus.h"
#include "Parser.h"

Shape::Shape(){
	
}



double Shape::DegreeToRad(double deg){
	return deg*M_PI_180;
}

void Shape::Set(ShapesUI::ShapeType type, int param1, int param2){
	cur_t = type;
	if (type == ShapesUI::SHAPE_BUNNY && !bunny){
		faces.clear();
		Parser a("bunny_low.m",faces);
		bunny = true;
	}
	if (type == ShapesUI::SHAPE_DING && !ding){
		faces.clear();
		Parser a("ding_hi.m", faces);
		ding = true;
	}
	if (type == ShapesUI::SHAPE_BUNNY && bunny){
		return;
	}
	if (type == ShapesUI::SHAPE_DING && ding){
		return;
	}
	bunny = ding = false;
	if (prev_visit == false){
		std::vector<Vertex_Normal> temp;
		Sphere a(temp);
		sphere_storage[1] = temp;
		prev_spheres.push_back(1);
		temp.clear();

		Sphere(sphere_storage[1], 5 - 1, temp);
		sphere_storage[5] = temp;
		prev_spheres.push_back(5);
		temp.clear();

		DrawCircle(100, temp);
		Circle_store[100] = temp;

		temp.clear();
		DrawCircle(90, temp);
		Circle_store[90] = temp;

		temp.clear();
		DrawCircle(80, temp);
		Circle_store[80] = temp;


		prev_visit = true;
		
	}
	if ((type == ShapesUI::SHAPE_CONE || type == ShapesUI::SHAPE_CYLINDER || type == ShapesUI::SHAPE_DIAMOND) && prev_t != cur_t){
		prev_t = cur_t;
		prev_h = -1;
		stacks.clear();
	}	
	if (type == ShapesUI::SHAPE_CUBE){
		s_iter = Squares.find(param1);
		if (s_iter == Squares.end()){
			std::vector<std::vector<Face_Normal>> temp;
			Square s(temp, param1);
			Squares[param1] = temp;
			square = Squares[param1];
		}
		else{
			square = Squares[param1];
		}
	}
	if (type == ShapesUI::SHAPE_CYLINDER){
		if (param1 < 3){
			param1 = 3;
		}
		if (prev_h != param2){
			stacks.clear();
			Cylinder(param1 ,param2, stacks);
			prev_h = param2;
		}
		if (prev_cd != param1){
			prev_cd = param1;
			cs_iter = Circle_store.find(param1);
			if (cs_iter == Circle_store.end()){
				std::vector<Vertex_Normal> temp;
				DrawCircle(param1, temp);
				Circle_store[param1] = temp;
				circles = Circle_store[param1];
				stacks.clear();
				Cylinder(param1, param2, stacks);
			}
			else{
				circles = Circle_store[param1];
				stacks.clear();
				Cylinder(param1, param2, stacks);
			}
		}
	}
	if (type == ShapesUI::SHAPE_CONE || type == ShapesUI::SHAPE_DIAMOND){
		if (param1 < 3){
			param1 = 3;
		}
		if (prev_h != param2){
			stacks.clear();
			Cone(param1, param2, stacks);
			prev_h = param2;
		}
		if (prev_cd != param1){
			prev_cd = param1;
			cs_iter = Circle_store.find(param1);
			if (cs_iter == Circle_store.end()){
				std::vector<Vertex_Normal> temp;
				DrawCircle(param1, temp);
				Circle_store[param1] = temp;
				circles = Circle_store[param1];
				stacks.clear();
				Cone(param1, param2, stacks);
			}
			else{
				circles = Circle_store[param1];
				stacks.clear();
				Cone(param1, param2, stacks);
			}
		}
	}
	if (type == ShapesUI::SHAPE_SPHERE){
		//Above 10 will crash the program. 
		if (param1 >=10){
			param1 = 5;
		}
		sphere_iter = sphere_storage.find(param1);
		if (sphere_iter == sphere_storage.end()){
			int closest = getClosestSphere(param1);
			std::vector<Vertex_Normal> temp;
			Sphere(sphere_storage[closest], param1 - closest,temp);
			sphere_storage[param1] = temp;
			Shpere_Sections = sphere_storage[param1];
			prev_spheres.push_back(param1);
		}
		else{
			Shpere_Sections = sphere_storage[param1];
		}
	}
	if (type == ShapesUI::SHAPE_TORUS){
		torus_vec.clear();
		torus_pts.clear();
		Torus(param1,param2,torus_vec,torus_pts);
	}
}

void Shape::Draw(){
	if (cur_t == ShapesUI::SHAPE_CUBE){
		Matrix4 base = Matrix4();
		Matrix4 vert = Matrix4();
		Matrix4 back = Matrix4();
		Matrix4 right = Matrix4();

		vert = base.translation(Vector3(0,1,0));
		back = base.translation(Vector3(-1,0,0));
		right = base.translation(Vector3(0,0,-1));
		for (int i = 0; i < int(square.size()) ;i++){	
			for (int j = 0; j < int(square[i].size()); j++){
				if (square[i][j].ID == 0){
					//Bottom Panel
					glBegin(GL_TRIANGLES);
						glNormal3d(0, -1 , 0);
						glVertex3d(square[i][j].points[0][0], square[i][j].points[0][1], square[i][j].points[0][2]);
						glVertex3d(square[i][j].points[1][0], square[i][j].points[1][1], square[i][j].points[1][2]);
						glVertex3d(square[i][j].points[2][0], square[i][j].points[2][1], square[i][j].points[2][2]);
					glEnd();

					//Top Panel
					Vector4 entry_0 = vert*square[i][j].points[0];
					Vector4 entry_1 = vert*square[i][j].points[1];
					Vector4 entry_2 = vert*square[i][j].points[2];
					
					//Have to draw in reverse order
					glBegin(GL_TRIANGLES);
						glNormal3d(0, 1, 0);
						glVertex3d(entry_2[0], entry_2[1], entry_2[2]);
						glVertex3d(entry_1[0], entry_1[1], entry_1[2]);
						glVertex3d(entry_0[0], entry_0[1], entry_0[2]);
					glEnd();
					continue;
				}
				if (square[i][j].ID == 1){
					//Front Panel
					glBegin(GL_TRIANGLES);
						glNormal3d(square[i][j].normal[0], square[i][j].normal[1], square[i][j].normal[2]);
						glVertex3d(square[i][j].points[0][0], square[i][j].points[0][1], square[i][j].points[0][2]);
						glVertex3d(square[i][j].points[1][0], square[i][j].points[1][1], square[i][j].points[1][2]);
						glVertex3d(square[i][j].points[2][0], square[i][j].points[2][1], square[i][j].points[2][2]);
					glEnd();

					//Back Panel
					Vector4 entry_0 = back*square[i][j].points[0];
					Vector4 entry_1 = back*square[i][j].points[1];
					Vector4 entry_2 = back*square[i][j].points[2];

					//Have to draw in reverse order
					glBegin(GL_TRIANGLES);
						glNormal3d(square[i][j].normal[0]*-1, square[i][j].normal[1], square[i][j].normal[2]);
						glVertex3d(entry_2[0], entry_2[1], entry_2[2]);
						glVertex3d(entry_1[0], entry_1[1], entry_1[2]);
						glVertex3d(entry_0[0], entry_0[1], entry_0[2]);
					glEnd();
					continue;
				}	
				if (square[i][j].ID == 2){
					//Front Panel
					glBegin(GL_TRIANGLES);
						glNormal3d(square[i][j].normal[0], square[i][j].normal[1], square[i][j].normal[2]);
						glVertex3d(square[i][j].points[0][0], square[i][j].points[0][1], square[i][j].points[0][2]);
						glVertex3d(square[i][j].points[1][0], square[i][j].points[1][1], square[i][j].points[1][2]);
						glVertex3d(square[i][j].points[2][0], square[i][j].points[2][1], square[i][j].points[2][2]);
					glEnd();

					//Back Panel
					Vector4 entry_0 = right*square[i][j].points[0];
					Vector4 entry_1 = right*square[i][j].points[1];
					Vector4 entry_2 = right*square[i][j].points[2];

					//Have to draw in reverse order
					glBegin(GL_TRIANGLES);
						glNormal3d(square[i][j].normal[0], square[i][j].normal[1], square[i][j].normal[2]*-1);
						glVertex3d(entry_2[0], entry_2[1], entry_2[2]);
						glVertex3d(entry_1[0], entry_1[1], entry_1[2]);
						glVertex3d(entry_0[0], entry_0[1], entry_0[2]);
					glEnd();
					continue;
				}
			}
		}
	}
	if (cur_t == ShapesUI::SHAPE_CYLINDER || cur_t == ShapesUI::SHAPE_CONE || cur_t == ShapesUI::SHAPE_DIAMOND){
		Matrix4 base = Matrix4();
		Matrix4 vert = base.translation(Vector3(0, 1, 0));
		for (int i = 0; i < int(circles.size()); i++){
			//Bottom 
			if (cur_t == ShapesUI::SHAPE_CYLINDER || cur_t == ShapesUI::SHAPE_CONE){
				glBegin(GL_TRIANGLES);
				glNormal3d(circles[i].normal[0][0], circles[i].normal[0][1], circles[i].normal[0][2]);
				glVertex3d(circles[i].points[0][0], circles[i].points[0][1], circles[i].points[0][2]);


				glNormal3d(circles[i].normal[1][0], circles[i].normal[1][1], circles[i].normal[1][2]);
				glVertex3d(circles[i].points[1][0], circles[i].points[1][1], circles[i].points[1][2]);


				glNormal3d(circles[i].normal[2][0], circles[i].normal[2][1], circles[i].normal[2][2]);
				glVertex3d(circles[i].points[2][0], circles[i].points[2][1], circles[i].points[2][2]);
				glEnd();
			}


			//Top
			if (cur_t == ShapesUI::SHAPE_CYLINDER){
				//Top HAVE TO DRAW IN REVERSE ORDER
				Vector4 entry_0 = vert*circles[i].points[0];
				Vector4 entry_1 = vert*circles[i].points[1];
				Vector4 entry_2 = vert*circles[i].points[2];


				glBegin(GL_TRIANGLES);
				glNormal3d(circles[i].normal[0][0], circles[i].normal[0][1] * -1, circles[i].normal[0][2]);
				glVertex3d(entry_2[0], entry_2[1], entry_2[2]);


				glNormal3d(circles[i].normal[1][0], circles[i].normal[1][1] * -1, circles[i].normal[1][2]);
				glVertex3d(entry_1[0], entry_1[1], entry_1[2]);


				glNormal3d(circles[i].normal[2][0], circles[i].normal[2][1] * -1, circles[i].normal[2][2]);
				glVertex3d(entry_0[0], entry_0[1], entry_0[2]);
				glEnd();
			}
		}
		if (cur_t == ShapesUI::SHAPE_CYLINDER || cur_t == ShapesUI::SHAPE_CONE){
			for (int i = 0; i < int(stacks.size()); i++){
				glBegin(GL_TRIANGLES);
				glNormal3d(stacks[i].normal[0][0], stacks[i].normal[0][1], stacks[i].normal[0][2]);
				glVertex3d(stacks[i].points[0][0], stacks[i].points[0][1], stacks[i].points[0][2]);


				glNormal3d(stacks[i].normal[1][0], stacks[i].normal[1][1], stacks[i].normal[1][2]);
				glVertex3d(stacks[i].points[1][0], stacks[i].points[1][1], stacks[i].points[1][2]);


				glNormal3d(stacks[i].normal[2][0], stacks[i].normal[2][1], stacks[i].normal[2][2]);
				glVertex3d(stacks[i].points[2][0], stacks[i].points[2][1], stacks[i].points[2][2]);
				glEnd();
			}
		}
		else{
			for (int i = 0; i < int(stacks.size()); i++){
				Matrix4 shift_up = Matrix4::translation(Point3(0,.5,0));
				Matrix4 scale = Matrix4::scaling(1,.5,1);


				/*Top of the Diamond*/
				Vector3 norm_0;
				Vector3 norm_1;
				Vector3 norm_2;

				Vector4 temp_0 = scale*shift_up*stacks[i].points[0];
				if (stacks[i].top){
					norm_0 = CalcNormal(Vector3(temp_0[0], temp_0[1], temp_0[2]), 5);
				}
				else{
					norm_0 = CalcNormal(Vector3(temp_0[0],temp_0[1],temp_0[2]),2);
				}
				
				Vector4 temp_1 = scale*shift_up*stacks[i].points[1];
				if (stacks[i].top){
					norm_1 = CalcNormal(Vector3(temp_1[0], temp_1[1], temp_1[2]), 5);
				}
				else{
					norm_1 = CalcNormal(Vector3(temp_1[0], temp_1[1], temp_1[2]), 2);
				}

				Vector4 temp_2 = scale*shift_up*stacks[i].points[2];
				if (stacks[i].top){
					norm_2 = CalcNormal(Vector3(temp_2[0], temp_2[1], temp_2[2]), 5);
				}
				else{
					norm_2 = CalcNormal(Vector3(temp_2[0], temp_2[1], temp_2[2]), 2);
				}


				

				glBegin(GL_TRIANGLES);
					glNormal3d(norm_0[0], norm_0[1], norm_0[2]);
					glVertex3d(temp_0[0], temp_0[1], temp_0[2]);

					glNormal3d(norm_1[0], norm_1[1], norm_1[2]);
					glVertex3d(temp_1[0], temp_1[1], temp_1[2]);

					glNormal3d(norm_2[0], norm_2[1], norm_2[2]);
					glVertex3d(temp_2[0], temp_2[1], temp_2[2]);

										
					glNormal3d(norm_0[0], -norm_0[1], norm_0[2]);
					glVertex3d(temp_2[0], -temp_2[1], temp_2[2]);

					glNormal3d(norm_1[0], -norm_1[1], norm_1[2]);
					glVertex3d(temp_1[0], -temp_1[1], temp_1[2]);

					glNormal3d(norm_2[0], -norm_2[1], norm_2[2]);
					glVertex3d(temp_0[0], -temp_0[1], temp_0[2]);

				
				glEnd();
			}	
		}
	}
	if (cur_t == ShapesUI::SHAPE_SPHERE){
		for (int i = 0; i < int(Shpere_Sections.size()); i++){
				glBegin(GL_TRIANGLES);
					glNormal3d(Shpere_Sections[i].normal[0][0], Shpere_Sections[i].normal[0][1], Shpere_Sections[i].normal[0][2]);
					glVertex3d(Shpere_Sections[i].points[0][0], Shpere_Sections[i].points[0][1], Shpere_Sections[i].points[0][2]);


					glNormal3d(Shpere_Sections[i].normal[1][0], Shpere_Sections[i].normal[1][1], Shpere_Sections[i].normal[1][2]);
					glVertex3d(Shpere_Sections[i].points[1][0], Shpere_Sections[i].points[1][1], Shpere_Sections[i].points[1][2]);


					glNormal3d(Shpere_Sections[i].normal[2][0], Shpere_Sections[i].normal[2][1], Shpere_Sections[i].normal[2][2]);
					glVertex3d(Shpere_Sections[i].points[2][0], Shpere_Sections[i].points[2][1], Shpere_Sections[i].points[2][2]);
				glEnd();
		}
	}
	if (cur_t == ShapesUI::SHAPE_TORUS){
		/*for (int i = 0; i < int(torus_pts.size()); i++){
			glBegin(GL_POINTS);
				glVertex3d(torus_pts[i].points[0][0], torus_pts[i].points[0][1],torus_pts[i].points[0][2]);
				glVertex3d(torus_pts[i].points[1][0], torus_pts[i].points[1][1], torus_pts[i].points[1][2]);
			glEnd();
		}*/
		for (int i = 0; i < int(torus_vec.size()); i++){
			glBegin(GL_TRIANGLES);

			glNormal3d(torus_vec[i].normal[0][0], torus_vec[i].normal[0][1], torus_vec[i].normal[0][2]);
			glVertex3d(torus_vec[i].points[0][0], torus_vec[i].points[0][1], torus_vec[i].points[0][2]);


			glNormal3d(torus_vec[i].normal[1][0], torus_vec[i].normal[1][1], torus_vec[i].normal[1][2]);
			glVertex3d(torus_vec[i].points[1][0], torus_vec[i].points[1][1], torus_vec[i].points[1][2]);


			glNormal3d(torus_vec[i].normal[2][0], torus_vec[i].normal[2][1], torus_vec[i].normal[2][2]);
			glVertex3d(torus_vec[i].points[2][0], torus_vec[i].points[2][1], torus_vec[i].points[2][2]);
			glEnd();
		}
	}
	if (cur_t == ShapesUI::SHAPE_BUNNY || cur_t == ShapesUI::SHAPE_DING){
		for (int i = 0; i < int(faces.size()); i++){
			//std::cout << i << std::endl;
			//faces[i].print();
			glBegin(GL_TRIANGLES);
				
				glNormal3d(faces[i].normal[0], faces[i].normal[1], faces[i].normal[2]);
				glVertex3d(faces[i].points[0][0], faces[i].points[0][1], faces[i].points[0][2]);
			
				glVertex3d(faces[i].points[1][0], faces[i].points[1][1], faces[i].points[1][2]);

				glVertex3d(faces[i].points[2][0], faces[i].points[2][1], faces[i].points[2][2]);
			glEnd();
		}
	}
}
//Smooth all three

Vector3 Shape::CalcNormal(Vector3 cord_0, int case_num){
	switch (case_num){

	//Cylinder
	case 1:
		return Vector3(cord_0[0], 0, cord_0[2]);
	
	//Cone
	case 2:
		return Vector3(cord_0[0], double(.5)*sqrt(pow(cord_0[0],2) + pow(cord_0[2],2)), cord_0[2]);

	//Shpere
	case 3: 
		return Vector3(cord_0[0],cord_0[1],cord_0[2]);

	//Circle
	case 4:
		return Vector3(0, -1, 0);

	//Cone top
	case 5:
		return Vector3(cord_0[0], double(.5)*sqrt(pow(cord_0[0], 2) + pow(cord_0[2], 2)), cord_0[2]);
	default:
		break;
	}
}

void Shape::AddTriangleWithVertexNormal(Vector3 cord_0, Vector3 cord_1, Vector3 cord_2, std::vector<Vertex_Normal> & vec, int case_num){

	Vector3 cord_0_norm = CalcNormal(cord_0, case_num);
	Vector3 cord_1_norm = CalcNormal(cord_1, case_num);
	Vector3 cord_2_norm = CalcNormal(cord_2, case_num);
	 
	Vector4 v0 = Vector4(cord_0[0], cord_0[1],cord_0[2] , 1);
	Vector4 v1 = Vector4(cord_1[0], cord_1[1], cord_1[2], 1);
	Vector4 v2 = Vector4(cord_2[0], cord_2[1], cord_2[2], 1);
	
	if (case_num == 5){
		cord_0_norm = Vector3(0,1,0);
	}
	Vertex_Normal triangle(v0,cord_0_norm,v1,cord_1_norm,v2,cord_2_norm);
	if (case_num == 5){
		triangle.top = true;
	}
	//triangle.print();
	vec.push_back(triangle);
}

void Shape::DrawCircle(int divisions, std::vector<Vertex_Normal> &  circles){
	Matrix4 base = Matrix4();
	double d_theta = double(double(360) / double(divisions));
	Matrix4 rot = base.yrotation(DegreeToRad(d_theta));


	Vector4 v0 = Vector4(0, -.5, 0, 1);
	Vector4 v1 = Vector4(0.5, -.5, 0, 1);
	double v2_x = double(.5)*cos(DegreeToRad(d_theta));
	double v2_z = double(.5)*sin(DegreeToRad(d_theta));
	Vector4 v2 = Vector4(v2_x, -.5, v2_z, 1);
	AddTriangleWithVertexNormal(Vector3(v0[0], v0[1], v0[2]), Vector3(v1[0], v1[1], v1[2]), Vector3(v2[0], v2[1], v2[2]), circles,4);

	int count = 1;
	Vector4 prev0 = v0;
	Vector4 prev1 = v1;
	Vector4 prev2 = v2;

	while (count < divisions){
		Vector4 t0 = rot*prev0;
		Vector4 t1 = rot*prev1;
		Vector4 t2 = rot*prev2;
		AddTriangleWithVertexNormal(Vector3(t0[0], t0[1], t0[2]), Vector3(t1[0], t1[1], t1[2]), Vector3(t2[0], t2[1], t2[2]), circles, 4);
		count++;
		prev0 = t0;
		prev1 = t1;
		prev2 = t2;
	}
}

int Shape::getClosestSphere(int value){
	int ret = -1;
	for (int i = 0; i < int(prev_spheres.size()); i++){
		if (prev_spheres[i] > ret && prev_spheres[i] < value){
			ret = prev_spheres[i];
		}
	}
	return ret;
}

Vector3 Shape::Torus_CalcNormal(double small_A, double big){
	Vector3 big_tangent = Vector3(-sin(big), 0, cos(big));
	Vector3 little_tangent = Vector3(cos(big)*(-sin(small_A)), cos(small_A), sin(big)*(-sin(small_A)));
	Vector3 ret = big_tangent ^ little_tangent;
	//ret.normalize();
	//Vector3 ret = Vector3(.5*cos(big)*cos(small_A), .5*cos(big)*sin(small_A), .5*sin(small_A));
	ret.normalize();
	//ret.print();
	return -1*ret;
}


void Shape::AddTriangleWithVertexNormal(Vector3 cord_0, Vector3 cord_1, Vector3 cord_2, std::vector<Vertex_Normal> & vec, double v0_angle, double v1_angle, double v2_angle, double v0_big_theta, double v1_big_theta, double v2_big_theta){

	Vector3 cord_0_norm = Torus_CalcNormal(v0_angle, v0_big_theta);
	Vector3 cord_1_norm = Torus_CalcNormal(v1_angle, v1_big_theta);
	Vector3 cord_2_norm = Torus_CalcNormal(v2_angle, v2_big_theta);

	Vector4 v0 = Vector4(cord_0[0], cord_0[1], cord_0[2], 0);
	Vector4 v1 = Vector4(cord_1[0], cord_1[1], cord_1[2], 0);
	Vector4 v2 = Vector4(cord_2[0], cord_2[1], cord_2[2], 0);


	Vertex_Normal triangle(v0, cord_0_norm, v1, cord_1_norm, v2, cord_2_norm);
	//triangle.print();
	vec.push_back(triangle);
}



bool Shape::IsZero(double num){
	if (num == 0){
		return true;
	}
	return false;
}

bool Shape::IsZero(Vector3 vec){
	if (vec[0] == 0 && vec[1] == 0 && vec[2] == 0){
		return true;
	}
	return false;
}

bool Shape::IsZero(Point3 pt){
	if (pt[0] == 0 && pt[1] == 0 && pt[2] == 0){
		return true;
	}
	return false;
}

Vector3 Shape::Vector_Squared(const Vector3 & vec){
	Vector3 ret = Vector3(vec[0]*vec[0],vec[1]*vec[1],vec[2]*vec[2]);
	return ret;
}

Point3 Shape::Point_Squared(const Point3 & pt){
	Point3 ret = Point3(pt[0]*pt[0],pt[1]*pt[1],pt[2]*pt[2]);
	return ret;
}


Vector3	Vec_times_Point(const Vector3 & lhs, const Point3 & rhs){
	Vector3 ret = Vector3(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);;
	return ret;
}

void Shape::Intersect(HitRecord & hr, Point3 Origin, Vector3 d){
	for (int i = 0; i < int(faces.size()); i++){
		Vector3 v1 = Vector3(faces[i].points[0][0], faces[i].points[0][1], faces[i].points[0][2]);
		Vector3 v2 = Vector3(faces[i].points[1][0], faces[i].points[1][1], faces[i].points[1][2]);
		Vector3 v3 = Vector3(faces[i].points[2][0], faces[i].points[2][1], faces[i].points[2][2]);
		
		Vector3 edge_1 = v2 - v1;
		Vector3 edge_2 = v3 - v1;
		
		double det, inv_Det, u, v;
		double t;
		Vector3 P, Q, T;

		P = d^edge_2;
		det = edge_1*P;

		if (det > -EPSILON && det < EPSILON){
			continue;
		}

		inv_Det = double(double(1) / det);
		T = Vector3(Origin[0], Origin[1], Origin[2]) - v1;
	
		u = T*P*inv_Det;

		if (u < 0 || u > 1){
			continue;
		}

		Q = T^edge_1;
		v = (d*Q)*inv_Det;

		if (v < 0 || u +  v > 1){
			continue;
		}

		t = (edge_2*Q)*inv_Det;
		
		if (t > EPSILON){
			Vector3 pt = Vector3(Origin[0], Origin[1], Origin[2]) + t*d;
			hr.addHit(t, 0, 0, Point3(pt[0],pt[1],pt[2]), faces[i].normal);
		}
	}
}


bool Shape::LiesOnTriangle(const Vector3 & P, const Vector3 & A, const Vector3 & B, const Vector3 & C){
	Vector3 v0, v1, v2;
	double alpha, beta, gamma;


	v0 = B - A;
	v1 = C - A;
	v2 = P - A;
	
	float dot_00 = float(v0*v0);
	float dot_01 = float(v0*v1);
	float dot_11 = float(v1*v1);
	float dot_20 = float(v2*v0);
	float dot_21 = float(v2*v1);

	float bot = dot_00*dot_11 - dot_01*dot_01;
	
	alpha = (dot_11*dot_20 - dot_01*dot_21) / bot;
	beta = (dot_00*dot_21 - dot_01*dot_20) / bot;
	gamma = float(1) - alpha - beta;
	if ((0 >= alpha && alpha <= 1) && (0 >= beta && beta <= 1) && (0 >= gamma && gamma <= 1)){
		return true;
	}
	if (alpha == 0 && (0 >= beta && beta <= 1) && (0 >= gamma && gamma <= 1)){
		return true;
	}
	if (beta == 0 && (0 >= alpha && alpha <= 1) && (0 >= gamma && gamma <= 1)){
		return true;
	}
	if (gamma == 0 && (0 >= alpha && alpha <= 1) && (0 >= beta && beta <= 1)){
		return true;
	}
	if (alpha == 1 && beta == 0 && gamma ==0){
		return true;
	}
	if (beta == 1 && alpha == 0 && gamma == 0){
		return true;
	}
	if (gamma == 1 && beta == 0 && alpha == 0){
		return true;
	}
	return false;
}
