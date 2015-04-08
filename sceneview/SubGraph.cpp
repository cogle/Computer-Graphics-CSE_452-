#include "SubGraph.h"

/*TREE SECTION*/

Tree::Tree(std::string n){
	name = n;
}

Tree::~Tree(){
	if (int(children.size()) > 0){
		for (int i = 0; i < int(children.size()); i++){
			delete children[i];
		}
	}
	children.clear();
}

void Tree::getName(){
	std::cout << "Encountered master subgraph " << name << std::endl;
}

void Tree::addTrans(Trans * t){
	children.push_back(t);
}

void Tree::printTree(){
	std::cout << "Printing the Family Tree for the following tree: " << name << std::endl;
	std::cout << "I Contain " << children.size() << " Transformations" << std::endl;
	for (int i = 0; i < int(children.size()); i++){
		Trans * temp = children[i];
		std::cout << "Transformation Number: " << i + 1 << std::endl;
		temp->PrintInfo();
		std::cout << std::endl;
	}
}


std::vector<Trans *> const & Tree::getChildList(){
	return children;
}
/*TRANS SECTION*/

Trans::Trans(){
	m = Matrix4::identity();
}

Trans::~Trans(){
	if (int(ObjectKids.size()) > 0){
		for (int i = 0; i < int(ObjectKids.size()); i++){
			delete ObjectKids[i];
		}
	}
	if (int(SubGraphKids.size()) > 0){
		for (int i = 0; i < int(SubGraphKids.size()); i++){
			delete SubGraphKids[i];
		}	
	}
	
	ObjectKids.clear();
	SubGraphKids.clear();
};

void Trans::getName(){
	std::cout << "In the Transformation block" << std::endl;
}

void Trans::UpdateMatrix(Matrix4 & m_in){
	m *= m_in;
}

Matrix4 Trans::getMatrix(){
	return m;
}

void Trans::AddObjectKid(Object * o){
	ObjectKids.push_back(o);
}

void Trans::AddSubGraphKid(Tree  * t){
	SubGraphKids.push_back(t);
}

void Trans::PrintInfo(){
	std::cout << "I am the Transformation" << std::endl;
	std::cout << "I contain the following Matrix" << std::endl;
	m.print();
	std::cout << "I contain " << int(ObjectKids.size()) << " Object Children" << std::endl;
	std::cout << "I contain " << int(SubGraphKids.size()) << " SubGraph Children" << std::endl;
	std::cout << "These are my following Object Children" << std::endl;
	for (int i = 0; i < int(ObjectKids.size()); i++){
		Object * temp = ObjectKids[i];
		temp->printInfo();
	}
	std::cout << std::endl;
	std::cout << "These are my following SubGraph Children" << std::endl;
	for (int i = 0; i < int(SubGraphKids.size()); i++){
		Tree * temp = SubGraphKids[i];
		temp->printTree();
	}
}

boolean Trans::NoSubGraphs(){
	if (int(ObjectKids.size()) == 1){
		return true;
	}
	return false;
}


Object * Trans::getObject(){
	return ObjectKids[0];
}

Tree *Trans::getTree(){
	return SubGraphKids[0];
}
/*OBJECT SECTION*/

Object::Object(){
	diffuse = Color(1,1,1);
	ambient = Color(0,0,0);
	reflect = Color(0, 0, 0);
	specular = Color(0, 0, 0);
	transparent = Color(0, 0, 0);
	emit = Color(0, 0, 0);
	index = 0;
	shine = 1;
	ior = 1;
	u = 2;
	v = 4;
	filename = "wood.ras";
	s = new Shape();
}

//Object::~Object(){ delete s;}

void Object::getName(){
	std::cout << "I am the object " << object_name << std::endl;
}

void Object::printInfo(){
	getName();
	std::cout << "Diffuse values " << " x: " << diffuse[0] << " y: " << diffuse[1] << " z: " << diffuse[2] << std::endl;
	std::cout << "Ambient values " << " x: " << ambient[0] << " y: " << ambient[1] << " z: " << ambient[2] << std::endl;
	std::cout << "Reflect values " << " x: " << reflect[0] << " y: " << reflect[1] << " z: " << reflect[2] << std::endl;
	std::cout << "Transparent values " << " x: " << transparent[0] << " y: " << transparent[1] << " z: " << transparent[2] << std::endl;
	std::cout << "Emit values " << " x: " << emit[0] << " y: " << emit[1] << " z: " << emit[2] << std::endl;
	std::cout << "Shine: " << shine;
	std::cout << std::endl;
}


void Object::drawObject(){

	GLfloat dif[] = { diffuse[0], diffuse[1], diffuse[2], 1 };
	GLfloat amb[] = { ambient[0], ambient[1], ambient[2], 1 };
	GLfloat spec[] = {specular[0], specular[1], specular[2], 1 };
	GLfloat em[] = { emit[0], emit[1], emit[2], 1 };
	GLfloat reflect[] = { reflect[0], reflect[0], reflect[0], 1 };
	GLfloat trans[] = {transparent[0], transparent[1], transparent[2], 1};
	GLfloat shiny = static_cast<GLfloat>(shine);


	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shiny);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, em);
	
	s->Draw();
}

