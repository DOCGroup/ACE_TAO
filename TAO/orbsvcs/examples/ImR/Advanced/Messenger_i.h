// -*- C++ -*-
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

  ~Messenger_i () override = default;

  CORBA::Long send_message (
      ::CORBA::Long thread,
      ::CORBA::Long iter,
      ::CORBA::Long obj,
      ::CORBA::Long req) override;

  void shutdownOrb () override;

  bool acknowledgeHit ();

  Messenger_i (Messenger_i const &) = delete;
  Messenger_i (Messenger_i &&) = delete;
  Messenger_i &operator= (Messenger_i const &) = delete;
  Messenger_i &operator= (Messenger_i &&) = delete;

private:
  long            instance_ {};
  CORBA::ORB_var  orb_;
  bool            isHit_ {};
};


#endif /* MESSENGERI_H_  */
