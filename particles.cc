#include "particles.hh"

ParticleEngine* ParticleEngine::pe_ = 0;

ParticleEngine* ParticleEngine::instanciate()
{
  if (!pe_)
    pe_ = new ParticleEngine(500, explosion);

  return pe_;
}

ParticleEngine::ParticleEngine(int nbPart, int type)
  : t_ (0),
    vpart_ (nbPart),
    nbPart_ (nbPart),
    type_ (type)

{
}

// FIXME: constructor must handle image.
Particle::Particle(/*Image img,*/ int r, int g, int b,
                   float x, float y, float z, float angle)
  : r_(r),
    g_(g),
    b_(b),
    x_(x),
    y_(y),
    z_(z),
    vx_(0),
    vy_(0),
    vz_(0),
    angle_(angle),
    // FIXME: Put it in parameter.
    lifeRemaining_(100),
    life_(100),
    isAlive_(true)
{
}

void Particle::resetParticle ()
{
  ParticleEngine* pe = ParticleEngine::instanciate();

  isAlive_ = true;
  lifeRemaining_ = life_;
  if (pe->type() == explosion)
  {
    vx_ = (float) (rand() % 2000 - 1000) / 1000;
    vy_ = (float) (rand() % 2000 - 1000) / 1000;
    vz_ = (float) (rand() % 2000 - 1000) / 1000;
  }
  else if (pe->type() == nova)
  {
    static size_t angle = 0;
    vx_ = 0.5;
    vy_ = 0.5;

    float tmp = vx_;
    vx_ *= vx_ * cos(angle) - vy_ * sin(angle);
    vy_ *= tmp * sin(angle) + vy_ * cos(angle);
    angle += 5;
  }
}

