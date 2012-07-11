#ifndef VECTOR_HH_
# define VECTOR_HH_

# include <cassert>
# include <iostream>
# include <cmath>

// vector in a U-dimmension space with T (int, float, ...) coords
template <int U, typename T>
class Vector
{
public:
  // constructors
  Vector ();
  Vector (T v1);
  Vector (T v1, T v2);
  Vector (T v1, T v2, T v3);

  Vector (const Vector<U, T>& v);
  Vector (const Vector<U + 1, T>& v);

  // destructor
  ~Vector ();

  // getter and setter
  T operator() (int pos) const;
  void operator() (int pos, T val);

  void set (T v1, T v2);
  void set (T v1, T v2, T v3);

  // operators
  Vector<U, T> operator+ (const Vector<U, T>& v);
  Vector<U, T> operator- (const Vector<U, T>& v);

  Vector<U, T> operator+ (T val);
  Vector<U, T> operator- (T val);
  Vector<U, T> operator* (T val);
  Vector<U, T> operator/ (T val);
  Vector<U, T> operator% (T val);

  // = operators
  Vector<U, T>& operator= (const Vector<U, T>& v);

  Vector<U, T>& operator+= (const Vector<U, T>& v);
  Vector<U, T>& operator-= (const Vector<U, T>& v);

  Vector<U, T>& operator+= (T val);
  Vector<U, T>& operator-= (T val);
  Vector<U, T>& operator*= (T val);
  Vector<U, T>& operator/= (T val);
  Vector<U, T>& operator%= (T val);

  // comparison operators
  bool operator== (const Vector<U, T>& v);
  bool operator!= (const Vector<U, T>& v);

  // get a vector filled with zero
  static Vector<U, T> zero ();

  // vector norm
  double norme () const;
  // vectorial operation
  Vector<U, T> vectorial (Vector<U, T>& vect);

protected:
  // vector values
  T vect_[U];
};

// print operator
template <int U, typename T>
std::ostream& operator<< (std::ostream& o, const Vector<U, T>& v);
#include "vector.hxx"

// int vectors in 2 and 3 dimmensions
typedef Vector<2, int> Vector2d;
typedef Vector<3, int> Vector3d;

// float vectors in 2 and 3 dimmensions
typedef Vector<2, float> Vector2f;
typedef Vector<3, float> Vector3f;

#endif /// !VECTOR_HH_
