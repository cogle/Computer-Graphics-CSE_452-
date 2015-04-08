#include "Parser.h"
#include <sstream>      
#include <iostream>
#include <fstream>

Parser_MINE::Parser_MINE(){
}

Parser_MINE::Parser_MINE(std::string file_name, std::vector<Triangle> & f){
	std::string type;
	int line_num;
	double v0, v1, v2;
	std::ifstream file;
	file.open(file_name);
	std::string line;
	if (file.is_open()){
		while (std::getline(file, line)){
			std::istringstream iss(line);
			iss >> type;
			if (type.compare("Vertex") == 0){
				iss >> line_num >> v0 >> v1 >> v2;
				if (file_name.compare("fish_mid.m") == 0 || file_name.compare("bowl_hi.m") == 0){
					vertices.push_back(Vector4(v0, v1, v2, 1));
				}
				else{
					vertices.push_back(Matrix4::scaling(.055)*Vector4(v0, v1, v2, 1));
				}
			}
			else{
				int v0,v1,v2;
				iss >> line_num >> v0 >> v1 >> v2;
				std::vector<int> pos;
				v0 += -1;
				v1 += -1;
				v2 += -1;
				if (file_name.compare("bowl_hi.m") == 0 || file_name.compare("fish_mid.m") == 0){
					f.push_back(Triangle(vertices[v0], vertices[v1], vertices[v2]));
				}
				else{
					f.push_back(Triangle(vertices[v2], vertices[v1], vertices[v0]));
				}
				v1 = v2;
				while (iss >> v2){
					v2 += -1;
					f.push_back(Triangle(vertices[v0], vertices[v2], vertices[v1]));
					v1 = v2;
				}
			}
		}
	}
	else std::cout << "Unable to open file";

	file.close();
}

