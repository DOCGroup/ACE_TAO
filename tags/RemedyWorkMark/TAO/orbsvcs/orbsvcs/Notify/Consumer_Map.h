/* -*- C++ -*- */
/**
 *  @file Consumer_Map.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONSUMER_MAP_H
#define TAO_Notify_CONSUMER_MAP_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "orbsvcs/Notify/Event_Map_T.h"
#include "orbsvcs/Notify/ProxySupplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Consumer_Map
 *
 * @brief The Event Map for Consumers.
 *
 */
typedef TAO_Notify_Event_Map_T<TAO_Notify_ProxySupplier,
                               TAO_SYNCH_RW_MUTEX>
  TAO_Notify_Consumer_Map;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Notify_CONSUMER_MAP_H */
