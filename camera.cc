#include "camera.hh"
#include <cmath>

Camera*
Camera::instanciate ()
{
  if (!ca_)
    ca_ = new Camera (Vector3f (0, -2, 20));

  return ca_;
}

Camera::Camera (const Vector3f& vect)
  : pos_ (vect)
  , theta_ (0)
  , phi_ (-89)
  , front_ (Vector3f (0, 1, 0))
  , up_ (Vector3f (0, 0, 1))
  , left_ (Vector3f (-1, 0, 0))
  , target_ (pos_ + front_)
  , elapsed_ (0)
  , prev_x_ (-1)
  , prev_y_ (-1)
{
  calc_vect ();
  target_ (0, 0);
  target_ (1, 0);
  target_ (2, 0);
}

Camera::~Camera ()
{
}

void
Camera::up ()
{
  pos_ += up_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::down ()
{
  pos_ -= up_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::left ()
{
  pos_ += left_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::right ()
{
  pos_ -= left_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::front ()
{
  pos_ += front_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::back ()
{
  pos_ -= front_ * elapsed_ * 100;
  target_ = pos_ + front_;
}

void
Camera::time_set (float elapsed_time)
{
  elapsed_ = elapsed_time;
}

void
Camera::move (int x, int y)
{
  if (prev_x_ == -1 && prev_y_ == -1)
    {
      prev_x_ = x;
      prev_y_ = y;
    }
  else
    {
      theta_ -= (x - prev_x_);
      phi_ -= (y - prev_y_);

      calc_vect ();
      prev_x_ = x;
      prev_y_ = y;
    }
  
}

void
Camera::calc_vect ()
{

  if (phi_ > 89)
    phi_ = 89;
  else if (phi_ < -89)
    phi_ = -89;

  float r_temp = cos (phi_ * M_PI / 180);

  front_ (0, r_temp * cos (theta_ * M_PI / 180));
  front_ (1, r_temp * sin (theta_ * M_PI / 180));
  front_ (2, sin (phi_ * M_PI / 180));

  Vector3f le (up_.vectorial (front_));
  left_ = le;

  left_ /= left_.norme ();
  front_ /= front_.norme ();

  target_ = pos_ + front_;
}

void
Camera::view ()
{
  gluLookAt (pos_ (0), pos_ (1), pos_ (2),
	     target_ (0), target_ (1), target_ (2),
	     0, 0, 1);
}
