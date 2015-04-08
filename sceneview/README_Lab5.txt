Christopher Ogle
0 Late days used so far.

Two NEW AWESOME FILES added.
	
The first file that I added is SubGraph.h in this file are all my class declarations.
The first class is called Node.
	There is really nothing in Node, its been awhile now, but I think that I made this because I was required to.

The second class is called Tree.
	This class describes the top level of the a mastersubgraph.
		A Tree has variables:
			A name - String 
			Children - A vector of Trans *
		
		A Tree has the following methods:
			getName() - Returns the name
			addTrans(Trans * t)  Allows user to add a Trans 
			getChildList - Returns all Children of the Tree
			printTree() - Prints out info associated with the tree.
			
The third class is called Trans	
	This class describes a trans
		A Trans has the following variables:
			Matrix4 m - This is the Matrix associated with the Trans block
			ObjectKids - This is a vector of the one Object node that the Trans Node might have hanging off it.
			SubGraphKids - This is a vector of the one subgraph node that the Trans might have hanging off it.
				NOTE - All Children are encapsulated in a std::vector for memory management purposes.
		
		A Trans has the following methods:
			getName - Returns the name
			UpdateMatrix - Updates Matrix ma
			AddObjectKid - Pushes onto the ObjectKids  vector the incoming child
			AddSubGraphKid - Pushed on the SubGraphKids vector the incoming child
			getMatrix - Returns the matrix
			printInfo - Prints this Trans' info
			NoSubGraphs - A boolean used for determining if a file can undergo quick loading a particular file
			getTree - returns this tree
			getObject - returns the Object child, used for quick loading.
			
			
The fourth class is called the Object Class
	This class contains all the information for rendering an object. 
	Therefore this class has a lot of getter and setters, because of this I will not go
	into detail of every method since that really would take up a lot of time and a lot of it is
	just getter and setters.
	
The fifth class is called a ShapeContainer Class	
	This class renders the following Shapes Just Once
		Cube
		Cone
		Cylinder
		Sphere 
		Fish - Extra Credit
		Bowl - Extra Credit

Finally the DrawObject struct is at the bottom 
	Used to quickload a graph when the file only contains a root mastersubgraph
	
SubGraph.cpp
	Contains the implementation of a lot of the outlined methods.

	
	
NO KNOWN BUGS, but my file does have prettier colors than the Demo since my tessellation values are higher.
