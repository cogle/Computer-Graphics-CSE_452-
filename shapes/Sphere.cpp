#include "Sphere.h"

Sphere::Sphere(std::vector<Vertex_Normal> & shpere){
	//This was developed using the methods from Wikipedia page on http://en.wikipedia.org/wiki/Regular_icosahedron#Spherical_coordinates
		
	//First thing I know is that the icosahedran by itself is just a set of 12 verticies from this we then must connect these 12 
	//in order to make 20 faces. I will do this manually, I can't think of an algorithm to do these connections for me. 
	//According to wikipedia I must make an exhaustive set of points.



	/*Again from the WIKI PAGE*/


	/*So on wikipedia it gives the coordinate system*/
	/* Verticies of the form (0, ±1, ±φ) */
	Vector3 v0 = Vector3(0, One,Phi);
	Vector3 v1 = Vector3(0, One, -Phi);
	Vector3 v2 = Vector3(0, -One, Phi);
	Vector3 v3 = Vector3(0, -One, -Phi);

	/* Verticies of the form (±1, ±φ, 0)*/
	Vector3 v4 = Vector3(One, Phi, 0);
	Vector3 v5 = Vector3(One, -Phi, 0);
	Vector3 v6 = Vector3(-One, Phi, 0);
	Vector3 v7 = Vector3(-One, -Phi, 0);

	/* Verticies of the form (±φ, 0, ±1)*/
	Vector3 v8 = Vector3(Phi, 0, One);
	Vector3 v9 = Vector3(-Phi, 0, One);
	Vector3 v10 = Vector3(Phi, 0, -One);
	Vector3 v11 = Vector3(-Phi, 0,-One);

	/*Now I have the pleasure(painful process) of knitting all these vertices into the 20 faces*/

	//T_0

	/*Top*/
	AddTriangleWithVertexNormal(v6, v0, v4, shpere, 3);
	AddTriangleWithVertexNormal(v6, v4, v1, shpere, 3);
	AddTriangleWithVertexNormal(v6, v1, v11, shpere, 3);
	AddTriangleWithVertexNormal(v6, v11, v9, shpere, 3);
	AddTriangleWithVertexNormal(v6, v9, v0, shpere, 3);

	/*Bot*/
	AddTriangleWithVertexNormal(v10, v8, v5, shpere, 3);
	AddTriangleWithVertexNormal(v3, v10, v5, shpere, 3);
	AddTriangleWithVertexNormal(v7, v3, v5, shpere, 3);
	AddTriangleWithVertexNormal(v2, v7, v5, shpere, 3);
	AddTriangleWithVertexNormal(v8, v2, v5, shpere, 3);
	/*Rest of the shape*/
	AddTriangleWithVertexNormal(v1, v4, v10, shpere, 3);		//Checked
	AddTriangleWithVertexNormal(v1, v10, v3, shpere, 3);		//Checked
	AddTriangleWithVertexNormal(v11, v1, v3, shpere, 3);		//Checked
	AddTriangleWithVertexNormal(v11, v3, v7, shpere, 3);		//Checked
	AddTriangleWithVertexNormal(v9, v11, v7, shpere, 3);		//Checked
	AddTriangleWithVertexNormal(v9, v7, v2, shpere, 3);			//Checked
	AddTriangleWithVertexNormal(v0, v2, v8, shpere, 3);			//Checked
	AddTriangleWithVertexNormal(v9, v2, v0, shpere, 3);			//Checked
	AddTriangleWithVertexNormal(v0, v8, v4, shpere, 3);			//Checked
	AddTriangleWithVertexNormal(v4, v8, v10, shpere, 3);		//Checked
}

Sphere::Sphere(std::vector<Vertex_Normal> & sphere, int numDiv, std::vector<Vertex_Normal> & ret){
	for (int i = 0; i < int(sphere.size()); i++){
			subDivide(sphere[i].points[0], sphere[i].points[1], sphere[i].points[2], numDiv, ret);
	}
}

void Sphere::subDivide(Vector4 v0, Vector4 v1, Vector4 v2, int depth, std::vector<Vertex_Normal> & vec){
	if (depth == 0){
		AddTriangleWithVertexNormal(Vector3(v0[0], v0[1], v0[2]), Vector3(v1[0], v1[1], v1[2]), Vector3(v2[0], v2[1], v2[2]), vec, 3);
		return;
	}
	Vector4 v01 = (v0 + v1);
	Vector4 v12 = (v1 + v2);
	Vector4 v02 = (v0 + v2);
	Vector4 pass_v01 = Vector4(v01[0], v01[1], v01[2],0);
	Vector4 pass_v12 = Vector4(v12[0], v12[1], v12[2], 0);
	Vector4 pass_v02 = Vector4(v02[0], v02[1], v02[2], 0);
	//v01.print();
	pass_v01.normalize();
	pass_v12.normalize();
	pass_v02.normalize();
	pass_v01 *= 0.5;
	pass_v12 *= 0.5;
	pass_v02 *= 0.5;
	depth -= 1;
	subDivide(v0, pass_v01, pass_v02, depth, vec);
	subDivide(v1, pass_v12, pass_v01, depth, vec);
	subDivide(v2, pass_v02, pass_v12, depth, vec);
	subDivide(pass_v01, pass_v12, pass_v02, depth, vec);
}