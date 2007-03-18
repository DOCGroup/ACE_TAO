// -*- C++ -*-
//
// $Id$
#ifndef MESSENGERI_H_
#define MESSENGERI_H_

#include "MessengerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Messenger_i
  : public virtual POA_Messenger
{
public:
  Messenger_i (CORBA::ORB_ptr orb, long instance);

  virtual ~Messenger_i (void);

  virtual
  CORBA::Long send_message (
      ::CORBA::Long thread,
      ::CORBA::Long iter,
      ::CORBA::Long obj,
      ::CORBA::Long req
   );

  virtual void shutdownOrb ();

  bool acknowledgeHit();

private:
  long            instance_;
  CORBA::ORB_var  orb_;
  bool            isHit_;
};


#endif /* MESSENGERI_H_  */
