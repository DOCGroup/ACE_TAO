#ifndef REPLY_HANDLER_H
#define REPLY_HANDLER_H
#include /**/ "ace/pre.h"
#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Reply_Handler : public POA_Test::AMI_HelloHandler
{
public:
  Reply_Handler (CORBA::ORB_ptr o);
  Reply_Handler (Reply_Handler const &) = delete;
  Reply_Handler &operator= (Reply_Handler const &) = delete;
  Reply_Handler (Reply_Handler &&) = delete;
  Reply_Handler &operator= (Reply_Handler &&) = delete;


  void next_prime (CORBA::ULong ami_return_val) override;

  void next_prime_excep (::Messaging::ExceptionHolder *excep_holder) override;

private:
  CORBA::ORB_var orb_;
};


#include /**/ "ace/post.h"
#endif /*REPLY_HANDLER_H*/
