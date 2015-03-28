#ifndef _INTERSECTION_UI_H_
#define _INTERSECTION_UI_H_

#include "../cse452.h"
#include "../shapes/ShapesUI.h"
#include "HitRecord.h"
#include "../UIInterface.h"
#include <FL/Fl_Window.H>
#include "../shapes/Shapes.h"
#include "../shapes/Sphere.h"
#include "../shapes/Cone.h"
#include "../shapes/Cylinder.h"
#include "../shapes/Square.h"
#include "../shapes/Diamond.h"


class IntersectionInterface;
class IntersectionUI : public UIInterface {
public:
    IntersectionUI();
    ~IntersectionUI();

    // Inherited from userInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);

    // Link to the intersection widget
    void setUI( const IntersectionInterface *in_ui ) { intersectionUI = in_ui; }
    void changeShape( ShapesUI::ShapeType type );

    void writeTest();

private:
    const IntersectionInterface *intersectionUI;
    int width, height;

    void drawHits(HitRecord& hr);

    // declare your variables here
	Shape * s;
	ShapesUI::ShapeType cur_type;
	void GetIntersect(ShapesUI::ShapeType t, HitRecord & hr, Point3 P, Vector3 d);


	Sphere * sphere_ptr;
	Cone * cone_ptr;
	Cylinder * cylinder_ptr;
	Square * square_ptr;
	Diamond * diamond_ptr;
};

#endif /* _INTERSECTION_UI_H_ */
