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

#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_Event_Manager.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_EventChannelFactory_i;
class TAO_Notify_Resource_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
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
 public:
  TAO_Notify_EventChannel_i (CosNotifyChannelAdmin::EventChannelFactory_ptr my_factory, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor.
  // <my_factory> is the parent.

  virtual ~TAO_Notify_EventChannel_i (void);
  // Destructor

  void init (const CosNotification::QoSProperties& initial_qos,
             const CosNotification::AdminProperties& initial_admin,
             PortableServer::POA_ptr my_POA,
             CORBA::Environment &ACE_TRY_ENV);
  // Initialize this object.
  // checks if the <initial_qos> and <initial admin> are valid.
  // creates default filter, consumer admin and supplier admin.
  // If any part of the initialization fails, the <cleanup_i> method
  // is called to undo any resource allocations.

  CosNotifyChannelAdmin::EventChannel_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Get the CORBA object for this servant

  TAO_Notify_Event_Manager* get_event_manager (void);
  // Get the event manager.

  void consumer_admin_destroyed (CosNotifyChannelAdmin::AdminID CA_ID);
  // This id is no longer in use.It can be reused by <consumer_admin_ids_>.

  void supplier_admin_destroyed (CosNotifyChannelAdmin::AdminID SA_ID);
  // This id is no longer in use.It can be reused by <supplier_admin_ids_>.

  // = Interface methods
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::FilterFactory_ptr default_filter_factory (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (
    CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID_out id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (
    CosNotifyChannelAdmin::InterFilterGroupOperator op,
    CosNotifyChannelAdmin::AdminID_out id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr get_consumeradmin (
    CosNotifyChannelAdmin::AdminID id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminNotFound
  ));

virtual CosNotifyChannelAdmin::SupplierAdmin_ptr get_supplieradmin (
    CosNotifyChannelAdmin::AdminID id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminNotFound
  ));

virtual CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual CosNotification::AdminProperties * get_admin (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_admin (
    const CosNotification::AdminProperties & admin,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedAdmin
  ));

virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
  ));

 protected:
// = Helper Methods
 CosNotifyFilter::FilterFactory_ptr create_default_filter_factory_i (CORBA::Environment& ACE_TRY_ENV);
 // Create the default filter factory.

 void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 // = Data Members
 CosNotifyChannelAdmin::EventChannelFactory_var my_factory_;
 // The factory that created us.

 PortableServer::POA_var my_POA_;
 // The POA in which i live.

 PortableServer::POA_var CA_POA_;
 // The POA in which we should activate ConsumerAdmins in.
 // We create and own this.

 PortableServer::POA_var SA_POA_;
 // The POA in which we should activate SupplierAdmins in.
 // We create and own this.

 CosNotifyFilter::FilterFactory_var default_filter_factory_;
 // The default filter factory.
 // We create and own this.

 TAO_Notify_Resource_Manager* resource_manager_;
 // We get this factory from the EventChannelFactory who owns it.
 // This factory is accessible to all the objects created in this
 // Event Channel.

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

  CORBA::Boolean is_destroyed_;
  // Flag to tell if we have be destroyed.

  // @@ Pradeep can you explain why there is any maximum for these
  // values? Should they be configurable by the user so the resource
  // requirements can be bounded?

  // = Admin. properties
  // for all these properties the default O implies no limit
  CORBA::Long max_queue_length_;
  // The maximum number of events that will be queued by the channel before
  // the channel begins discarding events or rejecting new events upon
  // receipt of each new event.

  CORBA::Long max_consumers_;
  // The maximum number of consumers that can be connected to the channel at
  // any given time.

  CORBA::Long max_suppliers_;
  // The maximum number of suppliers that can be connected to the channel at
  // any given time.

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_Event_Manager* event_manager_;
  // The event manager.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_EVENTCHANNEL_I_H_ */
