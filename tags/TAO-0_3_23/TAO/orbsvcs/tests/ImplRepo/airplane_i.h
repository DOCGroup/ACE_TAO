// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
// 
// = FILENAME
//    airplane_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA server which returns a random
//    paper airplane from the book "Oddballs, Wing-Flappers, & Spinners: 
//    Great Paper Airplanes" by John Bringhurst.  ISBN: 0-07-067910-X (pbk.)
//    An excellent book to have!  I personally recommend getting it just
//    for the wing-flappers.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (AIRPLANE_I_H)
#define	AIRPLANE_I_H

#include "AirplaneS.h"

// Forward declarations.
class Airplane_i;

// Typedefs.
typedef Airplane_i *Airplane_i_ptr;
typedef Airplane_i_ptr Airplane_i_ref;

class Airplane_i: public POA_Paper_Airplane_Server
{
  // = TITLE
  //    Paper Airplane Server Implementation
  //
  // = DESCRIPTION
  //    This server has one method that returns the featured paper airplane
  //    at this moment (in other words, a random airplane).
public:
  Airplane_i (int shutdown = 0);
  // Constructor that takes in an optional shutdown parameter which, if nonzero,
  // will shutdown the server after each call to get_plane.

  ~Airplane_i (void);
  // Destructor

  virtual char *get_plane (CORBA::Environment &ACE_TRY_ENV);
  // Returns a random plane.

private:
  int shutdown_;
  // Shutdown flag
};

#endif /* AIRPLANE_I_H */
