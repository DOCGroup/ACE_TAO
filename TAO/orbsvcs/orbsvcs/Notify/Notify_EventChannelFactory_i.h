/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_EVENTCHANNELFACTORY_I_H
#define NOTIFY_EVENTCHANNELFACTORY_I_H

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_EventChannel_i;

class TAO_ORBSVCS_Export TAO_Notify_EventChannelFactory_i :
public virtual POA_CosNotifyChannelAdmin::EventChannelFactory
{
public:
  // = TITLE
  //   TAO_Notify_EventChannelFactory_i
  // = DESCRIPTION
  //
  //

  TAO_Notify_EventChannelFactory_i (void);
  // Constructor

  virtual ~TAO_Notify_EventChannelFactory_i (void);
  // Destructor

  void activate (CORBA::Environment &ACE_TRY_ENV);
  // Activate the servant in the default POA.

  CosNotifyChannelAdmin::EventChannelFactory_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Transform to object ref.

  // = CosNotifyChannelAdmin::EventChannelFactory methods.
  virtual CosNotifyChannelAdmin::EventChannel_ptr create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS,
    CosNotification::UnsupportedAdmin
  ));

virtual CosNotifyChannelAdmin::ChannelIDSeq * get_all_channels (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::EventChannel_ptr get_event_channel (
    CosNotifyChannelAdmin::ChannelID id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ChannelNotFound
  ));

 protected:
  CosNotifyChannelAdmin::EventChannelFactory_var self_;
  // Reference to the CORBA Object associated with this servant.

  ID_Pool<CosNotifyChannelAdmin::AdminID> ec_ids;
  // Id generator for event channels

  typedef
  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::ChannelID,
                                               TAO_Notify_EventChannel_i*,
                                               ACE_SYNCH_MUTEX>
  EC_MAP;

  EC_MAP ec_map_;
  // Mapping of channel id's to EC's.

};

#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
