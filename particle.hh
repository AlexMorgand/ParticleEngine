#ifndef PARTICLES_HH
# define PARTICLES_HH

# include "tools.hh"
# include <map>
# include "particle-emittor.hh"
# include "particle-emittor-para.hh"
# include <tbb/task.h>

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
  float life() { return life_; }
  float lifeRemaining() { return lifeRemaining_; }
  void lifeRemaining(float lr) { lifeRemaining_ = lr; }
  bool isAlive() { return isAlive_; }
  void isAlive(bool state) { isAlive_ = state; }
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

class ParticleEngine // : public tbb::task
{
public:
  ParticleEngine();
  ~ParticleEngine();
  void parallel (bool para);
  void initParticles ();
  void update(float elaspedTime);
  void removeDeadEmittor();
  int addEmittor(ParticleEmittor* pe);
  void delEmittor(int pe);
  std::map<int, ParticleEmittor*>* lpe() { return lpe_; };
  std::list<Plane> walls() { return walls_; }

  // task method
  // tbb::task* execute();
  // void elapsed (float elapsed) { ela_ = elapsed; }

private:
  std::map<int, ParticleEmittor*>* lpe_;
  int cpt_;
  std::list<Plane> walls_;
  bool para_;
  float ela_;
};

#endif
