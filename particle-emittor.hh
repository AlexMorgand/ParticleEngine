#ifndef PARTICLES_EMITTOR_HH
# define PARTICLES_EMITTOR_HH

# include <list>
# include <vector>
# include "vector.hh"
# include "plane.hh"

class Particle;

class ParticleEmittor
{
public:
  ParticleEmittor(int nbPart, std::list<Plane> walls, std::string type = "explosion");
  virtual void initParticles() = 0;

  // Getters.
  int nbPart() { return nbPart_; }
  std::string type() { return type_; }
  std::string etype() { return etype_; }
  void wall_collision(Particle* p);

  // Setters.
  void type (std::string type) { type_ = type; }

  double t_;
protected:
  int nbPart_;
  std::string type_;
  std::string etype_;
  static ParticleEmittor* pe_;
  std::list<Plane> walls_;
};

class ProgressiveEmittor : public ParticleEmittor
{
public:
  ProgressiveEmittor(int nbPart, std::list<Plane> walls, std::string type = "explosion");
  void initParticles();

  // Getters.
  std::list<Particle*>& pvpart() { return pvpart_; }
  int partProd() { return partProd_; }
  void partProd(int partProd) { partProd_ = partProd; }

private:
  std::list<Particle*> pvpart_;
  int partProd_;
};

class ImmediateEmittor : public ParticleEmittor
{
  public:
    ImmediateEmittor(int nbPart, std::list<Plane> walls, std::string type = "explosion");
    void initParticles();
    void initParticles(Vector3f pos);
    void vpart (int index, Particle* val) { vpart_[index] = val; }
    std::vector<Particle*> vpart () { return vpart_; }
  private:
    std::vector<Particle*> vpart_;
};

# include "particle.hh"

#endif
