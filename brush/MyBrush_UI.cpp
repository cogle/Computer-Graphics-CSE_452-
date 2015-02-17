#include "cse452.h"
#include "ScreenPoint.h"
#include "BrushInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <cstring>
#include <cmath>

MyBrush::MyBrush()
{
	isMouseDown = false;

	imageWidth = screenWidth = 0;
	imageHeight = screenHeight = 0;

	// initialize your data here
}

MyBrush::~MyBrush() {
	// destroy your data here
}

void MyBrush::resize(int width, int height) {
	screenWidth = width;
	screenHeight = height;

	// First time initialization
	if (imageWidth == 0) {
		imageWidth = screenWidth;
		imageHeight = screenHeight;

		// Make image black
		pixelData.resize(width * height * 3, 0);
	}
}

void MyBrush::loadImage(Fl_Image* image) {
	imageWidth = image->w();
	imageHeight = image->h();
	// Reset viewport
	resize(screenWidth, screenHeight);
	pixelData.resize(imageWidth * imageHeight * 3, 0);

	// OpenGL's windows are reversed in y
	const int delta = imageWidth * 3;
	unsigned char* src = (unsigned char*)*image->data();
	for (int i = 0; i < imageHeight; i++) {
		// Ok, this is ugly
		unsigned char* dest = &pixelData[((imageHeight - 1 - i) * imageWidth * 3)];
		memcpy(dest, src, delta);
		src += delta;
	}
}

void MyBrush::draw() {
	// Set up camera for drawing
	setup2DDrawing(Color(0, 0, 0), screenWidth, screenHeight);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Draw a border around the actual image
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(0, 0);
	glVertex2i(imageWidth + 1, 0);
	glVertex2i(imageWidth + 1, imageHeight + 1);
	glVertex2i(0, imageHeight + 1);
	glEnd();


	glRasterPos2i(0, 0);
	// Copy data into window
	//for ( int iX = 0; iX < 100; iX++ )
	//putPixel( iX, iX, Color(1,0,0) );

	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, &pixelData[0]);

	// These 5 lines draw a white line across your canvas
	// Remove this and replace it with intelligent OpenGL preview code

	/*I ADDED THIS*/
	if (brushUI->getToolType() == TOOL_BRUSH && !isMouseDown){
		int radius = brushUI->getRadius();
		int x = 0;
		int y = radius;

		int x0 = mouseDrag[0];
		int y0 = mouseDrag[1];

		int deltaE = 2 * x + 3;
		int deltaSE = 2 * (x - y) + 5;

		float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - radius*radius);

		glColor3f(brushUI->getColor()[0], brushUI->getColor()[1], brushUI->getColor()[2]);
		glBegin(GL_POINTS);
		glVertex2i(x0, y0 + radius);
		glVertex2i(x0 + radius, y0);
		glVertex2i(x0, y0 - radius);
		glVertex2i(x0 - radius, y0);
		glEnd();

		while (y > x){
			if (decision < 0){
				x++;
				decision += deltaE;
				deltaE += 2;
				deltaSE += 2;
			}
			else{
				y--;
				x++;
				decision += deltaSE;
				deltaE += 2;
				deltaSE += 4;
			}
			glBegin(GL_POINTS);
				glVertex2i(x0 + x, y + y0);
				glVertex2i(y + x0, x + y0);
				glVertex2i(-x + x0, y + y0);
				glVertex2i(-y + x0, x + y0);
				glVertex2i(-x + x0, -y + y0);
				glVertex2i(-y + x0, -x + y0);
				glVertex2i(x + x0, -y + y0);
				glVertex2i(y + x0, -x + y0);
			glEnd();
		}

	}
	// Add in your OpenGL pre-view code here

	// display draw in progress (mouse is down)
	int start_x = mouseDown[0];
	int start_y = mouseDown[1];
	if (isMouseDown) {
		//Filters
		if (brushUI ->getToolType() == TOOL_FILTER){
			glLineStipple(1, 0xF0F0);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINE_LOOP);
				glVertex2i(mouseDown[0],mouseDown[1]);
				glVertex2i(mouseDrag[0],mouseDown[1]);
				glVertex2i(mouseDrag[0],mouseDrag[1]);
				glVertex2i(mouseDown[0],mouseDrag[1]);
			glEnd();
		}
		//Line
		if (brushUI->getToolType() == TOOL_LINE && brushUI->getRadius() > 1){
			//Starting Coordinates
			//Ending Coordinates
			int cur_x = mouseDrag[0];
			int cur_y = mouseDrag[1];

			//Setting up the color in order to make sure there is no dark black line against black canvas
			if (start_x == cur_x || start_y == cur_y){
				//We have not moved in dx horizontal
				if (cur_y == start_y){
					glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
					glBegin(GL_QUADS);
					glVertex2i(start_x, start_y + brushUI->getRadius()/2);
					glVertex2i(cur_x, cur_y + brushUI->getRadius()/2);
					glVertex2i(cur_x, cur_y - brushUI->getRadius()/2);
					glVertex2i(start_x, start_y - brushUI->getRadius()/2);
					glEnd();
				}
				//We have not moved in dy vertical
				if (cur_x == start_x){
					glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
					glBegin(GL_QUADS);
						glVertex2i(start_x - brushUI->getRadius()/2, start_y);
						glVertex2i(cur_x - brushUI->getRadius()/2, cur_y);
						glVertex2i(cur_x + brushUI->getRadius()/2, cur_y);
						glVertex2i(start_x + brushUI->getRadius()/2, start_y);
					glEnd();
				}
			}
			else{
				glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
				int draw_point0_x = start_y - cur_y;
				int draw_point0_y = cur_x - start_x;

				int r = brushUI->getRadius();
				Find_vertex(start_x,start_y,cur_x, cur_y,brushUI->getRadius());
				glBegin(GL_QUADS);
				//std::cout << "From the GUI: "<< round(vertex[0][0]) << std::endl;
				//std::cout << "From the GUI: " << round(vertex[0][1]) << std::endl;
					glVertex2i(int(round(vertex[0][0])),int(round(vertex[0][1])));
					glVertex2i(int(round(vertex[1][0])), int(round(vertex[1][1])));
					glVertex2i(int(round(vertex[2][0])),int(round(vertex[2][1])));
					glVertex2i(int(round(vertex[3][0])),int(round(vertex[3][1])));
				glEnd();

			//	glBegin(GL_QUADS);
					//glVertex2i();
					//glVertex2i();
					//glVertex2i();
					//glVertex2i();
			//	glEnd();
			}
		}
		if (brushUI->getToolType() == TOOL_LINE && brushUI->getRadius() == 1){
			glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
			glBegin(GL_LINES);
				glVertex2i(start_x, start_y);
				glVertex2i(mouseDrag[0], mouseDrag[1]);
			glEnd();
		}
		if (brushUI->getToolType() == TOOL_CIRCLE)
		{
			//std::cout << "HERE_1" << std::endl;
			const Color colBrush = brushUI->getColor();

			

			int x0 = mouseDown[0];
			int y0 = mouseDown[1];


			int cur_x = mouseDrag[0];
			int cur_y = mouseDrag[1];

			//int count = 1;
			const int radius = brushUI->getRadius();
			if (radius == 1){
				int radius =int(EcludianD(x0, y0, cur_x, cur_y));
				int x = 0;
				int y = radius;

				int x0 = mouseDown[0];
				int y0 = mouseDown[1];

				int deltaE = 2 * x + 3;
				int deltaSE = 2 * (x - y) + 5;

				float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - radius*radius);

				glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
				glBegin(GL_POINTS);
				glVertex2i(x0, y0 + radius);
				glVertex2i(x0 + radius, y0);
				glVertex2i(x0, y0 - radius);
				glVertex2i(x0 - radius, y0);
				glEnd();

				while (y > x){
					if (decision < 0){
						x++;
						decision += deltaE;
						deltaE += 2;
						deltaSE += 2;
					}
					else{
						y--;
						x++;
						decision += deltaSE;
						deltaE += 2;
						deltaSE += 4;
					}
					glBegin(GL_POINTS);
					glVertex2i(x0 + x, y + y0);
					glVertex2i(y + x0, x + y0);
					glVertex2i(-x + x0, y + y0);
					glVertex2i(-y + x0, x + y0);
					glVertex2i(-x + x0, -y + y0);
					glVertex2i(-y + x0, -x + y0);
					glVertex2i(x + x0, -y + y0);
					glVertex2i(y + x0, -x + y0);
					glEnd();
				}
				return;
			}
			if (r_ == -1){
				r_ = radius/2;
				prev_d = 0;
				store = r_;
				cur_d = 0;
			}
			else{
				cur_d = EcludianD(x0,y0,cur_x,cur_y);			
			}
			

			//Size is less than the radius grow the circle 
			if (r_ <= radius){
				if (isMouseDown){
					//std::cout << "HERE_2: " <<  r_ << std::endl;
					r_ = int(store+cur_d);
					prev_d = cur_d;
					int x = 0;
					int y = r_;
					int deltaE = 2 * x + 3;
					int deltaSE = 2 * (x - y) + 5;
						
					float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - r_*r_);

					glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
					glBegin(GL_LINES);
						glVertex2i(x0 + r_, y0);
						glVertex2i(x0 - r_, y0);
					glEnd();

					while (y > x){
						if (decision < 0){
							x++;
							decision += deltaE;
							deltaE += 2;
							deltaSE += 2;
						}
						else{
							y--;
							x++;
							decision += deltaSE;
							deltaE += 2;
							deltaSE += 4;
						}
						glBegin(GL_LINES);
							glVertex2i(x0 + x, y + y0);
							glVertex2i(-x + x0, y + y0);
						glEnd();

						glBegin(GL_LINES);
							glVertex2i(y + x0, x + y0);
							glVertex2i(-y + x0, x + y0);
						glEnd();

						glBegin(GL_LINES);
							glVertex2i(-x + x0, -y + y0);
							glVertex2i(x + x0, -y + y0);
						glEnd();

						glBegin(GL_LINES);
							glVertex2i(-y + x0, -x + y0);
							glVertex2i(y + x0, -x + y0);
						glEnd();
					}
				}
			}


			//Size is greater than the radius 
			else{
				glColor3f(brushUI->getColor()[0] * brushUI->getFlow(), brushUI->getColor()[1] * brushUI->getFlow(), brushUI->getColor()[2] * brushUI->getFlow());
				r_ = store +int(cur_d);
				prev_d = cur_d;
				int inner_r = r_ - radius;
				int outer_r = inner_r + radius;
				std::vector<std::vector<int>> pts(outer_r);
				for (unsigned int i = 0; i < pts.size();i++){
					std::vector<int> temp(2);
					temp[0] = x0;
					temp[1] = (outer_r+x0);
					pts[i] = temp;
				}
				//Outer
				int x = 0;
				int y = outer_r;
				int deltaE = 2 * x + 3;
				int deltaSE = 2 * (x - y) + 5;

				float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - outer_r*outer_r);

				//Inner
				int x_2 = 0;
				int y_2 = inner_r;
				int deltaE_2 = 2 * x_2 + 3;
				int deltaSE_2 = 2 * (x_2 - y_2) + 5;


				float decision_2 = float((x_2 + 1)*(x_2 + 1) + (y_2 - .5)*(y_2 - .5) - inner_r*inner_r);


				glBegin(GL_POINTS);
					//Inner circle
					glVertex2i(x0, y0 + inner_r);
					glVertex2i(x0 + inner_r, y0);
					glVertex2i(x0, y0 - inner_r);
					glVertex2i(x0 - inner_r, y0);
					
					//Outer circle
					glVertex2i(x0, y0 + outer_r);
					glVertex2i(x0 + outer_r, y0);
					glVertex2i(x0, y0 - outer_r);
					glVertex2i(x0 - outer_r, y0);
				glEnd();



				while (y > x){
					//Outer
					if (decision < 0){
						x++;
						decision += deltaE;
						deltaE += 2;
						deltaSE += 2;
					}
					else{
						y--;
						x++;
						decision += deltaSE;
						deltaE += 2;
						deltaSE += 4;
					}
					glBegin(GL_POINTS);
					//Outer circle
						if (y != outer_r){
							if (x0 + x < pts[y][1]){
								pts[y][1] = x0 + x;
							}
						}
						if (y != outer_r){
							if (x0 + y < pts[x][1]){
								pts[x][1] = y + x0;
							}
						}
						glVertex2i(x0 + x, y + y0);
						glVertex2i(-x + x0, y + y0);

						glVertex2i(y + x0, x + y0);
						glVertex2i(-y + x0, x + y0);

						glVertex2i(-x + x0, -y + y0);
						glVertex2i(x + x0, -y + y0);
						glVertex2i(-y + x0, -x + y0);
						glVertex2i(y + x0, -x + y0);
					glEnd();
				}

				while (y_2 > x_2){
					//Inner
					if (decision_2 < 0){
						x_2++;
						decision_2 += deltaE_2;
						deltaE_2 += 2;
						deltaSE_2 += 2;
						if (y != outer_r){
							if (x0 + x_2 > pts[y_2][0]){
								pts[y_2][0] = x0 + x_2;
							}
						}
						if (y != outer_r){
							if (y_2 + x0 > pts[x_2][0]){
								pts[x_2][0] = y_2 + x0;
							}
						}
					}
					else{
						y_2--;
						x_2++;
						decision_2 += deltaSE_2;
						deltaE_2 += 2;
						deltaSE_2 += 4;
					}
					//std::cout <<"x_2: " <<x_2 << " , y_2: "<<y_2 << std::endl;
					if (x0 + x_2 > pts[y_2][0]){
							pts[y_2][0] = x0 + x_2;
					}
					if (y_2 + x0 > pts[x_2][0]){
						pts[x_2][0] = y_2 + x0;
					}
					glBegin(GL_POINTS);
					//Inner circle
						glVertex2i(x0 + x_2, y_2 + y0);
						glVertex2i(-x_2 + x0, y_2 + y0);
						glVertex2i(y_2 + x0, x_2 + y0);
						glVertex2i(-y_2 + x0, x_2 + y0);
						glVertex2i(-x_2 + x0, -y_2 + y0);
						glVertex2i(x_2 + x0, -y_2 + y0);
						glVertex2i(-y_2 + x0, -x_2 + y0);
						glVertex2i(y_2 + x0, -x_2 + y0);
					glEnd();
				}
				pts[0][0] = x0 + inner_r;
				pts[0][1] = x0 + outer_r;
				for (unsigned int idx = 0; idx < pts.size(); idx++){
					if (pts[idx][0] == x0){
						//std::cout << pts[idx][0] << " "<<pts[idx][1] << std::endl;
						int inc = 0;
						int f_draw = x0;
						int r_draw = x0;
						while (f_draw < pts[idx][1]){
							//Quad 1 Top 1/8
							glBegin(GL_POINTS);
								glVertex2i(f_draw, y0+ idx);
							glEnd();
							
							//Quad 2 Bottom 1/8
							glBegin(GL_POINTS);
								glVertex2i(r_draw, y0 + idx);
							glEnd();

							//Quad 3 Bottom 1/8
							glBegin(GL_POINTS);
								glVertex2i(r_draw, y0 - idx);
							glEnd();

							//Quad 4 Top 1/8
							glBegin(GL_POINTS);
								glVertex2i(f_draw, y0 -idx);
							glEnd();

							r_draw--;
							f_draw++;
						}
					}
					else{
						int inc = 0;
						int f_draw = pts[idx][0];
						int distance = f_draw - x0;
						int r_draw = x0 - distance;
						while (f_draw < pts[idx][1]){
							
							//Quad 1 Bottom 1/8
							glBegin(GL_POINTS);
								glVertex2i(f_draw, y0 + idx);
							glEnd();
							
							//Quad 2 Bottom 1/8
							glBegin(GL_POINTS);
								glVertex2i(r_draw, y0 + idx);
							glEnd();

							//Quad 3 Bottom 1/8
							glBegin(GL_POINTS);
								glVertex2i(r_draw, y0 - idx);
							glEnd();

							//Quad 4 Top 1/8
							glBegin(GL_POINTS);
								glVertex2i(f_draw, y0 - idx);
							glEnd();

							r_draw--;
							f_draw++;
						}
					}
				}
			}
		}
	}
	endDrawing();


}

// This does pixel flow 
void MyBrush::draw_callback(void *in_data)
{
	MyBrush *opMe = static_cast<MyBrush *>(in_data);

	// Repeat the time out if we're not done yet
	if (opMe->isMouseDown == true) {
		opMe->drawBrush();

		Fl::repeat_timeout(0.05, MyBrush::draw_callback, (void *)opMe);

		RedrawWindow();
	}
}


int MyBrush::handle(int event) {
	// OpenGL & FLTK's y axes are oriented differently
	const ScreenPoint pt = ScreenPoint(Fl::event_x(), screenHeight - 1 - Fl::event_y());

	switch (event) {
	case FL_PUSH: {
					  mouseDrag = pt;
					  mouseDown = pt;

					  if (brushUI->getToolType() == TOOL_POLYGON) {
						  if (isMouseDown == true) {
							  polygon.push_back(mouseDrag);
						  }
						  else {
							  isMouseDown = true;
							  polygon.resize(0);
							  polygon.push_back(mouseDrag);
						  }
					  }
					  else {
						  isMouseDown = true;
						  if (brushUI->getToolType() == TOOL_BRUSH)
							  Fl::add_timeout(0, draw_callback, this);
					  }
					  return 1;
	}
	case FL_DRAG: mouseDrag = pt; RedrawWindow(); return 1;
	case FL_MOVE:
		mouseDrag = pt;
		if (brushUI->getToolType() == TOOL_BRUSH || (brushUI->getToolType() == TOOL_POLYGON && isMouseDown))
			RedrawWindow();
		return 1;
	case FL_RELEASE: {
						 mouseDrag = pt;
						 if (brushUI->getToolType() != TOOL_POLYGON) {
							 isMouseDown = false;
							 switch (brushUI->getToolType()) {
							 case TOOL_BRUSH:
								 break;
							 case TOOL_LINE:
								 drawLine();
								 break;
							 case TOOL_CIRCLE:
								 drawCircle();
								 break;
							 case TOOL_FILTER:
								 filterRegion();
								 break;
							 default: break;
							 }
						 }
						 else if (Fl::event_button3() || Fl::event_state(FL_SHIFT)) {
							 isMouseDown = false;
							 drawPolygon();
						 }
						 RedrawWindow();
						 return 1;
	}
	default: return 0;
	}
}

