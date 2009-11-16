// -*- C++ -*-

/**
 *  @file Supplier_Map.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SUPPLIER_MAP_H
#define TAO_Notify_SUPPLIER_MAP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Event_Map_T.h"
#include "orbsvcs/Notify/ProxyConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @typedef TAO_Notify_Supplier_Map
 *
 * @brief The Event Map for Suppliers.
 *
 */
typedef TAO_Notify_Event_Map_T<TAO_Notify_ProxyConsumer, TAO_SYNCH_RW_MUTEX> TAO_Notify_Supplier_Map;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SUPPLIER_MAP_H */
