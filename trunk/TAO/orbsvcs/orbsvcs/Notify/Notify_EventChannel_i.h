// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_EventChannel_i.h
//
// = DESCRIPTION
//   Implements the CosNotifyChannelAdmin::EventChannel interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_EVENTCHANNEL_I_H_
#define TAO_NOTIFY_EVENTCHANNEL_I_H_
#include "ace/pre.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_Collection.h"

class TAO_Notify_EventChannelFactory_i;
class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;
class TAO_Notify_CO_Factory;
class TAO_Notify_POA_Factory;
class TAO_Notify_EMO_Factory;
class TAO_Notify_EventListener;

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_EventChannel_i : public virtual POA_CosNotifyChannelAdmin::EventChannel, public PortableServer::RefCountServantBase
{
  // = TITLE
  //
  //   TAO_Notify_EventChannel_i
  //
  // = DESCRIPTION
  //   This class handles all the object factory functionality.All the event
  //   routing is handled by its contained Event Manager class.
  //
  // = MISC. NOTES
  //   This class creates and owns 2 child POA's. one to contain CA's and the other for SA's.
  //   Now, a Channel object can be destoyed in 2 ways - as a side effect of its parent POA being
  //   destroyed or if this class's <destroy> method is invoked. If the object is being destroyed
  //   as a result of its Parent POA being destroyed, it is illegal to destroy  a childPOA
  //   because, the root poa destruction will destroy all child poa's.
  //   So in the destructor we conditionally check if the child POAs should be destroyed explicitly
  //   or not.
  //
 public:
  TAO_Notify_EventChannel_i (TAO_Notify_EventChannelFactory_i* channel_factory);
  // Constructor.
  // <channel_factory> is the parent.

  virtual ~TAO_Notify_EventChannel_i (void);
  // Destructor

  void init (CosNotifyChannelAdmin::ChannelID channel_id,
             const CosNotification::QoSProperties& initial_qos,
             const CosNotification::AdminProperties& initial_admin,
             PortableServer::POA_ptr default_POA,
             PortableServer::POA_ptr my_POA
             TAO_ENV_ARG_DECL);
  // Initialize this object.
  // checks if the <initial_qos> and <initial admin> are valid.
  // creates default filter, consumer admin and supplier admin.

  // = Accessors
  PortableServer::POA_ptr get_default_POA (void);
  // Get the default POA.

  CosNotifyChannelAdmin::EventChannel_ptr get_ref (TAO_ENV_SINGLE_ARG_DECL);
  // Get the CORBA object for this servant

  TAO_Notify_Event_Manager* get_event_manager (void);
  // Get the event manager.

  // = Child destroyed notification.
  void consumer_admin_destroyed (CosNotifyChannelAdmin::AdminID CA_ID);
  // This id is no longer in use.It can be reused by <consumer_admin_ids_>.

  void supplier_admin_destroyed (CosNotifyChannelAdmin::AdminID SA_ID);
  // This id is no longer in use.It can be reused by <supplier_admin_ids_>.

  void unregister_listener (TAO_Notify_EventListener* group_listener TAO_ENV_ARG_DECL);
  // Consumer Admin's are Group Listeners that are registered automatically with the EC when a ConsumerAdmin
  // is created. When a consumer is destroyed, it asks the EC to unregister itself.

  // = Interface methods
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::FilterFactory_ptr default_filter_factory (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (
    CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID_out id
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (
    CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID_out id
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr get_consumeradmin (
    CosNotifyChannelAdmin::AdminID id
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminNotFound
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr get_supplieradmin (
    CosNotifyChannelAdmin::AdminID id
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminNotFound
  ));

virtual CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_qos (
    const CosNotification::QoSProperties & qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual CosNotification::AdminProperties * get_admin (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_admin (
    const CosNotification::AdminProperties & admin
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedAdmin
  ));

  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void destroy (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
  ));

  const TAO_Notify_QoSAdmin_i& qos_admin (void) const;

protected:
  // = Data Members
  ACE_Lock* lock_;
  // The locking strategy.

  CORBA::Boolean destory_child_POAs_;
  // Flag to tell our destructor if we should destroy the CA and SA POA's.
  // default is false, the parent poa destruction will remove these.
  // set to true if the <destroy> method is invoked.

  TAO_Notify_EventChannelFactory_i* channel_factory_;
  // The factory that created us.

  PortableServer::POA_var default_POA_;
  // The default POA in which we activate objects that don't have ids' pre-assigned.

  PortableServer::POA_var my_POA_;
  // The POA in which i live.

  PortableServer::POA_var CA_POA_;
  // The POA in which we should activate ConsumerAdmins in.
  // We create and own this.

  PortableServer::POA_var SA_POA_;
  // The POA in which we should activate SupplierAdmins in.
  // We create and own this.

  CosNotifyChannelAdmin::ChannelID channel_id_;
  // The ID assigned to this channel.

  TAO_Notify_CO_Factory* channel_objects_factory_;
  // The factory for channel objects.

  TAO_Notify_POA_Factory* poa_factory_;
  // The factory for POA based containers.

  TAO_Notify_EMO_Factory* event_manager_objects_factory_;
  // Event manager objects factory,

  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID,
    CosNotifyChannelAdmin::AdminIDSeq> consumer_admin_ids_;
  // Id generator for consumer admins.

  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::AdminID,
    CosNotifyChannelAdmin::AdminIDSeq> supplier_admin_ids_;
  // Id generator for supplier admins.

  const CosNotifyChannelAdmin::InterFilterGroupOperator default_op_;
  // Default InterFilterGroupOperator operator used when creating
  // the default ConsumerAdmin and SupplierAdmin.

  const CosNotifyChannelAdmin::AdminID default_id_;
  // Default id's to CosEventChannelAdmin::ConsumerAdmin, SupplierAdmin.

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_Event_Manager* event_manager_;
  // The event manager.

  TAO_Notify_EventListener_List* event_listener_list_;
  // The list of group event listeners that have registered with us.
};

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENTCHANNEL_I_H_ */
