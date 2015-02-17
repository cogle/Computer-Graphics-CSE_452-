#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>

Camera::Camera() 
{
    // initialize your data here
}

Camera::~Camera() {
    // destroy your data here
}

// The following three should be unit length and orthogonal to each other
// u vector
Vector3 Camera::getRight() const
{
    // Change this
    return Vector3(0,0,0);
}

// v vector
Vector3 Camera::getUp() const
{
    // Change this
    return Vector3(0,0,0);
}

// - n vector
Vector3 Camera::getLook() const
{
    // Change this
    return Vector3(0,0,0);
}

double Camera::getSkew() const
{
    // Change this to implement the extra credit
    return 0.0;
}

double Camera::getAspectRatioScale() const
{
    // Change this to implement the extra credit
    return 1.0;
}

Point3 Camera::getProjectionCenter() const
{
    // Change this to implement the extra credit
    return Point3( 0.0, 0.0, 0.0 );
}

Matrix4 Camera::getProjection() const {
    // return the current projection and scale matrix

    // Change this
    return Matrix4::identity();
}

Matrix4 Camera::getWorldToCamera() const {
    // return the current world to camera matrix
    // Rotation and translation

    // Change this
    return Matrix4::identity();
}

Matrix4 Camera::getRotationFromXYZ() const
{
    // return just the rotation matrix

    // Change this
    return Matrix4::identity();
}

Matrix4 Camera::getRotationToXYZ() const
{
    // return just the rotation matrix

    // Change this
    return Matrix4::identity();
}

Matrix4 Camera::getCameraToWorld() const {
    // return the current camera to world matrix
    // This is the inverse of the rotation, translation, and scale

    // Change this
    return Matrix4::identity();
}

int Camera::getWidth()  const{
    // return the current image width

    // Change this
    return 0;
}

int Camera::getHeight()  const{
    // return the current image height

    // Change this
    return 0;
}

Point3 Camera::getEye()  const{
    // return the current eye location

    // Change this
    return Point3(0.0, 0.0, 0.0);
}

double Camera::getZoom() const
{
    // Change this
    return 0;
}

void Camera::setFrom(const Point3& from) {
    // set the current viewpoint (eye point)
}

void Camera::setAt(const Point3& at) {
    // set the point the camera is looking at
    // calling this requires that the from (or eye) point already be valid
}

void Camera::setLook(const Vector3& l) {
    // set the direction the camera is looking at
}

void Camera::setUp(const Vector3& up) {
    // set the upwards direction of the camera
}

void Camera::setWidthHeight(int w, int h) {
    // set the current width and height of the film plane
}

void Camera::setZoom(double z) {
    // set field of view (specified in degrees)
    
}

void Camera::setNearFar(double n, double f) {
    // set the near and far clipping planes
}

void Camera::setSkew( double d )
{
}

void Camera::setAspectRatioScale( double d )
{
}

void Camera::setProjectionCenter( const Point3 &p )
{
}

void Camera::moveForward(double dist) {
    // move the camera forward (in the viewing direction)
    // by the amount dist
}

void Camera::moveSideways(double dist) {
    // move the camera sideways (orthogonal to the viewing direction)
    // by the amount dist
}

void Camera::moveVertical(double dist) {
    // move the camera vertically (along the up vector)
    // by the amount dist
}

void Camera::rotateYaw(double angle) {
    // rotate the camera left/right (around the up vector)
}

void Camera::rotatePitch(double angle) {
    // rotate the camera up/down (pitch angle)
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
    // Rotate the camera around the right (0), up (1), or look (2) vector
    // around the point at eye + look * focusDist

}


void Camera::moveKeyboard( )
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
