/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   NotifySubscribe_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

// @@ Pradeep: please make sure that you use unique names for these
// #define, for example TAO_NOTIFY_NOTIFY_SUBSCRIBE_H...

#ifndef NOTIFYSUBSCRIBE_H
#define NOTIFYSUBSCRIBE_H

#include "orbsvcs/orbsvcs/CosNotifyCommS.h"

// @@ Pradeep: be consistent with your use of #pragma once and similar
// things!

// @@ Pradeep: it seems like this is not implemented, could you please
// add a comment stating so? Or is this something that the application
// must provide.  If the latter, then why do you implement one?

class TAO_ORBSVCS_Export TAO_NotifySubscribe_i : public virtual POA_CosNotifyComm::NotifySubscribe
{
  // = TITLE
  //   TAO_NotifySubscribe_i
  // = DESCRIPTION
  //
  //

public:
  //Constructor
  TAO_NotifySubscribe_i (void);

  //Destructor
  virtual ~TAO_NotifySubscribe_i (void);

virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

};

#endif /* NOTIFYSUBSCRIBE_H */
