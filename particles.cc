#include "particles.hh"

ParticleEngine::ParticleEngine()
  : lpe_ (0),
    cpt_ (0),
    walls_ ()
{
  lpe_ = new std::map<int, ParticleEmittor*>();
  Vector3f w1 = Vector3f:: zero ();
  Vector3f w2 = Vector3f (100, 0, 0);
  Vector3f w3 = Vector3f (0, 100, 0);
  Vector3f w4 = Vector3f (0, 0, 100);
  Vector3f w5 = Vector3f (100, 100, 0);
  Vector3f w6 = Vector3f (100, 0, 100);
  Vector3f w7 = Vector3f (0, 100, 100);
  walls_.push_back (Plane (w1, w2, w3));
  walls_.push_back (Plane (w1, w2, w4));
  walls_.push_back (Plane (w1, w3, w4));
  walls_.push_back (Plane (w5, w2, w6));
  walls_.push_back (Plane (w3, w5, w7));
  walls_.push_back (Plane (w4, w6, w7));
}

void ParticleEngine::update(float elapsedTime)
{
  for (std::map<int, ParticleEmittor*>::iterator it = lpe()->begin();
      it != lpe()->end(); ++it)
  {
    ParticleEmittor* p = it->second;
    spin += 0.01f;
    for (int i = 0; i < p->nbPart(); i++)
    {
      p->vpart()[i]->r_ = rand() % 256;
      p->vpart()[i]->g_ = rand() % 256;
      p->vpart()[i]->b_ = rand() % 256;

      if (p->type () == "explosion")
      {
        p->vpart()[i]->x_ += p->vpart ()[i]->vx_ * elapsedTime;
        p->vpart()[i]->y_ += p->vpart ()[i]->vy_ * elapsedTime;
        p->vpart()[i]->z_ += (p->vpart ()[i]->vz_ /*- GRAVITY*/) * elapsedTime;
      }
      else if (p->type() == "nova")
      {
        p->vpart()[i]->x_ += p->vpart ()[i]->vx_ * elapsedTime;
        p->vpart()[i]->y_ += p->vpart ()[i]->vy_ * elapsedTime;
        p->vpart()[i]->z_ += p->vpart ()[i]->vz_ * elapsedTime;
      }
      else if (p->type() == "circle")
      {
        // FIXME: do a dispatcher for different patterns.
        p->vpart ()[i]->x_ = 3 * sin (p->t_ + i);
        p->vpart ()[i]->z_ = 2 * sin (2 * p->t_ + i);
      }

      // Handle remaining life.
      p->vpart ()[i]->lifeRemaining_-= elapsedTime;
      if (p->vpart ()[i]->lifeRemaining_ < 0)
        p->vpart ()[i]->resetParticle();
    }
  }
}

int ParticleEngine::addEmittor(ParticleEmittor* pe)
{
  int res = cpt_;
  lpe_->insert(std::pair<int, ParticleEmittor*>(cpt_, pe));
  ++cpt_;
  return res;
}

void ParticleEngine::delEmittor(int pe)
{
  lpe_->erase(pe);
}

ParticleEmittor::ParticleEmittor(int nbPart, std::string type)
  : t_ (0),
    vpart_ (nbPart),
    nbPart_ (nbPart),
    type_ (type)

{
}

// FIXME: init only ONE emittor, not EVERY emittor.
void
ParticleEngine::initParticles()
{
  for (std::map<int, ParticleEmittor*>::iterator it = lpe_->begin(); it != lpe_->end(); ++it)
  {
    ParticleEmittor* pe = it->second;
    for (int i = 0; i < pe->nbPart(); i++)
    {
      Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 0, 0, 0, 0, pe->type());
      p->resetParticle ();
      pe->vpart(i, p);
    }
  }
}

// FIXME: constructor must handle image.
Particle::Particle(int r, int g, int b,
                   float x, float y, float z, float angle, std::string type)
  : r_(r),
    g_(g),
    b_(b),
    origx_ (x),
    origy_ (y),
    origz_ (z),
    x_(x),
    y_(y),
    z_(z),
    vx_(0),
    vy_(0),
    vz_(0),
    angle_(angle),
    // FIXME: Put it in parameter.
    lifeRemaining_(1000),
    life_(1000),
    isAlive_(true),
    type_ (type)
{
}

void Particle::resetParticle ()
{
  isAlive_ = true;
  lifeRemaining_ = life_;
  x_ = origx_;
  y_ = origy_;
  z_ = origz_;

  if (type_ == "explosion")
  {
    vx_ = (float) (rand() % 2000 - 1000) / 1000;
    vy_ = (float) (rand() % 2000 - 1000) / 1000;
    vz_ = (float) (rand() % 2000 - 1000) / 1000;
    float tmp = sqrt(vx_ * vx_ + vy_ * vy_ + vz_ * vz_);
    vx_ /= tmp;
    vy_ /= tmp;
    vz_ /= tmp;

  }
  else if (type_ == "nova")
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

