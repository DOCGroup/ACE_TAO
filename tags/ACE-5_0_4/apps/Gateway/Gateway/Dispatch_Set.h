/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Dispatch_Set.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_DISPATCH_SET)
#define _DISPATCH_SET

#include "ace/Set.h"

// Forward reference.
class Proxy_Handler;

typedef ACE_Unbounded_Set<Proxy_Handler *> Dispatch_Set;
typedef ACE_Unbounded_Set_Iterator<Proxy_Handler *> Dispatch_Set_Iterator;

#endif /* _DISPATCH_SET */
