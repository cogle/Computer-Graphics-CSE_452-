#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
    if (!isLoaded) {
        return;
    }

    // Add your rendering code here.
    // Keep track of your progress as a value between 0 and 1
    // so the progress bar can update as the rendering progresses
    switch (type) {
        case RenderingUI::RENDER_SCANLINE:  break;
        case RenderingUI::RENDER_RAY_TRACING:  break;
        case RenderingUI::RENDER_PATH_TRACING:  break;
        default: break;
    }
}

void MyScene::stopRender()
{
    // Because this is threaded code, this function
    // can be called in the middle of your rendering code.
    // You should then stop at the next scanline
}

double MyScene::getRenderProgress() {
    // return the current progress as a value between 0 and 1
    return 1.0;
}

// add extra methods here

