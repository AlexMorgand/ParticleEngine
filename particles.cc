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

void
ParticleEmittor::wall_collision (ParticleEmittor* p)
{
  for (int i = 0; i < p->nbPart(); i++)
  {
    std::list<Plane>::iterator it;
    for (it = walls_.begin (); it != walls_.end (); ++it)
    {
     // if (std::abs (p.distanceToPoint (pos_)) <= size_ (0))
      {
        /*
        // Realining.
        Vector3f pos = (*i)->pos_get ();
        // Ratio for Thales.
        float ratio = (*i)->size_get () (0) / p.distanceToPoint (pos);
        Vector3f direction = (*i)->speed_get () / (*i)->speed_get ().get_norme ();

        Vector3f intersectP =
          p.intersectionPoint (pos, (*i)->speed_get ());

        float distanceFromCollision = (intersectP - pos).get_norme ();
        float res = distanceFromCollision * ratio - distanceFromCollision + 0.01;
        final_obj_mov_[*i] = direction * (-res);

        // Bouncing.
        // FIXME: Not sure.

        // Should be good to round the speed when colliding with the floor.

        Vector3f normal (p.a (), p.b (), p.c ());
        Vector3f finalSpeed = (*i)->speed_get ();
        float scal = (*i)->speed_get ().scalar(normal);
        scal *= 2;
        finalSpeed -= normal * scal;
        (*i)->speed_set (finalSpeed * 0.5);*/
      }
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
    for (int i = 0; i < p->nbPart(); i++)
    {
      p->vpart()[i]->rgb()(0, rand() % 256);
      p->vpart()[i]->rgb()(1, rand() % 256);
      p->vpart()[i]->rgb()(2, rand() % 256);

      if (p->type () == "explosion")
      {
        p->vpart()[i]->pos()(0,
          p->vpart()[i]->pos()(0) + p->vpart ()[i]->v()(0) * elapsedTime);
        p->vpart()[i]->pos()(1,
          p->vpart()[i]->pos()(1) + p->vpart ()[i]->v()(1) * elapsedTime);
        p->vpart()[i]->pos()(2,
          p->vpart()[i]->pos()(2) + p->vpart ()[i]->v()(2) /*- GRAVITY*/ * elapsedTime);
      }
      else if (p->type() == "nova")
      {
        p->vpart()[i]->pos()(0,
          p->vpart()[i]->pos()(0) + p->vpart ()[i]->v()(0) * elapsedTime);
        p->vpart()[i]->pos()(1,
          p->vpart()[i]->pos()(1) + p->vpart ()[i]->v()(1) * elapsedTime);
        p->vpart()[i]->pos()(2,
          p->vpart()[i]->pos()(2) + p->vpart ()[i]->v()(2) /*- GRAVITY*/ * elapsedTime);
      }
      else if (p->type() == "circle")
      {
        // FIXME: do a dispatcher for different patterns.
        p->vpart ()[i]->pos()(0, 3 * sin (p->t_ + i));
        p->vpart ()[i]->pos()(2, 2 * sin (2 * p->t_ + i));
      }

      // Handle remaining life.
      p->vpart ()[i]->lifeRemaining(p->vpart ()[i]->lifeRemaining() - elapsedTime);
      if (p->vpart ()[i]->lifeRemaining() < 0)
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
      Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 0, 0, 0, 0, pe->type());
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
    v_(0, v_(0) / tmp);
    v_(1, v_(1) / tmp);
    v_(2, v_(2) / tmp);
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

