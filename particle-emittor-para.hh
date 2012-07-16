#ifndef PARTICLE_EMITTOR_PARA_HH_
# define PARTICLE_EMITTOR_PARA_HH_

# include <tbb/task_scheduler_init.h>
# include <tbb/parallel_for.h>
# include "vector.hh"
# include "particle-emittor.hh"

class ImmediateEmittorPara
{
public:
  void values (ImmediateEmittor* pe, float elapsed) { pe_ = pe; elapsedTime = elapsed; }

  void operator() (const tbb::blocked_range<size_t>& r) const;
private:
  ImmediateEmittor* pe_;
  float elapsedTime;
};

class ProgressiveEmittorPara
{
public:
  void values (ProgressiveEmittor* pe, float elapsed) { pe_ = pe; elapsedTime = elapsed; }

  void operator() (const tbb::blocked_range<size_t>& r) const;
private:
  ProgressiveEmittor* pe_;
  float elapsedTime;
};

#endif /// !PARTICLE_EMITTOR_PARA_HH_
