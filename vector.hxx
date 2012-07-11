#include "vector.hh"

// constructors
template <int U, typename T>
Vector<U, T>::Vector ()
{
  for (int i = 0; i < U; ++i)
    vect_[i] = 0;
}

template <int U, typename T>
Vector<U, T>::Vector (T v1)
{
  vect_[0] = v1;
  for (int i = 1; i < U; ++i)
    vect_[i] = 0;
}

template <int U, typename T>
Vector<U, T>::Vector (T v1, T v2)
{
  vect_[0] = v1;
  vect_[1] = v2;

  for (int i = 2; i < U; ++i)
    vect_[i] = 0;
}

template <int U, typename T>
Vector<U, T>::Vector (T v1, T v2, T v3)
{
  assert (U >= 3);

  vect_[0] = v1;
  vect_[1] = v2;
  vect_[2] = v3;

  for (int i = 3; i < U; ++i)
    vect_[i] = 0;
}

template <int U, typename T>
Vector<U, T>::Vector (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    vect_[i] = v (i);
}

template <int U, typename T>
Vector<U, T>::Vector (const Vector<U + 1, T>& v)
{
  for (int i = 0; i < U; ++i)
    vect_[i] = v (i);
}

// destructor
template <int U, typename T>
Vector<U, T>::~Vector ()
{}

// getter and setter
template <int U, typename T>
T
Vector<U, T>::operator() (int pos) const
{
  return vect_[pos];
}

template <int U, typename T>
void
Vector<U, T>::operator() (int pos, T val)
{
  vect_[pos] = val;
}

template <int U, typename T>
void
Vector<U, T>::set (T v1, T v2)
{
  vect_[0] = v1;
  vect_[1] = v2;
}

template <int U, typename T>
void
Vector<U, T>::set (T v1, T v2, T v3)
{
  assert (U >= 3);

  vect_[0] = v1;
  vect_[1] = v2;
  vect_[2] = v3;
}

// operators
template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator+ (const Vector<U, T>& v)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] + v (i));

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator- (const Vector<U, T>& v)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] - v (i));

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator+ (T val)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] + val);

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator- (T val)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] - val);

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator* (T val)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] * val);

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator/ (T val)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] / val);

  return res;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::operator% (T val)
{
  Vector<U, T> res;

  for (int i = 0; i < U; ++i)
    res (i, vect_[i] % val);

  return res;
}

// = operators

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator= (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    vect_[i] = v (i);

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator+= (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    vect_[i] += v (i);

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator-= (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    vect_[i] -= v (i);

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator+= (T val)
{
  for (int i = 0; i < U; ++i)
    vect_[i] += val;

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator-= (T val)
{
  for (int i = 0; i < U; ++i)
    vect_[i] -= val;

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator*= (T val)
{
  for (int i = 0; i < U; ++i)
    vect_[i] *= val;

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator/= (T val)
{
  for (int i = 0; i < U; ++i)
    vect_[i] /= val;

  return *this;
}

template <int U, typename T>
Vector<U, T>&
Vector<U, T>::operator%= (T val)
{
  for (int i = 0; i < U; ++i)
    vect_[i] %= val;

  return *this;
}

// comparison operators
template <int U, typename T>
bool
Vector<U, T>::operator== (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    if (vect_[i] != v (i))
      return false;

  return true;
}

template <int U, typename T>
bool
Vector<U, T>::operator!= (const Vector<U, T>& v)
{
  for (int i = 0; i < U; ++i)
    if (vect_[i] != v (i))
      return true;

  return false;
}

template <int U, typename T>
Vector<U, T>
Vector<U, T>::zero ()
{
  Vector<U, T> res;

  return res;
}

// vector norm
template <int U, typename T>
double
Vector<U, T>::norme () const
{
  double res = 0;

  for (int i = 0; i < U; ++i)
    res += vect_[i] * vect_[i];

  return sqrt (res);
}

// vectorial operation
template <int U, typename T>
Vector<U, T>
Vector<U, T>::vectorial (Vector<U, T>& vect)
{
  Vector<U, T> res;

  assert (U == 3);

  res (0, vect_[1] * vect (2) - vect_[2] * vect (1));
  res (1, vect (0) * vect_[2] - vect (2) * vect_[0]);
  res (2, vect_[0] * vect (1) - vect_[1] * vect (0));

  return res;
}

template <int U, typename T>
std::ostream& operator<< (std::ostream& o, const Vector<U, T>& v)
{
  o << '[' << v (0);
  for (int i = 1; i < U; ++i)
    o << ", " << v (i);
  o << ']' << std::endl;

  return o;
}
