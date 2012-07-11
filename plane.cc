#include "plane.hh"

Plane::Plane (Vector3f& p1, Vector3f& p2, Vector3f& p3)
{
  Vector3f v1 = p2 - p1;
  Vector3f v2 = p3 - p1;
  v1 /= v1.get_norme ();
  v2 /= v2.get_norme ();

  Vector3f normal = v1.vectorial (v2);

  a_ = normal (0);
  b_ = normal (1);
  c_ = normal (2);

  d_ = - (a_ * p1 (0) + b_ * p1 (1) + c_ * p1 (2));
}


float Plane::distanceToPoint (Vector3f& p)
{
  float res = std::abs (a_ * p (0) + b_ * p (1) + c_ * p (2) + d_);
  res /= sqrt (a_ * a_ + b_ * b_ + c_ * c_);
  return res;
}

float Plane::distanceToPoint (const Vector3f& p)
{
  float res = std::abs (a_ * p (0) + b_ * p (1) + c_ * p (2) + d_);
  res /= sqrt (a_ * a_ + b_ * b_ + c_ * c_);
  return res;
}

Vector3f
Plane::intersectionPoint (const Vector3f&  p, const Vector3f& direction)
{
  // Find k to has the final result.
  Vector3f res;
  float k;
  k = - ( a_ * p (0) + b_ * p (1) + c_ * p (2) + d_);
  k /= a_ * direction (0) + b_ * direction (1) + c_ * direction (2);
  res (0, p (0) + k * direction (0));
  res (1, p (1) + k * direction (1));
  res (2, p (2) + k * direction (2));
  return res;
}

Vector3f
Plane::intersectionPoint (Vector3f&  p, Vector3f& direction)
{
  // Find k to has the final result.
  Vector3f res;
  float k;
  k = -(a_ * p (0) + b_ * p (1) + c_ * p(2) + d_);
  k /= a_ * direction (0) + b_ * direction (1) + c_ * direction (2);
  res (0, p (0) + k * direction (0));
  res (1, p (1) + k * direction (1));
  res (2, p (2) + k * direction (2));
  return res;
}
