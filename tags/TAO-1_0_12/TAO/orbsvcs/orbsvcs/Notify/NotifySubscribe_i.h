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

#ifndef NOTIFYSUBSCRIBE_H
#define NOTIFYSUBSCRIBE_H

#include "orbsvcs/orbsvcs/CosNotifyCommS.h"

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
