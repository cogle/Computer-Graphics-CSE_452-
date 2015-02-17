Added methods

Color fillColor(const int B_type, int x, int y, const float pixelFlow, const Color & colBrush, int wheres);
    This method is used to "paint" the pixels on the screen when the airbrush is painting. The first
parameter tells me the brush type. I have this so if i see the brush type is constant I can ignore calling
on the buffer. This is done so that the code is slightly more efficient avoiding unnecessary memory look ups.
The second and third are the position of the pixel that is getting "painted" The fourth parameter details
the pixelFlow, the fifth is the user selected color. The final parameter is there for debugging purposes and was
used in debugging.

 void OverLay(int x, int y, const float pixelFlow, const Color & colBrush);
    This method turns on the pixels for those functions that don't use a buffer. I noticed the in the Demo
provided it does not take into consideration the pixel flow when painting the circle, line, etc. I purposely
left the pixel flow in there so that it provides the user with more creative freedom. It is NOT a bug, it was
created with that intention.

voidfillBuffer(int radius, int brush_type);
    Based upon the brush type it fills the buffer with the correct value. Buffer is a a RxR C++ vector.
   
void C_Paint(const int radius, const float  pixelFlow, const Color  & color, const int brushType);
    This method will paint the airbrush onto the canvas. This method is pretty straight forward despite it being
long it uses the midpoint method that was discussed in class. In order to draw the circle I use the fact that a circle
is going to be symmetric around each axis. Thus I store the outer points of the circle in Draw_vec, points encountered
during the drawing of the outer ring of the circle. After storing these points, I extract them from the vector and then
draw starting from the origin(where the user clicks), up to the extracted point. Since the extracted point is a point on
the outer boundary this will fill in all points in between. Reflecting the points into quadrants 2,3, and 4 allow me to
only have to loop through 1/4 of the points.
   
void DrawCircle(const int radius, const float  pixelFlow, const Color  & color);
    This is the method that gets called when the user wants to draw a circle onto the screen. A last minute bug was detected
which prevented the user from properly drawing a circle of radius one. Thus this case is handled separately in the method that
calls this function. There are two cases here, one is when the distance between where the user clicked and the current mouse
position is less than the radius. The other condition that must be considered is when the distance between the mouse and initial
click position is greater than the radius.
    So for the first case the radius of the circle will start out as user selected radius/2. From here we basically apply the circle
algorithm in order to draw the circle on the canvas.
    For the second case its a little more complex, but not too bad. If the user has created a circle and dragged it to where the
current distance is greater than the original radius, I create a ring where the thickness of the ring is the user selected radius. This
is how it appeared to be in the demo and thus this is how I did it.

void DrawLine( const float  pixelFlow, const Color  & color);
    This draws a line of thickness == 1. This is used for the edge case when drawing line and in also helped in visualizing the thick line
which was useful during debugging. 
void DrawThickLine(const float  pixelFlow, const Color  & color);
	This draws the line 
void Breeshman(int x0, int y0, int x1, int y1, std::vector<COORDNIATES> & vec );
   
   
void Find_vertex(int x0, int y0, int x1, int y1, int r);
    This method is called in order to draw a line that has thickness greater than radius one. It is actually called only once and is
called in the MyBrush_UI.cpp. This method finds the pixels that are perpendicular to the line such that they lie at a distance given by the
user selected radius. This method does use sin and cos functions. This is unfortunate, but necessary.
I find these only once every change of the radius or position and then pass these vertices to multiple methods.
So while it is costly to find, they are found once for a given line configuration and then persists, until the line changes.
   
void OrderBy_Y(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2, std::vector<COORDNIATES> & ret);
    This method orders two vectors containing COORDNIATES by their Y value. This is used in the drawing of the line.
The basic reasoning for this is because I combine the 4 lines into two pairs. A left and right half. By ordering by Y
I can then combine the two halves and scan-line between the left and right half.
       
void LowerX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2);
    This is used to prevent double tapping of the pixels when scan-line drawing a line. In addition it provides me with a
way in order to find the lowest x for the right half of the parallelogram. This then is connected and paired up the with
lowest x from the right half. 
   
void GreaterX(std::vector<COORDNIATES> & vec_1, std::vector<COORDNIATES> & vec_2);
	This method is used also to prevent double tapping of the pixels when using the scan line. This function gets the greatest
right hand pixel and then puts this into vector these, will later be used to calculate which pixels to turn on. 

void GaussianBlur(); 
	This method calulates the gaussian of a 3x3 matrix just once. 
	
void Apply_Filter(std::vector<std::vector<float>>  & filter);
	This function applies the selected filter. The Gaussian is applied using the approach we talked about in class, by repeatedly hitting the
pixels with the blur funciton. This creates a drastic effect.

void Edge();
	This is extra credit and is the edge detection method for the filter. 
	
void trade(int & a, int & b);
	Basic swap function used in the line algorithm
	
void PrintVec(const std::vector<COORDNIATES> & p);
	For debugging prints out the vector.
	
float distance(int x, int y);
	
float LinearEq(float x, int radius);
	returns the linear distance from a given point and radius.
	
float QuadEq(float x, int radius);
	returns the quadratic distance from a given point. 
	
float ColorFormula(int Brush_x,int Brush_y, int Buf_x, int Buf_y, float PixelFlow, float Brush_color, float Canvas_color, bool C);
	This calculates the color at a given point for the airbrush given whether it is a constant distribution or some other one.

float EcludianD(int x0, int y0, int x1, int y1);
	This calculates the Ecludian distance, between two points.
	
float Gaussian(float x, int radius);
	This is for the airbrush and calculates the Gaussian, my constant is a low randomly picked variable, I picked it.

bool contains(int x, int y, std::vector<COORDNIATES> & cords);
	Determine if their is a point in the vector already.
	
bool Different_y(int y, std::vector<COORDNIATES>  & cords);
	Makes sure that the y is different 
	
bool Greatest_Y(int y, std::vector<COORDNIATES>  & cords);
	Determine if the y is different.
	
bool in_bounds(int x, int y);
	Checks if the point is in bounds.


There are no known bugs, the code underwent extensive testing to ensure this. 


The mask implementation is as follows:	
	My fillBuffer method takes care of this. I first determine which of the following modes that the user is in, this is in order to correctly determine which 
filter i will be applying. I then use a RxR C++ vector in order store the corresponding value. I must utilize a nested for loop in order to fill all the values. 
I push_back the calculated value of the filter at that point. I rely heavily on the fact that we are dealing with a circle which is a symmetric object. So I only have 
to know 1/4 of the values and then I can apply them to the rest. 

I have done extra credit:
	Gaussian Brush: I implemented the Gaussian brush by using the formula, which can be found on wikipedia. I think that it is permissible in order to use the formula
	from this website. It is implemented just as the formula is stated, on the site.

	Edge Filter: This filter works and finds the edges of a give selected area. 
	Blur Filter: This filter will blur an image. 
	
	Speed up using reflection property of the circle. 
	
I implemented Cirlce and Lines as my methods to be graded.


The draw routine of the Airbrush is of Complexity, O(p^2), where p is the number of pixels, I tried to get this down to linear but was encountering an issue where certain pixels
were being double tapped and thus altering the intensity of them on my screen.
The draw time for Line is the same and circle is the same as well.
Line is a little trickier since it I could not use symmetry arguements. Instead I was forced to construct two subpieces of the parallelogram and then connect these two pieces. 

Usage of vectors prevents me from having to mess with deconstructions since once the vectors are no longer in scope they destroy themselves. Other things I did use was something I called 
COORDINATES this is a struct that I created in order to push into my vectors points that were of use. This allowed me from having to to use 2D arrays for each point. Even if I had used 2D arrays
and pushed them into vectors they still would have destroyed themselves.
This concludes the README