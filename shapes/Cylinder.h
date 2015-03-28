#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shapes.h"

class Cylinder : public Shape{
public:
	Cylinder();
	Cylinder(int divisions,int stacks, std::vector<Vertex_Normal> &  circles);
	void Intersect(HitRecord & hr, Point3 P, Vector3 d) override;
private:
	void DrawStacks(int divisions,int stack_h, std::vector<Vertex_Normal> &  stacks);
};

#endif