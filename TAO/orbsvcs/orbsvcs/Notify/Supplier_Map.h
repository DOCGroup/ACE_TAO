/* -*- C++ -*- */
/**
 *  @file Supplier_Map.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_SUPPLIER_MAP_H
#define TAO_NS_SUPPLIER_MAP_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Event_Map_T.h"
#include "ProxyConsumer.h"

/**
 * @class TAO_NS_Supplier_Map
 *
 * @brief The Event Map for Suppliers.
 *
 */

typedef TAO_NS_Event_Map_T<TAO_NS_ProxyConsumer, TAO_SYNCH_RW_MUTEX> TAO_NS_Supplier_Map;

#include "ace/post.h"
#endif /* TAO_NS_SUPPLIER_MAP_H */
