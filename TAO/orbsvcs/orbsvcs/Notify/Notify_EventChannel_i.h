/* -*- C++ -*- */
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
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_EVENTCHANNEL_I_H_
#define NOTIFY_EVENTCHANNEL_I_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_QoSAdmin_i.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"

#include "ace/Auto_Ptr.h"
#include "Notify_Dispatcher.h"

class TAO_Notify_EventChannelFactory_i;
class TAO_Notify_ConsumerAdmin_i;
class TAO_Notify_SupplierAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class  TAO_Notify_EventChannel_i :
public virtual POA_CosNotifyChannelAdmin::EventChannel,
public virtual TAO_Notify_QoSAdmin_i
{
  // = TITLE
  //   TAO_Notify_EventChannel_i
  // = DESCRIPTION
  //
  //
 public:
  TAO_Notify_EventChannel_i (TAO_Notify_EventChannelFactory_i& my_factory);
  // Constructor

  virtual ~TAO_Notify_EventChannel_i (void);
  // Destructor

  void init (CORBA::Environment &ACE_TRY_ENV);
  //

  TAO_Notify_Dispatcher& get_dispatcher (void);
  //

  CosNotifyChannelAdmin::EventChannel_ptr
  get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Activate with the default POA

  // = EventChannel methods.
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
  TAO_Notify_EventChannelFactory_i& my_factory_;
  // The factory that created us.
  
  typedef ID_Pool<CosNotifyChannelAdmin::AdminID> IDGEN;
  
  IDGEN consumer_admin_ids;
  // Id generator for consumer admins.

  IDGEN supplier_admin_ids;
  // Id generator for supplier admins.

  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::AdminID,
                                               TAO_Notify_ConsumerAdmin_i*,
                                               ACE_SYNCH_MUTEX>
  consumer_admin_map_;
  //

  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::AdminID,
                                               TAO_Notify_SupplierAdmin_i*,
                                               ACE_SYNCH_MUTEX>
  supplier_admin_map_;
  //

  // ???? Pradeep: auto_ptr can't be used this way.
  // auto_ptr<TAO_Notify_Dispatcher> dispatcher_;
  TAO_Notify_Dispatcher *dispatcher_;
  //

  CosNotifyFilter::FilterFactory_var filter_factory_;
  // The default filter factory
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_EVENTCHANNEL_I_H_ */
