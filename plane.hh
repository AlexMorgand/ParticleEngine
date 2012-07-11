#ifndef PLANE_HH_
# define PLANE_HH_

# include "vector.hh"
# include <math.h>

class Plane
{
public:
  Plane (Vector3f& p1, Vector3f& p2, Vector3f& p3);

  float a () const { return a_; }
  float b () const { return b_; }
  float c () const { return c_; }
  float d () const { return d_; }
  float distanceToPoint (Vector3f& p);
  float distanceToPoint (const Vector3f& p);
  Vector3f intersectionPoint (Vector3f& p, Vector3f& direction);
  Vector3f intersectionPoint (const Vector3f& p, const Vector3f& direction);

protected:
  float a_;
  float b_;
  float c_;
  float d_;
};

#endif /// !PLANE_HH_
