#ifndef _SUBGRAPH_H
#define _SUBGRAPH_H

#include "../cse452.h"
#include "../vecmath/Matrix4.h"
#include <vector>
#include <string>
#include <iostream>
#include"../Shapes/Shapes.h"
#include <fstream>

class Object;
class Node;
class Tree;
class Trans;
class ShapeContainer;


class Node{
	virtual void getName() = 0;
};

//Top level 
class Tree : public Node{
public:
	Tree();
	Tree(std::string n);
	~Tree();

	void getName() override;
	void addTrans(Trans * t);

	std::vector<Trans *> const & getChildList();

	void printTree();

private:
	std::string name;
	std::vector<Trans *> children;

};

//Mid Level
class Trans : public Node{
public:
	Trans();
	~Trans();
	void getName() override;
	void UpdateMatrix(Matrix4 & m_in);
	void AddObjectKid(Object * o);
	void AddSubGraphKid(Tree * t);
	Matrix4 getMatrix();
	void PrintInfo();
	boolean NoSubGraphs();
	Tree * getTree();
	Object * getObject();

private:
	Matrix4 m;
	std::vector<Object *> ObjectKids;
	std::vector<Tree *> SubGraphKids;

};

//Very Bottom
class Object : public Node{
public:
	Object();
	~Object(){delete s;};
	void getName() override;

	//Diffuse getter and setter
	inline void setDiffuse(Color & c){ diffuse = c; };
	inline Color const & getDiffuse(){ return diffuse; };

	//Ambient 
	inline void setAmbient(Color & c){ ambient = c;};
	inline Color const & getAmbient(){ return ambient; };

	//Reflect
	inline void setReflect(Color & c){ reflect = c; };
	inline Color const & getReflect(){ return reflect; };

	//Specular
	inline void setSpecular(Color & c){ specular = c; };
	inline Color const & getSpecular(){ return specular; };

	//Transparent
	inline void setTransparent(Color & c){ transparent = c; };
	inline Color const & getTransparent(){ return transparent; };

	//Emit
	inline void setEmit(Color & c){ emit = c;};
	inline Color const & getEmit(){ return emit; };

	//Texture Map
	inline void setTextureFile(std::string & fileName){ filename = fileName; };
	inline void setU(double & u_v) { u = u_v; }
	inline void setV(double & v_v) { v = v_v; }
	inline std::string const & getFileName(){ return filename; };
	inline double getUValue(){ return u; };
	inline double getVVale() { return v; }

	inline void setShape(Shape * shape){ s = shape; }

	inline void setShine(double s){ shine = s; }
	inline double getShine(){ return shine; }
	inline void setIOR(double num){ ior = num; }

	inline void setName(std::string n){ object_name = n; }
	
	inline Shape * getShape(){ return s; };
	inline void SetShapeNum(int num){ shape_num = num; };
	inline int getShapeNum(){ return shape_num; };

	void printInfo();

	void drawObject();


	inline void setTest(Color & c){ test = c; };

private:

	Color diffuse;
	Color ambient;
	Color reflect;
	Color specular;
	Color emit;
	Color transparent;

	double index, shine, ior;

	int shape_num;

	std::string filename, object_name;

	//IDK what these are 
	double u, v;

	Shape * s;

	GLuint idx;
	Color test;
};

class ShapeContainer{
public:
	ShapeContainer(){	
		//Setting up Cube
		Cube = new Shape();
		Cube->Set(ShapesUI::SHAPE_CUBE, 20, 20);
		

		//Setting up Cylinder
		Cylinder = new Shape();
		Cylinder->Set(ShapesUI::SHAPE_CYLINDER, 60, 50);

		//Setting up Cone
		Cone = new Shape();
		Cone->Set(ShapesUI::SHAPE_CONE, 60, 50);

		//Setting up Sphere
		Sphere = new Shape();
		Sphere->Set(ShapesUI::SHAPE_SPHERE, 50, 50);	


		//Setting up Fish
		Fish = new Shape();
		Fish->Set(ShapesUI::SHAPE_FISH, 50, 50);

		//Setting up Bowl
		Bowl = new Shape();
		Bowl->Set(ShapesUI::SHAPE_BOWL, 50, 50);

	}

	~ShapeContainer(){
		delete Cube;
		delete Cone;
		delete Cylinder;
		delete Sphere;
		delete Fish;
		delete Bowl;
	}

	inline Shape * const getCube(){ return Cube; }

	inline Shape * const getCone(){ return Cone; }

	inline Shape * const getCylinder(){ return Cylinder; }

	inline Shape * const getSphere(){ return Sphere; }

	inline Shape * const getFish(){ return Fish; }

	inline Shape * const getBowl(){ return Bowl; }



private:
	Shape * Cube;
	Shape * Cone;
	Shape * Cylinder;
	Shape * Sphere;
	Shape * Fish;
	Shape * Bowl;
};


struct DrawObject{
	DrawObject(Matrix4 & in_m, Object * in_o){
		o = in_o;
		m = in_m;
		inverse_m = in_m.inverse();
	}
	Object * o = nullptr;
	Matrix4 m;
	Matrix4 inverse_m;
	void Print(){
		std::cout << "Matrix" << std::endl;
		m.print();
		std::cout << "Object" << std::endl;
		o->getName();
	}
};
#endif

