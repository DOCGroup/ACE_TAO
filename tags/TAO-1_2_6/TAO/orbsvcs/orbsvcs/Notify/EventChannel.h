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

#ifndef TAO_NS_EVENTCHANNEL_H
#define TAO_NS_EVENTCHANNEL_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/NotifyExtC.h"
#include "Container_T.h"
#include "Destroy_Callback.h"
#include "AdminProperties.h"

class TAO_NS_Admin;
class TAO_NS_EventChannelFactory;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_NS_EventChannel
 *
 * @brief Implementation of CosNotifyChannelAdmin::EventChannel
 *
 */
class TAO_Notify_Export TAO_NS_EventChannel : public POA_CosNotifyChannelAdmin::EventChannel, public virtual TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel, TAO_NS_EventChannelFactory>, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;
  typedef TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel, TAO_NS_EventChannelFactory> inherited;
  typedef ACE_Unbounded_Set <TAO_NS_Object_Id> TAO_NS_Object_Id_Seq;

public:
  /// Constuctor
  TAO_NS_EventChannel (void);

  /// Destructor
  ~TAO_NS_EventChannel ();

  /// Init
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Insert object to this container.
  virtual void insert (TAO_NS_Admin* admin ACE_ENV_ARG_DECL);

  /// Remove type from container_
  virtual void remove (TAO_NS_Admin* admin ACE_ENV_ARG_DECL);

  /// Return servant
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// = Data Members

  // Default Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var default_consumer_admin_;

  // Default Supplier Admin
  CosNotifyChannelAdmin::SupplierAdmin_var default_supplier_admin_;

  /// List of Consumer Admin ID's
  TAO_NS_Object_Id_Seq consumer_admin_id_list_;

  /// List of Supplier Admin ID's
  TAO_NS_Object_Id_Seq supplier_admin_id_list_;

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
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTCHANNEL_H */
