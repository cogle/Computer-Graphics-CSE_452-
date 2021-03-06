// generated by Fast Light User Interface Designer (fluid) version 1.0107

#ifndef ShapesInterface_h
#define ShapesInterface_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "ShapesUI.h"
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Input.H>

class ShapesInterface {
public:
  Fl_Double_Window* make_window();
  Fl_Double_Window *m_shapesWindow;
  Fl_Choice *m_iShapeType;
  static Fl_Menu_Item menu_m_iShapeType[];
private:
  void cb_Sphere_i(Fl_Menu_*, void*);
  static void cb_Sphere(Fl_Menu_*, void*);
  void cb_Cone_i(Fl_Menu_*, void*);
  static void cb_Cone(Fl_Menu_*, void*);
  void cb_Cylinder_i(Fl_Menu_*, void*);
  static void cb_Cylinder(Fl_Menu_*, void*);
  void cb_Cube_i(Fl_Menu_*, void*);
  static void cb_Cube(Fl_Menu_*, void*);
  void cb_Torus_i(Fl_Menu_*, void*);
  static void cb_Torus(Fl_Menu_*, void*);
  void cb_Diamond_i(Fl_Menu_*, void*);
  static void cb_Diamond(Fl_Menu_*, void*);
  void cb_Bunny_i(Fl_Menu_*, void*);
  static void cb_Bunny(Fl_Menu_*, void*);
  void cb_Ding_i(Fl_Menu_*, void*);
  static void cb_Ding(Fl_Menu_*, void*);
  void cb_Fish_i(Fl_Menu_*, void*);
  static void cb_Fish(Fl_Menu_*, void*);
  void cb_Bowl_i(Fl_Menu_*, void*);
  static void cb_Bowl(Fl_Menu_*, void*);
public:
  Fl_Choice *m_iDisplayType;
  static Fl_Menu_Item menu_m_iDisplayType[];
private:
  void cb_Wireframe_i(Fl_Menu_*, void*);
  static void cb_Wireframe(Fl_Menu_*, void*);
  void cb_Flat_i(Fl_Menu_*, void*);
  static void cb_Flat(Fl_Menu_*, void*);
  void cb_Smooth_i(Fl_Menu_*, void*);
  static void cb_Smooth(Fl_Menu_*, void*);
public:
  Fl_Value_Slider *m_dXRot;
private:
  void cb_m_dXRot_i(Fl_Value_Slider*, void*);
  static void cb_m_dXRot(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dYRot;
private:
  void cb_m_dYRot_i(Fl_Value_Slider*, void*);
  static void cb_m_dYRot(Fl_Value_Slider*, void*);
  void cb_Tessellate_i(Fl_Button*, void*);
  static void cb_Tessellate(Fl_Button*, void*);
public:
  Fl_Value_Input *m_iTessel1;
  Fl_Value_Input *m_iTessel2;
  ShapesInterface();
private:
  ShapesUI myShapes;
public:
  ShapesUI::ShapeType getShapeType() const;
  ShapesUI::DisplayType getDisplayType() const;
  int getTessel1() const;
  int getTessel2() const;
  UIInterface * getUI();
  double getXRot() const;
  double getYRot() const;
};
#endif
