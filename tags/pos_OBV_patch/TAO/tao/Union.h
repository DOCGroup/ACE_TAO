/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Union.h
//
// = AUTHOR
//
//    Aniruddha Gokhale (Idea by Carlos O'Ryan)
//
// ============================================================================

#ifndef TAO_UNION_H
#define TAO_UNION_H

#include "tao/corbafwd.h"

class TAO_Export TAO_Base_Union
{
  // = TITLE
  //   Base class for TAO Unions.
  //
  // = DESCRIPTION
  //   This class provides a common interface for all IDL unions,
  //   hence the interpreted marshal engine can manipulate them in a
  //   type-safe manner.
public:
  friend class TAO_Marshal_Union;
  // We give access to TAO_Marshal_Union, this allows a safe
  // implementation of the marshal engine.

  virtual ~TAO_Base_Union (void);
  // destructor.

  virtual void *_discriminant (void) = 0;
  // Return pointer to the discriminant.

  virtual void *_access (CORBA::Boolean flag) = 0;
  // Provides access to the right data member at the right offset. If
  // the flag is TRUE, we allocate the necessary storage. This will be
  // required for the decoding side.

  virtual void _reset (void) = 0;
  // Free existing storage.

protected:
  TAO_Base_Union (void);
  // Default constructor.

};

#endif /* TAO_BASE_UNION_H */
