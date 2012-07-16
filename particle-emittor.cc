#include "particle-emittor.hh"

ParticleEmittor::ParticleEmittor(int nbPart, std::list<Plane> walls, std::string type)
  : t_ (0),
    nbPart_ (nbPart),
    type_ (type),
    etype_ ("NOTHING"),
    walls_ (walls)
{
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

ProgressiveEmittor::ProgressiveEmittor(int nbPart, std::list<Plane> walls, std::string type)
  : ParticleEmittor(nbPart, walls, type),
    pvpart_ (),
    partProd_ (0)
{
  etype_ = "progressive";
}

void ProgressiveEmittor::initParticles()
{
  Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 30, 30, 30, 0, type_);
  p->resetParticle();
  pvpart_.push_front(p);
  partProd_++;
}

ImmediateEmittor::ImmediateEmittor(int nbPart, std::list<Plane> walls, std::string type)
  : ParticleEmittor(nbPart, walls, type),
    vpart_ (nbPart)
{
  etype_ = "immediate";
}

void ImmediateEmittor::initParticles()
{
  for (int i = 0; i < nbPart_; i++)
  {
    Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 30, 30, 30, 0, type_);
    p->resetParticle ();
    vpart(i, p);
  }
}
