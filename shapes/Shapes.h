#ifndef SHAPES_H
#define SHAPES_H

//Inclusion of files that derived classes will likely utilize
#include "cse452.h"
#include <cmath>
#include "vecmath\Matrix4.h"
#include "vecmath\Vector3.h"
#include <vector>
#include "ShapesUI.h"
#include <hash_map>
#include "../intersection/HitRecord.h"

#define EPSILON 0.000001

class Shape{

public:
	Shape();

	struct Face_Normal {
		std::vector<Vector4> points;
		Vector3 normal;
		int ID;
		Face_Normal(Vector4 cord_0, Vector4 cord_1, Vector4 cord_2, Vector3 norm, int ID_NUM){
			points.push_back(cord_0);
			points.push_back(cord_1);
			points.push_back(cord_2);
			normal = norm;
			normal.normalize();
			ID = ID_NUM;
		}
		void print(){
			std::cout << "v0: ";
			points[0].print();
			std::cout << "v1: ";
			points[1].print();
			std::cout << "v2: ";
			points[2].print();
			std::cout << "With normal: ";
			normal.print();
			std::cout << std::endl;
		}
	};

	struct Vertex_Normal {
		std::vector<Vector4> points;
		std::vector<Vector3> normal;

		//Only used for setting normal of Diamond 
		bool top = false;
		Vertex_Normal(Vector4 cord_0, Vector3 cord_0Norm, Vector4 cord_1, Vector3 cord_1Norm, Vector4 cord_2, Vector3 cord_2Norm){
			//Point 0 and its normal
			points.push_back(cord_0);
			normal.push_back(cord_0Norm);

			//Point 1 and its normal
			points.push_back(cord_1);
			normal.push_back(cord_1Norm);

			//Point 2 and its normal
			points.push_back(cord_2);
			normal.push_back(cord_2Norm);
		}
		void print(){
			std::cout << "v0: ";
			points[0].print();
			std::cout << "v1: ";
			points[1].print();
			std::cout << "v2: ";
			points[2].print();
			std::cout << "With normals: " << std::endl;
			normal[0].print();
			normal[1].print();
			normal[2].print();
			std::cout << std::endl << std::endl;
		}
	};


	struct Line_Points {
		std::vector<Vector3> points;
		std::vector<Vector3> normals;
		std::vector<double> angles;
		Line_Points(Vector3 cord_0, Vector3 cord_1, Vector3 norm0, Vector3 norm1, double theta_1, double theta_2){
			points.push_back(cord_0);
			points.push_back(cord_1);
			normals.push_back(norm0);
			normals.push_back(norm1);
			angles.push_back(theta_1);
			angles.push_back(theta_2);
		}
		void print(){
			std::cout << "v0: ";
			points[0].print();
			std::cout << "v1: ";
			points[1].print();
			std::cout << std::endl << std::endl;
		}
	};

	struct Triangle {
		std::vector<Vector4> points;
		Vector3 normal;
		Triangle(Vector4 cord_1, Vector4 cord_2, Vector4 cord_3){
			points.push_back(cord_1);
			points.push_back(cord_2);
			points.push_back(cord_3);
			Vector3 lhs = Vector3(cord_3[0], cord_3[1], cord_3[2]) - Vector3(cord_2[0], cord_2[1], cord_2[2]);
			Vector3 rhs = Vector3(cord_1[0], cord_1[1], cord_1[2]) - Vector3(cord_2[0], cord_2[1], cord_2[2]);
			normal = lhs^rhs;
			normal.normalize();
		}
		void print(){
			std::cout << "v0: ";
			points[0].print();
			std::cout << "v1: ";
			points[1].print();
			std::cout << "v2: ";
			points[2].print();
			std::cout << "With normal: ";
			normal.print();
			std::cout << std::endl;
		}
	};



	//Method read from handout and previous lab troubles inclined me to make this.
	double DegreeToRad(double degs);

	//Overloaded method to draw the triangles based upon how the vertex is determined.
	void DrawTriangles(std::vector<std::vector<Vertex_Normal>> & vec);
	void DrawTriangles(std::vector<std::vector<Face_Normal>> & vec);

	void AddTriangleWithFaceNormal(Vector3 cord_0, Vector3 cord_1, Vector3 cord_2, std::vector<std::vector<Face_Normal>>);
	void AddTriangleWithVertexNormal(Vector3 cord_0, Vector3 cord_1, Vector3 cord_2, std::vector<Vertex_Normal> & vec, int case_num);


	//Interacts with the ShapeUI carries information from that class to this class.
	void Set(ShapesUI::ShapeType type, int param_1, int param_2 = 1);


	//Go counter-clockwise!!!
	Vector3 CalcNormal(Vector3 cord_0, int case_num);
	//Vector4 CalcNormal(Vector4 cord_0, Vector4 cord_1, Vector4 cord_2);


	void Draw();


	bool IsZero(double num);
	bool IsZero(Vector3 vec);
	bool IsZero(Point3 pt);
	bool LiesOnTriangle(const Vector3 & P, const Vector3 & A, const Vector3 & B, const Vector3 & C);

	Vector3 Vector_Squared(const Vector3 & vec);
	Point3 Point_Squared(const Point3 & pt);
	Vector3	Vec_times_Point(const Vector3 & lhs, const Point3 & rhs);

	virtual void Intersect(HitRecord & hr, Point3 P, Vector3 d);
	void getCorrectIntersect(int const & num, HitRecord & hr, Point3 & P, Vector3 & d);

protected:
	std::vector<std::vector<Face_Normal>> square;

	std::vector<Vertex_Normal> circles;
	std::vector<Vertex_Normal> stacks;
	std::vector<Vertex_Normal> Shpere_Sections;
	std::vector<Vertex_Normal> torus_vec;


	std::hash_map<int, std::vector<Vertex_Normal>> Circle_store;
	std::hash_map<int, std::vector<Vertex_Normal>>::iterator cs_iter;

	Vector3 Shape::Torus_CalcNormal(double small_A, double big);
	void AddTriangleWithVertexNormal(Vector3 cord_0, Vector3 cord_1, Vector3 cord_2, std::vector<Vertex_Normal> & vec, double v0_angle, double v1_angle, double v2_angle, double v0_big_theta, double v1_big_theta, double v2_big_theta);


private:
	void DrawCircle(int divisions, std::vector<Vertex_Normal> &  circles);
	int getClosestSphere(int value);


	const double M_PI_180 = double(double(M_PI) / double(180));


	int Square_iter;
	int Square_Max;
	int prev_h = 0;
	int prev_cd = 0;

	bool prev_visit = false;

	ShapesUI::ShapeType cur_t;
	ShapesUI::ShapeType prev_t;


	std::hash_map<int, std::vector<std::vector<Face_Normal>>> Squares;
	std::hash_map<int, std::vector<std::vector<Face_Normal>>>::iterator s_iter;
	std::hash_map<int, std::vector<Vertex_Normal>> sphere_storage;
	std::hash_map<int, std::vector<Vertex_Normal>>::iterator sphere_iter;

	std::vector<int> prev_spheres;


	std::vector<Line_Points> torus_pts;

	bool bunny = false;
	bool ding = false;
	bool fish = false;
	bool bowl = false;
	std::vector<Triangle> faces;
};


#endif