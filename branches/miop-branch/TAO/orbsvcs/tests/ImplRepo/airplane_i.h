// -*- C++ -*-

//=============================================================================
/**
 *  @file    airplane_i.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA server which returns a random
 *  paper airplane from the book "Oddballs, Wing-Flappers, & Spinners: 
 *  Great Paper Airplanes" by John Bringhurst.  ISBN: 0-07-067910-X (pbk.)
 *  An excellent book to have!  I personally recommend getting it just
 *  for the wing-flappers.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#if !defined (AIRPLANE_I_H)
#define	AIRPLANE_I_H

#include "AirplaneS.h"

// Forward declarations.
class Airplane_i;

// Typedefs.
typedef Airplane_i *Airplane_i_ptr;
typedef Airplane_i_ptr Airplane_i_ref;

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
  /// Constructor 
  Airplane_i ();

  /// Destructor
  ~Airplane_i (void);

  /// Returns a random plane.
  virtual char *get_plane (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* AIRPLANE_I_H */
