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

#if !defined (CONSUMER_DISPATCH_SET)
#define CONSUMER_DISPATCH_SET

#include "ace/Containers.h"

// Forward reference.
class Connection_Handler;

typedef ACE_Unbounded_Set<Connection_Handler *> Consumer_Dispatch_Set;
typedef ACE_Unbounded_Set_Iterator<Connection_Handler *> Consumer_Dispatch_Set_Iterator;

#endif /* CONSUMER_DISPATCH_SET */
