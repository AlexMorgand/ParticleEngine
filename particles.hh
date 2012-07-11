#ifndef PARTICLES_HH
# define PARTICLES_HH

# include "tools.hh"
# include <map>

# define PI 3.14159265

// FIXME: think about better arguments.
class Particle
{
  public:
    Particle(int r, int g, int b, float x, float y, float z, float angle);
    void resetParticle ();
    float angle () { return angle_; }
    float x () { return x_; }
    float y () { return y_; }
    float z () { return z_; }
    float vx () { return vx_; }
    float vy () { return vy_; }
    float vz () { return vz_; }
    int r () { return r_; }
    int g () { return g_; }
    int b () { return b_; }
  //private:
    int r_, g_, b_;
    float origx_, origy_, origz_;
    float x_, y_, z_;
    float vx_, vy_, vz_;
    float angle_;
    float lifeRemaining_;
    float life_;
    bool isAlive_;
};

class ParticleEmittor
{
  public:
    // Singleton instanciation.
    static ParticleEmittor* instanciate();

    // Constructor.
    ParticleEmittor(int nbPart, std::string type = "explosion");

    // Getters.
    int nbPart () { return nbPart_; }
    std::string type () { return type_; }
    void vpart (int index, Particle* val) { vpart_[index] = val; }
    std::vector<Particle*> vpart () { return vpart_; }

    // Setters.
    void type (std::string type) { type_ = type; }

    // FIXME: put it in private.
    double t_;
  private:
    std::vector<Particle*> vpart_;
    int nbPart_;
    std::string type_;
    static ParticleEmittor* pe_;
};

class ParticleEngine
{
  public:
    ParticleEngine();
    ~ParticleEngine();
    void update();
    int addEmittor(ParticleEmittor* pe);
    void delEmittor(int pe);

  private:
    std::map<int, ParticleEmittor*> lpe_;
    int cpt_;
};

#endif
