/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Factory.h
//
// = DESCRIPTION
//   A collection of factories -
//   a) A factory for event channel objects - the EC factory, EC, Admins, Proxys, et al.
//   b) A factory for collections.
//   c) A factory for POA's.
//   d) A factory for manager objects.
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef NOTIFY_FACTORY_H
#define NOTIFY_FACTORY_H

#include "ace/pre.h"
#include "tao/corba.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_CO_Factory;
class TAO_Notify_POA_Factory;
class TAO_Notify_Collection_Factory;
class TAO_Notify_EMO_Factory;
class TAO_Notify_EventChannelFactory_i;

class TAO_Notify_Export TAO_Notify_Factory
{
  // = TITLE
  //   TAO_Notify_Factory
  //
  // = DESCRIPTION
  //   This classes initializes the static default factories.
  //
 public:
  static TAO_Notify_CO_Factory* get_channel_objects_factory (void);
  // The factory for channel objects.

  static TAO_Notify_POA_Factory* get_poa_factory (void);
  // The factory for POA's.

  static TAO_Notify_Collection_Factory* get_collection_factory (void);
  // Factory for all collections in the notify service.

  static TAO_Notify_EMO_Factory* create_event_manager_objects_factory (void);
  // The event manager objects.

 protected:
  friend class TAO_Notify_EventChannelFactory_i;

  static void init (TAO_ENV_SINGLE_ARG_DECL);
  // Initalize the services.

  static void shutdown (void);
  // shutdown the svcs.

  // = Data Members
  static TAO_Notify_CO_Factory* co_factory_;
  static TAO_Notify_POA_Factory* poa_factory_;
  static TAO_Notify_Collection_Factory* collection_factory_;
  static TAO_Notify_EMO_Factory* emo_factory_;
};

#define TAO_NOTIFY_DEF_CO_FACTORY_NAME "Notify_Default_Channel_Objects_Factory"
#define TAO_NOTIFY_DEF_POA_FACTORY_NAME "Notify_Default_POA_Factory"
#define TAO_NOTIFY_DEF_COLLECTION_FACTORY_NAME "Notify_Default_Collection_Factory"
#define TAO_NOTIFY_DEF_EMO_FACTORY_NAME "Notify_Default_Event_Manager_Objects_Factory"

#include "ace/post.h"
#endif /* NOTIFY_FACTORY_H */
