#ifndef VECTOR3
#define VECTOR3

class Vector3 {
public:
  float x, y, z;

  // Construtores
  Vector3();
  Vector3(float x, float y, float z);

  // Métodos
  float length() const;
  void clamp();

  // Operadores
  Vector3 operator+(const Vector3 &o) const;
  Vector3 operator-(const Vector3 &o) const;
  Vector3 operator*(float escalar) const;
  Vector3 operator/(float escalar) const;

  Vector3 &operator+=(const Vector3 &o);
  Vector3 &operator-=(const Vector3 &o);
  Vector3 &operator*=(float escalar);
  Vector3 &operator/=(float escalar);
};

Vector3 normalize(Vector3& o);
float dot(const Vector3 &o, const Vector3 &a);
Vector3 reflect(const Vector3 &n, const Vector3 &l);

#endif
