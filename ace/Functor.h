/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor.h
//
// = DESCRIPTION
//    Non-templatized classes for implementing the GOF Command Pattern,
//    also known as functors or function objects.
//
// = AUTHOR
//    Chris Gill           <cdgill@cs.wustl.edu>
//
//    Based on Command Pattern implementations originally done by
//
//    Carlos O'Ryan        <coryan@cs.wustl.edu>  and
//    Douglas C. Schmidt   <schmidt@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FUNCTOR_H
#define ACE_FUNCTOR_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Command_Base
{
  // = TITLE
  //    Defines an abstract class that allows us to invoke commands
  //    without knowing anything about the implementation.
  //
  // = DESCRIPTION
  //    This class declares an interface to execute a command
  //    independent of the effect of the command, or the objects used
  //    to implement it.
public:
  // = Initialization and termination methods.
  ACE_Command_Base (void);
  // Default constructor.

  virtual ~ACE_Command_Base (void);
  // Virtaul destructor.

  virtual int execute (void *arg = 0) = 0;
  // Invokes the method encapsulated by the command, passing along the
  // passed argument (if any).  Users of classes derived from this
  // class must ensure that the resulting invocation can tolerate a
  // null void pointer being passed, or otherwise ensure that this
  // will never occur.
};

#if defined (__ACE_INLINE__)
#include "ace/Functor.i"
#endif /* __ACE_INLINE__ */

// Include the templates here.
#include "ace/Functor_T.h"

#endif /* ACE_FUNCTOR_H */
