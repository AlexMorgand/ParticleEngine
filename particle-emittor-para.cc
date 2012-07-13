#include "particle-emittor-para.hh"

#include "particles.hh"

ParticleEmittorPara::ParticleEmittorPara (ParticleEmittor* pe)
  : pe_ (pe)
{
}

void
ParticleEmittorPara::operator() (const tbb::blocked_range<size_t>& r) const
{
  unsigned int i;

  for (i = r.begin (); i != r.end (); ++i)
  {
    Particle* p = pe_->vpart()[i];

    p->rgb()(0, rand() % 256);
    p->rgb()(1, rand() % 256);
    p->rgb()(2, rand() % 256);

    if (pe_->type () == "explosion")
    {

      // Gravity.
      p->v()(2, p->v()(2) - (10 * elapsedTime));

      p->pos()(0, p->pos()(0) + p->v()(0) * elapsedTime);
      p->pos()(1, p->pos()(1) + p->v()(1) * elapsedTime);
      p->pos()(2, p->pos()(2) + p->v()(2) * elapsedTime);
    }
    else if (pe_->type() == "nova")
    {
      p->pos()(0,
          p->pos()(0) + p->v()(0) * elapsedTime);
      p->pos()(1,
          p->pos()(1) + p->v()(1) * elapsedTime);
      p->pos()(2,
          p->pos()(2) + (p->v()(2) - 0.011) * elapsedTime);
    }
    else if (pe_->type() == "circle")
    {
      // FIXME: do a dispatcher for different patterns.
      p->pos()(0, 3 * sin (pe_->t_ + i));
      p->pos()(2, 2 * sin (2 * pe_->t_ + i));
    }

    // TODO: gerer le wall_collision directement grave au particleEmittor
    pe_->wall_collision(p);
    // Handle remaining life.
    p->lifeRemaining(p->lifeRemaining() - elapsedTime);
    if (p->lifeRemaining() < 0)
      p->resetParticle();
  }
}
