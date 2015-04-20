#include "../cse452.h"
#include "MyScene.h"
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <cmath>
//#include "SubGraph.h"

// string constants used for parsing
// feel free to add any new keywords that you think may be helpfull
const std::string TOKEN_BACKGROUND = "background";
const std::string TOKEN_COLOR = "color";
const std::string TOKEN_OB = "[";
const std::string TOKEN_CB = "]";
const std::string TOKEN_CAMERA = "camera";
const std::string TOKEN_EYE = "eye";
const std::string TOKEN_LOOK = "look";
const std::string TOKEN_FOCUS = "focus";
const std::string TOKEN_UP = "up";
const std::string TOKEN_ANGLE = "angle";
const std::string TOKEN_NEAR_FAR = "near-far";
const std::string TOKEN_LIGHT = "light";
const std::string TOKEN_LIGHTTYPE = "type";
const std::string TOKEN_POINT = "point";
const std::string TOKEN_DIRECTIONAL = "directional";
const std::string TOKEN_SPOTLIGHT = "spotlight";
const std::string TOKEN_POSITION = "position";
const std::string TOKEN_DIRECTION = "direction";
const std::string TOKEN_FUNCTION = "function";
const std::string TOKEN_APERTURE = "aperture";
const std::string TOKEN_EXPONENT = "exponent";
const std::string TOKEN_MASTERSUBGRAPH = "mastersubgraph";
const std::string TOKEN_TRANS = "trans";
const std::string TOKEN_ROTATE = "rotate";
const std::string TOKEN_TRANSLATE = "translate";
const std::string TOKEN_SCALE = "scale";
const std::string TOKEN_MATRIXRC = "matrixRC";
const std::string TOKEN_MATRIXCR = "matrixCR";
const std::string TOKEN_OBJECT = "object";
const std::string TOKEN_CUBE = "cube";
const std::string TOKEN_CYLINDER = "cylinder";
const std::string TOKEN_CONE = "cone";
const std::string TOKEN_SPHERE = "sphere";
const std::string TOKEN_FISH = "fish";
const std::string TOKEN_BOWL = "bowl";
const std::string TOKEN_AMBIENT = "ambient";
const std::string TOKEN_DIFFUSE = "diffuse";
const std::string TOKEN_SPECULAR = "specular";
const std::string TOKEN_REFLECT = "reflect";
const std::string TOKEN_TRANSPARENT = "transparent";
const std::string TOKEN_EMIT = "emit";
const std::string TOKEN_SHINE = "shine";
const std::string TOKEN_IOR = "ior";
const std::string TOKEN_TEXTURE = "texture";
const std::string TOKEN_SUBGRAPH = "subgraph";


MyScene::MyScene() :
ambientLight(0, 0, 0)
{
	// initialize your variables here
	//
	shape_c = new ShapeContainer();
	resetScene();
}

MyScene::~MyScene() {
	// destroy your variables here
	for (auto ele : master_subgraphs){
		delete ele.second;
	}
	delete shape_c;
}

void MyScene::resetScene() {
	// reset scene to defaults (empty the scene tree)
	background = Color();
	camera.setFrom(Point3(0, 0, 1));
	camera.setLook(Vector3(0, 0, -1));
	camera.setUp(Vector3(0, 1, 0));
	camera.setZoom(60.0);
	camera.setNearFar(0.01, 100.0);
	camera.setProjectionCenter(Point3(0, 0, 0));
	camera.setSkew(0.0);
	camera.setAspectRatioScale(1.0);
	lights.clear();
	ambientLight = Color(0, 0, 0);
	// reset your data here
	master_subgraphs.clear();
	quick_load.clear();
	found_root = false;
	added_trans = false;
	loaded = false;
}

bool MyScene::loadSceneFile(std::string filename) {
	// load the scenefile with the given filename
	// return true if the file was loaded succesfully
	resetScene();
	// parse file:
	Parser p(filename);
	bool done = false;

	errorMessage = "";
	do {
		done = p.nextToken();
		if (p.getToken() == TOKEN_BACKGROUND)
			done = !parseBackground(p);
		else if (p.getToken() == TOKEN_CAMERA)
			done = !parseCamera(p);
		else if (p.getToken() == TOKEN_LIGHT)
			done = !parseLight(p);
		else if (p.getToken() == TOKEN_MASTERSUBGRAPH){
			Tree * ret = parseMasterSubgraph(p);
			if (ret == nullptr){
				resetScene();
				return false;
			}
			done = (ret == 0);
		}
		else if (!done) {
			errorMessage = "Unrecognized token at root level: \"" + p.getToken() + "\"";
			done = true;
		}
	} while (!done);

	// ToDo: check that there is a root mastersubgraph
	// and that no errors occured while loading


	isLoaded = errorMessage.length() == 0;
	isLoaded = true;
	if (isLoaded) {
		// perform any post-loading computations here
		// (such as flattening the tree or building
		// ray-acceleration structures)
		if (added_trans){
			Tree *  root = master_subgraphs["root"];
			for (int i = 0; i < int(root->getChildList().size()); i++){
				Trans * t = root->getChildList()[i];
				RecursiveDescent(t, t->getMatrix());
			}
		}
		
	}
	else if (errorMessage.length() == 0 || !found_root){
		errorMessage = "Unable to locate root mastersubgraph";
		resetScene();
		return false;
	}

	found_root = false;
	return isLoaded;
}

bool MyScene::parseBackground(Parser& p) {
	// parse the background block
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_COLOR) {
			p.nextToken(); background[0] = static_cast<float>(p.getValue());
			p.nextToken(); background[1] = static_cast<float>(p.getValue());
			p.nextToken(); background[2] = static_cast<float>(p.getValue());
		}
		else {
			errorMessage = "Unrecognized token in background block: \"" + p.getToken() + "\"";
			return false;
		}
	} while (true);
	return true;
}

bool MyScene::parseCamera(Parser& p) {
	// parse the camera
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_EYE) {
			Point3 eye;
			p.nextToken(); eye[0] = p.getValue();
			p.nextToken(); eye[1] = p.getValue();
			p.nextToken(); eye[2] = p.getValue();
			camera.setFrom(eye);
		}
		else if (p.getToken() == TOKEN_LOOK) {
			Vector3 look;
			p.nextToken(); look[0] = p.getValue();
			p.nextToken(); look[1] = p.getValue();
			p.nextToken(); look[2] = p.getValue();
			camera.setLook(look);
		}
		else if (p.getToken() == TOKEN_FOCUS) {
			Point3 focus;
			p.nextToken(); focus[0] = p.getValue();
			p.nextToken(); focus[1] = p.getValue();
			p.nextToken(); focus[2] = p.getValue();
			camera.setAt(focus);
		}
		else if (p.getToken() == TOKEN_UP) {
			Vector3 up;
			p.nextToken(); up[0] = p.getValue();
			p.nextToken(); up[1] = p.getValue();
			p.nextToken(); up[2] = p.getValue();
			camera.setUp(up);
		}
		else if (p.getToken() == TOKEN_ANGLE) {
			p.nextToken(); camera.setZoom(p.getValue());
		}
		else if (p.getToken() == TOKEN_NEAR_FAR) {
			double n, f;
			p.nextToken(); n = p.getValue();
			p.nextToken(); f = p.getValue();
			camera.setNearFar(n, f);
		}
		else {
			errorMessage = "Unrecognized token in camera block: \"" + p.getToken() + "\"";
			return false;
		}
	} while (true);
	return true;
}

bool MyScene::parseLight(Parser& p) {
	// parse a light source block
	Light l;
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_LIGHTTYPE) {
			p.nextToken();
			if (p.getToken() == TOKEN_POINT)
				l.setType(Light::POINTLIGHT);
			else if (p.getToken() == TOKEN_DIRECTIONAL)
				l.setType(Light::DIRECTIONAL);
			else if (p.getToken() == TOKEN_SPOTLIGHT)
				l.setType(Light::SPOTLIGHT);
			else {
				errorMessage = "Unknown light type: \"" + p.getToken() + "\"";
				return false;
			}
		}
		else if (p.getToken() == TOKEN_POSITION) {
			Point3 pos;
			p.nextToken(); pos[0] = p.getValue();
			p.nextToken(); pos[1] = p.getValue();
			p.nextToken(); pos[2] = p.getValue();
			l.setPosition(pos);
		}
		else if (p.getToken() == TOKEN_COLOR) {
			Color color;
			p.nextToken(); color[0] = static_cast<float>(p.getValue());
			p.nextToken(); color[1] = static_cast<float>(p.getValue());
			p.nextToken(); color[2] = static_cast<float>(p.getValue());
			l.setColor(color);
		}
		else if (p.getToken() == TOKEN_FUNCTION) {
			float f0, f1, f2;
			p.nextToken(); f0 = static_cast<float>(p.getValue());
			p.nextToken(); f1 = static_cast<float>(p.getValue());
			p.nextToken(); f2 = static_cast<float>(p.getValue());
			l.setFalloff(f0, f1, f2);
		}
		else if (p.getToken() == TOKEN_DIRECTION) {
			Vector3 dir;
			p.nextToken(); dir[0] = p.getValue();
			p.nextToken(); dir[1] = p.getValue();
			p.nextToken(); dir[2] = p.getValue();
			dir.normalize();
			l.setDirection(dir);
		}
		else if (p.getToken() == TOKEN_APERTURE) {
			p.nextToken(); l.setAperture(p.getValue());
		}
		else if (p.getToken() == TOKEN_EXPONENT) {
			p.nextToken(); l.setExponent(p.getValue());
		}
		else if (p.getToken() == TOKEN_AMBIENT) {
			p.nextToken(); ambientLight[0] = static_cast<float>(p.getValue());
			p.nextToken(); ambientLight[1] = static_cast<float>(p.getValue());
			p.nextToken(); ambientLight[2] = static_cast<float>(p.getValue());
		}
		else {
			errorMessage = "Unrecognized token in light block: \"" + p.getToken() + "\"";
			return false;
		}

	} while (true);
	assert(l.getColor()[0] >= 0.0 && l.getColor()[0] <= 1.0);
	assert(l.getColor()[1] >= 0.0 && l.getColor()[1] <= 1.0);
	assert(l.getColor()[2] >= 0.0 && l.getColor()[2] <= 1.0);
	assert(l.getAperture() >  0.0 && l.getAperture() <= 180.0);
	assert(l.getExponent() >= 0.0);
	lights.push_back(l);
	return true;
}

Tree* MyScene::parseMasterSubgraph(Parser& p) {
	// ToDo: parse a named subgraph tree
	bool found_trans = false;
	p.nextToken();
	std::string name = p.getToken();
	Tree * t = new Tree(name);
	cur_name = name;
	if (name.compare("root") == 0){
		found_root = true;
	}
	else{
		added_trans = true;
	}
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_TRANS) {
			Node * n = parseTrans(p);
			if (n != nullptr){
				found_trans = true;
				t->addTrans(static_cast<Trans *>(n));
			}
			else{
				return nullptr;
			}
		}
		else {
			errorMessage = "Unrecognized token in mastersubgraph \"" + name + "\": \"" + p.getToken() + "\"";
			return 0;
		}
	} while (true);
	
	if (!found_trans){
		errorMessage = " INCORRECT TRANS FORMAT FOR SUBGRAPH " + name;
		return nullptr;
	}
	
	master_subgraphs[name] = t;
	// ToDo: Fix this
	return t;
}

Node* MyScene::parseTrans(Parser& p) {
	// parse a trans block node
	Trans * trans = new Trans();
	bool seen_object = false;
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_ROTATE) {
			Vector3 axis;
			p.nextToken(); axis[0] = p.getValue();
			p.nextToken(); axis[1] = p.getValue();
			p.nextToken(); axis[2] = p.getValue();
			p.nextToken(); double angle = p.getValue() * M_PI / 180.0;
			trans->UpdateMatrix(Matrix4::rotation(axis,angle));

		}
		else if (p.getToken() == TOKEN_TRANSLATE) {
			Vector3 v;
			p.nextToken(); v[0] = p.getValue();
			p.nextToken(); v[1] = p.getValue();
			p.nextToken(); v[2] = p.getValue();
			trans->UpdateMatrix(Matrix4::translation(v));

		}
		else if (p.getToken() == TOKEN_SCALE) {
			Vector3 v;
			p.nextToken(); v[0] = p.getValue();
			p.nextToken(); v[1] = p.getValue();
			p.nextToken(); v[2] = p.getValue();
			trans->UpdateMatrix(Matrix4::scaling(v));

		}
		else if (p.getToken() == TOKEN_MATRIXRC) {
			Vector4 r0, r1, r2, r3;
			p.nextToken(); r0[0] = p.getValue();
			p.nextToken(); r0[1] = p.getValue();
			p.nextToken(); r0[2] = p.getValue();
			p.nextToken(); r0[3] = p.getValue();

			p.nextToken(); r1[0] = p.getValue();
			p.nextToken(); r1[1] = p.getValue();
			p.nextToken(); r1[2] = p.getValue();
			p.nextToken(); r1[3] = p.getValue();

			p.nextToken(); r2[0] = p.getValue();
			p.nextToken(); r2[1] = p.getValue();
			p.nextToken(); r2[2] = p.getValue();
			p.nextToken(); r2[3] = p.getValue();

			p.nextToken(); r3[0] = p.getValue();
			p.nextToken(); r3[1] = p.getValue();
			p.nextToken(); r3[2] = p.getValue();
			p.nextToken(); r3[3] = p.getValue();
			trans->UpdateMatrix(Matrix4(r0,r1,r2,r3));

		}
		else if (p.getToken() == TOKEN_MATRIXCR) {
			Vector4 r0, r1, r2, r3;
			p.nextToken(); r0[0] = p.getValue();
			p.nextToken(); r1[0] = p.getValue();
			p.nextToken(); r2[0] = p.getValue();
			p.nextToken(); r3[0] = p.getValue();

			p.nextToken(); r0[1] = p.getValue();
			p.nextToken(); r1[1] = p.getValue();
			p.nextToken(); r2[1] = p.getValue();
			p.nextToken(); r3[1] = p.getValue();

			p.nextToken(); r0[2] = p.getValue();
			p.nextToken(); r1[2] = p.getValue();
			p.nextToken(); r2[2] = p.getValue();
			p.nextToken(); r3[2] = p.getValue();

			p.nextToken(); r0[3] = p.getValue();
			p.nextToken(); r1[3] = p.getValue();
			p.nextToken(); r2[3] = p.getValue();
			p.nextToken(); r3[3] = p.getValue();
			trans->UpdateMatrix(Matrix4(r0, r1, r2, r3));

		}
		else if (p.getToken() == TOKEN_OBJECT) {
			// ToDo: parse the object and add it to the node
			// call parseObject(p) here;
			Object * o = new Object();
			o = parseObject(p);
			if (o != nullptr){
				trans->AddObjectKid(o);
				seen_object = true;
				if (found_root){
					quick_load.push_back(DrawObject(trans->getMatrix(), o));
				}
			}
			else{
				return nullptr;
			}
		}
		else if (p.getToken() == TOKEN_SUBGRAPH) {
			// ToDo: find the subgraph and add it to the current node
			p.nextToken();
			std::string subgraphName = p.getToken();
			if (master_subgraphs.find(subgraphName) != master_subgraphs.end()){
				trans->AddSubGraphKid(master_subgraphs[subgraphName]);
				Tree * tree = master_subgraphs[subgraphName];
				added_trans = true;
				seen_object = true;
			}
			else{
				return nullptr;
			}
		}
		else {
			errorMessage = "Unrecognized token in trans block: \"" + p.getToken() + "\"";
			return nullptr;
		}
	} while (true);
	if (!seen_object){
		errorMessage = "  NO OBJECT OR SUBGRAPH FOUND for: " + cur_name;
		return nullptr;
	}
	// ToDo: Fix this
	return static_cast<Node *>(trans);
}

Object* MyScene::parseObject(Parser& p) {
	// ToDo: parse an object block
	Object * ret = new Object();
	p.nextToken();
	if (p.getToken() == TOKEN_CUBE) {
		// object is a cube
		ret->setShape(shape_c->getCube());
		ret->setName("Cube");
		ret->SetShapeNum(1);
	}
	else if (p.getToken() == TOKEN_CYLINDER) {
		// object is a cylinder
		ret->setShape(shape_c->getCylinder());
		ret->setName("Cylinder");
		ret->SetShapeNum(2);
	}
	else if (p.getToken() == TOKEN_CONE) {
		// object is a cone
		ret->setShape(shape_c->getCone());
		ret->setName("Cone");
		ret->SetShapeNum(3);
	}
	else if (p.getToken() == TOKEN_SPHERE) {
		// object is a sphere
		ret->setShape(shape_c->getSphere());
		ret->setName("Sphere");
		ret->SetShapeNum(4);
	}
	else if (p.getToken() == TOKEN_FISH) {
		ret->setShape(shape_c->getFish());
		ret->setName("Fish");
		ret->SetShapeNum(5);
	}
	else if (p.getToken() == TOKEN_BOWL) {
		ret->setShape(shape_c->getBowl());
		ret->setName("Bowl");
		ret->SetShapeNum(5);
	}
	else {
		errorMessage = "Unrecognized object type: \"" + p.getToken() + "\"";
		return nullptr;
	}
	do {
		p.nextToken();
		if (p.getToken() == TOKEN_OB) {
		}
		else if (p.getToken() == TOKEN_CB) {
			break;
		}
		else if (p.getToken() == TOKEN_AMBIENT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store ambient color here
			ret->setAmbient(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));

		}
		else if (p.getToken() == TOKEN_DIFFUSE) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store diffuse color here
			ret->setDiffuse(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));

		}
		else if (p.getToken() == TOKEN_SPECULAR) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store specular color here
			ret->setSpecular(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));
		}
		else if (p.getToken() == TOKEN_REFLECT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store reflect color here
			ret->setReflect(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));
		}
		else if (p.getToken() == TOKEN_TRANSPARENT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store transparent color here
			ret->setTransparent(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));
		}
		else if (p.getToken() == TOKEN_EMIT) {
			double r, g, b;
			p.nextToken(); r = p.getValue();
			p.nextToken(); g = p.getValue();
			p.nextToken(); b = p.getValue();
			// store emitted color here
			ret->setEmit(Color(static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b)));
		}
		else if (p.getToken() == TOKEN_SHINE) {
			double s;
			p.nextToken(); s = p.getValue();
			// store shine here
			ret->setShine(s);

		}
		else if (p.getToken() == TOKEN_IOR) {
			double ior;
			p.nextToken(); ior = p.getValue();
			// store ior here
			ret->setIOR(ior);

		}
		else if (p.getToken() == TOKEN_TEXTURE) {
			std::string filename;
			double textureU, textureV;
			p.nextToken(); filename = p.getToken();
			p.nextToken(); textureU = p.getValue();
			p.nextToken(); textureV = p.getValue();
			// store texture information here
			ret->setTextureFile(filename);
			ret->setU(textureU);
			ret->setV(textureV);
		
		}
		else {
			errorMessage = "Unrecognized token in object block: \"" + p.getToken() + "\"";
			return nullptr;
		}
	} while (true);
	// ToDo: Fix this
	return ret;
}

std::string MyScene::getErrorMessage() const {
	// return the current error message
	return errorMessage;
}

const Camera & MyScene::getCamera() const {
	// return the current camera
	return camera;
}

Camera & MyScene::changeCamera() {
	// return the current camera
	return camera;
}

const Color &MyScene::getBackground() const {
	return background;
}


void MyScene::RecursiveDescent(Trans * t, Matrix4 cur_m){
	if (t->NoSubGraphs()){
		//to_be_drawn.push_back(DrawObject(cur_m, t->getObject()));
		/*glPushMatrix();
			glMultMatrixd(&(cur_m(0, 0)));
			t->getObject()->drawObject();
			
		glPopMatrix();*/
		quick_load.push_back(DrawObject(cur_m, t->getObject()));
		return;
	}
	else{
		for (int i = 0; i < int(t->getTree()->getChildList().size()); i++){
			//RecursiveDescent(t, trans->getMatrix()*t->getMatrix());
			Matrix4 pass = cur_m*t->getTree()->getChildList()[i]->getMatrix();
			RecursiveDescent(t->getTree()->getChildList()[i],pass);
		}
	}
}



/*
Tree *  root = master_subgraphs["root"];
for (int i = 0; i < int(root->getChildList().size()); i++){
Trans * t = root->getChildList()[i];
RecursiveDescent(t, t->getMatrix());
}
*/