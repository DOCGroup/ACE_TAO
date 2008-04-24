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
  : public virtual POA_CosNotifyChannelAdmin::EventChannel,
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
             , const CosNotification::AdminProperties & initial_admin);

  /// Init (for reload)
  void init (TAO_Notify::Topology_Parent * parent);

  /// Remove ConsumerAdmin from its container.
  void remove (TAO_Notify_ConsumerAdmin* consumer_admin);

  /// Remove SupplierAdmin from its container.
  void remove (TAO_Notify_SupplierAdmin* supplier_admin);

  /// ServantBase refcount methods.
  virtual void _add_ref (void);
  virtual void _remove_ref (void);

  // TAO_Notify::Topology_Parent

  virtual void save_persistent (TAO_Notify::Topology_Saver& saver);
  virtual TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
                                                   CORBA::Long id,
                                                   const TAO_Notify::NVPList& attrs);
  virtual void reconnect (void);

  virtual TAO_Notify_Object::ID get_id () const {return id();}

  TAO_Notify_ProxyConsumer * find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position);
  TAO_Notify_ProxySupplier * find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position);

  // cleanup any proxy resources held by the EC
  virtual void cleanup_proxy (CosNotifyChannelAdmin::ProxyID id,
                              bool is_supplier);

  /// Shutdown
  virtual int shutdown (void);
  virtual void load_attrs(const TAO_Notify::NVPList& attrs);

  /// TAO_Notify_Container_T requires a destroy method
  virtual void destroy (void);

  /// This is public to allow TAO_MonitorConsumerAdmin access.
  virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr
    get_consumeradmin (CosNotifyChannelAdmin::AdminID id);

  /// This is public to allow TAO_MonitorSupplierAdmin access.
  virtual CosNotifyChannelAdmin::SupplierAdmin_ptr
    get_supplieradmin (CosNotifyChannelAdmin::AdminID id);

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

protected:
  /// =CosNotifyChannelAdmin::EventChannel methods

  virtual ::CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory ();

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (void);

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (void);

  virtual ::CosNotifyFilter::FilterFactory_ptr default_filter_factory (void);

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id);

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id);

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (void);

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (void);

  virtual ::CosNotification::QoSProperties * get_qos (void);

  virtual void set_qos (const CosNotification::QoSProperties & qos);

  virtual void validate_qos (const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos);

  virtual ::CosNotification::AdminProperties * get_admin (void);

  virtual void set_admin (const CosNotification::AdminProperties & admin);

  virtual ::CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (void);

  virtual ::CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (void);

private:
  TAO_Notify_ConsumerAdmin_Container& ca_container();
  TAO_Notify_SupplierAdmin_Container& sa_container();

  /// ConsumerAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_ConsumerAdmin_Container > ca_container_;

  /// SupplierAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_SupplierAdmin_Container > sa_container_;

  /// The default filter factory.
  CosNotifyFilter::FilterFactory_var default_filter_factory_;

  /// Release
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENTCHANNEL_H */
