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

// @@ Pradeep: it is recommended to #include local files first, then
// files in orbsvcs, then in tao and then in ace, check "Large Scale
// Software Development with C++" by John Lakos.

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/ID_Pool_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Notify_EventChannel_i;

//
// @@ Pradeep: we always put the class declaration in a single line,
// it is ugly, but otherwise the documentation tools get confused.
//
// @@ Pradeep: please consider using RefCountedServantBase, or
// implement the add_ref(), remove_ref() methods required by the POA
// to manage memory.  Otherwise it is almost impossible to deactivate
// the object safely.
//
class TAO_ORBSVCS_Export TAO_Notify_EventChannelFactory_i :
public virtual POA_CosNotifyChannelAdmin::EventChannelFactory
{
public:
  // = TITLE
  //   TAO_Notify_EventChannelFactory_i
  //
  // = DESCRIPTION
  //   @@ Pradeep, please include the description.
  //   @@ Pradeep, i tend to put this comment before the public: line,
  //   but i'm not sure if there is a guideline about it, please check
  //   it up.
  //
  //

  TAO_Notify_EventChannelFactory_i (void);
  // Constructor

  virtual ~TAO_Notify_EventChannelFactory_i (void);
  // Destructor

  void activate (CORBA::Environment &ACE_TRY_ENV);
  // Activate the servant in the default POA.
  // @@ Pradeep: you should let thge user pick how is this object
  // activated in the POA, after all you don't know how the
  // application will use this....
  // You may want to have an activate() method to create internal
  // threads and stuff like that.

  CosNotifyChannelAdmin::EventChannelFactory_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Transform to object ref.
  // @@ Pradeep: _this() does the same thing, and again it is better
  // for the user to select the right POA.

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

  // @@ Pradeep: indentation is busted here, please fix it....
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

  // @@ Pradeep: ID_Pool should have some prefix, TAO_Notify or
  // something similar.
  ID_Pool<CosNotifyChannelAdmin::AdminID> ec_ids;
  // Id generator for event channels

  // @@ Pradeep: using a SYNCH_MUTEX here will not help much, usually
  // you need an external lock to avoid changes to the collection, for
  // example your get_all_channels() method needs to lock the
  // collection while the complete iteration is performed.
  typedef
  ACE_Hash_Map_Manager <CosNotifyChannelAdmin::ChannelID,
                                               TAO_Notify_EventChannel_i*,
                                               ACE_SYNCH_MUTEX>
  EC_MAP;

  EC_MAP ec_map_;
  // Mapping of channel id's to EC's.

};

#endif /* NOTIFY_EVENTCHANNELFACTORY_I_H */
