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

#if !defined (ACE_SELECT_REACTOR_H)
#define ACE_SELECT_REACTOR_H

#include "ace/Select_Reactor_T.h"

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

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SELECT_REACTOR_H */
