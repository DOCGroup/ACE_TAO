/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   ID_Pool_T.h
//
// = DESCRIPTION
//   A class to generate ID's and recycle them.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef ID_POOL_T_H
#define ID_POOL_T_H

// Need this to get ACE macros
// @@ Pradeep, we *never* #include this directly, use ace/OS.h
// instead.
#include "ace/config.h"

  // @@ Pradeep: ID_Pool should have some prefix, TAO_Notify or
  // something similar.
template <class ID_TYPE>
class ID_Pool
{
  // = TITLE
  //   ID_Pool
  // = DESCRIPTION
  //   This class is used by factories that need to associate id's with the
  //   objects that they create.When the objects are destroyed, these id's
  //   can be reused, hence the need to recycle.
  //

  // @@ Pradeep: fix the indentation, leave some blank lines, in
  // general make this stuff more readable.
 public:
  ID_TYPE get (void);
  // Returns a new or recycled ID

  void put (ID_TYPE id);
  // Put an ID back so that it might be recycled.
 private:
  ID_TYPE id_;
};

#if defined (__ACE_INLINE__)
#include "ID_Pool_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ID_Pool_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ID_Pool.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ID_POOL_T_H */
