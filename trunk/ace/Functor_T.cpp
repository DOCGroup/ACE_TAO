/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Functor_T.cpp
//
// = DESCRIPTION
//    Non-inlinable method definitions for templatized classes
//    implementing the GOF Command Pattern, or STL-style functors.
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
//    and on STL-style functor implementations originally done by
//
//    Irfan Pyarali  <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FUNCTOR_T_C
#define ACE_FUNCTOR_T_C

#define ACE_BUILD_DLL
#include "ace/Functor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Functor_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Functor_T, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Command_Callback)


///////////////////////////////////
// GOF Command Pattern Templates //
///////////////////////////////////

// Constructor.

template <class RECEIVER, class ACTION>
ACE_Command_Callback<RECEIVER, ACTION>::ACE_Command_Callback (RECEIVER &recvr,
                                    ACTION action)
  : receiver_ (recvr),
    action_ (action)
{
}

// Invokes an operation.

template <class RECEIVER, class ACTION> int
ACE_Command_Callback<RECEIVER, ACTION>::execute (void *arg)
{
  return (receiver_.*action_) (arg);
}


#endif /* ACE_FUNCTOR_T_C */
