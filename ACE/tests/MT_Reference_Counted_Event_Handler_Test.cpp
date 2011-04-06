
//=============================================================================
/**
 *  @file    MT_Reference_Counted_Event_Handler_Test.cpp
 *
 *  $Id$
 *
 *
 *  This test tries to represents what happens in the ORB wrt to
 *  event handlers, reactors, timer queues, threads, and connection
 *  caches, minus the other complexities.  The following reactors
 *  are tested: Select, TP, WFMO, and Dev Poll (if enabled).
 *
 *  The test checks proper use and shutting down of client-side
 *  event handlers when it is used by invocation threads and/or
 *  event loop threads.  Server-side event handlers are either
 *  threaded or reactive. A purger thread is introduced to check the
 *  connection recycling and cache purging.  Nested upcalls are also
 *  tested.
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Auto_Event.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_THREADS)

static const char message[] = "abcdefghijklmnopqrstuvwxyz";
static const int message_size = 26;
static int test_select_reactor = 1;
static int test_tp_reactor = 1;
static int test_wfmo_reactor = 1;
static int test_dev_poll_reactor = 1;
static int debug = 0;
static int number_of_connections = 5;
static int max_nested_upcall_level = 10;
static int close_timeout = 500;
static int pipe_open_attempts = 10;
static int pipe_retry_timeout = 1;
static int make_invocations = -1;
static int run_event_loop_thread = -1;
static int run_purger_thread = -1;
static int run_receiver_thread = -1;
static int nested_upcalls = -1;

static ACE_HANDLE server_handle = ACE_INVALID_HANDLE;
static ACE_HANDLE client_handle = ACE_INVALID_HANDLE;

static int number_of_options = 5;
static int test_configs[][5] =
  {
    //
    // make_invocations, run_event_loop_thread, run_purger_thread, run_receiver_thread, nested_upcalls
    //

    // { 0, 0, 0, 0, 0, }, // At least one thread should be running.
    // { 0, 0, 0, 1, 0, }, // If event_loop_thread is not running and invocation_thread is not making invocations,
    // no thread will know that the socket is closed.
    // { 0, 0, 1, 0, 0, }, // If invocation_thread is not making invocations and if receiver is not threaded,
    // we cannot decide which socket to close.
    // { 0, 0, 1, 1, 0, }, // If event_loop_thread is not running and invocation_thread is not making invocations,
    // no thread will know that the socket is closed.
    // { 0, 1, 0, 0, 0, }, // If invocation_thread is not making invocations and if receiver is not threaded,
    // we cannot decide which socket to close.
    { 0, 1, 0, 1, 0, },
    // { 0, 1, 0, 1, 1, }, // No need for nested upcalls without invocations.
    // { 0, 1, 1, 0, 0, }, // If invocation_thread is not making invocations and if receiver is not threaded,
    // we cannot decide which socket to close.
    { 0, 1, 1, 1, 0, },
    // { 0, 1, 1, 1, 1, }, // No need for nested upcalls without invocations.
    // { 1, 0, 0, 0, 0, }, // If both event_loop_thread and receiver are not threaded,
    // no thread can receive the messages.
    { 1, 0, 0, 1, 0, },
    // { 1, 0, 0, 1, 1, }, // No need for nested upcalls without event loop being used by the receiver.
    // { 1, 0, 1, 0, 0, }, // If both event_loop_thread and receiver are not threaded,
    // no thread can receive the messages.
    { 1, 0, 1, 1, 0, },
    // { 1, 0, 1, 1, 1, }, // No need for nested upcalls without event loop being used by the receiver.
    { 1, 1, 0, 0, 0, },
    { 1, 1, 0, 0, 1, },
    { 1, 1, 0, 1, 0, },
    // { 1, 1, 0, 1, 1, }, // No need for nested upcalls without event loop being used by the receiver.
    { 1, 1, 1, 0, 0, },
    { 1, 1, 1, 0, 1, },
    { 1, 1, 1, 1, 0, },
    // { 1, 1, 1, 1, 1, }, // No need for nested upcalls without event loop being used by the receiver.
  };

static int
disable_signal (int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  // Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  // In multi-threaded application this is not POSIX compliant
  // but let's leave it just in case.
  if (ACE_OS::sigprocmask (SIG_BLOCK, &signal_set, 0) != 0)
# else
  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: (%P|%t): %p\n"),
                       ACE_TEXT ("SIG_BLOCK failed")),
                      -1);
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  return 0;
}

/* Replication of the ACE_Pipe class.  Only difference is that this
   class always uses two sockets to create the pipe, even on platforms
   that support pipes. */

class Pipe
{
public:

  Pipe (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  int open (void);

  ACE_HANDLE read_handle (void) const;

  ACE_HANDLE write_handle (void) const;

private:
  ACE_HANDLE handles_[2];
};

int
Pipe::open (void)
{
  ACE_INET_Addr my_addr;
  ACE_SOCK_Acceptor acceptor;
  ACE_SOCK_Connector connector;
  ACE_SOCK_Stream reader;
  ACE_SOCK_Stream writer;
  int result = 0;

  // Bind listener to any port and then find out what the port was.
  if (acceptor.open (ACE_Addr::sap_any) == -1
      || acceptor.get_local_addr (my_addr) == -1)
    result = -1;
  else
    {
      int af = my_addr.get_type ();
      const ACE_TCHAR *local = ACE_LOCALHOST;
#if defined (ACE_HAS_IPV6)
      if (af == AF_INET6)
        local = ACE_IPV6_LOCALHOST;
#endif /* ACE_HAS_IPV6 */
      ACE_INET_Addr sv_addr (my_addr.get_port_number (),
                             local,
                             af);

      // Establish a connection within the same process.
      if (connector.connect (writer, sv_addr) == -1)
        result = -1;
      else if (acceptor.accept (reader) == -1)
        {
          writer.close ();
          result = -1;
        }
    }

  // Close down the acceptor endpoint since we don't need it anymore.
  acceptor.close ();
  if (result == -1)
    return -1;

  this->handles_[0] = reader.get_handle ();
  this->handles_[1] = writer.get_handle ();

  return 0;
}

Pipe::Pipe (void)
{
  this->handles_[0] = ACE_INVALID_HANDLE;
  this->handles_[1] = ACE_INVALID_HANDLE;
}

ACE_HANDLE
Pipe::read_handle (void) const
{
  return this->handles_[0];
}

ACE_HANDLE
Pipe::write_handle (void) const
{
  return this->handles_[1];
}

class Connection_Cache;
class Event_Loop_Thread;

static Event_Loop_Thread *global_event_loop_thread_variable = 0;

class Sender : public ACE_Event_Handler
{
public:

  Sender (ACE_HANDLE handle,
          Connection_Cache &connection_cache);

  ~Sender (void);

  int handle_input (ACE_HANDLE);

  ssize_t send_message (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  void close (void);

  ACE_HANDLE handle_;

  Connection_Cache &connection_cache_;

};

class Connection_Cache
{
public:

  Connection_Cache (void);

  ~Connection_Cache (void);

  void add_connection (Sender *sender);

  void remove_connection (Sender *sender);

  Sender *acquire_connection (void);

  void release_connection (Sender *sender);

  int find (Sender *sender);

  ACE_SYNCH_MUTEX &lock (void);

  enum State
    {
      IDLE,
      BUSY,
      NOT_IN_CACHE
    };

  struct Entry
  {
    Sender *sender_;
    State state_;
  };

  Entry *entries_;

  ACE_SYNCH_MUTEX lock_;
};

Sender::Sender (ACE_HANDLE handle,
                Connection_Cache &connection_cache)
  : handle_ (handle),
    connection_cache_ (connection_cache)
{
  // Enable reference counting.
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Reference count in Sender::Sender() is %d\n"),
                this->reference_count_.value ()));
}

Sender::~Sender (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Reference count in ~Sender::Sender() is %d\n"),
                this->reference_count_.value ()));

  // Close the socket that we are responsible for.
  ACE_OS::closesocket (this->handle_);
}

int
Sender::handle_input (ACE_HANDLE)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Reference count in Sender::handle_input() is %d\n"),
                this->reference_count_.value ()));

  //
  // In this test, this method is only called when the connection has
  // been closed.  Remove self from Reactor.
  //

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Event loop thread calling Sender::close() ")
              ACE_TEXT ("for handle %d\n"),
              this->handle_));

  this->close ();

  return 0;
}

void
Sender::close (void)
{
  // Remove socket from Reactor (may fail if another thread has already
  // removed the handle from the Reactor).
  if (this->reactor() != 0)
    this->reactor ()->remove_handler (this->handle_,
                                      ACE_Event_Handler::ALL_EVENTS_MASK);

  // Remove self from connection cache (may fail if another thread has
  // already removed "this" from the cache).
  this->connection_cache_.remove_connection (this);
}

ssize_t
Sender::send_message (void)
{
  ACE_Time_Value timeout (0, close_timeout * 1000);

  return ACE::send_n (this->handle_,
                      message,
                      message_size,
                      &timeout);
}

class Event_Loop_Thread : public ACE_Task_Base
{
public:

  Event_Loop_Thread (ACE_Thread_Manager &thread_manager,
                     ACE_Reactor &reactor);

  int svc (void);

  ACE_Reactor &reactor_;

};

class Receiver : public ACE_Task_Base
{
public:

  Receiver (ACE_Thread_Manager &thread_manager,
            ACE_HANDLE handle,
            int nested_upcalls);

  ~Receiver (void);

  int svc (void);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  int close (u_long flags);

  int handle_input (ACE_HANDLE);

  int resume_handler (void);

  ACE_HANDLE handle_;

  int counter_;

  int nested_upcalls_;

  int nested_upcalls_level_;

};

Receiver::Receiver (ACE_Thread_Manager &thread_manager,
                    ACE_HANDLE handle,
                    int nested_upcalls)
  : ACE_Task_Base (&thread_manager),
    handle_ (handle),
    counter_ (1),
    nested_upcalls_ (nested_upcalls),
    nested_upcalls_level_ (0)
{
  // Enable reference counting.
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Reference count in Receiver::Receiver() is %d\n"),
                this->reference_count_.value ()));
}

Receiver::~Receiver (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Reference count in ~Receiver::Receiver() is %d\n"),
                this->reference_count_.value ()));

  // Close the socket that we are responsible for.
  ACE_OS::closesocket (this->handle_);
}

int
Receiver::svc (void)
{
  //
  // Continuously receive messages from the Sender.  On error, exit
  // thread.
  //

  int result = 0;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%t) Receiver::svc commencing, handle = %d\n"),
              this->handle_));

  disable_signal (SIGPIPE, SIGPIPE);

  while (result != -1)
    {
      result =
        this->handle_input (this->handle_);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%t) Receiver::svc terminating, handle = %d\n"),
              this->handle_));
  return 0;
}

int
Receiver::handle_input (ACE_HANDLE handle)
{
  char buf[message_size + 1];

  ACE_Time_Value timeout (0, close_timeout * 1000);

  // Receive message.
  ssize_t result =
    ACE::recv_n (handle,
                 buf,
                 message_size,
                 &timeout);

  if (debug && result < 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%t) Receiver::handle input, ")
                ACE_TEXT("h = %d, result = %d %p\n"),
                handle_, result, ACE_TEXT("ACE::recv_n")));

  if (this->reactor ())
    this->reactor ()->resume_handler (handle);

  if (result == message_size)
    {
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Message %d received on handle %d\n"),
                    this->counter_++,
                    handle));

      if (this->thr_count () == 0 &&
          this->nested_upcalls_)
        {
          this->nested_upcalls_level_++;

          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Nesting level %d\n"),
                        this->nested_upcalls_level_));

          if ((this->nested_upcalls_level_ != max_nested_upcall_level) &&
            (global_event_loop_thread_variable != 0))
            global_event_loop_thread_variable->svc ();

          this->nested_upcalls_level_--;
          return 0;
        }
      else
        return 0;
    }
  else
    {
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) /*** Problem in receiving message %d on handle")
                    ACE_TEXT (" %d: shutting down receiving thread ***/\n"),
                    this->counter_,
                    handle));

      return -1;
    }
}

int
Receiver::resume_handler (void)
{
  /// The application takes responsibility of resuming the handler.
  return ACE_APPLICATION_RESUMES_HANDLER;
}

int
Receiver::close (u_long)
{
  // If threaded, we are responsible for deleting this instance when
  // the thread completes.  If not threaded, Reactor reference
  // counting will handle the deletion of this instance.
  delete this;
  return 0;
}

class Connector
{
public:

  Connector (ACE_Thread_Manager &thread_manager,
             ACE_Reactor &reactor,
             int nested_upcalls);

  //FUZZ: disable check_for_lack_ACE_OS
  ///FUZZ: enable check_for_lack_ACE_OS
  int connect (ACE_HANDLE &client_handle,
               ACE_HANDLE &server_handle,
               int run_receiver_thread);

  ACE_Thread_Manager &thread_manager_;

  ACE_Reactor &reactor_;

  int nested_upcalls_;

};

Connector::Connector (ACE_Thread_Manager &thread_manager,
                      ACE_Reactor &reactor,
                      int nested_upcalls)
  : thread_manager_ (thread_manager),
    reactor_ (reactor),
    nested_upcalls_ (nested_upcalls)
{
}

int
Connector::connect (ACE_HANDLE &client_handle,
                    ACE_HANDLE &server_handle,
                    int run_receiver_thread)
{
  //
  // Create a connection and a receiver to receive messages on the
  // connection.
  //

  Pipe pipe;
  int result = 0;

  for (int i = 0; i < pipe_open_attempts; ++i)
    {
      result =
        pipe.open ();

      if (result == 0)
        break;

      if (result == -1)
        ACE_OS::sleep (pipe_retry_timeout);
    }

  ACE_TEST_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  Receiver *receiver =
    new Receiver (this->thread_manager_,
                  pipe.write_handle (),
                  this->nested_upcalls_);

  // Either the receiver is threaded or register it with the Reactor.
  if (run_receiver_thread)
    result =
      receiver->activate ();
  else
    {
      result =
        this->reactor_.register_handler (pipe.write_handle (),
                                         receiver,
                                         ACE_Event_Handler::READ_MASK);

      // The reference count on the receiver was increased by the
      // Reactor.
      ACE_Event_Handler_var safe_receiver (receiver);
    }

  ACE_TEST_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  client_handle =
    pipe.read_handle ();

  server_handle =
    pipe.write_handle ();

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) New connection: client handle = %d, ")
                ACE_TEXT ("server handle = %d\n"),
                client_handle, server_handle));

  return 0;
}

Connection_Cache::Connection_Cache (void)
{
  // Initialize the connection cache.
  this->entries_ =
    new Entry[number_of_connections];

  for (int i = 0; i < number_of_connections; ++i)
    {
      this->entries_[i].sender_ = 0;
      this->entries_[i].state_ = NOT_IN_CACHE;
    }
}

int
Connection_Cache::find (Sender *sender)
{
  for (int i = 0; i < number_of_connections; ++i)
    {
      if (this->entries_[i].sender_ == sender)
        return i;
    }

  return -1;
}

void
Connection_Cache::add_connection (Sender *sender)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  // Make sure that the state of the connection cache is as
  // expected. <sender> should not be already in the cache.
  ACE_TEST_ASSERT (this->find (sender) == -1);

  int empty_index =
    this->find (0);

  sender->add_reference ();
  this->entries_[empty_index].sender_ = sender;
  this->entries_[empty_index].state_ = BUSY;
}

void
Connection_Cache::remove_connection (Sender *sender)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  // Make sure that the state of the connection cache is as expected.
  // remove_connection() may already have been called.
  int index =
    this->find (sender);

  if (index == -1)
    return;

  // If we still have the sender, remove it.
  sender->remove_reference ();
  this->entries_[index].sender_ = 0;
  this->entries_[index].state_ = NOT_IN_CACHE;
}

Sender *
Connection_Cache::acquire_connection (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);

  // Find a valid and IDLE sender.

  int index = -1;

  for (int i = 0; i < number_of_connections; ++i)
    {
      if (this->entries_[i].sender_ &&
          this->entries_[i].state_ == IDLE)
        index = i;
    }

  if (index == -1)
    return 0;

  this->entries_[index].sender_->add_reference ();
  this->entries_[index].state_ = BUSY;

  return this->entries_[index].sender_;
}

void
Connection_Cache::release_connection (Sender *sender)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  // Make sure that the state of the connection cache is as expected.
  // remove_connection() may have already removed the connection from
  // the cache.
  int index =
    this->find (sender);

  if (index == -1)
    return;

  // If we still have the sender, idle it.
  this->entries_[index].state_ = IDLE;
}

ACE_SYNCH_MUTEX &
Connection_Cache::lock (void)
{
  return this->lock_;
}

Connection_Cache::~Connection_Cache (void)
{
  for (int i = 0; i < number_of_connections; ++i)
    {
      if (this->entries_[i].sender_)
        this->remove_connection (this->entries_[i].sender_);
    }

  delete[] this->entries_;
}

class Invocation_Thread : public ACE_Task_Base
{
public:

  Invocation_Thread (ACE_Thread_Manager &thread_manager,
                     ACE_Reactor &reactor,
                     Connection_Cache &connection_cache,
                     ACE_Auto_Event &new_connection_event,
                     int make_invocations,
                     int run_receiver_thread,
                     int nested_upcalls);

  int svc (void);

  Sender *create_connection (void);

  Connection_Cache &connection_cache_;

  ACE_Reactor &reactor_;

  ACE_Thread_Manager &thread_manager_;

  ACE_Auto_Event &new_connection_event_;

  int make_invocations_;

  int run_receiver_thread_;

  int nested_upcalls_;

};

Invocation_Thread::Invocation_Thread (ACE_Thread_Manager &thread_manager,
                                      ACE_Reactor &reactor,
                                      Connection_Cache &connection_cache,
                                      ACE_Auto_Event &new_connection_event,
                                      int make_invocations,
                                      int run_receiver_thread,
                                      int nested_upcalls)
  : ACE_Task_Base (&thread_manager),
    connection_cache_ (connection_cache),
    reactor_ (reactor),
    thread_manager_ (thread_manager),
    new_connection_event_ (new_connection_event),
    make_invocations_ (make_invocations),
    run_receiver_thread_ (run_receiver_thread),
    nested_upcalls_ (nested_upcalls)
{
}

Sender *
Invocation_Thread::create_connection (void)
{
  int result = 0;

  // Connector for creating new connections.
  Connector connector (this->thread_manager_,
                       this->reactor_,
                       this->nested_upcalls_);

  // <server_handle> is a global variable. It will be used later by
  // the Close_Socket_Thread.
  result =
    connector.connect (client_handle,
                       server_handle,
                       this->run_receiver_thread_);
  ACE_TEST_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  // Create a new sender.
  Sender *sender =
    new Sender (client_handle,
                this->connection_cache_);

  // Register it with the cache.
  this->connection_cache_.add_connection (sender);

  //
  // There might be a race condition here. The sender has been added
  // to the cache and is potentially available to other threads
  // accessing the cache. Therefore, the other thread may use this
  // sender and potentially close the sender before it even gets
  // registered with the Reactor.
  //
  // This is resolved by marking the connection as busy when it is
  // first added to the cache. And only once the thread creating the
  // connection is done with it, it is marked a available in the
  // cache.
  //
  // This order of registration is important.
  //

  // Register the handle with the Reactor.
  result =
    this->reactor_.register_handler (client_handle,
                                     sender,
                                     ACE_Event_Handler::READ_MASK);
#if 0
  ACE_TEST_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);
#else
  if (result != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) create_connection h %d, %p\n"),
                client_handle,
                ACE_TEXT ("register_handler")));
#endif
  return sender;
}

int
Invocation_Thread::svc (void)
{
  int connection_counter = 0;
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Invocation_Thread::svc commencing\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  for (int message_counter = 1;; ++message_counter)
    {
      // Get a connection from the cache.
      Sender *sender =
        this->connection_cache_.acquire_connection ();

      // If no connection is available in the cache, create a new one.
      if (sender == 0)
        {
          if (connection_counter < number_of_connections)
            {
              sender = this->create_connection ();

              // This lets the Close_Socket_Thread know that the new
              // connection has been created.
              int result =
                this->new_connection_event_.signal ();
              ACE_TEST_ASSERT (result == 0);
              ACE_UNUSED_ARG (result);

              ++connection_counter;
              message_counter = 1;
            }
          else
            // Stop the thread, if the maximum number of connections
            // for the test has been reached.
            break;
        }

      // The reference count on the sender was increased by the cache
      // before it was returned to us.
      ACE_Event_Handler_var safe_sender (sender);

      // If the test does not require making invocations, immediately
      // release the connection.
      if (!this->make_invocations_)
        {
          this->connection_cache_.release_connection (sender);

          // Sleep for a short while
          ACE_OS::sleep (ACE_Time_Value (0, 10 * 1000));
        }
      else
        {
          // Make invocation.
          ssize_t result =
            sender->send_message ();

          // If successful, release connection.
          if (result == message_size)
            {
              if (debug)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%t) Message %d:%d delivered on handle %d\n"),
                            connection_counter,
                            message_counter,
                            sender->handle_));

              this->connection_cache_.release_connection (sender);
            }
          else
            {
              // If failure in making invocation, close the sender.
              if (debug)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%t) /*** Problem in delivering message ")
                            ACE_TEXT ("%d:%d on handle %d: shutting down ")
                            ACE_TEXT ("invocation thread ***/\n"),
                            connection_counter,
                            message_counter,
                            sender->handle_));

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%t) Invocation thread calling ")
                          ACE_TEXT ("Sender::close() for handle %d\n"),
                          sender->handle_));

              sender->close ();
            }
        }
    }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Invocation_Thread::svc calling end_reactor_event_loop\n")));

  // Close the Reactor event loop.
  this->reactor_.end_reactor_event_loop ();
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Invocation_Thread::svc terminating\n")));

  return 0;
}

class Close_Socket_Thread : public ACE_Task_Base
{
public:

  Close_Socket_Thread (ACE_Thread_Manager &thread_manager,
                       ACE_Reactor &reactor,
                       ACE_Auto_Event &new_connection_event,
                       int make_invocations,
                       int run_receiver_thread);

  int svc (void);

  ACE_Auto_Event &new_connection_event_;

  ACE_Reactor &reactor_;

  int make_invocations_;

  int run_receiver_thread_;

};

Close_Socket_Thread::Close_Socket_Thread (ACE_Thread_Manager &thread_manager,
                                          ACE_Reactor &reactor,
                                          ACE_Auto_Event &new_connection_event,
                                          int make_invocations,
                                          int run_receiver_thread)
  : ACE_Task_Base (&thread_manager),
    new_connection_event_ (new_connection_event),
    reactor_ (reactor),
    make_invocations_ (make_invocations),
    run_receiver_thread_ (run_receiver_thread)
{
}

int
Close_Socket_Thread::svc (void)
{
  ACE_OS::srand ((u_int) ACE_OS::time ());
  ACE_Time_Value timeout (0, close_timeout * 1000);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Close_Socket_Thread::svc commencing\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  for (; !this->reactor_.reactor_event_loop_done ();)
    {
      // Wait for the new connection to be established.
      int result =
        this->new_connection_event_.wait (&timeout,
                                          0);
      ACE_TEST_ASSERT (result == 0 ||
                  (result == -1 && errno == ETIME));

      if (result == -1 &&
          errno == ETIME)
        continue;

      // Sleep for half a second.
      ACE_OS::sleep (timeout);

      int close_client = 0;

      // If the invocation thread is making invocations and if the
      // receiver is threaded, either socket can be closed.
      if (this->make_invocations_ &&
          this->run_receiver_thread_)
        // Randomize which socket to close.
        close_client = ACE_OS::rand () % 2;

      // If the invocation thread is making invocations, only close
      // the client socket.
      else if (this->make_invocations_)
        close_client = 1;
      else
        // If the receiver is threaded, only close the server socket.
        close_client = 0;

      if (close_client)
        {
          // Close the client socket.
          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Close socket thread closing client ")
                        ACE_TEXT ("handle %d\n"),
                        client_handle));

          ACE_OS::shutdown (client_handle, ACE_SHUTDOWN_BOTH);
          ACE_OS::closesocket (client_handle);
        }
      else
        {
          // Close the server socket.
          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Close socket thread closing server ")
                        ACE_TEXT ("handle %d\n"),
                        server_handle));
          ACE_OS::shutdown (server_handle, ACE_SHUTDOWN_BOTH);
          ACE_OS::closesocket (server_handle);
        }
    }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Close_Socket_Thread::svc terminating\n")));

  return 0;
}

Event_Loop_Thread::Event_Loop_Thread (ACE_Thread_Manager &thread_manager,
                                      ACE_Reactor &reactor)
  : ACE_Task_Base (&thread_manager),
    reactor_ (reactor)
{
}

int
Event_Loop_Thread::svc (void)
{
  // Simply run the event loop.
  this->reactor_.owner (ACE_Thread::self ());
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Event_Loop_Thread::svc commencing\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  while (!this->reactor_.reactor_event_loop_done ())
    {
      this->reactor_.handle_events ();
    }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Event_Loop_Thread::svc terminating\n")));

  return 0;
}

class Purger_Thread : public ACE_Task_Base
{
public:

  Purger_Thread (ACE_Thread_Manager &thread_manager,
                 ACE_Reactor &reactor,
                 Connection_Cache &connection_cache);

  int svc (void);

  ACE_Reactor &reactor_;

  Connection_Cache &connection_cache_;

};

Purger_Thread::Purger_Thread (ACE_Thread_Manager &thread_manager,
                              ACE_Reactor &reactor,
                              Connection_Cache &connection_cache)
  : ACE_Task_Base (&thread_manager),
    reactor_ (reactor),
    connection_cache_ (connection_cache)
{
}

int
Purger_Thread::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Purger_Thread::svc commencing\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  for (; !this->reactor_.reactor_event_loop_done ();)
    {
      // Get a connection from the cache.
      Sender *sender =
        this->connection_cache_.acquire_connection ();

      // If no connection is available in the cache, sleep for a while.
      if (sender == 0)
        ACE_OS::sleep (ACE_Time_Value (0, 10 * 1000));
      else
        {
          // The reference count on the sender was increased by the
          // cache before it was returned to us.
          ACE_Event_Handler_var safe_sender (sender);

          // Actively close the connection.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Purger thread calling Sender::close() ")
                      ACE_TEXT ("for handle %d\n"),
                      sender->handle_));

          sender->close ();
        }
    }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("(%t) Purger_Thread::svc terminating\n")));

  return 0;
}

void
testing (ACE_Reactor *reactor,
         int make_invocations,
         int run_event_loop_thread,
         int run_purger_thread,
         int run_receiver_thread,
         int nested_upcalls)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n(%t) Configuration:\n")
              ACE_TEXT ("\tInvocation thread = %d\n")
              ACE_TEXT ("\tEvent Loop thread = %d\n")
              ACE_TEXT ("\tPurger thread     = %d\n")
              ACE_TEXT ("\tReceiver thread   = %d\n")
              ACE_TEXT ("\tNested Upcalls    = %d\n\n"),
              make_invocations,
              run_event_loop_thread,
              run_purger_thread,
              run_receiver_thread,
              nested_upcalls));

  ACE_Thread_Manager thread_manager;

  int result = 0;

  // Create the connection cache.
  Connection_Cache connection_cache;
  ACE_Auto_Event new_connection_event;

  // Create the invocation thread.
  Invocation_Thread invocation_thread (thread_manager,
                                       *reactor,
                                       connection_cache,
                                       new_connection_event,
                                       make_invocations,
                                       run_receiver_thread,
                                       nested_upcalls);

  result =
    invocation_thread.activate ();
  ACE_TEST_ASSERT (result == 0);

  // Create the thread for closing the server socket.
  Close_Socket_Thread close_socket_thread (thread_manager,
                                           *reactor,
                                           new_connection_event,
                                           make_invocations,
                                           run_receiver_thread);
  result =
    close_socket_thread.activate ();
  ACE_TEST_ASSERT (result == 0);

  global_event_loop_thread_variable = 0;

  // Create a thread to run the event loop.
  Event_Loop_Thread event_loop_thread (thread_manager,
                                       *reactor);
  if (run_event_loop_thread)
    {
      global_event_loop_thread_variable =
        &event_loop_thread;

      result =
        event_loop_thread.activate ();
      ACE_TEST_ASSERT (result == 0);
    }

  // Create a thread to run the purger.
  Purger_Thread purger_thread (thread_manager,
                               *reactor,
                               connection_cache);
  if (run_purger_thread)
    {
      result =
        purger_thread.activate ();
      ACE_TEST_ASSERT (result == 0);
    }

  // Wait for threads to exit.
  result = thread_manager.wait ();
  ACE_TEST_ASSERT (result == 0);

  // Set the global variable to zero again because the
  // event_loop_thread exists on the stack and now
  // gets destructed.
  global_event_loop_thread_variable = 0;
}

template <class REACTOR_IMPL>
class test
{
public:
  test (int ignore_nested_upcalls,
        int require_event_loop_thread);
};

template <class REACTOR_IMPL>
test<REACTOR_IMPL>::test (int ignore_nested_upcalls,
                          int require_event_loop_thread)
{
  for (int i = 0;
       i < (int) (sizeof test_configs / (sizeof (int) * number_of_options));
       i++)
    {
      if ((make_invocations == -1 ||
           make_invocations == test_configs[i][0]) &&
          (run_event_loop_thread == -1 ||
           run_event_loop_thread == test_configs[i][1]) &&
          (run_purger_thread == -1 ||
           run_purger_thread == test_configs[i][2]) &&
          (run_receiver_thread == -1 ||
           run_receiver_thread == test_configs[i][3]) &&
          (nested_upcalls == -1 ||
           nested_upcalls == test_configs[i][4]))
        {

#if 0 // defined (linux)

          // @@ I am not sure why but when <make_invocations> is 0 and
          // there is no purger thread, the receiver thread does not
          // notice that the connection has been closed.
          if (!test_configs[i][0] && !test_configs[i][2])
            continue;

          // @@ Linux also does not work correctly in the following
          // case: Invocation thread starts and sends messages filling
          // the socket buffer.  It then blocks in write().  In the
          // meantime, the close connection thread closes the socket
          // used by invocation thread. However, the invocation thread
          // does not notice this as it does not return from write().
          // Meanwhile, the event loop thread notices that a socket in
          // it's wait set has been closed, and starts to spin in
          // handle_events() since the invocation thread is not taking
          // out the closed handle from the Reactor's wait set.
          if (test_configs[i][0] && test_configs[i][1] && !test_configs[i][3])
            continue;

#endif /* linux */

          if (test_configs[i][4] && ignore_nested_upcalls)
            continue;

          if (!test_configs[i][1] && require_event_loop_thread)
            continue;

          ACE_Reactor reactor (new REACTOR_IMPL,
                               1);

          testing (&reactor,
                   test_configs[i][0],
                   test_configs[i][1],
                   test_configs[i][2],
                   test_configs[i][3],
                   test_configs[i][4]);
        }
    }
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("a:b:c:d:f:g:k:l:m:n:o:uz:"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
      switch (cc)
        {
        case 'a':
          test_select_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'b':
          test_tp_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'c':
          test_wfmo_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          test_dev_poll_reactor = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'f':
          number_of_connections = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'g':
          close_timeout = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'k':
          make_invocations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          run_event_loop_thread = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'm':
          run_purger_thread = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'n':
          run_receiver_thread = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'o':
          nested_upcalls = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'z':
          debug = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\nusage: %s \n\n")
                      ACE_TEXT ("\t[-a test Select Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-b test TP Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-c test WFMO Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-d test Dev Poll Reactor] (defaults to %d)\n")
                      ACE_TEXT ("\t[-f number of connections] (defaults to %d)\n")
                      ACE_TEXT ("\t[-g close timeout] (defaults to %d)\n")
                      ACE_TEXT ("\t[-k make invocations] (defaults to %d)\n")
                      ACE_TEXT ("\t[-l run event loop thread] (defaults to %d)\n")
                      ACE_TEXT ("\t[-m run purger thread] (defaults to %d)\n")
                      ACE_TEXT ("\t[-n run receiver thread] (defaults to %d)\n")
                      ACE_TEXT ("\t[-o nested upcalls] (defaults to %d)\n")
                      ACE_TEXT ("\t[-z debug] (defaults to %d)\n")
                      ACE_TEXT ("\n"),
                      argv[0],
                      test_select_reactor,
                      test_tp_reactor,
                      test_wfmo_reactor,
                      test_dev_poll_reactor,
                      number_of_connections,
                      close_timeout,
                      make_invocations,
                      run_event_loop_thread,
                      run_purger_thread,
                      run_receiver_thread,
                      nested_upcalls,
                      debug));
          return -1;
        }
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reference_Counted_Event_Handler_Test"));

  // Validate options.
  int result =
    parse_args (argc, argv);
  if (result != 0)
    return result;

  disable_signal (SIGPIPE, SIGPIPE);

  int ignore_nested_upcalls = 1;
  int perform_nested_upcalls = 0;

  int event_loop_thread_required = 1;
  int event_loop_thread_not_required = 0;

  if (test_select_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing Select Reactor....\n\n")));

      test<ACE_Select_Reactor> test (ignore_nested_upcalls,
                                     event_loop_thread_not_required);
      ACE_UNUSED_ARG (test);
    }

  if (test_tp_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing TP Reactor....\n\n")));

      test<ACE_TP_Reactor> test (perform_nested_upcalls,
                                 event_loop_thread_not_required);
      ACE_UNUSED_ARG (test);
    }

#if defined (ACE_HAS_EVENT_POLL)

  if (test_dev_poll_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing Dev Poll Reactor....\n\n")));

      test<ACE_Dev_Poll_Reactor> test (perform_nested_upcalls,
                                       event_loop_thread_not_required);
      ACE_UNUSED_ARG (test);
    }

#endif

#if defined (ACE_WIN32)

  if (test_wfmo_reactor)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\n\n(%t) Testing WFMO Reactor....\n\n")));

      test<ACE_WFMO_Reactor> test (ignore_nested_upcalls,
                                   event_loop_thread_required);
      ACE_UNUSED_ARG (test);
    }

#else /* ACE_WIN32 */

  ACE_UNUSED_ARG (event_loop_thread_required);

#endif /* ACE_WIN32 */

  ACE_END_TEST;

  return 0;
}

#else /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("MT_Reference_Counted_Event_Handler_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_HAS_THREADS */
