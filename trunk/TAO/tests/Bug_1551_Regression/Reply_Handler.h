//
// $Id$
//

#ifndef REPLY_HANDLER_H
#define REPLY_HANDLER_H

#include "ace/pre.h"

#include "TestS.h"

class Reply_Handler
  : public POA_Test::AMI_HelloHandler
  , public PortableServer::RefCountServantBase
{
public:
  Reply_Handler (
      Test::Hello_ptr hello,
      CORBA::ORB_ptr orb);

  void short_sleep (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void short_sleep_excep (Test::AMI_HelloExceptionHolder * excep_holder
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  void check_counter(ACE_ENV_SINGLE_ARG_DECL);

private:
  ACE_Atomic_Op<ACE_Thread_Mutex::ACE_Thread_Mutex,long> counter_;
  Test::Hello_var hello_;
  CORBA::ORB_var orb_;
};

#include "ace/post.h"
#endif /* REPLY_HANDLER_H */
