/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Reactor_Singleton.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef _REACTOR_SINGLETON_H
#define _REACTOR_SINGLETON_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"

// Our global Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> REACTOR;

#endif /* _REACTOR_SINGLETON_H */
