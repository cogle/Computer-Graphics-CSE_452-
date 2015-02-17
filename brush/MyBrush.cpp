#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>

void MyBrush::changedBrush() {
    // this is called anytime the brush type or brush radius changes
    // it should recompute the brush mask appropriately
    const int radius = brushUI->getRadius();
	const int brush_type = brushUI->getBrushType();
	fillBuffer(radius, brush_type);
	Edge();
	
	//Sharpen();
	if (!calc_filter){
		GaussianBlur();
		bottomG = float(float(1.0)/sqrt(M_PI* float(2.0)*.241364));
		calc_filter = true;
	}
}

void MyBrush::drawBrush( ) {
    // apply the current brush mask to image location (x,y)
    // the mouse location is in mouseDrag
    const int radius = brushUI->getRadius();
    const float pixelFlow = brushUI->getFlow();
    const Color colBrush = brushUI->getColor();
	const int brush_type = brushUI->getBrushType();
	C_Paint(radius, pixelFlow, colBrush, brush_type);
}

void MyBrush::drawLine() {
	// draw a thick line from mouseDown to mouseDrag
	// the width of the line is given by the current brush radius
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();
	if (radius == 1){
		DrawLine(brushUI->getFlow(), colBrush);
	}
	else{
		if (mouseDown[0] - mouseDrag[0] != 0 && mouseDown[1] - mouseDrag[1]){
			DrawThickLine(brushUI->getFlow(), colBrush);
		}
		else if (mouseDown[0] == mouseDrag[0] && mouseDown[1] == mouseDrag[1]){
			OverLay(mouseDown[0], mouseDown[1], brushUI->getFlow(), colBrush);
		}
		else{
			int half_r = radius / 2;

			//Vertical line
			if (mouseDown[0]-mouseDrag[0] == 0){
				int start_x, start_y,fin_x,fin_y;
				start_x = mouseDown[0] - half_r;
				fin_x = mouseDown[0] + half_r;
				if (mouseDown[1] < mouseDrag[1]){
					start_y = mouseDown[1];
					fin_y = mouseDrag[1];
				}
				else{
					start_y = mouseDrag[1];
					fin_y = mouseDown[1];
				}
				
				for (int x = start_x; x < fin_x; x++){
					for (int y = start_y; y < fin_y; y++){
						if (in_bounds(x, y)){
							OverLay(x, y, brushUI->getFlow(), colBrush);
						}
					}
				}
			}
			//Horizonatal
			else{
				int start_x, start_y, fin_x, fin_y;
				start_y = mouseDown[1] - half_r;
				fin_y = mouseDrag[1] + half_r;
				if (mouseDown[0] < mouseDrag[0]){
					start_x = mouseDown[0];
					fin_x = mouseDrag[0];
				}
				else{
					start_x = mouseDrag[0];
					fin_x = mouseDown[0];
				}
				for (int x = start_x; x < fin_x; x++){
					for (int y = start_y; y < fin_y; y++){
						if (in_bounds(x, y)){
							OverLay(x, y, brushUI->getFlow(), colBrush);
						}
					}
				}
			}
		}
	}

	line_pts.clear();
	vertex.clear();
	left_line.clear();
	right_line.clear();
	top_line.clear();
	bot_line.clear();
}

void MyBrush::drawCircle() {
    // draw a thick circle at mouseDown with radius r
    // the width of the circle is given by the current brush radius

	
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();
	const float pixelFlow = brushUI->getFlow();

	if (mouseDown[0] != mouseDrag[0] && mouseDown[1] != mouseDrag[1] && radius != 1){
		DrawCircle(radius, pixelFlow, colBrush);
	}
	else if (mouseDown[0] == mouseDrag[0] && mouseDown[1] == mouseDrag[1]){
		OverLay(mouseDown[0],mouseDown[1],pixelFlow,colBrush);
	}
	else{
		int radius = int(EcludianD(mouseDown[0], mouseDown[1], mouseDrag[0], mouseDrag[1]));
		int x = 0;
		int y = radius;

		int x0 = mouseDown[0];
		int y0 = mouseDown[1];

		int deltaE = 2 * x + 3;
		int deltaSE = 2 * (x - y) + 5;

		float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - radius*radius);

		OverLay(x0, y0 + radius,pixelFlow,colBrush);
		OverLay(x0 + radius, y0, pixelFlow, colBrush);
		OverLay(x0, y0 - radius, pixelFlow, colBrush);
		OverLay(x0 - radius, y0, pixelFlow, colBrush);


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

			OverLay(x0 + x, y + y0, pixelFlow, colBrush);
			OverLay(y + x0, x + y0, pixelFlow, colBrush);
			OverLay(-x + x0, y + y0, pixelFlow, colBrush);
			OverLay(-y + x0, x + y0, pixelFlow, colBrush);
			OverLay(-x + x0, -y + y0, pixelFlow, colBrush);
			OverLay(-y + x0, -x + y0, pixelFlow, colBrush);
			OverLay(x + x0, -y + y0, pixelFlow, colBrush);
			OverLay(y + x0, -x + y0, pixelFlow, colBrush);

		}
	}




	//Reset UI variables
	r_ = -1;
}


void MyBrush::drawPolygon() {
    // draw a polygon with numVertices whos coordinates are stored in the
    // polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
}

void MyBrush::filterRegion( ) {
    // apply the filter indicated by filterType to the square
    // defined by the two corner points mouseDown and mouseDrag
    // these corners are not guarenteed to be in any order
    // The filter width is given by the brush radius

	if (brushUI->getFilterType() == FILTER_BLUR){
		Apply_Filter(filter_B);
		return;
	}
	/*if (brushUI->getFilterType() ==  FILTER_SHARPEN){
		std::cout << "Sharpen" << std::endl;
		Apply_Filter(filter_S);
		return;
	}*/
	else{
		Apply_Filter(filter_E);
		return;
	}
}


void MyBrush::fillBuffer(int radius, int brush_type){
	buffer.clear();
	if (brush_type == BRUSH_LINEAR){
		for (int x = 0; x <= radius; x++){
			std::vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(LinearEq(input, radius));
				}
				
			}
			buffer.push_back(temp);
		}
		return;
	}
	if (brush_type == BRUSH_QUADRATIC){
		for (int x = 0; x <= radius; x++){
			std::vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(QuadEq(input, radius));
				}

			}
			buffer.push_back(temp);
		}
		return;
	}
	if (brush_type == BRUSH_GAUSSIAN){
		for (int x = 0; x <= radius; x++){
			std::vector<float> temp;
			for (int y = 0; y <= radius; y++){
				float input = distance(x, y);
				if (input > float(radius)){
					temp.push_back(0.0);
				}
				else{
					temp.push_back(Gaussian(input, radius));
				}

			}
			buffer.push_back(temp);
		}
		return;
	}
}

float MyBrush::distance(int x, int y){
	return float(sqrt(pow(double(x), 2.0) + pow(double(y), 2.0)));
}

float MyBrush::LinearEq(float distance, int radius){
	return (1/float(radius))*(-distance+float(radius));
}


float MyBrush::Gaussian(float distance, int radius){
	float x = float(distance) / float(radius);
	float bot = float(float(2) * pow(.241364, 2));
	float term = x / bot;
	return float(bottomG*exp(term*float(-1.0)));
}

Color MyBrush::fillColor(const int B_type,int x,int y,const float pixelFlow, const Color & colBrush, int wheres =1){
	if (B_type == BRUSH_CONSTANT){
		//Debugging print Really Useful
		/*if (wheres == 2){
			std::cout<<"From 2:" << "(" << x << "," << y << ")" << std::endl;
		}
		if (wheres == 3){
			std::cout << "From 3:" << "(" << x << "," << y << ")" << std::endl;
		}
		if (wheres == 4){
			std::cout << "From 4:" << "(" << x << "," << y << ")" << std::endl;
		}*/
		float r = ColorFormula(x, y, 0, 0, pixelFlow, colBrush[0], getPixel(x, y)[0], true);
		float g = ColorFormula(x, y, 0, 0, pixelFlow, colBrush[1], getPixel(x, y)[1], true);
		float b = ColorFormula(x, y, 0, 0, pixelFlow, colBrush[2], getPixel(x, y)[2], true);

		return Color(r,g,b);
	}
	else{
		int x_idx = x - mouseDrag[0];
		int y_idx = y - mouseDrag[1];
		if (x_idx < 0){
			x_idx *= -1;
		}
		if (y_idx < 0){
			y_idx *= -1;
		}

		float r = ColorFormula(x, y, x_idx, y_idx, pixelFlow, colBrush[0], getPixel(x, y)[0],false);
		float g = ColorFormula(x, y, x_idx, y_idx, pixelFlow, colBrush[1], getPixel(x, y)[1],false);
		float b = ColorFormula(x, y, x_idx, y_idx, pixelFlow, colBrush[2], getPixel(x, y)[2],false);

		return Color(r,g,b);
	}
}

void MyBrush::C_Paint(const int radius, const float pixelFlow, const Color  & colBrush, const int  brushType){
	std::vector<COORDNIATES> Draw_vec;
	std::vector<COORDNIATES> Past_Visit;
	//std::cout << "Max Vertical" << screenHeight << std::endl;
	//Obtaining the mouse's initial (x,y) position.
	int x0 = mouseDrag[0];
	int y0 = mouseDrag[1];
	COORDNIATES start;
	start._x = x0;
	start._y = y0;
	
	int x = 0;
	int y = radius;
	int prev_y = y;
	
	int prev_x = x;

	int deltaE = 2 * x + 3;
	int deltaSE = 2 * (x - y) + 5;
	std::vector<std::vector<int>> BitMap(radius, std::vector<int>(radius));
	

	float decision =float((x + 1)*(x + 1) + (y - .5)*(y - .5) - radius*radius);
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
		//Special
		if (!contains(x + x0, y + y0, Past_Visit)){
			COORDNIATES p;
			p._x = x + x0;
			p._y = y + y0;
			if (x + x0 < screenWidth && y + y0 < screenHeight && x + x0 >= 0 && y + y0 >= 0){
				putPixel(x0 + x, y0 + y, fillColor(brushType, x + x0, y + y0, pixelFlow, colBrush, 2));
			}
			Past_Visit.push_back(p);
			if (Different_y(y + y0, Draw_vec)){
				Draw_vec.push_back(p);
			}
		}

		if (!contains(x0 - x, y0 + y, Past_Visit) && x0 - x < screenWidth && y0 + y < screenHeight && x0 - x >= 0 && y + y0 >= 0){
			putPixel(x0 - x, y0 + y, fillColor(brushType, x0 - x, y0 + y, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 - x;
			p._y = y0 + y;
			Past_Visit.push_back(p);
		}

		if (!contains(x0 + x, y0 - y, Past_Visit) && x0 + x < screenWidth && y0 - y < screenHeight && x0 + x >= 0 && y0 - y >= 0){
			putPixel(x0 + x, y0 - y, fillColor(brushType, x0 + x, y0 - y, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 + x;
			p._y = y0 - y;
			Past_Visit.push_back(p);
		}

		if (!contains(x0 - x, y0 - y, Past_Visit) && x0 - x < screenWidth && y0 - y < screenHeight && x0 - x >= 0 && y0 - y >= 0){
			putPixel(x0 - x, y0 - y, fillColor(brushType, x0 - x, y0 - y, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 - x;
			p._y = y0 - y;
			Past_Visit.push_back(p);
		}


		//Special
		if (!contains(x0 + y, y0 + x, Past_Visit)){
			COORDNIATES p;
			p._x = x0 + y;
			p._y = y0 + x;
			if (x0 + y < screenWidth && y0 + x < screenHeight && x0 + y >= 0 && y0 + x >= 0){
				putPixel(x0 + y, y0 + x, fillColor(brushType, x0 + y, y0 + x, pixelFlow, colBrush, 2));
			}
			if (Different_y(y0 + x, Draw_vec)){
				Draw_vec.push_back(p);
			}
		}
		if (!contains(x0 - y, y0 + x, Past_Visit) && x0 - y >= 0 && y0 + x < screenHeight && x0 - y < screenWidth && y0 + x >= 0){
			putPixel(x0 - y, y0 + x, fillColor(brushType, x0 - y, y0 + x, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 - y;
			p._y = y0 + x;
			Past_Visit.push_back(p);
		}
		if (!contains(x0 + y, y0 - x, Past_Visit) && x0 + y< screenWidth && y0 - x >= 0 && x0 + y >= 0 && y0 - x < screenHeight){
			putPixel(x0 + y, y0 - x, fillColor(brushType, x0 + y, y0 - x, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 + y;
			p._y = y0 - x;
			Past_Visit.push_back(p);
		}
		if (!contains(x0 - y, y0 - x, Past_Visit) && x0 - y >= 0 && y0 - x >= 0 && x0 - y < screenWidth && y0 - x < screenHeight){
			putPixel(x0 - y, y0 - x, fillColor(brushType, x0 - y, y0 - x, pixelFlow, colBrush, 2));
			COORDNIATES p;
			p._x = x0 - y;
			p._y = y0 - x;
			Past_Visit.push_back(p);
		}
	}

	//Drawing 
	unsigned int iter = 0;
	int x_reflect = -1;

	//Draws middle line
	int right_S = x0 + 1;
	int left_S = x0 -1;
	for (int i = start._x; i < x0 + radius;i++){
		if (right_S < screenWidth && y0 < screenHeight && right_S >= 0 && y0 >= 0){
			putPixel(right_S, y0, fillColor(brushType, right_S, y0 , pixelFlow, colBrush,3));
		}

		//Circle Quadrant 2
		if (left_S >= 0 && y0 < screenHeight && left_S < screenWidth && y0 >= 0){
			putPixel(left_S, y0, fillColor(brushType, left_S, y0, pixelFlow, colBrush,3));
		}
		left_S--;
		right_S++;
	}

	//Draws the center dot
	if (x0 >= 0 && x0 < screenWidth && y0 < screenHeight && y0 >= 0){
		putPixel(x0, y0, fillColor(brushType, x0, y0, pixelFlow, colBrush));
	}
	
	
	//Loops through the top 1/4 of the circle and reflects the pieces
	while (iter < Draw_vec.size()){
			
		//Getting the stored coordinates 
		COORDNIATES c = Draw_vec[iter];
		int max_right = c._x;
		int max_left = x0 - (c._x - x0);
		int Y_reflex = y0 - (c._y - y0);

		//draw the middle coordinate then branch out left and right. off the circle
		if (c._y < screenHeight && c._y >=0 && x0 < screenWidth && x0 >= 0){
			putPixel(x0, c._y, fillColor(brushType, x0, c._y, pixelFlow, colBrush, 4));
		}
		if (Y_reflex >= 0 && Y_reflex < screenHeight && x0 < screenWidth && x0 >= 0){
			putPixel(x0, Y_reflex, fillColor(brushType, x0, Y_reflex, pixelFlow, colBrush, 4));
		}
		//std::cout << c._x << "   "<<c._y << std::endl;
		int right_inc =1;
		int left_inc =-1;
		//Start at x0+1 and go until the x0
		for (int Right_iter = 0; x0 + Right_iter < c._x; Right_iter++){
			int Right_half_x = x0 + right_inc;
			int Left_half_x = x0 - right_inc;
			if (c._x != Right_half_x){
				//Circle Quadrant 1 
				if (Right_half_x < screenWidth && c._y < screenHeight  && Right_half_x >= 0 && c._y >= 0){
					//std::cout << "HERE_1 " << std::endl;
					putPixel(Right_half_x, c._y, fillColor(brushType, Right_half_x, c._y, pixelFlow, colBrush, 4));
				}

				//Circle Quadrant 2
				if (Left_half_x >= 0 && Left_half_x < screenWidth && c._y < screenHeight && c._y >= 0){
					//std::cout << "HERE_2" << std::endl;
					putPixel(Left_half_x, c._y, fillColor(brushType, Left_half_x, c._y, pixelFlow, colBrush, 4));
				}

				//Circle Quadrant 3
				if (Left_half_x >= 0 && Left_half_x < screenWidth  && Y_reflex >= 0 && Y_reflex < screenHeight){
					//std::cout << "HERE_3" << std::endl;
					putPixel(Left_half_x, Y_reflex, fillColor(brushType, Left_half_x, Y_reflex, pixelFlow, colBrush, 4));
				}

				//Circle Quadrant 4
				if (Right_half_x < screenWidth && Y_reflex >= 0 && Right_half_x >= 0  && Y_reflex < screenHeight){
					//std::cout << "HERE_4" << std::endl;
					putPixel(Right_half_x, Y_reflex, fillColor(brushType, Right_half_x, Y_reflex, pixelFlow, colBrush, 4));
				}
			}
			
			left_inc--;
			right_inc++;
		}
		iter++;
	}
}


bool MyBrush::contains(int x, int y, std::vector<COORDNIATES> & cords){
	for (unsigned int i = 0; i < cords.size(); i++){
		if (cords[i]._x == x && cords[i]._y == y){
			return true;
		}
	}
	return false;
}

bool MyBrush::Different_y(int y, std::vector<COORDNIATES>  & cords){
	for (unsigned int i = 0; i < cords.size(); i++){
		if (cords[i]._y == y){
			return false;
		}
	}
	return true;
}

float MyBrush::QuadEq(float x, int radius){
	float top_left = x - float(radius);
	float top_right = x + float(radius);
	float combo = float(-1.0*top_left*top_right);
	float bot = float(pow(float(radius),2.0));
	return combo / bot;
}

float MyBrush::ColorFormula(int Brush_x, int Brush_y, int Buf_x, int Buf_y, float PixelFlow, float Brush_color,float Canvas_Color, bool C){
	if (C){
		float lhs = float(Brush_color*PixelFlow);
		float Canvas_inner = float(1.0 - PixelFlow);
		float rhs = float(Canvas_Color*Canvas_inner);
		return lhs + rhs;
	}
	float lhs = float(Brush_color * PixelFlow * buffer[Buf_x][Buf_y]);
	float Canvas_inner = float(1.0 - PixelFlow * buffer[Buf_x][Buf_y]);
	float rhs = float(Canvas_Color*Canvas_inner);
	return lhs + rhs;
}


float MyBrush::EcludianD(int x0, int y0, int x1, int y1){
	float d_x = float(pow(x1 - x0, 2.0));
	float d_y = float(pow(y1 - y0, 2.0));
	float d = float(sqrt(d_x + d_y));
	return d;
}

void MyBrush::OverLay(int x, int y, const float pixelFlow, const Color & colBrush){
	putPixel(x,y, Color(colBrush[0]*pixelFlow,colBrush[1]*pixelFlow,colBrush[2]*pixelFlow));
}

void MyBrush::DrawCircle(const int radius, const float  pixelFlow, const Color  & color){

	int x0 = mouseDown[0];
	int y0 = mouseDown[1];


	int cur_x = mouseDrag[0];
	int cur_y = mouseDrag[1];

	if (r_ == -1 && radius > 1){
		r_ = radius / 2;
		prev_d = 0;
		store = r_;
		cur_d = 0;
	}
	else{
		cur_d = EcludianD(x0, y0, cur_x, cur_y);
	}

	//Size is less than the radius grow the circle 
	if (r_ <= radius){
			r_ = int(store + cur_d);
			prev_d = cur_d;
			int x = 0;
			int y = r_;
			int deltaE = 2 * x + 3;
			int deltaSE = 2 * (x - y) + 5;

			float decision = float((x + 1)*(x + 1) + (y - .5)*(y - .5) - r_*r_);


			//Setting up drawing vector
			std::vector<std::vector<int>> pts(r_);
			for (unsigned int i = 0; i < pts.size(); i++){
				std::vector<int> temp(2);
				temp[0] = x0;
				temp[1] = (r_ + x0);
				pts[i] = temp;
			}


			if (in_bounds(x0 + r_, y0)){
				OverLay(x0 + r_, y0, pixelFlow, color);
			}
			if (in_bounds(x0 - r_, y0)){
				OverLay(x0 - r_, y0, pixelFlow, color);
			}
			if (in_bounds(x0, y0 - r_)){
				OverLay(x0, y0 - r_, pixelFlow, color);
			}
			if (in_bounds(x0, y0 + r_)){
				OverLay(x0, y0 + r_, pixelFlow, color);
			}

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

				if (x + x0 < screenWidth && y + y0 < screenHeight && x + x0 >= 0 && y + y0 >= 0){
					OverLay(x0 + x, y + y0, pixelFlow, color);
				}

				if (x0 - x < screenWidth && y0 + y < screenHeight && x0 - x >= 0 && y + y0 >= 0){
					OverLay(-x + x0, y + y0, pixelFlow, color);
				}

				if (x0 + x < screenWidth && y0 - y < screenHeight && x0 + x >= 0 && y0 - y >= 0){
					OverLay(x + x0, -y + y0, pixelFlow, color);
				}

				if (x0 - x < screenWidth && y0 - y < screenHeight && x0 - x >= 0 && y0 - y >= 0){
					OverLay(-x + x0, -y + y0, pixelFlow, color);
				}

				if (x0 + y < screenWidth && y0 + x < screenHeight && x0 + y >= 0 && y0 + x >= 0){
					OverLay(y + x0, x + y0, pixelFlow, color);
				}

				if (x0 - y >= 0 && y0 + x < screenHeight && x0 - y < screenWidth && y0 + x >= 0){
					OverLay(-y + x0, x + y0, pixelFlow, color);
				}

				if (x0 + y< screenWidth && y0 - x >= 0 && x0 + y >= 0 && y0 - x < screenHeight){
					OverLay(y + x0, -x + y0, pixelFlow, color);
				}

				if (x0 - y >= 0 && y0 - x >= 0 && x0 - y < screenWidth && y0 - x < screenHeight){
					OverLay(-y + x0, -x + y0, pixelFlow, color);
				}

				if (y != r_){
					if (x0 + x < pts[y][1]){
						pts[y][1] = x0 + x;
					}
				}
				if (y != r_){
					if (x0 + y < pts[x][1]){
						pts[x][1] = y + x0;
					}
				}
			}


			pts[0][0] = x0 ;
			pts[0][1] = x0 + r_;

			for (unsigned int idx = 0; idx < pts.size(); idx++){
				if (pts[idx][0] == x0){
					//std::cout << pts[idx][0] << " "<<pts[idx][1] << std::endl;
					int inc = 0;
					int f_draw = x0;
					int r_draw = x0;
					while (f_draw < pts[idx][1]){
						//Quad 1 
						if (f_draw < screenWidth && y0 + idx < screenHeight && f_draw >= 0 && y0 + idx >= 0){
							OverLay(f_draw, y0+idx, pixelFlow, color);
						}

						//Quad 2
						if (r_draw < screenWidth && y0 + idx < screenHeight && r_draw >= 0 && y0 + idx >= 0){
							OverLay(r_draw, y0 + idx, pixelFlow, color);
						}

						//Quad 3
						if (f_draw < screenWidth && y0 - idx < screenHeight && f_draw >= 0 && y0 - idx >= 0){
							OverLay(f_draw, y0 - idx, pixelFlow, color);
						}

						//Quad 4
						if (r_draw < screenWidth && y0 - idx < screenHeight && r_draw >= 0 && y0 - idx >= 0){
							OverLay(r_draw, y0 - idx, pixelFlow, color);
						}
						r_draw--;
						f_draw++;
					}
				}
			}
		}



	//Size is greater than radius
	else{
		r_ = store + cur_d;
		prev_d = cur_d;
		int inner_r = r_ - radius;
		int outer_r = inner_r + radius;
		std::vector<std::vector<int>> pts(outer_r);
		for (unsigned int i = 0; i < pts.size(); i++){
			std::vector<int> temp(2);
			temp[0] = x0;
			temp[1] = (outer_r + x0);
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


		float decision_2 =float( (x_2 + 1)*(x_2 + 1) + (y_2 - .5)*(y_2 - .5) - inner_r*inner_r);


		//Inner circle
		if (in_bounds(x0, y0 + inner_r)){
			OverLay(x0, y0 + inner_r, pixelFlow, color);
		}
		if (in_bounds(x0 + inner_r, y0)){
			OverLay(x0 + inner_r, y0, pixelFlow, color);
		}
		if (in_bounds(x0, y0 - inner_r)){
			OverLay(x0, y0 - inner_r, pixelFlow, color);
		}
		if (in_bounds(x0 - inner_r, y0)){
			OverLay(x0 - inner_r, y0, pixelFlow, color);
		}

		//Outer circle
		if (in_bounds(x0, y0 + outer_r)){
			OverLay(x0, y0 + outer_r, pixelFlow, color);
		}
		if (in_bounds(x0 + outer_r, y0)){
			OverLay(x0 + outer_r, y0, pixelFlow, color);
		}
		if (in_bounds(x0, y0 - outer_r)){
			OverLay(x0, y0 - outer_r, pixelFlow, color);
		}
		if (in_bounds(x0 - outer_r, y0)){
			OverLay(x0 - outer_r, y0, pixelFlow, color);
		}

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

			//Inner
			if (decision_2 < 0 && y_2 > x_2){
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
			else if (y_2 > x_2 && decision_2 >= 0){
				y_2--;
				x_2++;
				decision_2 += deltaSE_2;
				deltaE_2 += 2;
				deltaSE_2 += 4;
			}
			
			//Inner circle array
			if (x0 + x_2 > pts[y_2][0]){
				pts[y_2][0] = x0 + x_2;
			}
			if (y_2 + x0 > pts[x_2][0]){
				pts[x_2][0] = y_2 + x0;
			}

			//Inner circle
			if (in_bounds(x0 + x_2, y_2 + y0)){
				OverLay(x0 + x_2 , y_2 + y0, pixelFlow, color);
			}
			if (in_bounds(-x_2 + x0, y_2 + y0)){
				OverLay(-x_2 + x0, y_2 + y0, pixelFlow, color);
			}
			if (in_bounds(y_2 + x0, x_2 + y0)){
				OverLay(y_2 + x0, x_2 + y0, pixelFlow, color);
			}
			if (in_bounds(-y_2 + x0, x_2 + y0)){
				OverLay(-y_2 + x0, x_2 + y0,pixelFlow, color);
			}
			if (in_bounds(-x_2 + x0, -y_2 + y0)){
				OverLay(-x_2 + x0, -y_2 + y0, pixelFlow, color);
			}
			if (in_bounds(x_2 + x0, -y_2 + y0)){
				OverLay(x_2 + x0, -y_2 + y0, pixelFlow, color);
			}
			if (in_bounds(-y_2 + x0, -x_2 + y0)){
				OverLay(-y_2 + x0, -x_2 + y0, pixelFlow, color);
			}
			if (in_bounds(y_2 + x0, -x_2 + y0)){
				OverLay(y_2 + x0, -x_2 + y0, pixelFlow, color);
			}

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
			//Outer Circle
			if (in_bounds(x0 + x, y + y0)){
				OverLay(x0 + x, y + y0, pixelFlow, color);
			}
			if (in_bounds(-x + x0, y + y0)){
				OverLay(-x + x0, y + y0, pixelFlow, color);
			}
			if (in_bounds(y + x0, x + y0)){
				OverLay(y + x0, x + y0, pixelFlow, color);
			}
			if (in_bounds(-y + x0, x + y0)){
				OverLay(-y + x0, x + y0, pixelFlow, color);
			}
			if (in_bounds(-x + x0, -y + y0)){
				OverLay(-x + x0, -y + y0, pixelFlow, color);
			}
			if (in_bounds(x + x0, -y + y0)){
				OverLay(x + x0, -y + y0, pixelFlow, color);
			}
			if (in_bounds(-y + x0, -x + y0)){
				OverLay(-y + x0, -x + y0, pixelFlow, color);
			}
			if (in_bounds(y + x0, -x + y0)){
				OverLay(y + x0, -x + y0, pixelFlow, color);
			}
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
					if (in_bounds(f_draw,y0+idx)){
						OverLay(f_draw, y0 + idx, pixelFlow, color);
					}
					//Quad 2 Top 1/8
					if (in_bounds(r_draw,y0+idx)){
						OverLay(r_draw, y0+idx, pixelFlow, color);
					}
					//Quad 3 Bottom 1/8
					if (in_bounds(r_draw, y0 - idx)){
						OverLay(r_draw, y0 - idx, pixelFlow, color);
					}
					//Quad 4 Bottom 1/8
					if (in_bounds(f_draw, y0 - idx)){
						OverLay(f_draw, y0 - idx, pixelFlow, color);
					}
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
					if(in_bounds(f_draw, y0 + idx)){
						OverLay(f_draw, y0 + idx,pixelFlow,color);
					}
					
					//Quad 2 Bottom 1/8
					if(in_bounds(r_draw, y0 + idx)){
						OverLay(r_draw, y0 + idx,pixelFlow,color);
					}

					//Quad 3 Bottom 1/8
					if(in_bounds(r_draw, y0 - idx)){
						OverLay(r_draw, y0 - idx,pixelFlow,color);
					}
					
					//Quad 4 Bottom 1/8
					if(in_bounds(f_draw, y0 - idx)){
						OverLay(f_draw, y0 - idx,pixelFlow,color);
					}

					r_draw--;
					f_draw++;
				}
			}
		}
	}//End of else for different radius size
}

bool MyBrush::in_bounds(int x, int y){
	if (x >=0 && x < screenWidth && y>=0 && y < screenHeight){
		return true;
	}
	return false;
}


void MyBrush::DrawLine(const float  pixelFlow, const Color  & color){
	int x0 = mouseDown[0];
	int y0 = mouseDown[1];
	int x1 = mouseDrag[0];
	int y1 = mouseDrag[1];
	if (x0 == x1 && y0 == y1){
		OverLay(x0, y0, pixelFlow, color);
		return;
	}
	if (x0-x1 == 0){
		for (int y = y0; y < y1; y++){
			OverLay(x0,y,pixelFlow,color);
		}
		return;
	}
	if (y0-y1 ==0){
		for (int x = x0; x < x1; x++){
			OverLay(x,y0,pixelFlow,color);
		}
		return;
	}
	if (x0-x1 != 0 || y0-y1 !=0){
		Breeshman(x0, y0, x1, y1,line_pts);
	}
	OverLay(x0,y0, pixelFlow, color);
	for (unsigned int idx = 0; idx < line_pts.size(); idx++){
		if (in_bounds(line_pts[idx]._x, line_pts[idx]._y)){
			OverLay(line_pts[idx]._x, line_pts[idx]._y, pixelFlow, color);
		}
	}
}

void MyBrush::trade(int & a, int & b){
	int temp;
	temp = a;
	a = b;
	b = temp;

}



void MyBrush::Breeshman(int x0, int y0, int x1, int y1, std::vector<COORDNIATES> & vec){

	const bool Greater_than_1 = (abs(y1 - y0) > abs(x1 - x0));
	
	
	
	if (Greater_than_1){
		trade(x0, y0);
		trade(x1, y1);
	}

	if (x0 > x1){
		trade(x0, x1);
		trade(y0, y1);
	}
	
	int dy = y1 - y0;
	if (dy < 1){
		dy *= -1;
	}

	int dx = x1 - x0;
	float error = float(dx / 2);
	int pos_slope;
	if (y0 < y1){
		pos_slope = 1;
	}
	else{
		pos_slope = -1;
	}
	int y = y0;

	for (int x = x0; x < x1; x++)
	{
		if (Greater_than_1)
		{
			COORDNIATES point;
			point._x = y;
			point._y = x;

			vec.push_back(point);
		}
		else
		{
			COORDNIATES point;
			point._x = x;
			point._y = y;
			vec.push_back(point);
		}

		error -= dy;
		if (error < 0)
		{
			y += pos_slope;
			error += dx;
		}
	}


	if (Greater_than_1){
		COORDNIATES point;
		point._x = y;
		point._y = x1;
		vec.push_back(point);
	}
	else{
		COORDNIATES point;
		point._x = x1;
		point._y = y;
		vec.push_back(point);
	}
}

void MyBrush::Find_vertex(int x0, int y0, int x1, int y1, int r){
	vertex.clear();
	//Idea behind this method is to shift the coordinate plane
	//and to have (x0,y0) act as (0,0)
	//std::cout <<"x0: " <<  x0 <<std::endl;
	//std::cout <<"y0: " <<  y0 << std::endl;
	//std::cout <<"x1: " <<  x1 << std::endl;
	//std::cout <<"y1: " << y1 << std::endl;

	int faux_x0 = 0;
	int faux_y0 = 0;
	int faux_x1 = x1 - x0;
	int faux_y1 = y1 - y0;
	
	int lhs, rhs;
	
	int half_r = r/2;

	if (r & 1){
		lhs = rhs = half_r;
	}
	else{
		lhs = half_r; 
		rhs = lhs - 1;
	}
	//std::cout << "faux_x1: " << faux_x1 << std::endl;
	//std::cout << "faux_y1: " << faux_y1 << std::endl;
	
	double my_angle = atan(float(faux_y1) / float(faux_x1));
	
	float lhs_x, lhs_y,rhs_x, rhs_y;

	lhs_x = float(lhs*cos(.5 * M_PI  + my_angle));
	lhs_y = float(lhs*sin(.5 * M_PI + my_angle));
	//std::cout << "LHS_x: " << lhs_x << std::endl;
	//std::cout << "LHS_y: " << lhs_y << std::endl;
	
	rhs_x = float(lhs*cos(.5 * M_PI + my_angle));
	rhs_y = float(lhs*sin(.5 * M_PI + my_angle));
	//std::cout << "LHS_x: " << lhs_x << std::endl;
	//std::cout << "LHS_y: " << lhs_y << std::endl;


	std::vector<float> temp;
	temp.push_back(float(x0) + lhs_x);
	temp.push_back(float(y0) + lhs_y);
	//std::cout << "From the vertex_0: " << float(x0) + lhs_x << std::endl;
	//std::cout << "From the vertex_0: " << float(y0) + lhs_y << std::endl;
	vertex.push_back(temp);

	temp.clear();
	temp.push_back(float(x1) + lhs_x);
	temp.push_back(float(y1) + lhs_y);
	//std::cout << "From the vertex_1: " << float(x1) + lhs_x << std::endl;
	//std::cout << "From the vertex_1: " << float(y1) + lhs_y << std::endl;
	vertex.push_back(temp);

	temp.clear();
	temp.push_back(float(x1) - rhs_x);
	temp.push_back(float(y1) - rhs_y);
	//std::cout << "From the vertex_1: " << float(x1) - rhs_x << std::endl;
	//std::cout << "From the vertex_1: " << float(y1) - rhs_y << std::endl;
	vertex.push_back(temp);

	temp.clear();
	temp.push_back(float(x0) - rhs_x);
	temp.push_back(float(y0) - rhs_y);
	//std::cout << "From the vertex_1: " << float(x0) - rhs_x << std::endl;
	//std::cout << "From the vertex_1: " << float(y0) - rhs_y << std::endl;
	vertex.push_back(temp);
}


void MyBrush::DrawThickLine(const float  pixelFlow, const Color  & color){

	bool trip_wire = false;
	std::vector<COORDNIATES> cur_vec;
	std::vector<COORDNIATES> pair_vec;
	if (round(vertex[0][1]) < round(vertex[1][1])){
		Breeshman(int(round(vertex[0][0])), int(round(vertex[0][1])), int(round(vertex[3][0])), int(round(vertex[3][1])), left_line);
		Breeshman(int(round(vertex[1][0])), int(round(vertex[1][1])), int(round(vertex[2][0])), int(round(vertex[2][1])), right_line);
		Breeshman(int(round(vertex[0][0])), int(round(vertex[0][1])), int(round(vertex[1][0])), int(round(vertex[1][1])), top_line);
		Breeshman(int(round(vertex[3][0])), int(round(vertex[3][1])), int(round(vertex[2][0])), round(vertex[2][1]), bot_line);
		trip_wire = true;
	}
	else{
		Breeshman(int(round(vertex[2][0])), int(round(vertex[2][1])), int(round(vertex[1][0])), int(round(vertex[1][1])), left_line);
		Breeshman(int(round(vertex[3][0])), int(round(vertex[3][1])), int(round(vertex[0][0])), int(round(vertex[0][1])), right_line);
		Breeshman(int(round(vertex[1][0])), int(round(vertex[1][1])), int(round(vertex[0][0])), int(round(vertex[0][1])), top_line);
		Breeshman(int(round(vertex[3][0])), int(round(vertex[3][1])), int(round(vertex[2][0])), int(round(vertex[2][1])), bot_line);
	}
	std::vector<COORDNIATES> left_side;
	std::vector<COORDNIATES> right_side;
	std::vector<COORDNIATES> draw_left;
	std::vector<COORDNIATES> draw_right;

	for (unsigned int i = 0; i < left_line.size(); i++){
		if(in_bounds(left_line[i]._x,left_line[i]._y))OverLay(left_line[i]._x,left_line[i]._y,pixelFlow,color);
	}
	for (unsigned int i = 0; i < right_line.size(); i++){
		if (in_bounds(right_line[i]._x, right_line[i]._y))OverLay(right_line[i]._x, right_line[i]._y, pixelFlow, color);
	}
	for (unsigned int i = 0; i < top_line.size(); i++){
		if (in_bounds(top_line[i]._x, top_line[i]._y))OverLay(top_line[i]._x, top_line[i]._y, pixelFlow, color);
	}
	for (unsigned int i = 0; i < bot_line.size(); i++){
		if (in_bounds(bot_line[i]._x, bot_line[i]._y))OverLay(bot_line[i]._x, bot_line[i]._y, pixelFlow, color);
	}

	OrderBy_Y(left_line,top_line,left_side);
	OrderBy_Y(right_line, bot_line, right_side);
	if (trip_wire){
		if (round(vertex[0][0]) < round(vertex[2][0])){
			LowerX(left_side, draw_left);
			GreaterX(right_side, draw_right);
		}
		else{
			LowerX(right_side, draw_left);
			GreaterX(left_side, draw_right);
		}
	}
	else{
		if (round(vertex[1][0]) < round(vertex[3][0])){
			LowerX(left_side, draw_left);
			GreaterX(right_side, draw_right);
		}
		else{
			LowerX(right_side, draw_left);
			GreaterX(left_side, draw_right);
		}
	}

	int l_iter = 0;
	int r_iter = 0;
	while (true){
		if (l_iter == int(draw_left.size())){
			return;
		}
		int y = draw_left[l_iter]._y;
		int l_x = draw_left[l_iter]._x;
		int r_x = draw_right[r_iter]._x;
		for (l_x; l_x < r_x; l_x++){
			if (in_bounds(l_x, y)){
				OverLay(l_x, y, pixelFlow, color);
			}
		}

		l_iter++;
		r_iter++;
	}

}


void MyBrush::PrintVec(const std::vector<COORDNIATES> & p){
	unsigned int i;
	for (i = 0; i < p.size(); i++){
		std::cout << "x: " << p[i]._x << "  y: " << p[i]._y << std::endl;
	}
	std::cout << std::endl << std::endl;
}

void MyBrush::OrderBy_Y(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2, std::vector<COORDNIATES> & ret){
		for (unsigned int i = 0; i < vec_1.size(); i ++){
			ret.push_back(vec_1[i]);
		}
		for (unsigned int i = 0; i < vec_2.size(); i++){
			ret.push_back(vec_2[i]);
		}
		for (unsigned int i = 0; i < ret.size(); i++){
			for (unsigned int j = i + 1; j < ret.size(); j++){
				if (ret[j]._y < ret[i]._y){
					std::swap(ret[i],ret[j]);
				}
			}
		}


}

void MyBrush::LowerX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2){
	int prev_y = -10000;
	for (unsigned int i = 0; i < vec_1.size(); i++){
		if (vec_1[i]._y == prev_y){
			if (vec_1[i]._x < vec_1[i - 1]._x ){
				vec_1[i - 1]._x = vec_1[i]._x;
			}
		}
		else{
			vec_2.push_back(vec_1[i]);
		}
		prev_y = vec_1[i]._y;
		
	}
}

void MyBrush::GreaterX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2){
	int prev_y = -10000;
	for (unsigned int i = 0; i < vec_1.size(); i++){
		if (vec_1[i]._y == prev_y){
			if (vec_1[i]._x > vec_1[i - 1]._x){
				vec_1[i - 1]._x = vec_1[i]._x;
			}
		}
		else{
			vec_2.push_back(vec_1[i]);
		}
		prev_y = vec_1[i]._y;
	}
}


void MyBrush::GaussianBlur(){
	filter_B.clear();
	
	float Normilization_value = 0.0;
	float half_r = 1;
	float bot = float(M_PI* float(2.0));
	for (int i = 0; i < 3; i++){
		std::vector<float> temp;
		for (int j = 0; j < 3; j++){
			float v = float(exp(-0.5 * (pow((float(i) - half_r), 2.0) + pow((float(j) - half_r), 2.0)))/bot);
			Normilization_value += v;
			temp.push_back(v);
		}
		filter_B.push_back(temp);
	}

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){	
			filter_B[i][j] /= Normilization_value;
			//std::cout << filter_B[i][j] << " ";
		}
		//std::cout << std::endl;
	}

}





void MyBrush::Edge(){
	filter_E.clear();
	int radius = 2 * brushUI->getRadius() + 1;
	int count = 0;
	for (int i = 0; i < radius; i++){
		std::vector<float> temp;
		for (int j = 0; j < radius; j++){
			if (i == radius / 2 && j == radius / 2){
				count++;
				temp.push_back(float(float(4.0)/float(5.0)));
				continue;
			}
			if (i == radius / 2 || j == radius / 2){
				count++;
				temp.push_back(float(float(-1.0) / float(5.0)));
				continue;
			}
			temp.push_back(0.0);
		}
		filter_E.push_back(temp);
	}
	filter_E[radius / 2][radius / 2] = float(count - float(1.0));
	for (int i = 0; i < radius; i++){
		for (int j = 0; j < radius; j++){
			filter_E[i][j] /= count;
		}
		std::cout << std::endl;
	}
}

void MyBrush::Apply_Filter(std::vector<std::vector<float>>  & filter){
	int start_x, start_y, fin_x, fin_y;
	
	//Set up x and y
	if (mouseDown[0] < mouseDrag[0]){
		start_x = mouseDown[0];
		fin_x = mouseDrag[0];
	}
	else{
		start_x = mouseDrag[0];
		fin_x = mouseDown[0];
	}

	if (mouseDown[1] < mouseDrag[1]){
		start_y = mouseDown[1];
		fin_y = mouseDrag[1];
	}
	else{
		start_y = mouseDrag[1];
		fin_y = mouseDown[1];
	}

	//Gives the pixels to the left and right of the current filter
	int left_right;
	int start;
	int outter;
	if (brushUI->getFilterType() == FILTER_BLUR){
		left_right = 1;
		start = 0 - left_right;
		outter = brushUI->getRadius();
	}
	else{
		left_right = brushUI->getRadius();
		start = 0 - left_right;
		outter = 1;
	}
	for (int iter = 0; iter < outter; iter++){
		for (int i = start_x; i < fin_x; i++){
			for (int j = start_y; j < fin_y; j++){
				float r_sum = 0.0;
				float g_sum = 0.0;
				float b_sum = 0.0;


				for (int f_i = start; f_i <= left_right; f_i++){
					for (int f_j = start; f_j <= left_right; f_j++){
								if (!in_bounds(i + f_i, j + f_j)){
									r_sum += 0.0;
									b_sum += 0.0;
									g_sum += 0.0;
								}
								else{
									Color temp = getPixel(i + f_i, j + f_j);
									r_sum += filter[left_right + f_i][left_right + f_j] * temp[0];
									g_sum += filter[left_right + f_i][left_right + f_j] * temp[1];
									b_sum += filter[left_right + f_i][left_right + f_j] * temp[2];

								}
					}
				}

				if (in_bounds(i, j)){
					//std::cout << "r: " << r_sum<<std::endl;
					//std::cout << "g: " << g_sum << std::endl;
					//std::cout << "b: " << b_sum<< std::endl;
					if (int(round(r_sum)) < 0 ){
						r_sum = float(0);
					}
					if (int(round(g_sum)) < 0){
						g_sum = float(0);
					}
					if (int(round(b_sum)) < 0){
						b_sum = float(0);
					}
					if (int(round(r_sum)) > 1){
						r_sum = float(1);
					}
					if (int(round(g_sum)) > 1){
						g_sum = float(1);
					}
					if (int(round(b_sum)) > 1){
						b_sum = float(1);
					}
					putPixel(i, j, Color(r_sum, g_sum, b_sum));
				}

			}
		}
	}
}

/*

if (brushUI->getFilterType() == FILTER_SHARPEN){
if (!in_bounds(i + f_i, j + f_j)){
r_sum += 0.0;
b_sum += 0.0;
g_sum += 0.0;
}
if (i != i+f_i &&  j != j+f_j){
Color temp = getPixel(i + f_i, j + f_j);
r_sum += temp[0];
g_sum += temp[1];
b_sum += temp[2];
}
}*/