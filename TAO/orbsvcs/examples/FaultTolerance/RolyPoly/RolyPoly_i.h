// file      : RolyPoly/RolyPoly_i.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef ROLY_POLY_I_H
#define ROLY_POLY_I_H

#include "RolyPolyS.h"
#include "StateUpdate.h"

class RolyPoly_i : public virtual POA_RolyPoly,
                   public virtual Checkpointable
{
public:
  explicit RolyPoly_i (CORBA::ORB_ptr orb);

  // Checkpointable
  CORBA::Any *get_state () override;

  void set_state (CORBA::Any const &state) override;

  // RolyPoly
  CORBA::Short number (char *&s) override;

  void shutdown () override;

  RolyPoly_i (RolyPoly_i const &) = delete;
  RolyPoly_i (RolyPoly_i &&) = delete;
  RolyPoly_i &operator= (RolyPoly_i const &) = delete;
  RolyPoly_i &operator= (RolyPoly_i &&) = delete;

private:
  CORBA::Short number_ {};

  CORBA::ORB_var orb_;
};

#endif  /* ROLY_POLY_I_H */
