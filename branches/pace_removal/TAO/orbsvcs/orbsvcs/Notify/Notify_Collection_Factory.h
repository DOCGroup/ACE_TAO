/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Collection_Factory.h
 *
 *  $Id$
 *
 * A factory fpr the collection types used by Notify.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_COLLECTION_FACTORY_H
#define NOTIFY_COLLECTION_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_export.h"
#include "Notify_Collection.h"

/**
 * @class TAO_Notify_Collection_Factory
 *
 * @brief TAO_Notify_Collection_Factory
 *
 * Collection factory
 */
class TAO_Notify_Export TAO_Notify_Collection_Factory : public ACE_Service_Object
{
public:
  // = Methods to create listener collections.
  /// Create the event listener list.
  virtual TAO_Notify_EventListener_List* create_event_listener_list (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Create update listener list.
 virtual TAO_Notify_UpdateListener_List* create_update_listener_list (ACE_ENV_SINGLE_ARG_DECL) = 0;
};

#include "ace/post.h"

#endif /* NOTIFY_COLLECTION_FACTORY_H */
