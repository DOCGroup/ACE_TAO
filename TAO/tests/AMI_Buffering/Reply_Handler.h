
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
  Reply_Handler (Reply_Handler const &) = delete;
  Reply_Handler &operator= (Reply_Handler const &) = delete;
  Reply_Handler (Reply_Handler &&) = delete;
  Reply_Handler &operator= (Reply_Handler &&) = delete;

  // = The skeleton methods
  void receive_data () override;
  void receive_data_excep (::Messaging::ExceptionHolder *holder) override;

  void sync () override;
  void sync_excep (::Messaging::ExceptionHolder *holder) override;

  void shutdown () override;
  void shutdown_excep (::Messaging::ExceptionHolder *holder) override;
};

#include /**/ "ace/post.h"
#endif /* REPLY_HANDLER_H */
