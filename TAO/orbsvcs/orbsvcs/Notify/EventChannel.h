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
#include "Object.h"

class TAO_NS_ConsumerAdmin;
class TAO_NS_SupplierAdmin;
class TAO_NS_EventChannelFactory;
template <class TYPE> class TAO_NS_Container_T;

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
class TAO_Notify_Export TAO_NS_EventChannel : public POA_CosNotifyChannelAdmin::EventChannel
                                            , public virtual TAO_NS_Object
{
  friend class TAO_NS_Builder;

public:
  typedef CosNotifyChannelAdmin::ChannelIDSeq SEQ;
  typedef CosNotifyChannelAdmin::ChannelIDSeq_var SEQ_VAR;

  /// Constuctor
  TAO_NS_EventChannel (void);

  /// Destructor
  ~TAO_NS_EventChannel ();

  /// Init
  void init (TAO_NS_EventChannelFactory* ecf
             , const CosNotification::QoSProperties & initial_qos
             , const CosNotification::AdminProperties & initial_admin
             ACE_ENV_ARG_DECL);

  /// Remove ConsumerAdmin from its container.
  void remove (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL);

  /// Remove SupplierAdmin from its container.
  void remove (TAO_NS_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL);

  /// ServantBase refcount methods.
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// Release
  virtual void release (void);

  /// Shutdown
  virtual int shutdown (ACE_ENV_SINGLE_ARG_DECL);

protected:
  typedef TAO_NS_Container_T <TAO_NS_ConsumerAdmin> TAO_NS_ConsumerAdmin_Container;
  typedef TAO_NS_Container_T <TAO_NS_SupplierAdmin> TAO_NS_SupplierAdmin_Container;

  /// = Data Members
  /// The parent object.
  TAO_NS_EventChannelFactory* ecf_;

  /// ConsumerAdmin Container.
  TAO_NS_ConsumerAdmin_Container *ca_container_;

  /// SupplierAdmin Container.
  TAO_NS_SupplierAdmin_Container *sa_container_;

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
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
#include "EventChannel.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_EVENTCHANNEL_H */
