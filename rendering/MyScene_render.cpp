#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
	if (!isLoaded) {
		return;
	}
	continue_rendering = true;
	progress = 0.0;


	w = double(width);
	h = double(height);

	eye = camera.getEye();

	//Only focusing on Ray Tracing this lab so, no point in allowing the other
	//options. In testing I frequently misclicked and it was annoying. 
	type = 1;
	
	//TESTING CODE
	std::ofstream out("out_mine.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!



	switch (type) {
	case RenderingUI::RENDER_SCANLINE:
		progress = 1.0;
		break;
	case RenderingUI::RENDER_RAY_TRACING:
		RayTrace(width, height, pixels);
		break;
	case RenderingUI::RENDER_PATH_TRACING:
		progress = 1.0;
		break;
	default: break;
	}


	//TESTING CODE
	std::cout.rdbuf(coutbuf); //reset to standard output again
}


void MyScene::stopRender(){
	continue_rendering = false;
}

double MyScene::getRenderProgress() {
	// return the current progress as a value between 0 and 1
	return progress;
}

// add extra methods here

void MyScene::RayTrace(int & width, int & height, unsigned char* pixels){
	double red, green, blue;
	for (int j = 0; j<height; ++j){
		for (int i = 0; i<width; ++i){
			red = green = blue = 0.0;
			createScene(i, j, red, green, blue, RECURSIVE_LIMIT);
			int cur = (i + (height - 1 - j)*width) * 3;
			pixels[cur] = (unsigned char)(red * 255.0f);
			pixels[cur + 1] = (unsigned char)(green * 255.0f);
			pixels[cur + 2] = (unsigned char)(blue * 255.0f);
		}
		progress = (double)(j + 1) / (double)(height);
		if (!continue_rendering){
			progress = 1.0;
			return;
		}
		Fl::check();
	}
}

void MyScene::createScene(const int & x, const int & y, double & r, double & g, double & b, int depth){
	Vector3 dir;
	Point3 qs = Point3(((x + 0.5) * 2.0 / w - 1.0), (1.0 - (y + 0.5) * 2.0 / h), -1.0);
	Point3  qw = getCamera().getCameraToWorld() * (qs);
	dir = qw - eye;
	dir.normalize();
	SceneLighting(dir, eye, r, g, b, depth);
}

void MyScene::rayToObject(const Vector3 ray, const Point3 point, DrawObject *& hitObject, Vector3 & n, Point3 & hit_point){
	double lowest_T = DBL_MAX;
	double t, u, v;
	Point3 temp_p;
	Vector3 temp_d;
	HitRecord hits;
	Matrix4 m;
	Matrix4 m_inv;

	bool hit = false;

	for (int i = 0; i < int(quick_load.size()); i++){
		temp_p = quick_load[i].inverse_m* point;
		temp_d = quick_load[i].inverse_m* ray;

		quick_load[i].o->getShape()->getCorrectIntersect(quick_load[i].o->getShapeNum(), hits, temp_p, temp_d);
		hits.sortHits();
		if (hits.getFirstHit(t, u, v, temp_p, temp_d)){
			if (t < lowest_T && t > 0.){
				lowest_T = t;
				hitObject = &quick_load[i];
				hit_point = temp_p;
				n = temp_d;
				m = quick_load[i].m;
				m_inv = quick_load[i].inverse_m;
				hit = true;
			}
		}
	}
	//Post hit computations
	if (hit){
		hit_point = m*hit_point;		
		n = m_inv.transpose()*n;
		n.normalize();
	}
}

void MyScene::SceneLighting(const Vector3& ray, const Point3& point, double & r, double & g, double & b, int levels){
	DrawObject * intersecting_shape = nullptr;
	Vector3  hit_n;
	Point3 hit_p;

	rayToObject(ray, point, intersecting_shape, hit_n, hit_p);


	Color ambient_comp;
	Color diffuse_comp;
	Color specular_comp;
	Color reflect_comp;
	Color transparent_comp;

	if (intersecting_shape != nullptr){



		//First up is to calculate the reflection

		//Obtain the reflection
		
		Color shape_reflect = intersecting_shape->o->getReflect();
		if (levels < RECURSIVE_LIMIT && shape_reflect.getMax() > 0.0){
			double reflect_red = 0.0;
			double reflect_green = 0.0;
			double reflect_blue = 0.0;

			Vector3 reflected_ray = ray;
			reflected_ray.normalize();
			reflected_ray = (2 * ( hit_n* -reflected_ray)) * hit_n + reflected_ray;
			reflected_ray.normalize();

			SceneLighting(reflected_ray, hit_p + reflected_ray*SHIFT_FACTOR, reflect_red, reflect_green, reflect_blue, levels++);

			//At end of recursion store value to be added here
			//Pass by value is a beautiful thing
			reflect_comp = Color(static_cast<GLfloat>(reflect_red), static_cast<GLfloat>(reflect_green), static_cast<GLfloat>(reflect_blue));
		}



		Color objTrans = intersecting_shape->o->getTransparent();
		if (levels > 0 && shape_reflect.getMax() > 0.){
			double trans_red = 0.0;
			double trans_green = 0.0;
			double trans_blue = 0.0;

			SceneLighting(ray, hit_p + ray*SHIFT_FACTOR, trans_red, trans_green, trans_blue, levels++);
			
			reflect_comp = Color(static_cast<GLfloat>(trans_red), static_cast<GLfloat>(trans_green), static_cast<GLfloat>(trans_blue));
		}



		ambient_comp = ambientLight*intersecting_shape->o->getAmbient();

		for (auto ele : lights){
			DrawObject * hits_another_shape = nullptr;
			Vector3 vec_to_light = ele.getPos() - hit_p;
			Point3 temp;
			Vector3 another_temp;
			rayToObject(vec_to_light, hit_p + vec_to_light*SHIFT_FACTOR, hits_another_shape, another_temp, temp);


			if (hits_another_shape == nullptr){
				double attenuation, diffuse, specular;
				attenuation = 1. / (ele.getFalloff()[0] + ele.getFalloff()[1] * vec_to_light.length() + ele.getFalloff()[2] * vec_to_light.lengthSquared());
				vec_to_light.normalize();
				another_temp = (2 * (hit_n * -vec_to_light)) * hit_n + vec_to_light;
				another_temp.normalize();


				Color lightColor = ele.getColor();
				diffuse = hit_n * vec_to_light;
				if (diffuse < 0.) { diffuse = 0.; }
				diffuse_comp += lightColor*attenuation*intersecting_shape->o->getDiffuse()*diffuse;


				specular = (ray * another_temp);
				if (specular > 0.0){
					pow((specular), intersecting_shape->o->getShine());
				}
				else{
					specular = 0.0;
				}
				
				specular_comp += lightColor*attenuation*intersecting_shape->o->getSpecular()*specular;

			}
		}
		Color ret_values = diffuse_comp + ambient_comp + reflect_comp + specular_comp + transparent_comp;
		ret_values.print();
		ret_values.Color_Check();
		r = ret_values[0];
		g = ret_values[1];
		b = ret_values[2];
		return;
	}
	else{
		r = g = b = 0.0;
		return;
	}

}
