#include "../cse452.h"
#include "MyScene.h"

void MyScene::resize(int w, int h) {
    // resize the film plane to the specified width/height
    camera.setWidthHeight(w, h);
}

/// Note: your camera and screen clear, etc, will be set up by
/// SceneviewUI.cpp *before* this gets called
void MyScene::draw() {
	
    // render the scene using OpenGL
    if (!isLoaded) // Don't draw if loadSceneFile hasn't been called yet
        return;


    // Turn off all lights
    for ( int i = 0; i < 7; i++ )
        glDisable( GL_LIGHT0 + i );

    //  .. and reset
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, &ambientLight[0] );
    for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i].setOpenGLLight( GL_LIGHT0 + i );
    }

    // TODO: draw the rest of the scene here
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (loaded){
		glCallList(the_scene);
	}
	else{
		glDeleteLists(the_scene, 1);
		the_scene = glGenLists(1);
		glNewList(the_scene, GL_COMPILE);
			if (!added_trans){
				for (int i = 0; i < int(quick_load.size()); i++){
					glPushMatrix();
					glMultMatrixd(&(quick_load[i].m(0, 0)));
					quick_load[i].o->drawObject();
					glPopMatrix();
				}
			}

			else{
				Tree *  root = master_subgraphs["root"];
				for (int i = 0; i < int(root->getChildList().size()); i++){
					Trans * t = root->getChildList()[i];
					RecursiveDescent(t, t->getMatrix());
				}
			}
		glEndList();
		loaded = true;
	}
	//
	
	/*if (!added_trans){
		for (int i = 0; i < int(quick_load.size()); i++){
			glPushMatrix();
			glMultMatrixd(&(quick_load[i].m(0, 0)));
			quick_load[i].o->drawObject();
			glPopMatrix();
		}
	}

	else{
		Tree *  root = master_subgraphs["root"];
		for (int i = 0; i < int(root->getChildList().size()); i++){
			Trans * t = root->getChildList()[i];
			RecursiveDescent(t, t->getMatrix());
		}
	}*/

	
	


	/*
	for (auto ele : to_be_drawn){
		glPushMatrix();
			glMultMatrixd(&(ele.m(0, 0)));
			ele.o->drawObject();
		glPopMatrix();
	}*/
	glDisable(GL_BLEND);
	
}
