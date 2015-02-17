#ifndef _MY_BRUSH_H_
#define _MY_BRUSH_H_

#include "cse452.h"
#include "ScreenPoint.h"
#include "UIInterface.h"
#include <vector>




class BrushInterface;
class MyBrush : public UIInterface {
public:
    enum ToolType {
        TOOL_BRUSH = 0,
        TOOL_LINE,
        TOOL_CIRCLE,
        TOOL_POLYGON,
        TOOL_FILTER
    };

    enum FilterType {
        FILTER_BLUR = 0,
        FILTER_SHARPEN,
        FILTER_EDGE
    };
    
    enum BrushType {
        BRUSH_CONSTANT = 0,
        BRUSH_LINEAR,
        BRUSH_QUADRATIC,
        BRUSH_GAUSSIAN,
        BRUSH_SPECIAL
    };


	struct COORDNIATES{
		COORDNIATES(){}
		COORDNIATES(int x, int y){
			_x = y;
			_y = y;
		}

		int _x;
		int _y;
		int _d;

	}coord;

    MyBrush();
    ~MyBrush();

    // Link to the brush widget
    void setUI( const BrushInterface *in_ui ) { brushUI = in_ui; }

    // Inherited from UIInterface
    // Implemented in MyBrush_UI.cpp
    virtual void resize( int width, int height );
    virtual void draw();
    virtual int  handle( int event );

    void loadImage(Fl_Image* image);
    
    // Called by the UI when the brush type or radius changes
    void changedBrush();


private:
    // read a pixel from the canvas, returns (r, g, b) values in the [0,1] range
    Color getPixel(int x, int y );
    // put a pixel on the canvas, (r, g, b) are to be given in the [0,1] range
    void putPixel(int x, int y, const Color &in_col);

    // Pixel and screen data
    std::vector<unsigned char> pixelData;
    int screenWidth, screenHeight;
    int imageWidth, imageHeight;

    // For interactive drawing, set by handle
    // You do not need to set these; the interface for that is
    // already set up
    bool isMouseDown;
    ScreenPoint mouseDown, mouseDrag;
    std::vector< ScreenPoint > polygon;
    static void draw_callback( void *in_data );

    // All of the widget data can be accessed through
    // methods on this class
    const BrushInterface *brushUI;

    // You need to write these
    // Stubs are in MyBrush.cpp
    void drawBrush( );
    void drawLine( );
    void drawCircle( );
    void drawPolygon( );
    void filterRegion( );

    // declare your extra variables/methods here:

	//Methods
	Color fillColor(const int B_type, int x, int y, const float pixelFlow, const Color & colBrush, int wheres);
	

	void OverLay(int x, int y, const float pixelFlow, const Color & colBrush);
	void fillBuffer(int radius, int brush_type);
	void C_Paint(const int radius, const float  pixelFlow, const Color  & color, const int brushType);
	void DrawCircle(const int radius, const float  pixelFlow, const Color  & color);
	void DrawLine( const float  pixelFlow, const Color  & color);
	void DrawThickLine(const float  pixelFlow, const Color  & color);
	void Breeshman(int x0, int y0, int x1, int y1, std::vector<COORDNIATES> & vec ); 
	void Find_vertex(int x0, int y0, int x1, int y1, int r);
	void OrderBy_Y(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2, std::vector<COORDNIATES> & ret);
	void LowerX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2);
	void GreaterX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2);
	void GaussianBlur(); 
	void Apply_Filter(std::vector<std::vector<float>>  & filter);
	void Sharpen();
	void Edge();
	void trade(int & a, int & b);

	void PrintVec(const std::vector<COORDNIATES> & p);

	float distance(int x, int y);
	float LinearEq(float x, int radius);
	float QuadEq(float x, int radius);
	float ColorFormula(int Brush_x,int Brush_y, int Buf_x, int Buf_y, float PixelFlow, float Brush_color, float Canvas_color, bool C);
	float EcludianD(int x0, int y0, int x1, int y1);
	float Gaussian(float x, int radius);


	bool contains(int x, int y, std::vector<COORDNIATES> & cords);
	bool Different_y(int y, std::vector<COORDNIATES>  & cords);
	bool Greatest_Y(int y, std::vector<COORDNIATES>  & cords);
	bool in_bounds(int x, int y);




	//Variables
	
	std::vector<std::vector<float>> buffer;
	std::vector<std::vector<float>> vertex;
	std::vector<std::vector<float>> filter_B;
	std::vector<std::vector<float>> filter_S;
	std::vector<std::vector<float>> filter_E;


	std::vector<COORDNIATES> line_pts;
	std::vector<COORDNIATES> left_line;
	std::vector<COORDNIATES> right_line;
	std::vector<COORDNIATES> top_line;
	std::vector<COORDNIATES> bot_line;


	float prev_d = 0;  
	float cur_d;
	
	int md_x;
	int md_y;
	int r_ = -1; 
	int store;
	float bottomG;
	bool calc_filter = false;
};

inline
Color MyBrush::getPixel( int x, int y ) {
    Color col;
    const int i = (y * imageWidth + x) * 3;
    col[0] = pixelData[i    ] / 255.0f;
    col[1] = pixelData[i + 1] / 255.0f;
    col[2] = pixelData[i + 2] / 255.0f;

    return col;
}

inline
void MyBrush::putPixel(int x, int y, const Color & col) {
    const int i = (y * imageWidth + x) * 3;
    pixelData[i    ] = (unsigned char) (col[0] * 255.0f);
    pixelData[i + 1] = (unsigned char) (col[1] * 255.0f);
    pixelData[i + 2] = (unsigned char) (col[2] * 255.0f);
}

#endif /* _MY_BRUSH_H_ */
