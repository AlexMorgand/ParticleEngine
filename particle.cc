#include "particle.hh"

ParticleEngine::ParticleEngine()
  : lpe_ (0),
    cpt_ (0),
    walls_ (),
    para_ (false),
    ela_ (0)
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

void ParticleEngine::removeDeadEmittor()
{
  std::list<int> toerase;
  for (std::map<int, ParticleEmittor*>::iterator it = lpe_->begin();
      it != lpe_->end(); ++it)
  {
    int deadPart = 0;
    if (it->second->etype() == "immediate")
    {
      ImmediateEmittor* p = (ImmediateEmittor*) it->second;
      // FIXME: maybe parralelism of the suppression !
      for (int i = 0; i < p->nbPart(); ++i)
      {
        if (!p->vpart()[i]->isAlive())
          ++deadPart;
      }
      if (deadPart == p->nbPart())
        toerase.push_front(it->first);
    }
    else
    {
      ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;
      std::list<Particle*>::iterator pi;
      for (pi = p->pvpart().begin(); pi != p->pvpart().end(); ++pi)
      {
        if (!(*pi)->isAlive())
          ++deadPart;
      }
      if (deadPart == p->partProd())
        toerase.push_front(it->first);
    }
  }
  std::list<int>::iterator it;
  for (it = toerase.begin(); it != toerase.end(); ++it)
    lpe_->erase(*it);
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
      ImmediateEmittorPara* para_pe = 0;

      if (para_)
      {
        para_pe = new ImmediateEmittorPara();
        para_pe->values(p, elapsedTime);

        parallel_for(tbb::blocked_range<size_t> (0, p->nbPart ()),
            *para_pe);
      }
      else
      {
        unsigned int i;

        for (i = 0; i < p->vpart().size (); ++i)
        {
          Particle* pa = p->vpart()[i];

          if (p->type() != "fragmentation")
          {
            pa->rgb()(0, rand() % 256);
            pa->rgb()(1, rand() % 256);
            pa->rgb()(2, rand() % 256);
          }

          if ((p->type() == "explosion") || (p->type() == "fragmentation"))
          {
            // Gravity.
            pa->v()(2, pa->v()(2) - (10 * elapsedTime));

            pa->pos()(0, pa->pos()(0) + pa->v()(0) * elapsedTime);
            pa->pos()(1, pa->pos()(1) + pa->v()(1) * elapsedTime);
            pa->pos()(2, pa->pos()(2) + pa->v()(2) * elapsedTime);
          }
          else if (p->type() == "nova")
          {
            // Gravity.
            pa->v()(2, pa->v()(2) - (10 * elapsedTime));

            pa->pos()(0,
                pa->pos()(0) + pa->v()(0) * elapsedTime);
            pa->pos()(1,
                pa->pos()(1) + pa->v()(1) * elapsedTime);
            pa->pos()(2,
                pa->pos()(2) + pa->v()(2) * elapsedTime);
          }
          else if (p->type() == "circle")
          {
            // FIXME: do a dispatcher for different patterns.
            pa->pos()(0, 3 * sin (p->t_ + i));
            pa->pos()(2, 2 * sin (2 * p->t_ + i));
          }

          // Handle remaining life.
          pa->lifeRemaining(pa->lifeRemaining() - elapsedTime);
          if (p->type() != "fragmentation" && pa->lifeRemaining() < 0)
            pa->isAlive(false);
        }
          for (i = 0; i < p->vpart().size (); ++i)
          {
            Particle* pa = p->vpart()[i];
            p->wall_collision(pa);
            if (pa->life() - pa->lifeRemaining() > 3)
              for (unsigned int j = 0; j < p->vpart().size (); ++j)
              {
                if (j != i)
                  p->intra_collision(pa, p->vpart()[j]);
              }
          }

	}

      if (p->type() == "fragmentation")
      {
        for (int i = 0; i < p->nbPart(); ++i)
        {
          if (p->vpart()[i]->lifeRemaining() < 0 && p->vpart()[i]->isAlive())
          {
            ImmediateEmittor* ie = new ImmediateEmittor(3, walls_,
                                                        p->orig(), "explosion");
            addEmittor(ie);
            ie->initParticles(p->vpart()[i]->pos());
            p->vpart()[i]->isAlive(false);
          }
        }
      }
      if (para_)
	delete para_pe;
    }
    else
    {
      ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;

      ProgressiveEmittorPara* para_pe = 0;

      if (para_)
	{
	  para_pe = new ProgressiveEmittorPara();
	  para_pe->values(p, elapsedTime);

	  parallel_for(tbb::blocked_range<size_t> (0, p->pvpart ().size ()), *para_pe);
	}
      else
	{
	  std::list<Particle*>::iterator it = p->pvpart ().begin ();

	  for (; it != p->pvpart().end (); ++it)
	    {
	      if (p->type() == "smoke")
		// Wind.
		(*it)->v()(2, (*it)->v()(2) + (elapsedTime));

	      (*it)->pos()(0, (*it)->pos()(0) + (*it)->v()(0) * elapsedTime);
	      (*it)->pos()(1, (*it)->pos()(1) + (*it)->v()(1) * elapsedTime);
	      (*it)->pos()(2, (*it)->pos()(2) + (*it)->v()(2) * elapsedTime);
	      p->wall_collision(*it);
	      (*it)->lifeRemaining((*it)->lifeRemaining() - elapsedTime);
	      // FIXME: maybe better for the FPS to erase immediatly.
	      if (p->type() != "fire" && (*it)->lifeRemaining() < 0)
		(*it)->isAlive(false);
	    }

	}

      if (p->type() == "smoke")
      {
        if (p->partProd() < p->nbPart())
        {
          Particle* pa = new Particle (rand() % 256, rand() % 256, rand() % 256,
                                       p->orig()(0), p->orig()(1), p->orig()(2),
                                       0, p->type());
          pa->resetParticle();
          p->pvpart().push_front(pa);
          p->partProd(p->partProd() + 1);
        }
      }
      else if (p->type() == "fire")
      {
        if (p->pvpart().size() < (unsigned int) p->nbPart())
        {
          Particle* pa = new Particle (rand() % 256, rand() % 256, rand() % 256,
                                       p->orig()(0), p->orig()(1), p->orig()(2),
                                       0, p->type());
          pa->resetParticle();
          p->pvpart().push_front(pa);
          p->partProd(p->partProd() + 1);
        }

        std::list<Particle*>::iterator it;
        for (it = p->pvpart().begin(); it != p->pvpart().end(); ++it)
        {
          if ((*it)->lifeRemaining() < 0 && (*it)->isAlive())
          {
            ProgressiveEmittor* pe = new ProgressiveEmittor(1, walls_,
                                                            (*it)->pos(), "smoke");
            addEmittor(pe);
            pe->initParticles((*it)->pos());
            (*it)->isAlive(false);
          }
        }
      }

      if (para_)
	delete para_pe;
    }
  }
  removeDeadEmittor();
}

void ParticleEngine::parallel (bool para)
{
  para_ = para;
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
  pos_ = origpos_;

  if ((type_ == "explosion") || (type_ == "fragmentation"))
  {
    if (type_ == "fragmentation")
    {
      rgb_(0, 5);
      rgb_(1, 5);
      rgb_(2, 200);
      lifeRemaining_ = 1;
    }
    else
      lifeRemaining_ = 10;
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
    lifeRemaining_ = 10;
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
  else if (type_ == "fire")
  {
    pos_(0, origpos_(0));
    pos_(1, origpos_(1));
    pos_(2, origpos_(2));
    lifeRemaining_ = 5;
    rgb_(0, 200);
    rgb_(1, 100);
    rgb_(2, 5);
    v_(0, (float) (rand() % 10 - 5) / 5);
    v_(1, (float) (rand() % 10 - 5) / 5);
    v_(2, (float) (rand() % 2000) / 2000);
    float tmp = sqrt(v_(0) * v_(0) + v_(1) * v_(1) + v_(2) * v_(2));
    v_(0, (v_(0) / tmp));
    v_(1, (v_(1) / tmp));
    v_(2, (v_(2) / tmp));
  }
  life_ = lifeRemaining_;
}


// tbb::task*
// ParticleEngine::execute()
// {
//   for (std::map<int, ParticleEmittor*>::iterator it = lpe()->begin();
//       it != lpe()->end(); ++it)
//   {
//     spin += 0.01f;

//     if (it->second->etype() == "immediate")
//     {
//       ImmediateEmittor* p = (ImmediateEmittor*) it->second;
//       ImmediateEmittorPara* para_pe = 0;

//       if (para_)
//       {
//         para_pe = new ImmediateEmittorPara();
//         para_pe->values(p, ela_);

//         parallel_for(tbb::blocked_range<size_t> (0, p->nbPart ()),
//             *para_pe);
//       }
//       else
//       {
//         unsigned int i;

//         for (i = 0; i < p->vpart().size (); ++i)
//         {
//           Particle* pa = p->vpart()[i];

//           if (p->type() != "fragmentation")
//           {
//             pa->rgb()(0, rand() % 256);
//             pa->rgb()(1, rand() % 256);
//             pa->rgb()(2, rand() % 256);
//           }

//           if ((p->type() == "explosion") || (p->type() == "fragmentation"))
//           {
//             // Gravity.
//             pa->v()(2, pa->v()(2) - (10 * ela_));

//             pa->pos()(0, pa->pos()(0) + pa->v()(0) * ela_);
//             pa->pos()(1, pa->pos()(1) + pa->v()(1) * ela_);
//             pa->pos()(2, pa->pos()(2) + pa->v()(2) * ela_);
//           }
//           else if (p->type() == "nova")
//           {
//             // Gravity.
//             pa->v()(2, pa->v()(2) - (10 * ela_));

//             pa->pos()(0,
//                 pa->pos()(0) + pa->v()(0) * ela_);
//             pa->pos()(1,
//                 pa->pos()(1) + pa->v()(1) * ela_);
//             pa->pos()(2,
//                 pa->pos()(2) + pa->v()(2) * ela_);
//           }
//           else if (p->type() == "circle")
//           {
//             // FIXME: do a dispatcher for different patterns.
//             pa->pos()(0, 3 * sin (p->t_ + i));
//             pa->pos()(2, 2 * sin (2 * p->t_ + i));
//           }

//           // Handle remaining life.
//           pa->lifeRemaining(pa->lifeRemaining() - ela_);
//           if (p->type() != "fragmentation" && pa->lifeRemaining() < 0)
//             pa->isAlive(false);
//         }
//           for (i = 0; i < p->vpart().size (); ++i)
//           {
//             Particle* pa = p->vpart()[i];
//             p->wall_collision(pa);
//             if (pa->life() - pa->lifeRemaining() > 3)
//               for (unsigned int j = 0; j < p->vpart().size (); ++j)
//               {
//                 if (j != i)
//                   p->intra_collision(pa, p->vpart()[j]);
//               }
//           }

// 	}

//       if (p->type() == "fragmentation")
//       {
//         for (int i = 0; i < p->nbPart(); ++i)
//         {
//           if (p->vpart()[i]->lifeRemaining() < 0 && p->vpart()[i]->isAlive())
//           {
//             ImmediateEmittor* ie = new ImmediateEmittor(3, walls_,
//                                                         p->orig(), "explosion");
//             addEmittor(ie);
//             ie->initParticles(p->vpart()[i]->pos());
//             p->vpart()[i]->isAlive(false);
//           }
//         }
//       }
//       if (para_)
// 	delete para_pe;
//     }
//     else
//     {
//       ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;

//       ProgressiveEmittorPara* para_pe = 0;

//       if (para_)
// 	{
// 	  para_pe = new ProgressiveEmittorPara();
// 	  para_pe->values(p, ela_);

// 	  parallel_for(tbb::blocked_range<size_t> (0, p->pvpart ().size ()), *para_pe);
// 	}
//       else
// 	{
// 	  std::list<Particle*>::iterator it = p->pvpart ().begin ();

// 	  for (; it != p->pvpart().end (); ++it)
// 	    {
// 	      if (p->type() == "smoke")
// 		// Wind.
// 		(*it)->v()(2, (*it)->v()(2) + (ela_));

// 	      (*it)->pos()(0, (*it)->pos()(0) + (*it)->v()(0) * ela_);
// 	      (*it)->pos()(1, (*it)->pos()(1) + (*it)->v()(1) * ela_);
// 	      (*it)->pos()(2, (*it)->pos()(2) + (*it)->v()(2) * ela_);
// 	      p->wall_collision(*it);
// 	      (*it)->lifeRemaining((*it)->lifeRemaining() - ela_);
// 	      // FIXME: maybe better for the FPS to erase immediatly.
// 	      if (p->type() != "fire" && (*it)->lifeRemaining() < 0)
// 		(*it)->isAlive(false);
// 	    }

// 	}

//       if (p->type() == "smoke")
//       {
//         if (p->partProd() < p->nbPart())
//         {
//           Particle* pa = new Particle (rand() % 256, rand() % 256, rand() % 256,
//                                        p->orig()(0), p->orig()(1), p->orig()(2),
//                                        0, p->type());
//           pa->resetParticle();
//           p->pvpart().push_front(pa);
//           p->partProd(p->partProd() + 1);
//         }
//       }
//       else if (p->type() == "fire")
//       {
//         if (p->pvpart().size() < (unsigned int) p->nbPart())
//         {
//           Particle* pa = new Particle (rand() % 256, rand() % 256, rand() % 256,
//                                        p->orig()(0), p->orig()(1), p->orig()(2),
//                                        0, p->type());
//           pa->resetParticle();
//           p->pvpart().push_front(pa);
//           p->partProd(p->partProd() + 1);
//         }

//         std::list<Particle*>::iterator it;
//         for (it = p->pvpart().begin(); it != p->pvpart().end(); ++it)
//         {
//           if ((*it)->lifeRemaining() < 0 && (*it)->isAlive())
//           {
//             ProgressiveEmittor* pe = new ProgressiveEmittor(1, walls_,
//                                                             (*it)->pos(), "smoke");
//             addEmittor(pe);
//             pe->initParticles((*it)->pos());
//             (*it)->isAlive(false);
//           }
//         }
//       }

//       if (para_)
// 	delete para_pe;
//     }
//   }
//   removeDeadEmittor();

//   return 0;
// }
