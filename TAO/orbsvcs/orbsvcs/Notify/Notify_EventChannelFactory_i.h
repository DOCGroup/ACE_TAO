// $Id$
// ==========================================================================
//
// = LIBRARY
//   Orbsvcs
//
// = FILENAME
//   Notify_EventChannelFactory_i
//
// = DESCRIPTION
//   Implements the CosNotifyChannelAdmin::EventChannelFactory interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_EVENTCHANNELFACTORY_I_H
#include "ace/pre.h"
#define NOTIFY_EVENTCHANNELFACTORY_I_H

#include "Notify_ID_Pool_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Resource_Manager;

class TAO_Notify_Export TAO_Notify_EventChannelFactory_i : public virtual POA_CosNotifyChannelAdmin::EventChannelFactory, public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_EventChannelFactory_i
  //
  // = DESCRIPTION
  //   The EventChannelFactory_i creates Event Channel objects and asigns
  //   IDs to it.
  //   It also has methods to get a previously created channel based on its
  //   ID.
  //
public:
  virtual ~TAO_Notify_EventChannelFactory_i (void);
  // Destructor

  static CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV);
  // Create a factory servant and activates it with the default POA.
  // Also creates a resource factory and assigns it this default_POA.

  TAO_Notify_Resource_Manager* get_resource_manager (void);
  // Get the resource manager;

  // = CosNotifyChannelAdmin::EventChannelFactory methods.
virtual CosNotifyChannelAdmin::EventChannel_ptr create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS,
    CosNotification::UnsupportedAdmin
  ));

virtual CosNotifyChannelAdmin::ChannelIDSeq * get_all_channels (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::EventChannel_ptr get_event_channel (
    CosNotifyChannelAdmin::ChannelID id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ChannelNotFound
  ));

 protected:
  TAO_Notify_EventChannelFactory_i (void);
  // Constructor

  void init_i (PortableServer::POA_ptr default_POA, CORBA::Environment &ACE_TRY_ENV);
  // Initializes this object.

  void cleanup_i (void);
  // Cleanup this object.

  // = Data members
 TAO_Notify_Resource_Manager* resource_manager_;
 // This factory is owned by the Event Channel Factory.

 CosNotifyChannelAdmin::EventChannelFactory_var my_ref_;
 // My CORBA object.

 PortableServer::POA_var my_POA_;
 // The POA in which we live.

 PortableServer::POA_var ec_POA_;
 // The POA in which we should activate EC's in.
 // We create and own this.

 TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID,
   CosNotifyChannelAdmin::ChannelIDSeq> ec_ids_;
 // Id generator for event channels
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
