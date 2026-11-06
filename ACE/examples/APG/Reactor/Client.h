/**
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "ace/Synch_Traits.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"

// Listing 1 code/ch07
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

class Client :
    public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  using super = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

public:
  Client ()
    : notifier_ (nullptr, this, ACE_Event_Handler::WRITE_MASK)
  { }

  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  int open (void * = nullptr) override;

  // Called when input is available from the client.
  int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE) override;

  // Called when output is possible.
  int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE) override;

  // Called when a timer expires.
  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act = nullptr) override;

private:
  static constexpr int ITERATIONS = 5;
  int iterations_ {};
  ACE_Reactor_Notification_Strategy notifier_;
};
// Listing 1

#endif /* __CLIENT_H_ */
