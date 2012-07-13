#ifndef PARTICLES_HH
# define PARTICLES_HH

# include "tools.hh"
# include <map>
# include <list>
# include "plane.hh"
# include "vector.hh"
# include "particle-emittor-para.hh"

# define PI 3.14159265
# define WSIZE 1000

// FIXME: think about better arguments.
class Particle
{
public:
  Particle(int r, int g, int b, float x, float y, float z, float angle, std::string type);
  void resetParticle();
  float angle() { return angle_; }
  Vector3f& pos() { return pos_; }
  Vector3f& rgb() { return rgb_; }
  Vector3f& origpos() { return origpos_; }
  Vector3f& v() { return v_; }
  float lifeRemaining() { return lifeRemaining_; }
  void lifeRemaining(float lr) { lifeRemaining_ = lr; }
private:
  Vector3f rgb_;
  Vector3f origpos_;
  Vector3f pos_;
  Vector3f v_;
  float angle_;
  float lifeRemaining_;
  float life_;
  bool isAlive_;
  std::string type_;
};

class ParticleEmittor
{
public:
  // Constructor.
  ParticleEmittor(int nbPart, std::list<Plane> walls, std::string type = "explosion");

  // Getters.
  int nbPart () { return nbPart_; }
  std::string type () { return type_; }
  void vpart (int index, Particle* val) { vpart_[index] = val; }
  std::vector<Particle*> vpart () { return vpart_; }

  // Setters.
  void type (std::string type) { type_ = type; }

  // FIXME: put it in private.
  double t_;
private:
  std::vector<Particle*> vpart_;
  int nbPart_;
  std::string type_;
  static ParticleEmittor* pe_;
  std::list<Plane> walls_;
};

class ParticleEngine
{
public:
  ParticleEngine();
  ~ParticleEngine();
  void initParticles ();
  void update(float elaspedTime);
  void wall_collision(Particle* p);
  int addEmittor(ParticleEmittor* pe);
  void delEmittor(int pe);
  std::map<int, ParticleEmittor*>* lpe() { return lpe_; };
  std::list<Plane> walls() { return walls_; }

private:
  std::map<int, ParticleEmittor*>* lpe_;
  int cpt_;
  std::list<Plane> walls_;
  ParticleEmittorPara* para_pe_;
};

#endif
