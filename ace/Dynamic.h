/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    ACE_Dynamic.h 
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyrarli.
// 
// ============================================================================

#if !defined (ACE_DYNAMIC_H)
#define ACE_DYNAMIC_H 

#include "ace/ACE.h"

class ACE_Export ACE_Dynamic
  // = TITLE
  //     Checks to see if an object was dynamically allocated.
  //
  // = DESCRIPTION
  //     This class holds the pointer in a thread-safe manner between
  //     the call to operator new and the call to the constructor.
{
public:
  ACE_Dynamic (void);

  void set (void);
  // Sets a flag that indicates that the object was dynamically
  // created. This method is usually called in operator new and then
  // checked and reset in the constructor.

  int is_dynamic (void);
  // 1 if we were allocated dynamically, else 0.

  void reset (void);
  // Resets state flag.

private:
  int is_dynamic_;
  // Flag that indicates that the object was dynamically created. This
  // method is usually called in operator new and then checked and
  // reset in the constructor.
};

#if defined (__ACE_INLINE__)
#include "ace/Dynamic.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_DYNAMIC_H */

