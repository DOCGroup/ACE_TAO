/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ConsumerAdmin_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef NOTIFY_CONSUMERADMIN_I_H
#define NOTIFY_CONSUMERADMIN_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/NotifySubscribe_i.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"
#include "ace/Hash_Map_Manager.h"

#include "ace/Auto_Ptr.h"
#include "Notify_Dispatcher.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_ProxySupplier_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_ConsumerAdmin_i :
public virtual POA_CosNotifyChannelAdmin::ConsumerAdmin,
public virtual TAO_Notify_QoSAdmin_i,
public virtual TAO_NotifySubscribe_i,
public virtual TAO_Notify_FilterAdmin_i
{
  // = TITLE
  //   TAO_Notify_ConsumerAdmin_i
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i& myChannel);
                              //                              CosNotifyChannelAdmin::AdminID myID)
  // Constructor

  virtual ~TAO_Notify_ConsumerAdmin_i (void);
  // Destructor

  void init (CORBA::Environment &ACE_TRY_ENV);
  //

  TAO_Notify_Dispatcher& get_dispatcher (void);
  //

  CosNotifyChannelAdmin::ConsumerAdmin_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  //

  virtual CosNotifyChannelAdmin::AdminID MyID (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

  virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_pull_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_push_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual void destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  // = CosEventChannelAdmin::ConsumerAdmin methods
virtual CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator_;
  //

  TAO_Notify_EventChannel_i& myChannel_;
  // The channel to which we belong.

  CosNotifyChannelAdmin::AdminID myID_;
  // My ID.

  ID_Pool<CosNotifyChannelAdmin::ProxyID> proxy_supplier_ids;
  // Id generator for proxy suppliers.

  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::ProxyID,
                                               TAO_Notify_ProxySupplier_i*,
                                               ACE_SYNCH_MUTEX>
  proxysupplier_map_;

  // ???? Pradeep: auto_ptr can't be used this way.
  // auto_ptr<TAO_Notify_Dispatcher> dispatcher_;
  TAO_Notify_Dispatcher *dispatcher_;
  //
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_CONSUMERADMIN_I_H */
