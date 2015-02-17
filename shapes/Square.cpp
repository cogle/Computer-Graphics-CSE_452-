#include "Square.h"
#include "Cylinder.h"

Square::Square(std::vector<std::vector<Face_Normal>> & square, int scale){
	std::vector<Face_Normal> temp;
	const double base = 0.5;
	const double scale_factor = double(1.0 / double(scale));
	const double entry = base*scale_factor;
	//double start_x, start_y, start_z;

	/*IMPORTANT REMINDER*/

	//Normal is associated with the middle Coordinate FROM CALC NORMAL METHOD MUST DRAW WITH THAT MIDDLE COORDINATE FIRST!!
	//DRAW ORDER IS MID OF CALC_NORMAL, FIRST OF CALC_NORMAL, LAST OF CALC_NORMAL

	/*<----------------------------------->*/
	//Bottom of the Square Start points
	double start_bx = -base;
	double start_bz = -base;

	//Normal calc for bottom
	Vector3 b_p0 = Vector3(start_bx, -base, start_bz);
	Vector3 b_p1 = Vector3(start_bx, -base, start_bz + scale_factor);
	Vector3 b_p2 = Vector3(start_bx + scale_factor, -base, start_bz + scale_factor);			//p1 for right triangle
	Vector3 b_p3 = Vector3(start_bx + scale_factor, -base, start_bz);							//p2 for right trianle

	/*<----------------------------------->*/
	//Front and Back of the Square Start points
	double start_fz = -base;
	double start_fy = base;

	//Normal calc for the Front
	Vector3 f_p0 = Vector3(base, start_fy, start_fz);
	Vector3 f_p1 = Vector3(base, start_fy, start_fz + scale_factor);
	Vector3 f_p2 = Vector3(base, start_fy - scale_factor, start_fz + scale_factor);			//p1 for right triangle
	Vector3 f_p3 = Vector3(base, start_fy - scale_factor, start_fz);							//p2 for right trianle


	/*<----------------------------------->*/
	//Left and Right of the Sqaure's Start Points
	double start_lx = base;
	double start_ly = base;

	//Normal Calc for the Front 
	Vector3 l_p0 = Vector3(start_lx, start_ly, base);
	Vector3 l_p1 = Vector3(start_lx - scale_factor, start_ly, base);
	Vector3 l_p2 = Vector3(start_lx - scale_factor, start_ly - scale_factor, base);				//p1 for right triangle
	Vector3 l_p3 = Vector3(start_lx, start_ly - scale_factor, base);							//p2 for right trianle


	for (int z = 0; z < scale; z++){
		for (int x = 0; x < scale; x++){

			//Bottom and Top
			Vector4 b_p0 = Vector4(start_bx, -base, start_bz, 1.0);
			Vector4 b_p1 = Vector4(start_bx, -base, start_bz + scale_factor, 1.0);
			Vector4 b_p2 = Vector4(start_bx + scale_factor, -base, start_bz + scale_factor, 1.0);			//p1 for right triangle
			Vector4 b_p3 = Vector4(start_bx + scale_factor, -base, start_bz, 1.0);							//p2 for right trianle


			Face_Normal b_left = Face_Normal(b_p1, b_p0, b_p2, Vector3(0,1,0), 0);
			Face_Normal b_right = Face_Normal(b_p3, b_p2, b_p0, Vector3(0, 1, 0), 0);

			temp.push_back(b_left);
			temp.push_back(b_right);

			//Front and Back
			Vector4 f_p0 = Vector4(base, start_fy, start_fz, 1.0);
			Vector4 f_p1 = Vector4(base, start_fy, start_fz + scale_factor, 1.0);
			Vector4 f_p2 = Vector4(base, start_fy - scale_factor, start_fz + scale_factor, 1.0);			//p1 for right triangle
			Vector4 f_p3 = Vector4(base, start_fy - scale_factor, start_fz, 1.0);							//p2 for right trianle

			Face_Normal f_left = Face_Normal(f_p0, f_p1, f_p2, Vector3(1, 0 , 0), 1);
			Face_Normal f_right = Face_Normal(f_p0, f_p2, f_p3, Vector3(1, 0, 0), 1);

			temp.push_back(f_left);
			temp.push_back(f_right);

			//Left and Right

			Vector4 l_p0 = Vector4(start_lx, start_ly, base, 1.0);
			Vector4 l_p1 = Vector4(start_lx - scale_factor, start_ly, base, 1.0);
			Vector4 l_p2 = Vector4(start_lx - scale_factor, start_ly - scale_factor, base, 1.0);			//p1 for right triangle
			Vector4 l_p3 = Vector4(start_lx, start_ly - scale_factor, base, 1.0);							//p2 for right trianle



			Face_Normal l_left = Face_Normal(l_p0, l_p1, l_p2, Vector3(0, 0, 1), 2);
			Face_Normal l_right = Face_Normal(l_p3, l_p0, l_p2, Vector3(0, 0, 1), 2);

			temp.push_back(l_left);
			temp.push_back(l_right);

			//Push it onto the whole thing
			square.push_back(temp);

			//Various Increments
			start_bx += scale_factor;
			start_fz += scale_factor;
			start_lx -= scale_factor;
			temp.clear();
		}
		start_bx = -base;
		start_bz += scale_factor;
		start_fz = -base;
		start_fy -= scale_factor;
		start_lx = base;
		start_ly -= scale_factor;
	}
}