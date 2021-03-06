#include "particle-emittor-para.hh"

#include "particle.hh"

static tbb::mutex sm;


void
ImmediateEmittorPara::operator() (const tbb::blocked_range<size_t>& r) const
{
  unsigned int i;

  for (i = r.begin(); i != r.end(); ++i)
  {
    Particle* p = pe_->vpart()[i];

    if (pe_->type() != "fragmentation")
    {
      p->rgb()(0, rand() % 256);
      p->rgb()(1, rand() % 256);
      p->rgb()(2, rand() % 256);
    }

    if ((pe_->type() == "explosion") || (pe_->type() == "fragmentation"))
    {
      // Gravity.
      p->v()(2, p->v()(2) - (10 * elapsedTime));

      p->pos()(0, p->pos()(0) + p->v()(0) * elapsedTime);
      p->pos()(1, p->pos()(1) + p->v()(1) * elapsedTime);
      p->pos()(2, p->pos()(2) + p->v()(2) * elapsedTime);
    }
    else if (pe_->type() == "nova")
    {
      // Gravity.
      p->v()(2, p->v()(2) - (10 * elapsedTime));

      p->pos()(0,
          p->pos()(0) + p->v()(0) * elapsedTime);
      p->pos()(1,
          p->pos()(1) + p->v()(1) * elapsedTime);
      p->pos()(2,
          p->pos()(2) + p->v()(2) * elapsedTime);
    }
    else if (pe_->type() == "circle")
    {
      // FIXME: do a dispatcher for different patterns.
      p->pos()(0, 3 * sin (pe_->t_ + i));
      p->pos()(2, 2 * sin (2 * pe_->t_ + i));
    }

    // Handle remaining life.
    p->lifeRemaining(p->lifeRemaining() - elapsedTime);
    if (pe_->type() != "fragmentation" && p->lifeRemaining() < 0)
      p->isAlive(false);
  }

  for (i = r.begin(); i != r.end(); ++i)
  {
    Particle* p = pe_->vpart()[i];
    pe_->wall_collision(p);
    if (p->life() - p->lifeRemaining() > 3)
    {
      // FIXME : NOT SURE !
      tbb::mutex::scoped_lock lock;
      lock.acquire(sm);
      for (unsigned int j = 0; j < pe_->vpart().size (); ++j)
      {
        if (j != i)
          pe_->intra_collision(p, pe_->vpart()[j]);
      }
      lock.release();
    }
  }
}

void
ProgressiveEmittorPara::operator() (const tbb::blocked_range<size_t>& r) const
{
  //unsigned int i;
  std::list<Particle*>::iterator it = pe_->pvpart ().begin ();
 // std::list<Particle*> toerase;
  unsigned int i = 0;

  for (; i < r.begin (); ++i, ++it);

  for (; i != r.end () && it != pe_->pvpart().end (); ++it, ++i)
  {
    if (pe_->type() == "smoke")
      // Wind.
      (*it)->v()(2, (*it)->v()(2) + (elapsedTime));

    (*it)->pos()(0, (*it)->pos()(0) + (*it)->v()(0) * elapsedTime);
    (*it)->pos()(1, (*it)->pos()(1) + (*it)->v()(1) * elapsedTime);
    (*it)->pos()(2, (*it)->pos()(2) + (*it)->v()(2) * elapsedTime);
    pe_->wall_collision(*it);
    (*it)->lifeRemaining((*it)->lifeRemaining() - elapsedTime);
    // FIXME: maybe better for the FPS to erase immediatly.
    if (pe_->type() != "fire" && (*it)->lifeRemaining() < 0)
      (*it)->isAlive(false);
//      toerase.push_front(*it);
  }
/*
  for (it = toerase.begin (); it != toerase.end (); ++it)
  {
    pe_->partProd(pe_->partProd() - 1);
    pe_->pvpart().remove(*it);
  }
  */
}
