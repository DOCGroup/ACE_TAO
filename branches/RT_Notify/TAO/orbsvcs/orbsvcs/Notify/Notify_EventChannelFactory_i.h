//=============================================================================
/**
 * @file Notify_EventChannelFactory_i.h
 *
 * $Id$
 *
 * Implements the CosNotifyChannelAdmin::EventChannelFactory interface.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_EVENTCHANNELFACTORY_I_H
#define NOTIFY_EVENTCHANNELFACTORY_I_H
#include "ace/pre.h"

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

class TAO_Notify_CO_Factory;
class TAO_Notify_POA_Factory;

// @@ Pradeep: here is a crazy idea: some people may want to create
// event channels as separate processes, for protection purposes.  You
// may need to strategize this class to be able to do so...  You could
// probably use the ImplRepo or something similar to make that work
// portably.  I don't mean that you should do that right now, but
// consider it in your design.

/**
 * @class TAO_Notify_EventChannelFactory_i
 *
 * @brief The EventChannelFactory_i creates Event Channel objects and asigns
 * IDs to it.
 *
 * It also has methods to get a previously created channel based on its
 * ID.
 */
class TAO_Notify_Export TAO_Notify_EventChannelFactory_i
  : public virtual POA_CosNotifyChannelAdmin::EventChannelFactory
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Destructor
  virtual ~TAO_Notify_EventChannelFactory_i (void);

  /// Create a factory servant and activates it with the default POA.
  /// Also creates a resource factory and assigns it this default_POA.
  static CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL);

  /// Get the CORBA object.
  CosNotifyChannelAdmin::EventChannelFactory_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Destroy the factory
   * By default, the factory's lifetime is not coupled with its creations
   * Setting the <destroy_children> flag to 'true' will destory all event_channels that were created by this factory.
   */
  void shutdown (ACE_ENV_SINGLE_ARG_DECL
#if !defined (TAO_HAS_EXCEPTIONS) || defined (ACE_ENV_BKWD_COMPAT)
  ,    // This is ugly -- destroy_children should come BEFORE the env arg.
#endif
                 CORBA::Boolean destroy_children = 0);

  /// Called by child EC's when they're about to go away.
  void event_channel_destroyed (CosNotifyChannelAdmin::ChannelID channel_id);

  // = Accesors
  /// Accesor for the default filter factory shared by all EC's.
  CosNotifyFilter::FilterFactory_ptr get_default_filter_factory (void);

  // = CosNotifyChannelAdmin::EventChannelFactory methods.
  virtual CosNotifyChannelAdmin::EventChannel_ptr create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS,
    CosNotification::UnsupportedAdmin
  ));

  virtual CosNotifyChannelAdmin::ChannelIDSeq * get_all_channels (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::EventChannel_ptr get_event_channel (
    CosNotifyChannelAdmin::ChannelID id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ChannelNotFound
  ));

 protected:
  /// Constructor
  TAO_Notify_EventChannelFactory_i (void);

  /// Initializes this object.
  void init_i (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL);

  /// Create the default filter factory.
  CosNotifyFilter::FilterFactory_ptr create_default_filter_factory_i (ACE_ENV_SINGLE_ARG_DECL);

  // = Data members
  /// The factory for channel objects.
  TAO_Notify_CO_Factory* channel_objects_factory_;

  /// The factory for POA based containers.
  TAO_Notify_POA_Factory* poa_factory_;

  /// The POA in which we live.
  /// Filter objects and COS EC style proxys live here too.
  PortableServer::POA_var my_POA_;

  /// The POA in which we should activate EC's in.
  /// We create and own this.
  PortableServer::POA_var ec_POA_;

  /// Id generator for event channels
  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ChannelID,
    CosNotifyChannelAdmin::ChannelIDSeq> ec_ids_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// The default filter factory.
  CosNotifyFilter::FilterFactory_var default_filter_factory_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
