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

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  TAO_Notify_EventChannelFactory_i :
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

  CosNotifyChannelAdmin::EventChannelFactory_ptr get_refx (CORBA::Environment &ACE_TRY_ENV);
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
};

#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
