/* -*- C++ -*- $Id$ */

#ifndef MESSENGERI_H_
#define MESSENGERI_H_

// This is needed on LynxOS 4.0 with GCC 2.95
#include "ace/OS_NS_stdio.h"

#include <openssl/ssl.h>
#include <openssl/x509.h>
#include "orbsvcs/SecurityC.h"
#include "orbsvcs/SSLIOPC.h"

#include "MessengerS.h"

class  Messenger_i : public virtual POA_Messenger
{
public:
  Messenger_i (
      CORBA::ORB_ptr orb,
      SSLIOP::Current_ptr ssliop_current
    );

  virtual ~Messenger_i (void);

   virtual CORBA::Boolean send_message (
     const char * user_name,
     const char * subject,
     char *& message
    );

  virtual void shutdown (
        const char * user_name
    );

protected:
  CORBA::ORB_var orb_;
  SSLIOP::Current_var ssliop_current_;
};

#endif /* MESSENGERI_H_  */
