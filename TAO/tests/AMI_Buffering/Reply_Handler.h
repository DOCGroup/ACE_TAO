
#ifndef REPLY_HANDLER_H
#define REPLY_HANDLER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the AMI_AMI_BufferingHandler interface
class Reply_Handler
  : public virtual POA_Test::AMI_AMI_BufferingHandler
{
public:
  /// Constructor
  Reply_Handler ();

  // = The skeleton methods
  virtual void receive_data ();
  virtual void receive_data_excep (::Messaging::ExceptionHolder *holder);

  virtual void sync ();
  virtual void sync_excep (::Messaging::ExceptionHolder *holder);

  virtual void shutdown ();
  virtual void shutdown_excep (::Messaging::ExceptionHolder *holder);
};

#include /**/ "ace/post.h"
#endif /* REPLY_HANDLER_H */
