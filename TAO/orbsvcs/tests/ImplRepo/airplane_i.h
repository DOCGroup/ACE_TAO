// -*- C++ -*-

//=============================================================================
/**
 *  @file    airplane_i.h
 *
 *  This class implements a simple CORBA server which returns a random
 *  paper airplane from the book "Oddballs, Wing-Flappers, & Spinners:
 *  Great Paper Airplanes" by John Bringhurst.  ISBN: 0-07-067910-X (pbk.)
 *  An excellent book to have!  I personally recommend getting it just
 *  for the wing-flappers.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#if !defined (AIRPLANE_I_H)
#define AIRPLANE_I_H

#include "AirplaneS.h"

/**
 * @class Airplane_i:
 *
 * @brief Paper Airplane Server Implementation
 *
 * This server has one method that returns the featured paper airplane
 * at this moment (in other words, a random airplane).
 */
class Airplane_i: public POA_Paper_Airplane_Server
{
public:
  Airplane_i ();

  Airplane_i (Airplane_i const &) = delete;
  Airplane_i &operator= (Airplane_i const &) = delete;
  Airplane_i (Airplane_i &&) = delete;
  Airplane_i &operator= (Airplane_i &&) = delete;

  ~Airplane_i () override = default;

  /// Returns a random plane.
  char *get_plane () override;
};

#endif /* AIRPLANE_I_H */
