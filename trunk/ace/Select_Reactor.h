/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Select_Reactor.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SELECT_REACTOR_H
#define ACE_SELECT_REACTOR_H

#include "ace/Select_Reactor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#if defined (ACE_SELECT_REACTOR_HAS_DEADLOCK_DETECTION)
#include "ace/Local_Tokens.h"
typedef ACE_Select_Reactor_Token_T<ACE_Local_Mutex> ACE_Select_Reactor_Token;
#else
typedef ACE_Select_Reactor_Token_T<ACE_Token> ACE_Select_Reactor_Token;
#endif /* ACE_SELECT_REACTOR_HAS_DEADLOCK_DETECTION */
#else
typedef ACE_Select_Reactor_Token_T<ACE_Noop_Token> ACE_Select_Reactor_Token;
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

typedef ACE_Select_Reactor_T<ACE_Select_Reactor_Token> ACE_Select_Reactor;

class ACE_Export ACE_Guard< ACE_Select_Reactor_Token_T<ACE_Noop_Token> >
{
  // = TITLE
  //     Template specialization of <ACE_Guard> for the
  //     <ACE_Null_Mutex>.
  //
  // = DESCRIPTION
  //     This specialization is useful since it helps to speedup
  //     performance of the "Null_Mutex" considerably.
public:
  // = Initialization and termination methods.
  ACE_Guard (ACE_Select_Reactor_Token_T<ACE_Noop_Token> &) {}
  ACE_Guard (ACE_Select_Reactor_Token_T<ACE_Noop_Token> &, int) {}
  ~ACE_Guard (void) {}

  int acquire (void) { return 0; }
  int tryacquire (void) { return 0; }
  int release (void) { return 0; }
  int locked (void) { return 1; }
  int remove (void) { return 0; }
  void dump (void) const {}

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Guard< ACE_Select_Reactor_Token_T<ACE_Noop_Token> > &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Guard (const ACE_Guard< ACE_Select_Reactor_Token_T<ACE_Noop_Token> > &))
};

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SELECT_REACTOR_H */
