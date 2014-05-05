//=============================================================================
/**
 *  @file    Network_Adapters_Test.cpp
 *
 *  $Id$
 *
 *  Tests the ICMP-echo support in ACE.
 *
 *
 *  @author Robert S. Iakobashvili <coroberti@gmail.com> <coroberti@walla.co.il> Gonzalo A. Diethelm <gonzalo.diethelm@aditiva.com>
 */
//=============================================================================


// We need this to be able to check for ACE_HAS_ICMP_SUPPORT
#include "ace/config-all.h"
#include "test_config.h"

#if defined (ACE_HAS_ICMP_SUPPORT) && (ACE_HAS_ICMP_SUPPORT == 1)

#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ace/Signal.h"
#include "ace/High_Res_Timer.h"
#include "ace/Atomic_Op.h"
#include "ace/Sched_Params.h"
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_signal.h"
#include "ace/Timer_Heap.h"
#include "ace/Auto_Ptr.h"

#include "Network_Adapters_Test.h"

/**
 * There are two major uses of the functionality:
 *
 * 1. to check a local network adapter;
 * 2. to check which of the remote CEs (computer elements) are alive.
 *
 * For the first purpose we are creating a raw socket, binding it to
 * the IP-address in question (adapter to be monitored), and are
 * sending via the adapter ICMP echo-checks to a list of 3rd party
 * ping-points. If at least a single 3rd party replies us within a
 * configurable timeout by an ICMP-reply, our adapter is OK. If not, we
 * may wish to repeat ICMP-probing once or twice more. We may also
 * wish to make such tests regular with a configurable interval in seconds.
 *
 * For the second purpose we are creating a raw socket, and without
 * binding it are sending via any our CE's adapter ICMP echo-checks to
 * a list of CEs to be monitored. An array of chars (named ping_status
 * in main ()), corresponding to the array of addresses
 * (ping_points_addrs in main ()), contains status of each monitored
 * CE. When we get ICMP-reply from a ping_points_addrs[I] IP-address,
 * we are placing 0 to the ping_status[I]. The ICMP-probing may be
 * configured to test 2-3 times each pinged CE.  We may also wish to
 * make such tests regular with a configurable interval in seconds.
 *
 * Command line options:
 *
 * -b  IPv4 of the interface to bind to the socket (only for the
 *     purpose 1), e.g. -b 192.168.5.5;
 *
 * -p  IPv4 addresses of the remote CEs, which we are going to check
 *     (purpose 2), or they are 3rd points for the purpose 1,
 *     e.g. -p 192.168.5.120: 192.168.5.122: 192.168.5.125
 *
 * -w  milliseconds to wait for echo-reply, on lan 100-200 msec, on
 *     WAN may be 2000-5000 msec, for GPRS may reach 10000 - 20000
 *     mseconds;
 *
 * -t  as we are doing such checks regularly time in seconds between
 *     checks.
 *
 * In main we are activating by open () an instance of Echo_Handler
 * with parameters.
 *
 * Repeats_Handler serves to repeat the checks each
 * repeats_seconds_timer seconds.
 *
 * Stop_Handler contains a list of handlers to be stopped and is
 * supposed to close this business.

 * Attention: Running the test without parameters (just using defaults)
 * makes pinging to the loopback address. Therefore, the raw socket
 * sees both ICMP_ECHO and ICMP_ECHOREPLY with the first output in log
 * as not a ICMP_ECHOREPLY message and further ICMP_ECHOREPLY
 * received. Don't worry, be happy - it's ok.
 */


Echo_Handler::Echo_Handler (void)
  : ping_socket_ (),
    reply_wait_ (),
    remote_addrs_ (0),
    number_remotes_ (0),
    success_status_ (0),
    delete_success_status_ (0),
    max_attempts_num_ (0),
    current_attempt_ (0),
    connect_to_remote_ (0)
{
}

Echo_Handler::~Echo_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_Handler::~Echo_Handler - entered.\n")));

  this->ping_socket ().close ();
  if (this->remote_addrs_)
    {
      delete [] this->remote_addrs_;
      this->remote_addrs_ = 0;
    }
  if (this->success_status_ && this->delete_success_status_)
    {
      delete this->success_status_;
    }
  this->success_status_ = 0;

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Echo_Handler::~Echo_Handler - completed.\n")));
}

int
Echo_Handler::open (ACE_Reactor * const    reactor,
                    ACE_Time_Value const & reply_wait,
                    ACE_INET_Addr const &  remote_addr,
                    ACE_TCHAR *            success_status,
                    size_t                 max_attempts_num,
                    ACE_Addr const &       local_addr,
                    int                    connect_to_remote)
{
  if (this->reactor ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("reactor is already set.\n")),
                      -1);
  if (!reactor)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed : ")
                       ACE_TEXT ("NULL pointer to reactor provided.\n")),
                      -1);

  this->reactor (reactor);
  this->reply_wait_ = reply_wait;

  if (this->remote_addrs_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("remote_addrs_ already initialized.\n")),
                      -1);

  ACE_NEW_RETURN (this->remote_addrs_, ACE_INET_Addr, -1);

  // now copy to keep it locally
  this->remote_addrs_[0] = remote_addr;
  this->number_remotes_ = 1;
  if (this->success_status_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("success_status_ already initialized.\n")),
                      -1);

  if (! success_status)
    {
      ACE_NEW_RETURN (this->success_status_, ACE_TCHAR, -1);
      this->delete_success_status_ = 1;
    }
  else
    {
      this->success_status_ = success_status;
    }

  // place 'failed' to the array.
  this->success_status_[0] = 1;

  this->max_attempts_num_ = max_attempts_num;
  this->current_attempt_ = this->max_attempts_num_;

  if (this->ping_socket ().open (local_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open: %p\n"),
                       ACE_TEXT ("ping_socket_")),
                      -1);

  this->connect_to_remote_ = connect_to_remote;

  // Register with the reactor for input.
  if (this->reactor ()->register_handler (this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open: %p\n"),
                       ACE_TEXT ("register_handler for input")),
                      -1);
  return 0;
}

int
Echo_Handler::open (ACE_Reactor * const reactor,
                    ACE_Time_Value const & reply_wait,
                    ACE_INET_Addr const remote_addrs[],
                    size_t number_remotes,
                    ACE_TCHAR *success_status,
                    size_t max_attempts_num,
                    ACE_Addr const & local_addr)
{
  if (this->reactor ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("reactor is already set.\n")),
                      -1);

  //FUZZ: disable check_for_NULL
  if (!reactor)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: NULL ")
                       ACE_TEXT ("pointer to reactor provided.\n")),
                      -1);
  //FUZZ: enable check_for_NULL

  this->reactor (reactor);
  this->reply_wait_ = reply_wait;

  if (!remote_addrs)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("NULL remote_addr pointer provided.\n")),
                      -1);

  if (!number_remotes)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("size of remote_addrs array is 0.\n")),
                      -1);

  this->number_remotes_ = number_remotes;

  if (this->remote_addrs_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("remote_addrs_ already initialized.\n")),
                      -1);

  ACE_NEW_RETURN (this->remote_addrs_,
                  ACE_INET_Addr[this->number_remotes_],
                  -1);

  // now copy to keep them locally
  for (size_t i = 0; i < this->number_remotes_; ++i)
    {
      this->remote_addrs_[i] = remote_addrs[i];
    }

  if (this->success_status_)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open - failed: ")
                       ACE_TEXT ("success_status_ already initialized.\n")),
                      -1);

  if (! success_status)
    {
      ACE_NEW_RETURN (this->success_status_,
                      ACE_TCHAR[this->number_remotes_],
                      -1);
      this->delete_success_status_ = 1;
    }
  else
    {
      this->success_status_ = success_status;
    }

  // place 'failed' to the this->success_status_ array.
  for (size_t j = 0; j < this->number_remotes_; ++j)
    {
      this->success_status_[j] = 1;
    }

  this->max_attempts_num_ = max_attempts_num;
  this->current_attempt_ = this->max_attempts_num_;

  // If this process doesn't have privileges to open a raw socket, log
  // a warning instead of an error.
  if (this->ping_socket ().open (local_addr) == -1)
    {
      if (errno == EPERM || errno == EACCES)
        ACE_ERROR_RETURN ((LM_WARNING,
                           ACE_TEXT ("(%P|%t) Echo_Handler::open: ")
                           ACE_TEXT ("ping_socket_: insufficient privs to ")
                           ACE_TEXT ("run this test\n")),
                          -1);
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Echo_Handler::open: %p\n"),
                           ACE_TEXT ("ping_socket_")),
                          -1);
    }

  // register with the reactor for input
  if (this->reactor ()->register_handler (this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::open: %p\n"),
                       ACE_TEXT ("register_handler for input")),
                      -1);
  return 0;
}

ACE_Ping_Socket &
Echo_Handler::ping_socket (void)
{
  return this->ping_socket_;
}

int
Echo_Handler::dispatch_echo_checks (int first_call)
{
  // Set ones , if this is the first call (not from handle_timeout)
  if (first_call)
    {
      for (size_t j = 0; j < this->number_remotes_; ++j)
        {
          this->success_status_[j] = 1;
        }
      this->current_attempt_ = this->max_attempts_num_;
    }

  // Send echo-checks.
  for (size_t i = 0; i < this->number_remotes_; ++i)
    {
      if (this->success_status_[i] != 0)
        {
          if (this->ping_socket ().send_echo_check (
                this->remote_addrs_[i],
                this->connect_to_remote_) == -1)
            ACE_ERROR
              ((LM_ERROR,
                ACE_TEXT ("(%P|%t) Echo_Handler::dispatch_echo_checks - ")
                ACE_TEXT ("failed for this->remote_addrs_[%d].\n"),
                i));
        }
    }

  int rval_sched = -1;
  if ((rval_sched =
         this->reactor ()->schedule_timer (this,
                                           0,
                                           ACE_Time_Value (1),
                                           this->reply_wait_)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Echo_Handler::dispatch_echo_checks:")
                       ACE_TEXT (" %p\n"),
                       ACE_TEXT ("schedule_timer")),
                      -1);
  return 0;
}

int
Echo_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_Handler::handle_close - started.\n")));

#if 0
  this->ping_socket ().close ();
#endif

  this->reactor ()->cancel_timer (this);

#if 0
  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::ALL_EVENTS_MASK |
                                    ACE_Event_Handler::DONT_CALL);
#endif

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_Handler::handle_close - completed.\n")));
  return 0;
}

ACE_HANDLE
Echo_Handler::get_handle (void) const
{
  return ((ACE_ICMP_Socket &) this->ping_socket_).get_handle ();
}

int
Echo_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_Handler::handle_input - ")
              ACE_TEXT ("activity occurred on handle %d!\n"),
              this->ping_socket ().get_handle ()));

  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::memset (buf, 0, sizeof buf);

  ACE_INET_Addr addr;
  int rval_recv = -1;

  // Receive an <n> byte <buf> from the datagram socket
  // (uses<recvfrom(3)>).
  rval_recv =
    this->ping_socket ().recv (this->ping_socket ().icmp_recv_buff (),
                               ACE_Ping_Socket::PING_BUFFER_SIZE,
                               addr);
  switch (rval_recv)
    {
    case -1:
      // Complain and leave, but keep registered, returning 0.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Echo_Handler::handle_input - ")
                         ACE_TEXT ("%p: bad read\n"),
                         ACE_TEXT ("client")),
                        0);
      // NOTREACHED

    case 0:
      // Complain and leave
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Echo_Handler::handle_input - ")
                         ACE_TEXT ("closing daemon (fd = %d)\n"),
                         this->get_handle ()),
                        0);
      // NOTREACHED

    default:
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Echo_Handler::handle_input - ")
                  ACE_TEXT ("message from %d bytes received.\n"),
                  rval_recv));

      if (! this->ping_socket ().process_incoming_dgram (
             this->ping_socket ().icmp_recv_buff (),
             rval_recv))
        {
          for (size_t k = 0; k <this->number_remotes_; ++k)
            {
              if (addr.is_ip_equal (this->remote_addrs_[k]))
                {
                  if (addr.addr_to_string (buf, sizeof buf) == -1)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("%p\n"),
                                  ACE_TEXT ("can't obtain peer's address")));
                    }
                  else
                    {
                      ACE_DEBUG
                        ((LM_INFO,
                          ACE_TEXT ("(%P|%t) Echo_Handler::handle_input - ")
                          ACE_TEXT ("ECHO_REPLY received ")
                          ACE_TEXT ("from %s; marking this peer alive\n"),
                          buf));
                    }
                  // mark as successful
                  this->success_status_[k] = 0;
                  break;
                }
            }
        }
      break;
    }

  return 0;
}

int
Echo_Handler::handle_timeout (ACE_Time_Value const &,
                              void const *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Echo_Handler::handle_timeout - ")
              ACE_TEXT ("timer for ping_socket_ with handle %d.\n"),
              this->ping_socket ().get_handle ()));

  int need_to_proceed = 0;

  for (size_t i = 0; i < this->number_remotes_; ++i)
    {
      if (this->success_status_[i])
        {
          need_to_proceed = 1;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Echo_Handler::handle_timeout - ")
                      ACE_TEXT ("this->success_status_[%d] is not zero. ")
                      ACE_TEXT ("Need to proceed echo-checks.\n"), i));
          break;
        }
    }

  if (!need_to_proceed)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Echo_Handler::handle_timeout - ")
                  ACE_TEXT ("need_to_proceed == 0. ")
                  ACE_TEXT ("Completed echo-checks.\n")));
    }

  if (!this->current_attempt_ || !need_to_proceed)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Echo_Handler::handle_timeout - ")
                  ACE_TEXT ("completed ECHO-checks for handle (%d).\n"),
                  this->ping_socket ().get_handle ()));
      return -1; // to de-register from Reactor and make clean-up
                 // in handle-close
    }

  if (this->current_attempt_)
    {
      --this->current_attempt_;
    }

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Echo_Handler::handle_timeout - attempt %d.\n"),
      this->current_attempt_));

  this->dispatch_echo_checks ();
  return 0;
}

int
Echo_Handler::does_echo_test_successful (void)
{
  for (size_t i = 0; i < this->number_remotes_; ++i)
    {
      if (!this->success_status_[i])
        {
          return 1;
        }
    }
  return 0;
}


Stop_Handler::Stop_Handler (ACE_Reactor * const reactor)
  : counter_ ((counter_sig) 1)
{
  this->reactor (reactor);
  ACE_OS::memset (this->handlers_to_stop_,
                  0,
                  sizeof this->handlers_to_stop_);
}

Stop_Handler::~Stop_Handler (void)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Stop_Handler::~Stop_Handler.\n")));
}

int
Stop_Handler::open (void)
{
  // Register the signal handler object to catch the signals.
#if (SIGINT != 0)
  if (this->reactor ()->register_handler (SIGINT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::open: %p <%d>\n"),
                       ACE_TEXT ("register_handler for SIGINT"), SIGINT),
                      -1);
  this->registered_signals_.sig_add (SIGINT);
#endif /* SIGINT != 0 */

#if (SIGTERM != 0)
  if (this->reactor ()->register_handler (SIGTERM, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::open: %p <%d>\n"),
                       ACE_TEXT ("register_handler for SIGTERM"), SIGTERM),
                      -1);
  this->registered_signals_.sig_add (SIGTERM);
#endif /* SIGTERM != 0 */

#if (SIGQUIT != 0)
  if (this->reactor ()->register_handler (SIGQUIT, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::open: %p <%d>\n"),
                       ACE_TEXT ("register_handler for SIGQUIT"), SIGQUIT),
                      -1);
  this->registered_signals_.sig_add (SIGQUIT);
#endif /* SIGQUIT != 0 */
  return 0;
}

int
Stop_Handler::handle_signal (int signum,
                             siginfo_t * ,
                             ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Stop_Handler::handle_signal - started.\n")));
  if (! --this->counter_)
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("\n-- Stop_Handler::handle_signal --- ")
                  ACE_TEXT ("SIGNAL %d RECEIVED -----------.\n"),
                  signum));
      return reactor ()->notify (this, ACE_Event_Handler::READ_MASK);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Stop_Handler::handle_signal - ")
              ACE_TEXT ("finished.\n")));
  return 0;
}

int
Stop_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Stop_Handler::handle_input - entered\n")));

  for (size_t i = 0; i < HANDLERS_TO_STOP_TABLE_SIZE; ++i)
    {
      // remove from the reactor's tables all non-null entries
      if (this->handlers_to_stop_[i])
        {
#if defined ACE_HAS_EXCEPTIONS

          // protect from deleted pointer
          try
            {
#endif // ACE_HAS_EXCEPTIONS

              this->reactor ()->cancel_timer (this->handlers_to_stop_[i]);
              this->reactor ()->remove_handler
                (this->handlers_to_stop_[i],
                 ACE_Event_Handler::ALL_EVENTS_MASK
                 | ACE_Event_Handler::DONT_CALL);
#if defined ACE_HAS_EXCEPTIONS
            }
          catch (...)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Stop_Handler::handle_input - ")
                          ACE_TEXT ("EXCEPTION CATCHED. Most probably ")
                          ACE_TEXT ("handler's pointer has been deleted.\n")));
            }
#endif // ACE_HAS_EXCEPTIONS
          this->handlers_to_stop_[i] = 0;
        }
    }

  this->reactor ()->remove_handler (this->registered_signals_);

  if (reactor ()->end_reactor_event_loop () == -1)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("(%P|%t) Stop_Handler::handle_signal:%p\n"),
                         ACE_TEXT ("end_reactor_event_loop")),
                        -1);
    }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Stop_Handler::handle_input - completed.\n")));
  return 0;
}

int
Stop_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask m)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Stop_Handler::handle_close - entered.\n")));
  if (m == ACE_Event_Handler::SIGNAL_MASK)
    return 0;
  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::SIGNAL_MASK |
                                    ACE_Event_Handler::DONT_CALL);

  if (reactor ()->end_reactor_event_loop () == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Stop_Handler::handle_close: %p\n"),
                       ACE_TEXT ("end_reactor_event_loop")),
                      -1);
  return 0;
}

int
Stop_Handler::handle_timeout (ACE_Time_Value const &,
                              void const *)
{
  return 0;
}

// Register handler with us for stopping.
int
Stop_Handler::register_handler (ACE_Event_Handler *handler)
{
  if (!handler)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::register_handler - ")
                       ACE_TEXT ("error, handler is a null pointer.\n")),
                      -1);

  size_t index = 0;

  for (index = 0;
       (index < HANDLERS_TO_STOP_TABLE_SIZE &&
        this->handlers_to_stop_[index]);
       ++index)
    ;

  if (index == HANDLERS_TO_STOP_TABLE_SIZE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) Stop_Handler::register_handler ")
                         ACE_TEXT ("- error, no space in ")
                         ACE_TEXT ("handlers_to_stop_table.\nIncrease ")
                         ACE_TEXT ("HANDLERS_TO_STOP_TABLE_SIZE.\n")),
                        -1);
    }

  this->handlers_to_stop_[index] = handler;
  return 0;
}

// Unregister handler, registered before with us for stopping.
int
Stop_Handler::unregister_handler (ACE_Event_Handler *handler)
{
  if (!handler)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::unregister_handler - ")
                       ACE_TEXT ("error, handler is a null pointer.\n")),
                      -1);

  size_t index = 0;

  for (index = 0;
       (index < HANDLERS_TO_STOP_TABLE_SIZE &&
        handler != this->handlers_to_stop_[index]);
       ++index)
    ;

  size_t entrance = 0;
  if (index == HANDLERS_TO_STOP_TABLE_SIZE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::unregister_")
                       ACE_TEXT ("handler - error, the handler was not ")
                       ACE_TEXT ("found among registered handlers.\n")),
                      -1);

  entrance = index;
  // null the entrance. Nulled entrances cannot be destroyed
  this->handlers_to_stop_[entrance] = 0;

  return 0;
}


Repeats_Handler::Repeats_Handler (void)
  : check_handler_ (0),
    seconds_timer_ (60),
    counter_ (0)
{
}

Repeats_Handler::~Repeats_Handler (void)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Repeats_Handler::~Repeats_Handler.\n")));
}

int
Repeats_Handler::open (Echo_Handler * check_handler,
                       ACE_Reactor * const reactor,
                       unsigned int seconds_timer)
{
  if (!check_handler)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Repeats_Handler::open - error: ")
                       ACE_TEXT ("NULL check_handler.\n")),
                      -1);

  this->check_handler_ = check_handler;

  if (!reactor)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Stop_Handler::open - error: ")
                       ACE_TEXT ("NULL reactor.\n")),
                      -1);

  this->reactor (reactor);
  this->seconds_timer_ = seconds_timer;

  if (this->reactor ()->schedule_timer (
        this,
        0,
        ACE_Time_Value (1),
        ACE_Time_Value (this->seconds_timer_)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Repeats_Handler::open: %p\n"),
                       ACE_TEXT ("schedule_timer")),
                      -1);
  return 0;
}

int
Repeats_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG
    ((LM_INFO,
      ACE_TEXT ("(%P|%t) Repeats_Handler::handle_close - entered.\n")));

  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::ALL_EVENTS_MASK |
                                    ACE_Event_Handler::DONT_CALL);
  return 0;
}

static int one_button_test = 0;

int
Repeats_Handler::handle_timeout (ACE_Time_Value const &,
                                 void const *)
{
  this->counter_++ ;
  if (one_button_test && this->counter_ > 3)
    {
      ACE_OS::raise (SIGINT);
    }
  if (this->check_handler_)
    {
      return this->check_handler_->dispatch_echo_checks (true);
    }

  return -1;
}

// to create core on some UNIX platforms
#if defined (ACE_HAS_SIG_C_FUNC)
extern "C"
{
#endif /* #if defined (ACE_HAS_SIG_C_FUNC) */

#if ! defined (ACE_WIN32)
  static void sigsegv_handler (int)
  {
    ACE_OS::abort ();
  }
#endif /* #if ! defined (ACE_WIN32) */

#if defined (ACE_HAS_SIG_C_FUNC)
}
#endif /* #if defined (ACE_HAS_SIG_C_FUNC) */

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
BOOL CtrlHandler(DWORD fdwCtrlType)
{
  switch (fdwCtrlType)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
      ACE_OS::raise (SIGINT);
      return TRUE;

      // Pass other signals to the next handler.
    default:
      return FALSE;
    }
}
#endif /* #if defined (ACE_WIN32) */

#define MAX_NUMBER_OF_PING_POINTS   16

static int number_of_ping_points  = 0;
static char ping_points_ips [MAX_NUMBER_OF_PING_POINTS][16];
static ACE_INET_Addr ping_points_addrs [MAX_NUMBER_OF_PING_POINTS];
static char local_ip_to_bind [16];

static int wait_echo_reply_timer = 500; // 500 ms to wait is the default
static int repeats_seconds_timer = 60; // 60 seconds between repeats

static int
is_ip_address_local (char const * const ip_to_bind)
{
  ACE_INET_Addr *the_addr_array = 0;
  size_t how_many = 0;
  int rc = ACE::get_ip_interfaces (how_many, the_addr_array);

  if (rc != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("is_ip_address_local: %p\n"),
                       ACE_TEXT ("ACE::get_ip_interfaces")),
                      -1);

  if (how_many == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("is_ip_address_local: "),
                       ACE_TEXT ("No interfaces presently configured ")
                       ACE_TEXT ("in the kernel\n")),
                      -1);

  // debugging messages
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("is_ip_address_local () - there are %d interfaces\n"),
              how_many));

  for (size_t i = 0; i < how_many; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\t%s\n"),
                  the_addr_array[i].get_host_addr ()));
    }

  for (size_t j = 0; j < how_many; ++j)
    {
      if (!ACE_OS::strcmp (the_addr_array[j].get_host_addr (), ip_to_bind))
        {
          return 0;
        }
    }
  return -1;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_OS::memset (ping_points_ips, 0, sizeof ping_points_ips);
  ACE_OS::memset (local_ip_to_bind, 0, sizeof local_ip_to_bind);

  if (argc == 1) // one button test
    {
      one_button_test = 1;
      repeats_seconds_timer = 2;
      number_of_ping_points = 1;

      ACE_OS::strncpy (ping_points_ips [0],
                       "127.0.0.1",
                       sizeof ping_points_ips [0]);

      ping_points_addrs[0].set ((u_short) 0, ping_points_ips[0]);
      return 0;
    }

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("b:p:t:w:"));
  int c, counter = 0;
  ACE_INET_Addr b_temp_addr;
  ACE_TCHAR *token = 0;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'b':     // ip-address of the interface to bind to
          ACE_OS::strncpy (local_ip_to_bind,
                           ACE_TEXT_ALWAYS_CHAR (get_opt.optarg),
                           sizeof local_ip_to_bind);

          if (!ACE_OS::strlen (local_ip_to_bind) ||
              b_temp_addr.set ((u_short)0, local_ip_to_bind) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("-b  should be followed by a valid ")
                          ACE_TEXT ("IPv4 address.\n")));
              // print_usage ();
              return -1;
            }
          if (is_ip_address_local (local_ip_to_bind) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("the -b address (%s) ")
                                 ACE_TEXT ("is not a local ")
                                 ACE_TEXT ("address of your computer.\n")
                                 ACE_TEXT ("\tPlease correct it.\n"),
                                 local_ip_to_bind),
                                -1);
            }
          break;

        case 'p':  // ping-point (target) ip-addresses, separated by ":'"

          // tokenizing the string
          for (token = ACE_OS::strtok (get_opt.optarg, ACE_TEXT (":"));
               token != 0 && counter < MAX_NUMBER_OF_PING_POINTS;
               token = ACE_OS::strtok (0, ACE_TEXT (":")))
            {
              if (ping_points_addrs[counter].set ((u_short)0, token) != 0)
                ACE_ERROR_RETURN
                  ((LM_ERROR,
                    ACE_TEXT ("Error: the address \"%s\" is not ")
                    ACE_TEXT ("a valid IPv4 address.\n"),
                    token),
                   -1);
              ++number_of_ping_points;
              ++counter;
            }
          break;

        case 't':
          repeats_seconds_timer = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'w':
          wait_echo_reply_timer = ACE_OS::atoi (get_opt.optarg);
          break;

        default:
          // return print_usage (argc,argv);
          break;

        }
    }

  if (!number_of_ping_points)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Error: no valid IPv4 addresses ")
                         ACE_TEXT ("were provided, using -p option.\n")),
                        -1);
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Network_Adapters_Test"));

#if defined (ACE_WIN32)
#if !defined (ACE_HAS_WINCE)
  SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
#endif
#else /* #if defined (ACE_WIN32) */
  // Set a handler for SIGSEGV signal to call for abort.
  ACE_Sig_Action sa1 ((ACE_SignalHandler) sigsegv_handler, SIGSEGV);
#endif /* #if defined (ACE_WIN32) */
  if (::parse_args (argc, argv) == -1)
    {
      return -1;
    }

  ACE_Reactor * main_reactor = 0;
  ACE_NEW_RETURN (main_reactor, ACE_Reactor, -1);

  (void) ACE_High_Res_Timer::global_scale_factor ();

  // Change the source of time in the reactor to the high-resolution
  // timer.  Why does this test require such precision for a 1 second
  // timer is beyond me ...  I think it is a cut&paste error.
  //
  // The use of auto_ptr<> is optional, ACE uses dangerous memory
  // management idioms everywhere, I thought I could demonstrate how
  // to do it right in at least one test.  Notice the lack of
  // ACE_NEW_RETURN, that monstrosity has no business in proper C++
  // code ...
  auto_ptr<ACE_Timer_Heap_Variable_Time_Source> tq(
      new ACE_Timer_Heap_Variable_Time_Source);
  // ... notice how the policy is in the derived timer queue type.
  // The abstract timer queue does not have a time policy ...
  tq->set_time_policy(&ACE_High_Res_Timer::gettimeofday_hr);
  // ... and then the timer queue is replaced.  Strangely, the reactor
  // does *not* copy the timers, it just deletes the existing timer
  // queue ....
  main_reactor->timer_queue(tq.get());
  // ... the reactor does not assume ownership

  /**
   * Stop_Handler's is supposed to stop the activity of all
   * handlers by a SIGINT signal.  We create and activate here an object of
   * Stop_Handler and pass an instance of reactor (main_reactor),
   * running demultiplexing event loop in the "main thread".
   */
  Stop_Handler* stop_handler = 0;
  ACE_NEW_RETURN (stop_handler, Stop_Handler (main_reactor), -1);
  if (stop_handler->open () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("main() - stop_handler->open")));
      ACE_OS::exit(-2);
    }

  ACE_TCHAR *ping_status = 0;
  ACE_NEW_RETURN (ping_status, ACE_TCHAR[number_of_ping_points], -1);

  // wait_echo_reply_timer is in msec
  int seconds = 0;
  int milliseconds = 0;
  seconds = wait_echo_reply_timer / 1000;
  milliseconds =  wait_echo_reply_timer % 1000;
  ACE_Time_Value const wait_timer (seconds, milliseconds);

  Echo_Handler *ping_handler = 0;
  ACE_NEW_RETURN (ping_handler, Echo_Handler, -1);

  if (ACE_OS::strlen (local_ip_to_bind))
    {
      // We are willing to bind the raw-socket to a certain adapter,
      // probably because we are willing to check connectivity/etc
      // of the local adapter.
      ACE_INET_Addr local_adapter;
      local_adapter.set ((u_short) 0, local_ip_to_bind);
      if (ping_handler->open (main_reactor,
                              wait_timer,
                              ping_points_addrs,
                              number_of_ping_points,
                              ping_status,
                              2,  // max_attempts_number
                              local_adapter) == -1)
        {
          int res = 0;
          // If this process doesn't have privileges to open a raw socket, log
          // a warning instead of an error.
          if (errno == EPERM || errno == EACCES)
            {
              ACE_ERROR ((LM_WARNING,
                          ACE_TEXT ("(%P|%t) main() - ping_handler->open: ")
                          ACE_TEXT ("insufficient privs to run this test\n")));
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p\n"),
                          ACE_TEXT ("main() - ping_handler->open")));
              res = -4;
            }
          delete ping_handler;
          delete [] ping_status;
          delete main_reactor;
          delete stop_handler;

          ACE_END_TEST;
          return res;
        }
    }
  else
    {
      // Binding to a local adapter is not of our interest. We just
      // are willing to check all these remote IPs, to monitor, that
      // they are alive.
      if (ping_handler->open (main_reactor,
                              wait_timer,
                              ping_points_addrs,
                              number_of_ping_points,
                              ping_status,
                              2) == -1)   // max_attempts_number
        {
          int res = 0;
          if (errno == EPERM || errno == EACCES)
            {
              ACE_ERROR ((LM_WARNING,
                          ACE_TEXT ("(%P|%t) main() - ping_handler->open: ")
                          ACE_TEXT ("insufficient privs to run this test\n")));
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) %p\n"),
                          ACE_TEXT ("main() - ping_handler->open")));
              res = -4;
            }
          delete ping_handler;
          delete [] ping_status;
          delete main_reactor;
          delete stop_handler;

          ACE_END_TEST;
          return res;
        }
    }

  Repeats_Handler *repeats_handler = 0;
  ACE_NEW_RETURN (repeats_handler, Repeats_Handler, -1);
  if (repeats_handler->open (ping_handler,
                             main_reactor,
                             repeats_seconds_timer) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("main() - repeats_handler->open")));

      delete repeats_handler;
      delete ping_handler;
      delete [] ping_status;
      delete main_reactor;
      delete stop_handler;

      ACE_END_TEST;
      return -4;
    }

  stop_handler->register_handler (repeats_handler);
  stop_handler->register_handler (ping_handler);

  // Demultiplexing event loop of the main_reactor.
  while (main_reactor->reactor_event_loop_done () == 0)
    {
      main_reactor->run_reactor_event_loop ();
    }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t|%T) \"Network_Adapters_Test\" main() - ")
              ACE_TEXT ("out of reactor's loop.\n")));

  delete repeats_handler;
  delete ping_handler;
  delete [] ping_status;
  delete main_reactor;
  delete stop_handler;

  ACE_END_TEST;
  return 0;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Network_Adapters_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t|%T) \"Network_Adapters_Test\" main() - ")
              ACE_TEXT ("ICMP support not configured.\n")
              ACE_TEXT ("Define ACE_HAS_ICMP_SUPPORT = 1 in your config.h ")
              ACE_TEXT ("file to enable.\n")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_HAS_ICMP_SUPPORT == 1 */
