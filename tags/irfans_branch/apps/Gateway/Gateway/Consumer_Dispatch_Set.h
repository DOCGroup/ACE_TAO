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

#if !defined (_DISPATCH_SET)
#define _DISPATCH_SET

#include "ace/Containers.h"

// Forward reference.
class Proxy_Handler;

typedef ACE_Unbounded_Set<Proxy_Handler *> Consumer_Dispatch_Set;
typedef ACE_Unbounded_Set_Iterator<Proxy_Handler *> Consumer_Dispatch_Set_Iterator;

#endif /* _DISPATCH_SET */
