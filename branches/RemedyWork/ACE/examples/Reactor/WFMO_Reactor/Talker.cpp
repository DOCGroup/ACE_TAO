// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Talker.cpp
//
// = DESCRIPTION
//
//    This test application tests a wide range of events that can be
//    demultiplexed using various ACE utilities.  Events used include
//    ^C events, reading from STDIN, vanilla Win32 events, thread
//    exits, Reactor notifications, proactive reads, and proactive
//    writes.
//
//    The proactive I/O events are demultiplexed by the ACE_Proactor.
//    The thread exits, notications, and vanilla Win32 events are
//    demultiplexed by the ACE_Reactor.  To enable a single thread
//    to run all these events, the Proactor is integrated with the
//    Reactor.
//
//    The test application prototypes a simple talk program.  Two
//    instances of the application connect.  Input from either console
//    is displayed on the others console also.  Because of the evils
//    of Win32 STDIN, a separate thread is used to read from STDIN.
//    To test the Proactor and Reactor, I/O between the remote
//    processes is performed proactively and interactions between the
//    STDIN thread and the main thread are performed reactively.
//
//    The following description of the test application is in two
//    parts.  The participants section explains the main components
//    involved in the application.  The collaboration section
//    describes how the partipants interact in response to the
//    multiple event types which occur.
//
//    The Reactor test application has the following participants:
//
//    . Reactor -- The Reactor demultiplexes Win32 "waitable"
//    events using WaitForMultipleObjects.
//
//    . Proactor -- The proactor initiates and demultiplexes
//    overlapped I/O operations.  The Proactor registers with the
//    Reactor so that a single-thread can demultiplex all
//    application events.
//
//    . STDIN_Handler -- STDIN_Handler is an Active Object which reads
//    from STDIN and forwards the input to the Peer_Handler.  This
//    runs in a separate thread to make the test more interesting.
//    However, STDIN is "waitable", so in general it can be waited on
//    by the ACE Reactor, thanks MicroSlush!
//
//    . Peer_Handler -- The Peer_Handler connects to another instance
//    of test_reactor.  It Proactively reads and writes data to the
//    peer.  When the STDIN_Handler gives it messages, it fowards them
//    to the remote peer.  When it receives messages from the remote
//    peer, it prints the output to the console.
//
//    The collaborations of the participants are as follows:
//
//    . Initialization
//
//      Peer_Handler -- connects to the remote peer.  It then begins
//      proactively reading from the remote connection.  Note that it
//      will be notified by the Proactor when a read completes.  It
//      also registers a notification strategy with message queue so
//      that it is notified when the STDIN_Handler posts a message
//      onto the queue.
//
//      STDIN_Handler -- STDIN_Handler registers a signal handler for
//      SIGINT.  This just captures the exception so that the kernel
//      doesn't kill our process; We want to exit gracefully.  It also
//      creates an Exit_Hook object which registers the
//      STDIN_Handler's thread handle with the Reactor.  The
//      Exit_Hook will get called back when the STDIN_Handler thread
//      exits.  After registering these, it blocks reading from STDIN.
//
//      Proactor -- is registered with the Reactor.
//
//      The main thread of control waits in the Reactor.
//
//    . STDIN events -- When the STDIN_Handler thread reads from
//    STDIN, it puts the message on Peer_Handler's message queue.  It
//    then returns to reading from STDIN.
//
//    . Message enqueue -- The Reactor thread wakes up and calls
//    Peer_Handler::handle_output.  The Peer_Handler then tries to
//    dequeue a message from its message queue.  If it can, the
//    message is Proactively sent to the remote peer.  Note that the
//    Peer_Handler will be notified with this operation is complete.
//    The Peer_Handler then falls back into the Reactor event loop.
//
//    . Send complete event -- When a proactive send is complete, the
//    Proactor is notified by the Reactor.  The Proactor, in turn,
//    notifies the Peer_Handler.  The Peer_Handler then checks for
//    more messages from the message queue.  If there are any, it
//    tries to send them.  If there are not, it returns to the
//    Reactor event loop.
//
//    . Read complete event -- When a proactive read is complete (the
//    Peer_Handler initiated a proactive read when it connected to the
//    remote peer), the Proactor is notified by the Reactor.  The
//    Proactor, in turn notifies the Peer_Handler.  If the read was
//    successful the Peer_Handler just displays the received msg to
//    the console and reinvokes a proactive read from the network
//    connection.  If the read failed (i.e. the remote peer exited),
//    the Peer_Handler sets a flag to end the event loop and returns.
//    This will cause the application to exit.
//
//    . ^C events -- When the user types ^C at the console, the
//    STDIN_Handler's signal handler will be called.  It does nothing,
//    but as a result of the signal, the STDIN_Handler thread will
//    exit.
//
//    . STDIN_Handler thread exits -- The Exit_Hook will get called
//    back from the Reactor.  Exit_Hook::handle_signal sets a flag
//    to end the event loop and returns.  This will cause the
//    application to exit.
//
//
//    To run example, start an instance of the test with an optional
//    local port argument (as the acceptor). Start the other instance
//    with -h <hostname> and -p <server port>. Type in either the
//    client or server windows and your message should show up in the
//    other window.  Control C to exit.
//
// = AUTHOR
//    Tim Harrison
//    Irfan Pyarali
//
// ============================================================================

#include "ace/OS_main.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

#include "ace/Reactor.h"
#include "ace/Reactor_Notification_Strategy.h"
#include "ace/WIN32_Proactor.h"
#include "ace/Proactor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"



typedef ACE_Task<ACE_MT_SYNCH> MT_TASK;

class Peer_Handler : public MT_TASK, public ACE_Handler
// = TITLE
//     Connect to a server.  Receive messages from STDIN_Handler
//     and forward them to the server using proactive I/O.
{
public:
  // = Initialization methods.
  Peer_Handler (int argc, ACE_TCHAR *argv[]);
  ~Peer_Handler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void * =0);
  // This method creates the network connection to the remote peer.
  // It does blocking connects and accepts depending on whether a
  // hostname was specified from the command line.
  //FUZZ: enable check_for_lack_ACE_OS

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This method will be called when an asynchronous read completes on a stream.
  // The remote peer has sent us something.  If it succeeded, print
  // out the message and reinitiate a read.  Otherwise, fail.  In both
  // cases, delete the message sent.

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This method will be called when an asynchronous write completes on a strea_m.
  // One of our asynchronous writes to the remote peer has completed.
  // Make sure it succeeded and then delete the message.

  virtual ACE_HANDLE handle (void) const;
  // Get the I/O handle used by this <handler>. This method will be
  // called by the ACE_Asynch_* classes when an ACE_INVALID_HANDLE is
  // passed to <open>.

  void handle (ACE_HANDLE);
  // Set the ACE_HANDLE value for this Handler.

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // We've been removed from the Reactor.

  virtual int handle_output (ACE_HANDLE fd);
  // Called when output events should start.  Note that this is
  // automatically invoked by the
  // <ACE_Reactor_Notificiation_Strategy>.

private:
  ACE_SOCK_Stream stream_;
  // Socket that we have connected to the server.

  ACE_Reactor_Notification_Strategy strategy_;
  // The strategy object that the reactor uses to notify us when
  // something is added to the queue.

  // = Remote peer info.
  ACE_TCHAR *host_;
  // Name of remote host.

  u_short port_;
  // Port number for remote host.

  ACE_Asynch_Read_Stream rd_stream_;
  // Read stream

  ACE_Asynch_Write_Stream wr_stream_;
  // Write stream

  ACE_Message_Block mb_;
  // Message Block for reading from the network
};

class STDIN_Handler : public ACE_Task<ACE_NULL_SYNCH>
// = TITLE
//    Active Object.  Reads from STDIN and passes message blocks to
//    the peer handler.
{
public:
  STDIN_Handler (MT_TASK &ph);
  // Initialization.

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Activate object.

  virtual int close (u_long = 0);
  // Shut down.
  //FUZZ: enable check_for_lack_ACE_OS

  int svc (void);
  // Thread runs here as an active object.

  int handle_close (ACE_HANDLE,
                    ACE_Reactor_Mask);

private:
  static void handler (int signum);
  // Handle a ^C.  (Do nothing, this just illustrates how we can catch
  // signals along with the other things).

  void register_thread_exit_hook (void);
  // Helper function to register with the Reactor for thread exit.

  virtual int handle_signal (int index, siginfo_t *, ucontext_t *);
  // The STDIN thread has exited.  This means the user hit ^C.  We can
  // end the event loop.

  MT_TASK &ph_;
  // Send all input to ph_.

  ACE_HANDLE thr_handle_;
  // Handle of our thread.
};

Peer_Handler::Peer_Handler (int argc, ACE_TCHAR *argv[])
  : strategy_ (ACE_Reactor::instance (),
               this,
               ACE_Event_Handler::WRITE_MASK),
    host_ (0),
    port_ (ACE_DEFAULT_SERVER_PORT),
    mb_ (BUFSIZ)
{
  // This code sets up the message to notify us when a new message is
  // added to the queue.  Actually, the queue notifies Reactor which
  // then notifies us.
  this->msg_queue ()->notification_strategy (&this->strategy_);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("h:p:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'h':
          host_ = get_opt.opt_arg ();
          break;
        case 'p':
          port_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        }
    }
}

Peer_Handler::~Peer_Handler (void)
{
}

// This method creates the network connection to the remote peer.  It
// does blocking connects and accepts depending on whether a hostname
// was specified from the command line.

int
Peer_Handler::open (void *)
{
  if (host_ != 0) // Connector
    {
      ACE_INET_Addr addr (port_, host_);
      ACE_SOCK_Connector connector;

      // Establish connection with server.
      if (connector.connect (stream_, addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%t) connected.\n"));
    }
  else // Acceptor
    {
      ACE_SOCK_Acceptor acceptor;
      ACE_INET_Addr local_addr (port_);

      if ((acceptor.open (local_addr) == -1) ||
          (acceptor.accept (this->stream_) == -1))
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "accept failed"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%t) accepted.\n"));
    }

  int result = this->rd_stream_.open (*this);
  if (result != 0)
    return result;

  result = this->wr_stream_.open (*this);
  if (result != 0)
    return result;

  result = this->rd_stream_.read (this->mb_,
                                  this->mb_.size ());
  return result;
}

// One of our asynchronous writes to the remote peer has completed.
// Make sure it succeeded and then delete the message.

void
Peer_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  if (result.bytes_transferred () <= 0)
    ACE_DEBUG ((LM_DEBUG, "(%t) %p bytes = %d\n", "Message failed",
                result.bytes_transferred ()));

  // This was allocated by the STDIN_Handler, queued, dequeued, passed
  // to the proactor, and now passed back to us.
  result.message_block ().release ();
}

// The remote peer has sent us something.  If it succeeded, print
// out the message and reinitiate a read.  Otherwise, fail.  In both
// cases, delete the message sent.


void
Peer_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  if (result.bytes_transferred () > 0 &&
      this->mb_.length () > 0)
    {
      this->mb_.rd_ptr ()[result.bytes_transferred ()] = '\0';
      // Print out the message received from the server.
      ACE_DEBUG ((LM_DEBUG, "%s", this->mb_.rd_ptr ()));
    }
  else
    {
      // If a read failed, we will assume it's because the remote peer
      // went away.  We will end the event loop.  Since we're in the
      // main thread, we don't need to do a notify.
      ACE_Reactor::end_event_loop();
      return;
    }

  // Reset pointers
  this->mb_.wr_ptr (this->mb_.wr_ptr () - result.bytes_transferred ());

  // Start off another read
  if (this->rd_stream_.read (this->mb_,
                             this->mb_.size ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p Read initiate.\n", "Peer_Handler"));
}

// This is so the Proactor can get our handle.
ACE_HANDLE
Peer_Handler::handle (void) const
{
  return this->stream_.get_handle ();
}

void
Peer_Handler::handle (ACE_HANDLE handle)
{
  this->stream_.set_handle (handle);
}

// We've been removed from the Reactor.
int
Peer_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Peer_Handler closing down\n"));
  return 0;
}

// New stuff added to the message queue.  Try to dequeue a message.
int
Peer_Handler::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *mb = 0;

  ACE_Time_Value tv (ACE_Time_Value::zero);

  // Forward the message to the remote peer receiver.
  if (this->getq (mb, &tv) != -1)
    {
      if (this->wr_stream_.write (*mb,
                                  mb->length ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p Write initiate.\n", "Peer_Handler"), -1);
    }
  return 0;
}

void
STDIN_Handler::handler (int signum)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) signal = %S\n", signum));
}

STDIN_Handler::STDIN_Handler (MT_TASK &ph)
  : ph_ (ph)
{
  // Register for ^C from the console.  We just need to catch the
  // exception so that the kernel doesn't kill our process.
  // Registering this signal handler just tells the kernel that we
  // know what we're doing; to leave us alone.

  ACE_OS::signal (SIGINT, (ACE_SignalHandler) STDIN_Handler::handler);
};

// Activate object.

int
STDIN_Handler::open (void *)
{
  if (this->activate (THR_NEW_LWP | THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), -1);

  return 0;
}

// Shut down.

int
STDIN_Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) thread is exiting.\n"));
  return 0;
}

// Thread runs here.

int
STDIN_Handler::svc (void)
{
  this->register_thread_exit_hook ();

  for (;;)
    {
      ACE_Message_Block *mb = new ACE_Message_Block (BUFSIZ);

      // Read from stdin into mb.
      int read_result = ACE_OS::read (ACE_STDIN,
                                      mb->rd_ptr (),
                                      mb->size ());

      // If read succeeds, put mb to peer handler, else end the loop.
      if (read_result > 0)
        {
          mb->wr_ptr (read_result);
          // Note that this call will first enqueue mb onto the peer
          // handler's message queue, which will then turn around and
          // notify the Reactor via the Notification_Strategy.  This
          // will subsequently signal the Peer_Handler, which will
          // react by calling back to its handle_output() method,
          // which dequeues the message and sends it to the peer
          // across the network.
          this->ph_.putq (mb);
        }
      else
        {
          mb->release ();
          break;
        }
    }

  // handle_signal will get called on the main proactor thread since
  // we just exited and the main thread is waiting on our thread exit.
  return 0;
}

// Register an exit hook with the reactor.

void
STDIN_Handler::register_thread_exit_hook (void)
{
  // Get a real handle to our thread.
  ACE_Thread_Manager::instance ()->thr_self (this->thr_handle_);

  // Register ourselves to get called back when our thread exits.

  if (ACE_Reactor::instance ()->
      register_handler (this, this->thr_handle_) == -1)
    ACE_ERROR ((LM_ERROR, "Exit_Hook Register failed.\n"));
}

// The STDIN thread has exited.  This means the user hit ^C.  We can
// end the event loop and delete ourself.

int
STDIN_Handler::handle_signal (int, siginfo_t *si, ucontext_t *)
{
  if (si != 0)
    {
      ACE_ASSERT (this->thr_handle_ == si->si_handle_);
      ACE_Reactor::end_event_loop ();
    }
  return 0;
}

int
STDIN_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  delete this;
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Let the proactor know that it will be used with Reactor
  // Create specific proactor
  ACE_WIN32_Proactor win32_proactor (0, 1);
  // Get the interface proactor
  ACE_Proactor proactor (&win32_proactor);
  // Put it as the instance.
  ACE_Proactor::instance (&proactor);

  // Open handler for remote peer communications this will run from
  // the main thread.
  Peer_Handler peer_handler (argc, argv);

  if (peer_handler.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p open failed, errno = %d.\n",
                       "peer_handler", errno), 0);

  // Open active object for reading from stdin.
  STDIN_Handler *stdin_handler =
    new STDIN_Handler (peer_handler);

  // Spawn thread.
  if (stdin_handler->open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p open failed, errno = %d.\n",
                       "stdin_handler", errno), 0);

  // Register proactor with Reactor so that we can demultiplex
  // "waitable" events and I/O operations from a single thread.
  if (ACE_Reactor::instance ()->register_handler
      (ACE_Proactor::instance ()->implementation ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p failed to register Proactor.\n",
                       argv[0]), -1);

  // Run main event demultiplexor.
  ACE_Reactor::run_event_loop ();

  // Remove proactor with Reactor.
  if (ACE_Reactor::instance ()->remove_handler
      (ACE_Proactor::instance ()->implementation (), ACE_Event_Handler::DONT_CALL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p failed to register Proactor.\n",
                       argv[0]), -1);

  return 0;
}
#else /* !ACE_HAS_WIN32_OVERLAPPED_IO */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_HAS_WIN32_OVERLAPPED_IO */
