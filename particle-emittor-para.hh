#ifndef PARTICLE_EMITTOR_PARA_HH_
# define PARTICLE_EMITTOR_PARA_HH_

# include <tbb/task_scheduler_init.h>
# include <tbb/parallel_for.h>
# include "vector.hh"

class ParticleEmittor;

class ParticleEmittorPara
{
public:
  ParticleEmittorPara (ParticleEmittor* pe = 0);

  void values (ParticleEmittor* pe, float elapsed) { pe_ = pe; elapsedTime = elapsed; }

  void operator() (const tbb::blocked_range<size_t>& r) const;
private:
  ParticleEmittor* pe_;
  float elapsedTime;
};

#endif /// !PARTICLE_EMITTOR_PARA_HH_
