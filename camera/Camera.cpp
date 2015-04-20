#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>
#include <stdlib.h> 


Camera::Camera()
{
	// initialize your data here
	aspect_ratio = 1.0;
	height = 600;
	width = 800;

	setFrom(Point3(3.0, 2.0, 6.0));
	setAt(Point3(0.0, 0.0, 0.0));
	setUp(Vector3(0.0, 1.0, 0.0));
	setZoom(90.0);
	setNearFar(0.1, 30.0);
}

Camera::~Camera() {
	// destroy your data here

}

// The following three should be unit length and orthogonal to each other

// u vector
Vector3 Camera::getRight() const
{
	// Change this
	Vector3 right = Up_Vector^n;
	right.normalize();
	return right;
}

// v vector
Vector3 Camera::getUp() const
{
	// Change this
	return Up_Vector;
}

// - n vector
Vector3 Camera::getLook() const
{
	// Change this
	return n*-1;
}

//EXTRA
double Camera::getSkew() const
{
	// Change this to implement the extra credit
	return skew;
}

double Camera::getAspectRatioScale() const
{
	// Change this to implement the extra credit
	return aspect_ratio;
}

//EXTRA
Point3 Camera::getProjectionCenter() const
{
	return Point3(Projection_Center[0],Projection_Center[1],Projection_Center[2]);
}


Matrix4 Camera::getProjection() const {
	// return the current projection and scale matrix
	Matrix4 ret = D*S_xyz*S_xy;
	return ret;
}

Matrix4 Camera::getWorldToCamera() const {
	// return the current world to camera matrix
	// Rotation and translation
	Matrix4 ret;
	ret = R*T;
	return ret;
}

Matrix4 Camera::getRotationFromXYZ() const
{
	// return just the rotation matrix

	// Change this
	return R_inv;
}

Matrix4 Camera::getRotationToXYZ() const
{
	// return just the rotation matrix

	// Change this
	return R;
}

Matrix4 Camera::getCameraToWorld() const {
	// return the current camera to world matrix
	// This is the inverse of the rotation, translation, and scale

	// Change this
	return T_inv*R_inv*S_xy_inv*S_xyz_inv;
}

int Camera::getWidth()  const{
	return width;
}

int Camera::getHeight()  const{
	return height;
}

Point3 Camera::getEye()  const{
	// return the current eye location
	return Point3(eye[0], eye[1], eye[2]);
}

double Camera::getZoom() const{
	return zoom;
}

void Camera::setFrom(const Point3& from) {
	// set the current viewpoint (eye point)
	eye[0] = from[0];
	eye[1] = from[1];
	eye[2] = from[2];
	T = Matrix4::translation(-eye);
	T_inv = Matrix4::translation(eye);
	//Check_Matrix(1);
}

void Camera::setAt(const Point3& at) {
	// set the point the camera is looking at
	// calling this requires that the from (or eye) point already be valid
	Looking_at[0] = at[0];
	Looking_at[1] = at[1];
	Looking_at[2] = at[2];
	setLook(Looking_at);

}

void Camera::setLook(const Vector3& l) {
	// set the direction the camera is looking at
	Look_Vector = l - eye;
	n = Look_Vector;
	n.normalize();
	n *= -1;
}

void Camera::setUp(const Vector3& up) {
	// set the upwards direction of the camera
	Up = up;;
	Up_Vector = up - (up*n)*n;
	Up_Vector.normalize();
	Update_R();
}

void Camera::setWidthHeight(int w, int h) {
	// set the current width and height of the film plane
	width = w;
	height = h;
}

void Camera::setZoom(double z) {
	// set field of view (specified in degrees)
	zoom = z;
	theta_w = Theta_W(DegreeToRad(z));

	double cot_w = double(1) / tan(theta_w / double(2));
	double cot_h = double(1) / tan(DegreeToRad(zoom / double(2)));

	S_xy = Matrix4(	Vector4(cot_w, 0, 0, 0),
					Vector4(0, cot_h, 0, 0),
					Vector4(0, 0, 1, 0),
					Vector4(0, 0, 0, 1));
	S_xy_inv = Matrix4(Vector4(tan(DegreeToRad(zoom / double(2))), 0, 0, 0),
						Vector4(0, tan(DegreeToRad(zoom / double(2))), 0, 0),
						Vector4(0, 0, 1, 0),
						Vector4(0, 0, 0, 1));
	//Check_Matrix(3);
}

void Camera::setNearFar(double n, double f) {
	// set the near and far clipping planes
	near_plane = n;
	far_plane = f;
	k = double(n) / double(f);
	double One_over = double(1) / far_plane;
	S_xyz = Matrix4(Vector4(One_over, 0, 0, 0),
					Vector4(0, One_over, 0, 0),
					Vector4(0, 0, One_over, 0),
					Vector4(0, 0, 0, 1));

	S_xyz_inv = Matrix4(Vector4(far_plane, 0, 0, 0),
						Vector4(0,far_plane, 0, 0),
						Vector4(0, 0,far_plane, 0),
						Vector4(0, 0, 0, 1));

	//Check_Matrix(4);

	D = Matrix4(Vector4(1, 0, 0, 0),
				Vector4(0, 1, 0, 0),
				Vector4(0, 0, double(1) / (k - double(1)), k / (k - double(1))), Vector4(0, 0, -1, 0));

}

void Camera::setSkew(double d){
	skew = d;

}

void Camera::setAspectRatioScale(double d){
	aspect_ratio = d;
	theta_w = Theta_W(DegreeToRad(zoom));


}

void Camera::setProjectionCenter(const Point3 &p){
	Projection_Center = Vector3(p[0],p[1],p[2]);
}

void Camera::moveForward(double dist) {
	// move the camera forward (in the viewing direction)
	// by the amount dist
	Vector3 temp = eye + -n*dist;
	Point3 pts = Point3(temp[0],temp[1], temp[2]);
	setFrom(pts);
}

void Camera::moveSideways(double dist) {
	// move the camera sideways (orthogonal to the viewing direction)
	// by the amount dist
	Vector3 temp_r = getRight();
	Vector3 temp = eye + temp_r*dist;
	Point3 pts = Point3(temp[0], temp[1], temp[2]);
	setFrom(pts);
}

void Camera::moveVertical(double dist) {
	// move the camera vertically (along the up vector)
	// by the amount dist
	Vector3 temp = eye + Up_Vector*dist;
	Point3 pts = Point3(temp[0], temp[1], temp[2]);
	setFrom(pts);
}

void Camera::rotateYaw(double angle) {
	// rotate the camera left/right (around the up vector)
	Matrix4 rot = Matrix4::rotation(Up_Vector, angle);
	Vector4 temp = Vector4(eye[0], eye[1], eye[2], 1) + rot*Vector4(Look_Vector[0], Look_Vector[1], Look_Vector[2], 1);
	Point3 at = Point3(temp[0],temp[1],temp[2]);
	setAt(at);
	setUp(Up);
}

void Camera::rotatePitch(double angle) {
	// rotate the camera up/down (pitch angle)
	Matrix4 rot = Matrix4::rotation(getRight(), angle);
	Vector4 temp = Vector4(eye[0], eye[1], eye[2], 1) + rot*Vector4(Look_Vector[0], Look_Vector[1], Look_Vector[2], 1);
	Point3 at = Point3(temp[0], temp[1], temp[2]);
	setAt(at);
	setUp(Up);
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
	// Rotate the camera around the right (0), up (1), or look (2) vector
	// around the point at eye + look * focusDist
}


void Camera::moveKeyboard()
{
	// you may change key controls for the interactive
	// camera controls here, make sure you document your changes
	// in your README file

	if (Fl::event_key('w'))
		moveForward(+0.05);
	if (Fl::event_key('s'))
		moveForward(-0.05);
	if (Fl::event_key('a'))
		moveSideways(-0.05);
	if (Fl::event_key('d'))
		moveSideways(+0.05);
	if (Fl::event_key(FL_Up))
		moveVertical(+0.05);
	if (Fl::event_key(FL_Down))
		moveVertical(-0.05);
	if (Fl::event_key(FL_Left))
		rotateYaw(+0.05);
	if (Fl::event_key(FL_Right))
		rotateYaw(-0.05);
	if (Fl::event_key(FL_Page_Up))
		rotatePitch(+0.05);
	if (Fl::event_key(FL_Page_Down))
		rotatePitch(-0.05);
}



double Camera::DegreeToRad(double deg) const{
	return deg*M_PI_180;
}

double Camera::Theta_W(double deg_h){
	double half_deg_h = deg_h / double(2);
	double h_a = tan(half_deg_h)*Aspect_Ratio();
	double ret = atan(h_a);
	return double(double(2)* ret);
}

double Camera::Aspect_Ratio() const{
	double lhs = double(width) / double(height);
	return lhs*aspect_ratio;
}

void Camera::Update_R(){
	Vector3 right = getRight();
	R = Matrix4(Vector4(right[0], right[1], right[2], 0),
		Vector4(Up_Vector[0], Up_Vector[1], Up_Vector[2], 0),
		Vector4(n[0], n[1], n[2], 0),
		Vector4(0, 0, 0, 1));
	
	R_inv = Matrix4(Vector4(R.row(0)[0], R.row(1)[0], R.row(2)[0], 0),
					Vector4(R.row(0)[1], R.row(1)[1], R.row(2)[1], 0),
					Vector4(R.row(0)[2], R.row(1)[2], R.row(2)[2], 0),
					Vector4(0, 0, 0, 1));

	
	//Check_Matrix(2);
}


void Camera::Check_Matrix(int Matrix_num) const{
	Matrix4 test;
	
	switch (Matrix_num)
	{
		case 1: {
					test = T*T_inv;
					
					if (!Round_Matrix(test)){
						std::cout << "The Translation Matrix has been incorrectly calulated" << std::endl;
						std::cout << "Translation Matrix" << std::endl;
						T.print();
						std::cout << "Inverse of Translation Matrix" << std::endl;
						T_inv.print();
						std::cout << "Their product " << std::endl;
						test.print();
					}
					break;
		}
		case 2: {
					test = R*R_inv;
					if (!Round_Matrix(test)){
						std::cout << "The Rotation Matrix has been incorrectly calulated" << std::endl;
						std::cout << "Rotation Matrix" << std::endl;
						R.print();
						std::cout << "Inverse of Rotation Matrix" << std::endl;
						R_inv.print();
						std::cout << "Their product " << std::endl;
						test.print();
					}
					break;
		}
		case 3: {
					test = S_xy*S_xy_inv;

					if (!Round_Matrix(test)){
						std::cout << "The Scale(xy) Matrix has been incorrectly calulated" << std::endl;
						std::cout << "Scale(xy) Matrix" << std::endl;
						S_xy.print();
						std::cout << "Inverse of Scale(xy) Matrix" << std::endl;
						S_xy_inv.print();
						std::cout << "Their product " << std::endl;
						test.print();
					}
					break;
		}
		case 4: {
					test = S_xyz*S_xyz_inv;
					if (!Round_Matrix(test)){
						std::cout << "The Scale(xyz) Matrix has been incorrectly calulated" << std::endl;
						std::cout << "Scale(xyz) Matrix" << std::endl;
						S_xyz.print();
						std::cout << "Inverse of Scale(xyz) Matrix" << std::endl;
						S_xyz_inv.print();
						std::cout << "Their product " << std::endl;
						test.print();
					}
					break;
		}

	default:
		break;
	}
}


bool Camera::Round_Matrix(Matrix4 & m) const{
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			if (i == j){
				if (int(round(m.row(i)[j])) != 1){
					return false;
				}
			}
			else{
				if (int(round(m.row(i)[j])) != 0){
					return false;
				}
			}
		}
	}
	return true;
}