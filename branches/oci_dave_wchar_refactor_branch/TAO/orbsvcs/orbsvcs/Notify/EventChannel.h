/* -*- C++ -*- */
/**
 *  @file EventChannel.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_EVENTCHANNEL_H
#define TAO_Notify_EVENTCHANNEL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/Object.h"
#include "orbsvcs/Notify/EventChannelFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ConsumerAdmin;
class TAO_Notify_SupplierAdmin;
class TAO_Notify_EventChannelFactory;
class TAO_Notify_ProxyConsumer;
class TAO_Notify_ProxySupplier;
template <class TYPE> class TAO_Notify_Container_T;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_EventChannel
 *
 * @brief Implementation of CosNotifyChannelAdmin::EventChannel
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_EventChannel
  : public POA_CosNotifyChannelAdmin::EventChannel,
    public TAO_Notify::Topology_Parent
{
  friend class TAO_Notify_Builder;

public:
  typedef TAO_Notify_Refcountable_Guard_T< TAO_Notify_EventChannel > Ptr;
  typedef CosNotifyChannelAdmin::ChannelIDSeq SEQ;
  typedef CosNotifyChannelAdmin::ChannelIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_Notify_EventChannel (void);

  /// Destructor
  virtual ~TAO_Notify_EventChannel ();

  /// Init
  void init (TAO_Notify_EventChannelFactory* ecf
             , const CosNotification::QoSProperties & initial_qos
             , const CosNotification::AdminProperties & initial_admin
             ACE_ENV_ARG_DECL);

  /// Init (for reload)
  void init (TAO_Notify::Topology_Parent * parent
             ACE_ENV_ARG_DECL);

  /// Remove ConsumerAdmin from its container.
  void remove (TAO_Notify_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL);

  /// Remove SupplierAdmin from its container.
  void remove (TAO_Notify_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  // TAO_Notify::Topology_Parent

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
                                                   CORBA::Long id,
                                                   const TAO_Notify::NVPList& attrs
                                                   ACE_ENV_ARG_DECL);
  virtual void reconnect (ACE_ENV_SINGLE_ARG_DECL);

  virtual TAO_Notify_Object::ID get_id () const {return id();}

  TAO_Notify_ProxyConsumer * find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position  ACE_ENV_ARG_DECL);
  TAO_Notify_ProxySupplier * find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position  ACE_ENV_ARG_DECL);

  /// Shutdown
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);
  virtual void load_attrs(const TAO_Notify::NVPList& attrs);

private:
  typedef TAO_Notify_Container_T <TAO_Notify_ConsumerAdmin> TAO_Notify_ConsumerAdmin_Container;
  typedef TAO_Notify_Container_T <TAO_Notify_SupplierAdmin> TAO_Notify_SupplierAdmin_Container;

  virtual void save_attrs(TAO_Notify::NVPList& attrs);

  /// = Data Members
  /// The parent object.
  TAO_Notify_EventChannelFactory::Ptr ecf_;

  TAO_SYNCH_MUTEX default_admin_mutex_;

  // Default Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var default_consumer_admin_;

  // Default Supplier Admin
  CosNotifyChannelAdmin::SupplierAdmin_var default_supplier_admin_;

  /// =CosNotifyChannelAdmin::EventChannel methods

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory (ACE_ENV_SINGLE_ARG_DECL
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyFilter::FilterFactory_ptr default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id
                                                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id
                                                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr get_consumeradmin (CosNotifyChannelAdmin::AdminID id
                                                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr get_supplieradmin (CosNotifyChannelAdmin::AdminID id
                                                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties * get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_qos (const CosNotification::QoSProperties & qos
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void validate_qos (const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual ::CosNotification::AdminProperties * get_admin (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_admin (const CosNotification::AdminProperties & admin ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedAdmin
                     ));

  virtual ::CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  TAO_Notify_ConsumerAdmin_Container& ca_container();
  TAO_Notify_SupplierAdmin_Container& sa_container();

  /// ConsumerAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_ConsumerAdmin_Container > ca_container_;

  /// SupplierAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_SupplierAdmin_Container > sa_container_;

  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENTCHANNEL_H */
