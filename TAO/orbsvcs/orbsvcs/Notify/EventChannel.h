/* -*- C++ -*- */
/**
 *  @file EventChannel.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
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
#include "orbsvcs/Notify/FilterFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_FilterFactory;
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

  /// Constructor
  TAO_Notify_EventChannel ();

  /// Destructor
  ~TAO_Notify_EventChannel () override;

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
  void _add_ref (void) override;
  void _remove_ref (void) override;

  // TAO_Notify::Topology_Parent

  void save_persistent (TAO_Notify::Topology_Saver& saver) override;
  TAO_Notify::Topology_Object* load_child (const ACE_CString &type,
                                                   CORBA::Long id,
                                                   const TAO_Notify::NVPList& attrs) override;
  void reconnect (void) override;
  virtual void validate ();

  TAO_Notify_Object::ID get_id () const override {return id();}

  TAO_Notify_ProxyConsumer * find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position);
  TAO_Notify_ProxySupplier * find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position);

  // cleanup any proxy resources held by the EC
  virtual void cleanup_proxy (CosNotifyChannelAdmin::ProxyID id,
                              bool is_supplier,
                              bool experienced_timeout);

  /// Shutdown
  int shutdown (void) override;
  void load_attrs(const TAO_Notify::NVPList& attrs) override;

  /// TAO_Notify_Container_T requires a destroy method
  void destroy (void) override;

  /// This is public to allow TAO_MonitorConsumerAdmin access.
  CosNotifyChannelAdmin::ConsumerAdmin_ptr
    get_consumeradmin (CosNotifyChannelAdmin::AdminID id) override;

  /// This is public to allow TAO_MonitorSupplierAdmin access.
  CosNotifyChannelAdmin::SupplierAdmin_ptr
    get_supplieradmin (CosNotifyChannelAdmin::AdminID id) override;

  TAO_Notify_FilterFactory* default_filter_factory_servant () const;

private:
  typedef TAO_Notify_Container_T <TAO_Notify_ConsumerAdmin> TAO_Notify_ConsumerAdmin_Container;
  typedef TAO_Notify_Container_T <TAO_Notify_SupplierAdmin> TAO_Notify_SupplierAdmin_Container;

  void save_attrs(TAO_Notify::NVPList& attrs) override;

  /// = Data Members
  /// The parent object.
  TAO_Notify_EventChannelFactory::Ptr ecf_;

  TAO_SYNCH_MUTEX default_admin_mutex_;

  /// Default Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var default_consumer_admin_;

  /// Default Supplier Admin
  CosNotifyChannelAdmin::SupplierAdmin_var default_supplier_admin_;

protected:
  /// =CosNotifyChannelAdmin::EventChannel methods

  ::CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory () override;

  ::CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (void) override;

  ::CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (void) override;

  ::CosNotifyFilter::FilterFactory_ptr default_filter_factory (void) override;

  ::CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id) override;

  ::CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id) override;

  ::CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (void) override;

  ::CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (void) override;

  ::CosNotification::QoSProperties * get_qos (void) override;

  void set_qos (const CosNotification::QoSProperties & qos) override;

  void validate_qos (const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos) override;

  ::CosNotification::AdminProperties * get_admin (void) override;

  void set_admin (const CosNotification::AdminProperties & admin) override;

  ::CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (void) override;

  ::CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (void) override;

private:
  TAO_Notify_ConsumerAdmin_Container& ca_container();
  TAO_Notify_SupplierAdmin_Container& sa_container();

  /// ConsumerAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_ConsumerAdmin_Container > ca_container_;

  /// SupplierAdmin Container.
  ACE_Auto_Ptr< TAO_Notify_SupplierAdmin_Container > sa_container_;

  /// The default filter factory.
  CosNotifyFilter::FilterFactory_var default_filter_factory_;
  TAO_Notify_FilterFactory * default_filter_factory_servant_;

  void release (void) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_EVENTCHANNEL_H */
