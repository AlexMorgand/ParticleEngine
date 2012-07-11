#ifndef CAMERA_HH_
# define CAMERA_HH_

# include "vector.hh"
# include <GL/glu.h>

class Camera
{
public:
  static Camera* instanciate ();
  ~Camera ();

  void up ();
  void down ();
  void left ();
  void right ();
  void front ();
  void back ();

  void view ();

  void time_set (float elapsed_time);

  void move (int x, int y);

  int window () { return window_; }
  void window (int win) { window_ = win; }

protected:
  Camera (const Vector3f& vect);

  void calc_vect ();

  Vector3f pos_;
  float theta_;
  float phi_;
  Vector3f front_;
  Vector3f up_;
  Vector3f left_;
  Vector3f target_;
  float elapsed_;
  int prev_x_;
  int prev_y_;

  int window_;

  static Camera* ca_;
};

#endif /// !CAMERA_HH_
