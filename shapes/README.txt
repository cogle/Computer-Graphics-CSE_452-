README
Christopher Ogle
Computer Graphics Lab 2 

Description and Listing of additional .cpp/.h files

Shape.h
    In this file there are variables that and structs that help provide functionality
to the project as a whole. In addition all the function declarations for the Shape are here.
The variables ad data structures that I use are either primitive types or those that do not require
deconstruction through a deconstructor. They are handled through C++, and I don't have to worry about them.

Shapes.cpp
    In this file there are all the vital function declarations for the Shape Object, which in my program acts like a base class
and the children class are specific shapes. General functions such as finding the normal or creating a vector of circles for the base of
the cylinder or cone. Included are some specific functions for the Torus, but that was really an after thought.

The draw method here is a crucial method. It handles the rendering of all object in a unique if statement. While this is verbose it does provide for easier debugging. 

Sphere.h
    This contains the basic outline for the sphere. I tesselated the sphere using the icosahedron, which was a learning experience in geometry.

Sphere.cpp   (Extra Credit method!!!)
    This contains all the methods for the construction of the sphere which is tesselated using the icosahedron method. All the information for the icosahedron can be found
on the wikipedia link provided in the code. This was the basis behind how I tesselated my shape. In addition I used a recursive subdivide in order to render
shapes with higher tesselation. This recursive subdivide method is based off of how one would create the Sierpinski triangle. Which we kind of discussed in class. My max for the sphere is 9.

Cone.h
    This contains all the variables and function declarations for the Cone.

Cone.cpp
    This contains all the methods needed to render a cone. What I do is in order to render the Cone I create a base circle based upon the current radial division of the circle. Then I use the Matrix4 c
class in order to move these points up a height determined by the height parameter. Then connecting triangles I move up the side of the cylinder until top has been reached.
   
Square.h
	Simply contains the outline for the Cube shape.

Square.cpp
	In this file we render and create the Square. The square is created by calculating 3 of the 6 sides and then applying symmetry in order to get the other 3 sides. The rendering of this is done in
the Shape file.

Torus.h
	This contains the methods needed in order to render a Torus! Which is a really pretty shape, in the wire frame mode. 

Torus.cpp
    This is my extra credit!!! I rendered a Torus using triangles. The manner in which I thought of it was to use two parameters. The first controls the number of subdivision of the
circle that makes up the ring. The second parameter is how many circles appear in between 90 degrees. I did this because since it is extra credit I feel that I have some
creative freedom on how I render the object. So therefore the default and smallest object that one could have is basically a square with a hole in the middle, sort of like a window frame.


This assignment is much more organized than my last one. The class structure I feel is very neat and compact, and tries to adhere to the Object Oriented Programming paradigms. 

There are currently no know bugs. Ther might be slight discrepencies in the shading but, everything renders correctly as in there are no missing edges or edges that are visible but should not be.  

In order to speed up the program Sphere I sacrificed space complexity, in order to reduce my time complexity. What I do is I store my previously generated Spheres in a Hash Map, then when it comes time
to generate the next level I attempt to find the closest sphere to that size and start my recurssive subdivide algorithm from this point, instead of generating a new sphere. The max for this on the CEC 
machine is 9, but on a faster machine it might be able to do more. It really comes down to how many points can we store in the vector. In addition I used the icosahedron in order to render my sphere which was 
an interesting challenge. Overall the rendering wasn't too bad, but the calculation of the 20 faces was a terrible.

Torus, I generated this shape by looking at the wikipedia page and then determining the parameters that my torus would take. This was easier than the circle and pretty fun. The size is a little larger than the rest
of the shapes, but I just played with the values until I found a size that really demonstrated and rendered the shape in a visually pleasing manner. 
