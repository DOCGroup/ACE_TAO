/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_SupplierAdmin_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_SUPPLIERADMIN_I_H
#define NOTIFY_SUPPLIERADMIN_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/NotifyPublish_i.h"
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"
#include "ace/Hash_Map_Manager.h"

#include "ace/Auto_Ptr.h"
#include "Notify_Dispatcher.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_ProxyConsumer_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_SupplierAdmin_i :
  public virtual POA_CosNotifyChannelAdmin::SupplierAdmin,
  public virtual TAO_Notify_QoSAdmin_i,
  public virtual TAO_NotifyPublish_i,
  public virtual TAO_Notify_FilterAdmin_i
{
  // = TITLE
  //   TAO_Notify_SupplierAdmin_i
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_SupplierAdmin_i (TAO_Notify_EventChannel_i& myChannel);
  // Constructor

  virtual ~TAO_Notify_SupplierAdmin_i (void);
  // Destructor


  void init (CORBA::Environment &ACE_TRY_ENV);
  //

  TAO_Notify_Dispatcher& get_dispatcher (void);
  //

  CosNotifyChannelAdmin::SupplierAdmin_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);

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

  virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_pull_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_push_consumer (
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

  // = CosEventChannelAdmin::SupplierAdmin method
virtual CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer (
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

  ID_Pool<CosNotifyChannelAdmin::ProxyID> proxy_consumer_ids;
  // Id generator for proxy suppliers.

  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::ProxyID,
                                               TAO_Notify_ProxyConsumer_i*,
                                               ACE_SYNCH_MUTEX>
  proxyconsumer_map_;

  // ???? Pradeep: auto_ptr can't be used this way.
  // auto_ptr<TAO_Notify_Dispatcher> dispatcher_;
  TAO_Notify_Dispatcher *dispatcher_;
  //
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_SUPPLIERADMIN_I_H */
