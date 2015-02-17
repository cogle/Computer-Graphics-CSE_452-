#ifndef SPHERE_H
#define SPHERE_H




#include "Shapes.h"

class Sphere : public Shape{
public:
	Sphere();
	Sphere(std::vector<Vertex_Normal> & sphere);
	Sphere(std::vector<Vertex_Normal> & cur, int numDiv,  std::vector<Vertex_Normal> & ret);
private:
	void subDivide(Vector4 v0, Vector4 v1, Vector4 v , int numDiv, std::vector<Vertex_Normal> & vec);
	

	/*From Wikipedia 
	The following Cartesian coordinates define the vertices of an icosahedron with edge-length 2, centered at the origin:
		(0, ±1, ±φ)
		(±1, ±φ, 0)
		(±φ, 0, ±1) 

		Thus I multiplied the coordinates by to get r = 1,


		//This gives a number that goes into the Cartesian Coordinates for an icosahedron. 
		.5*(1/(2sin((2*PI)/5)))
	*/
	const double One = double(.525731112119133606) / double(2);	//Divide by 2 for radius .5
	const double Phi = double(.850650808352039932) / double(2);	//Divide by 2 for radius .5
	const double radius = .5;
};

#endif