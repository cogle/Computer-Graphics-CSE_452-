#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shapes.h"

class Cylinder : public Shape{
public:
	Cylinder(int divisions,int stacks, std::vector<Vertex_Normal> &  circles);
private:
	void DrawStacks(int divisions,int stack_h, std::vector<Vertex_Normal> &  stacks);
};

#endif