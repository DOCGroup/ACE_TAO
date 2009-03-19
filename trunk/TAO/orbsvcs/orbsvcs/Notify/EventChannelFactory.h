/* -*- C++ -*- */
/**
 *  @file EventChannelFactory.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENTCHANNELFACTORY_H
#define TAO_Notify_EVENTCHANNELFACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/Topology_Factory.h"
#include "orbsvcs/Notify/Reconnection_Registry.h"
#include "orbsvcs/Notify/Routing_Slip.h"
#include "orbsvcs/Notify/Validate_Client_Task.h"
#include "orbsvcs/Notify/Name_Value_Pair.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtS.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_FilterFactory;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_EventChannel;
template <class TYPE> class TAO_Notify_Container_T;

/**
 * @class TAO_Notify_EventChannelFactory
 *
 * @brief Implementation of CosNotifyChannelAdmin::EventChannelFactory
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_EventChannelFactory
  : public virtual POA_NotifyExt::EventChannelFactory
  , public TAO_Notify::Topology_Parent
{
  friend class TAO_Notify_Builder;
  typedef ACE_Unbounded_Set <TAO_Notify::Routing_Slip_Ptr> Routing_Slip_Set;

public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_EventChannelFactory > Ptr;

  /// Constructor
  TAO_Notify_EventChannelFactory (void);

  /// Init the factory
  void init (PortableServer::POA_ptr poa);

  /// Destructor
  virtual ~TAO_Notify_EventChannelFactory ();

  /// = ServantBase  Methods
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

  /// Remove @a channel from the <ec_container_>
  virtual void remove (TAO_Notify_EventChannel* channel);

  /// This method is called by the Notify_Service when the event channel
  /// is automatically created and bound in the name service.
  virtual CosNotifyChannelAdmin::EventChannel_ptr create_named_channel (
      const CosNotification::QoSProperties& initial_qos,
      const CosNotification::AdminProperties& initial_admin,
      CosNotifyChannelAdmin::ChannelID_out id,
      const char* name);

  //////////////////////////
  // The following methods are for
  // unit testing and debugging

  /// Use the registered Topology_Factory to create a loader, and
  /// load the topology. If no Topology_Factory is registered
  /// then nothing will be loaded.
  void load_topology (void);

  /// Use the passed in saver factory to generate topology saver objects.
  /// Does not take ownership.
  void set_topology_factory(TAO_Notify::Topology_Factory* sf);

  //-- Topology_Parent

  virtual bool is_persistent (void) const;

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  virtual bool change_to_parent (void);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
                                                   CORBA::Long id,
                                                   const TAO_Notify::NVPList& attrs);
  CosNotifyChannelAdmin::EventChannelFactory_ptr activate_self (void);
  virtual void reconnect (void);
  virtual void validate ();

  /// at shutdown time, this causes the validator thread to exit.
  void stop_validator (void);

  /// Handle change notifications
  bool handle_change (void);

  void load_event_persistence (void);

  virtual void save_topology (void);

  TAO_Notify_ProxyConsumer * find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position);
  TAO_Notify_ProxySupplier * find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position);
  TAO_Notify_Object * follow_id_path (TAO_Notify::IdVec & id_path, size_t position);
  virtual TAO_Notify_Object::ID get_id (void) const;

private:

  /// = Data Members

  /// = NotifyExt methods
  virtual void destroy (void);

  /// shutdown
  virtual int shutdown (void);

  virtual
  NotifyExt::ReconnectionRegistry::ReconnectionID register_callback (
      NotifyExt::ReconnectionCallback_ptr reconnection);

  virtual void unregister_callback (
      NotifyExt::ReconnectionRegistry::ReconnectionID id);

  virtual CORBA::Boolean is_alive (void);

protected:
  /// = CosNotifyChannelAdmin Methods
  virtual ::CosNotifyChannelAdmin::EventChannel_ptr create_channel (
      const CosNotification::QoSProperties & initial_qos,
      const CosNotification::AdminProperties & initial_admin,
      CosNotifyChannelAdmin::ChannelID_out id);

  virtual ::CosNotifyChannelAdmin::ChannelIDSeq * get_all_channels (void);

  virtual ::CosNotifyChannelAdmin::EventChannel_ptr get_event_channel (
      CosNotifyChannelAdmin::ChannelID id);

private:
  typedef TAO_Notify_Container_T<TAO_Notify_EventChannel> TAO_Notify_EventChannel_Container;

  TAO_Notify_EventChannel_Container& ec_container();

  /// Container for Event Channels.
  ACE_Auto_Ptr< TAO_Notify_EventChannel_Container > ec_container_;

  TAO_SYNCH_MUTEX topology_save_lock_;

  CosNotifyChannelAdmin::EventChannelFactory_var channel_factory_;

  /// Change-in-progress detector to avoid duplicates
  short topology_save_seq_;
  TAO_Notify::Topology_Factory* topology_factory_;
  TAO_Notify::Reconnection_Registry reconnect_registry_;
  bool loading_topology_;

  Routing_Slip_Set routing_slip_restart_set_;

  /// Release this object.
  virtual void release (void);

  ACE_Auto_Ptr <TAO_Notify_validate_client_Task> validate_client_task_;

  PortableServer::POA_var poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENTCHANNELFACTORY_H */
