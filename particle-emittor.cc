#include "particle-emittor.hh"

ParticleEmittor::ParticleEmittor(int nbPart, std::list<Plane> walls,
                                 Vector3f orig,  std::string type)
  : t_ (0),
    nbPart_ (nbPart),
    orig_ (orig),
    type_ (type),
    etype_ ("NOTHING"),
    walls_ (walls)
{
}

void ParticleEmittor::intra_collision(Particle* p1, Particle* p2)
{
  Vector3f pos1 = p1->pos();
  Vector3f pos2 = p2->pos();
  Vector3f res = pos1 - pos2;
  float distance = res.norme();

  if (distance < 1)
  {
    double ratio = (pos2 - pos1).norme ();
    Vector3f norme = (pos2 - pos1) / ratio;
    Vector3f res = norme * (distance);

    // Realining.
    pos1(0, pos1(0) - res(0));
    pos1(1, pos1(1) - res(1));
    pos1(2, pos1(2) - res(2));

    // Elastic collision.
    double nx = (pos2(0) - pos1(0)) / (1 + 1);
    double ny = (pos2(1) - pos1(1)) / (1 + 1);
    double nz = (pos2(2) - pos1(2)) / (1 + 1);
    double gx = -ny;
    double gy = nx;
    double gz = nz;

    double v1n = nx * (p1->v()(0) + ny * p1->v()(1) + nz * p1->v()(2));
    double v1g = gx * (p1->v()(0) + gy * p1->v()(1) + gz * p1->v()(2));

    double v2n = nx * (p2->v()(0) + ny * p2->v()(1) + nz * p2->v()(2));
    double v2g = gx * (p2->v()(0) + gy * p2->v()(1) + gz * p2->v()(2));

    Vector3f speed_obj1 = Vector3f (nx * v2n + gx * v1g,
        ny * v2n + gy * v1g,
        nz * v2n + gz * v1g);
    Vector3f speed_obj2 = Vector3f (nx * v1n + gx * v2g,
        ny * v1n + gy * v2g,
        nz * v1n + gz * v2g);

    p1->v()(0, speed_obj1(0));
    p1->v()(1, speed_obj1(1));
    p1->v()(2, speed_obj1(2));

    p2->v()(0, speed_obj2(0));
    p2->v()(1, speed_obj2(1));
    p2->v()(2, speed_obj2(2));
  }
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
      float scal = p->v()(0) * normal(0) + p->v()(1) * normal(1) + p->v()(2) * normal(2);
      scal *= 2;
      finalSpeed -= normal * scal;
      p->v()(0, (finalSpeed * 0.5)(0));
      p->v()(1, (finalSpeed * 0.5)(1));
      p->v()(2, (finalSpeed * 0.5)(2));
    }
  }
}

ProgressiveEmittor::ProgressiveEmittor(int nbPart, std::list<Plane> walls, Vector3f orig, std::string type)
  : ParticleEmittor(nbPart, walls, orig, type),
    pvpart_ (),
    partProd_ (0)
{
  etype_ = "progressive";
}

void ProgressiveEmittor::initParticles()
{
  Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, orig_ (0), orig_ (1), orig_ (2), 0, type_);
  p->resetParticle();
  pvpart_.push_front(p);
  partProd_++;
}

void ProgressiveEmittor::initParticles(Vector3f pos)
{
  Particle* p = new Particle (rand() % 256, rand() % 256,
                              rand() % 256, pos(0), pos(1), pos(2), 0, type_);
  p->resetParticle();
  pvpart_.push_front(p);
  partProd_++;
}

ImmediateEmittor::ImmediateEmittor(int nbPart, std::list<Plane> walls, Vector3f orig, std::string type)
  : ParticleEmittor(nbPart, walls, orig, type),
    vpart_ (nbPart)
{
  etype_ = "immediate";
}

void ImmediateEmittor::initParticles()
{
  for (int i = 0; i < nbPart_; i++)
  {
    Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, orig_ (0), orig_ (1), orig_ (2), 0, type_);
    p->resetParticle ();
    vpart(i, p);
  }
}

void ImmediateEmittor::initParticles(Vector3f pos)
{
  for (int i = 0; i < nbPart_; i++)
  {
    Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, pos(0),
                                pos(1), pos(2), 0, type_);
    p->resetParticle ();
    vpart(i, p);
  }
}
