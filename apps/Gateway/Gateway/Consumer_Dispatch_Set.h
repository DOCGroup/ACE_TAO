/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
//
// = FILENAME
//    Consumer_Dispatch_Set.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef CONSUMER_DISPATCH_SET
#define CONSUMER_DISPATCH_SET

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward reference.
class Connection_Handler;

typedef ACE_Unbounded_Set<Connection_Handler *> Consumer_Dispatch_Set;
typedef ACE_Unbounded_Set_Iterator<Connection_Handler *> Consumer_Dispatch_Set_Iterator;

#endif /* CONSUMER_DISPATCH_SET */
