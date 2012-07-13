#include "particles.hh"

ParticleEngine::ParticleEngine()
  : lpe_ (0),
    cpt_ (0),
    walls_ (),
    para_pe_ (new ParticleEmittorPara ())
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

void
ParticleEmittor::wall_collision (Particle* p)
{
  std::list<Plane>::iterator it;
  for (it = walls_.begin (); it != walls_.end (); ++it)
  {
    // FIXME: put a size in a variable.
    if (std::abs (it->distanceToPoint (p->pos())) <= 1)
    {
      // Realining.
      Vector3f pos = p->pos();
      // Ratio for Thales.
      float ratio = 1 / it->distanceToPoint (p->pos());
      Vector3f direction = p->v() / p->v().norme();

      Vector3f intersectP =
        it->intersectionPoint (p->pos(), p->v());

      float distanceFromCollision = (intersectP - p->pos()).norme();
      float res = distanceFromCollision * ratio - distanceFromCollision + 0.01;
      Vector3f final = direction * (-res);
      p->pos()(0, p->pos()(0) + final(0));
      p->pos()(1, p->pos()(1) + final(1));
      p->pos()(2, p->pos()(2) + final(2));

      // Bouncing.
      // FIXME: Not sure.

      // Should be good to round the speed when colliding with the floor.

      Vector3f normal (it->a(), it->b(), it->c());
      Vector3f finalSpeed = p->v();
      //float scal = p->v().scalar(normal);
      float scal = p->v()(0) * normal(0) + p->v()(1) * normal(1) + p->v()(2) * normal(2);
      scal *= 2;
      finalSpeed -= normal * scal;
      p->v()(0, (finalSpeed * 0.5)(0));
      p->v()(1, (finalSpeed * 0.5)(1));
      p->v()(2, (finalSpeed * 0.5)(2));
    }
  }
}

void ParticleEngine::update(float elapsedTime)
{
  for (std::map<int, ParticleEmittor*>::iterator it = lpe()->begin();
      it != lpe()->end(); ++it)
  {
    ParticleEmittor* p = it->second;
    spin += 0.01f;

    para_pe_->values(p, elapsedTime);

    parallel_for(tbb::blocked_range<size_t> (0, p->nbPart ()),
		 *para_pe_);
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

ParticleEmittor::ParticleEmittor(int nbPart, std::list<Plane> walls, std::string type)
  : t_ (0),
    vpart_ (nbPart),
    nbPart_ (nbPart),
    type_ (type),
    walls_ (walls)

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
      Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 30, 30, 30, 0, pe->type());
      p->resetParticle ();
      pe->vpart(i, p);
    }
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
  pos_ = origpos_;

  if (type_ == "explosion")
  {
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
    static size_t angle = 0;
    v_(0, 0.5);
    v_(1, 0.5);

    float tmp = v_(0);
    v_(0, v_(0) * v_(0) * cos(angle) - v_(1) * sin(angle));
    v_(1, v_(1) * tmp * sin(angle) + v_(2) * cos(angle));
    angle += 5;
  }
}

