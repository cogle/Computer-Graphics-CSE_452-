#ifndef DIAMOND_H
#define DIAMOND_H

#include "Shapes.h"

class Diamond : public Shape{
public:
	Diamond();
	void Intersect(HitRecord & hr, Point3 P, Vector3 d) override;
};

#endif