/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor_T.h
//
// = DESCRIPTION
//     Templatized classes for implementing the GOF Command Pattern,
//     also known as functors or function objects.
//
// = AUTHOR
//     Chris Gill          <cdgill@cs.wustl.edu>
//
//     Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FUNCTOR_T_H
#define ACE_FUNCTOR_T_H

#include "ace/Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class RECEIVER, class ACTION>
class ACE_Command_Callback : public ACE_Command_Base
{
  // = TITLE
  //    Defines a class template that allows us to invoke a callback to an
  //    object without knowing anything about the object except its type.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute operations,
  //    binding a RECEIVER object with an ACTION.  The RECEIVER knows
  //    how to implement the operation.  A class can invoke operations
  //    without knowing anything about it, or how it was implemented.
public:

  ACE_Command_Callback (RECEIVER &recvr, ACTION action);
  // Constructor: sets the <receiver_> of the Command to recvr, and the
  // <action_> of the Command to <action>.

  virtual ~ACE_Command_Callback (void) {}
  // Virtual destructor.

  virtual int execute (void *arg = 0);
  // Invokes the method <action_> from the object <receiver_>.

private:

  RECEIVER &receiver_;
  // Object where the method resides.

  ACTION action_;
  // Method that is going to be invoked.
};


#if defined (__ACE_INLINE__)
#include "ace/Functor_T.i"
#endif /* __ACE_INLINE__ */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Functor_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Functor_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_FUNCTOR_T_H */

// EOF
