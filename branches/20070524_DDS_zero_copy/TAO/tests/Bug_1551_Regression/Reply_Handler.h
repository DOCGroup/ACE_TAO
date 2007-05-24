//
// $Id$
//

#ifndef REPLY_HANDLER_H
#define REPLY_HANDLER_H

#include /**/ "ace/pre.h"

#include "TestS.h"

class Reply_Handler
  : public POA_Test::AMI_HelloHandler
{
public:
  Reply_Handler (
      Test::Hello_ptr hello,
      CORBA::ORB_ptr orb);

  void short_sleep (void);

  void short_sleep_excep (::Messaging::ExceptionHolder * excep_holder);

private:
  void check_counter(void);

private:
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> counter_;
  Test::Hello_var hello_;
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* REPLY_HANDLER_H */
