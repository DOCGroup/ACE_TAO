/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_QoSAdmin_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_QOSADMIN_I_H
#define TAO_NOTIFY_QOSADMIN_I_H

#include "orbsvcs/orbsvcs/CosNotificationS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_Notify_QoSAdmin_i : public virtual POA_CosNotification::QoSAdmin
{
  // = TITLE
  //   TAO_Notify_QoSAdmin_i
  // = DESCRIPTION
  //
  //
public:
  TAO_Notify_QoSAdmin_i (void);
  // Constructor

  virtual ~TAO_Notify_QoSAdmin_i (void);
  // Destructor

  virtual CosNotification::QoSProperties * get_qos (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void set_qos (
    const CosNotification::QoSProperties & qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

  virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));
};
#endif /* TAO_NOTIFY_QOSADMIN_I_H */
