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

//#include "Admin.h"

class TAO_NS_Admin;
class TAO_NS_EventChannelFactory;

/**
 * @class TAO_NS_EventChannel
 *
 * @brief Implementation of CosNotifyChannelAdmin::EventChannel
 *
 */
class TAO_Notify_Export TAO_NS_EventChannel : public POA_CosNotifyChannelAdmin::EventChannel, public TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel, TAO_NS_EventChannelFactory>, public TAO_NS_Destroy_Callback
{
  friend class TAO_NS_Builder;
  typedef TAO_NS_Container_T <TAO_NS_Admin, TAO_NS_EventChannel, TAO_NS_EventChannelFactory> inherited;

public:
  /// Constuctor
  TAO_NS_EventChannel (void);

  /// Destructor
  ~TAO_NS_EventChannel ();

  /// Return servant
  virtual PortableServer::Servant servant (void);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// = Data Members


  /// =CosNotifyChannelAdmin::EventChannel methods

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::EventChannelFactory_ptr MyFactory (
  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr default_consumer_admin (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr default_supplier_admin (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyFilter::FilterFactory_ptr default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr new_for_consumers (
                                                                        CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id
                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr new_for_suppliers (
                                                                        CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                                                        CosNotifyChannelAdmin::AdminID_out id
                                                                        ACE_ENV_ARG_DECL
                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::ConsumerAdmin_ptr get_consumeradmin (
                                                                        CosNotifyChannelAdmin::AdminID id
                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::SupplierAdmin_ptr get_supplieradmin (
                                                                        CosNotifyChannelAdmin::AdminID id
                                                                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotifyChannelAdmin::AdminNotFound
                     ));

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_consumeradmins (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotifyChannelAdmin::AdminIDSeq * get_all_supplieradmins (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosNotification::QoSProperties * get_qos (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_qos (
                        const CosNotification::QoSProperties & qos
                        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual void validate_qos (
                             const CosNotification::QoSProperties & required_qos,
                             CosNotification::NamedPropertyRangeSeq_out available_qos
                             )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedQoS
                     ));

  virtual ::CosNotification::AdminProperties * get_admin (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void set_admin (
                          const CosNotification::AdminProperties & admin
                          )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , CosNotification::UnsupportedAdmin
                     ));

  virtual ::CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual ::CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (

  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
};

#if defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTCHANNEL_H */
