/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   NotifyPublish_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFYPUBLISH_I_H
#define NOTIFYPUBLISH_I_H

#include "orbsvcs/orbsvcs/CosNotifyCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_NotifyPublish_i :
public virtual POA_CosNotifyComm::NotifyPublish
{
public:
  // = TITLE
  //   TAO_NotifyPublish_i
  // = DESCRIPTION
  //
  //

  TAO_NotifyPublish_i (void);
  // Constructor

  virtual ~TAO_NotifyPublish_i (void);
  // Destructor

virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));
};

#endif /* NOTIFYPUBLISH_I_H */
