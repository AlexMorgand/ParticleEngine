#include "particles.hh"
#include "particles.hh"

ParticleEngine::ParticleEngine()
  : lpe_ (0),
    cpt_ (0),
    walls_ ()
{
  tbb::task_scheduler_init init;
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
    spin += 0.01f;

    if (it->second->etype() == "immediate")
    {
      ImmediateEmittor* p = (ImmediateEmittor*) it->second;

      ImmediateEmittorPara* para_pe = new ImmediateEmittorPara();
      para_pe->values(p, elapsedTime);

      parallel_for(tbb::blocked_range<size_t> (0, p->nbPart ()),
          *para_pe);
      delete para_pe;
    }
    else
    {
      ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;

      ProgressiveEmittorPara* para_pe = new ProgressiveEmittorPara();
      para_pe->values(p, elapsedTime);

      parallel_for(tbb::blocked_range<size_t> (0, 1 /*FIXME: Put size */), *para_pe);
      if (p->partProd() < p->nbPart())
      {
        Particle* pa = new Particle (rand() % 256, rand() % 256, rand() % 256, 30, 30, 30, 0, p->type());
        pa->resetParticle();
        p->pvpart().push_front(pa);
        p->partProd(p->partProd() + 1);
      }
      delete para_pe;
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

// FIXME: init only ONE emittor, not EVERY emittor.
void
ParticleEngine::initParticles()
{
  for (std::map<int, ParticleEmittor*>::iterator it = lpe_->begin(); it != lpe_->end(); ++it)
  {
    ParticleEmittor* pe = it->second;
    pe->initParticles();
  }
}

// FIXME: constructor must handle image.
Particle::Particle(int r, int g, int b,
                   float x, float y, float z, float angle, std::string type)
  : rgb_ (r, g, b),
    origpos_ (x, y, z),
    pos_ (x, y, z),
    v_ (0, 0, 0),
    angle_(angle),
    // FIXME: Put it in parameter.
    life_(1000),
    isAlive_(true),
    type_ (type)
{
}

void Particle::resetParticle ()
{
  isAlive_ = true;
  lifeRemaining_ = life_;
  pos_ = origpos_;

  if (type_ == "explosion")
  {
    lifeRemaining_ = 100;
    v_(0, (float) (rand() % 2000 - 1000) / 1000);
    v_(1, (float) (rand() % 2000 - 1000) / 1000);
    v_(2, (float) (rand() % 2000 - 1000) / 1000);
    float tmp = sqrt(v_(0) * v_(0) + v_(1) * v_(1) + v_(2) * v_(2));
    v_(0, (v_(0) / tmp) * 10);
    v_(1, (v_(1) / tmp) * 10);
    v_(2, (v_(2) / tmp) * 10);
  }
  else if (type_ == "nova")
  {
    lifeRemaining_ = 100;
    static size_t angle = 0;
    v_(0, 0.5);
    v_(1, 0.5);

    float tmp = v_(0);
    v_(0, v_(0) * v_(0) * cos(angle) - v_(1) * sin(angle) * 50);
    v_(1, v_(1) * tmp * sin(angle) + v_(1) * cos(angle) * 50);
    angle += 5;
  }
  else if (type_ == "smoke")
  {
    pos_(0, origpos_(0));
    pos_(1, origpos_(1));
    pos_(2, origpos_(2));
    lifeRemaining_ = 5;
    rgb_(0, 56);
    rgb_(1, 56);
    rgb_(2, 56);
    v_(0, (float) (rand() % 10 - 5) / 5);
    v_(1, (float) (rand() % 10 - 5) / 5);
    v_(2, (float) (rand() % 2000) / 2000);
    float tmp = sqrt(v_(0) * v_(0) + v_(1) * v_(1) + v_(2) * v_(2));
    v_(0, (v_(0) / tmp));
    v_(1, (v_(1) / tmp));
    v_(2, (v_(2) / tmp));
  }
}

