/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Dispatch_Set.h
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef CONSUMER_DISPATCH_SET
#define CONSUMER_DISPATCH_SET

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward reference.
class Connection_Handler;

using Consumer_Dispatch_Set = ACE_Unbounded_Set<Connection_Handler *>;
using Consumer_Dispatch_Set_Iterator = ACE_Unbounded_Set_Iterator<Connection_Handler *>;

#endif /* CONSUMER_DISPATCH_SET */
