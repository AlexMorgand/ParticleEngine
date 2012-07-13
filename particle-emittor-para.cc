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
    pe_->vpart()[i]->rgb()(0, rand() % 256);
    pe_->vpart()[i]->rgb()(1, rand() % 256);
    pe_->vpart()[i]->rgb()(2, rand() % 256);

    if (pe_->type () == "explosion")
      {
        pe_->vpart()[i]->pos()(0,
			     pe_->vpart()[i]->pos()(0) + pe_->vpart ()[i]->v()(0) * elapsedTime);
        pe_->vpart()[i]->pos()(1,
			     pe_->vpart()[i]->pos()(1) + pe_->vpart ()[i]->v()(1) * elapsedTime);
        pe_->vpart()[i]->pos()(2,
			     pe_->vpart()[i]->pos()(2) + pe_->vpart ()[i]->v()(2) /*- GRAVITY*/ * elapsedTime);
      }
    else if (pe_->type() == "nova")
      {
        pe_->vpart()[i]->pos()(0,
			     pe_->vpart()[i]->pos()(0) + pe_->vpart ()[i]->v()(0) * elapsedTime);
        pe_->vpart()[i]->pos()(1,
			     pe_->vpart()[i]->pos()(1) + pe_->vpart ()[i]->v()(1) * elapsedTime);
        pe_->vpart()[i]->pos()(2,
			     pe_->vpart()[i]->pos()(2) + pe_->vpart ()[i]->v()(2) /*- GRAVITY*/ * elapsedTime);
      }
    else if (pe_->type() == "circle")
      {
        // FIXME: do a dispatcher for different patterns.
        pe_->vpart()[i]->pos()(0, 3 * sin (pe_->t_ + i));
        pe_->vpart()[i]->pos()(2, 2 * sin (2 * pe_->t_ + i));
      }

    // TODO: gerer le wall_collision directement grave au particleEmittor
    // wall_collision(pe_->vpart()[i]);
    // Handle remaining life.
    pe_->vpart ()[i]->lifeRemaining(pe_->vpart ()[i]->lifeRemaining() - elapsedTime);
    if (pe_->vpart ()[i]->lifeRemaining() < 0)
      pe_->vpart ()[i]->resetParticle();
  }
}
