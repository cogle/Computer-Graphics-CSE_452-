#ifndef CONE_H
#define CONE_H

#include "Shapes.h"

class Cone : public Shape{
public:
	Cone(int divisions, int stacks, std::vector<Vertex_Normal> &  circles);
private:
	void DrawStacks(int divisions, int stack_h, std::vector<Vertex_Normal> &  stacks);
	double CalcG(double d);
	double Cur_y(double y_pos);
};

#endif