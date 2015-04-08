#ifndef PARSER_H
#define PARSER_H

#include "cse452.h"
#include <cmath>
#include "vecmath\Matrix4.h"
#include "vecmath\Vector3.h"
#include <vector>
#include "Shapes.h"



class Parser_MINE : public Shape{

public:
	Parser_MINE();
	Parser_MINE(std::string file_name, std::vector<Triangle> & f);

private:
	std::vector<Vector4> vertices;
	
};


#endif