// -*- C++ -*-

//=============================================================================
/**
 *  @file    Transport_Cache_Manager.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TRANSPORT_CACHE_MANAGER_H
#define TAO_TRANSPORT_CACHE_MANAGER_H

#include /**/ "ace/pre.h"

#include "tao/Transport_Cache_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;
class TAO_Transport_Descriptor_Interface;

namespace TAO
{
  typedef Transport_Cache_Manager_T<TAO_Transport, TAO_Transport_Descriptor_Interface, TAO_Connection_Purging_Strategy> Transport_Cache_Manager;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_CACHE_MANAGER_H */
