#ifndef SQUARE_H
#define SQUARE_H

#include "Shapes.h"

class Square: public Shape{
public:
	Square();
	Square(std::vector<std::vector<Face_Normal>> & square, int scale);
	void Intersect(HitRecord & hr, Point3 P, Vector3 d) override;
};

#endif