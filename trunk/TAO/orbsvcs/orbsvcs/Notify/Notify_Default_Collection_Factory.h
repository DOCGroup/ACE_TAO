/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Notification
//
// = FILENAME
//   Notify_Default_Collection_Factory.h
//
// = DESCRIPTION
//   Default factory for collections
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef NOTIFY_DEFAULT_COLLECTION_FACTORY_H
#define NOTIFY_DEFAULT_COLLECTION_FACTORY_H
#include "ace/pre.h"

#include "notify_export.h"
#include "Notify_Collection_Factory.h"
#include "Notify_Listeners.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_Export TAO_Notify_Default_Collection_Factory : public TAO_Notify_Collection_Factory
{
  // = TITLE
  //   TAO_Notify_Default_Collection_Factory
  //
  // = DESCRIPTION
  //   Default factory for collections
  //
public:
  TAO_Notify_Default_Collection_Factory (void);
  ~TAO_Notify_Default_Collection_Factory ();

  static int init_svc (void);
  // Helper function to register the default factory into the service
  // configurator.

  // = The Service_Object entry points
  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  // = Methods to create listener collections.
  virtual TAO_Notify_EventListener_List* create_event_listener_list (ACE_ENV_SINGLE_ARG_DECL);
  // Create the event listener list.

 virtual TAO_Notify_UpdateListener_List* create_update_listener_list (ACE_ENV_SINGLE_ARG_DECL);
  // create update listener list.
};

ACE_STATIC_SVC_DECLARE (TAO_Notify_Default_Collection_Factory)
ACE_FACTORY_DECLARE (TAO_Notify,TAO_Notify_Default_Collection_Factory)

#include "ace/post.h"
#endif /* NOTIFY_DEFAULT_COLLECTION_FACTORY_H */
