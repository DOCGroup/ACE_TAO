// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_reactorEx.cpp
//
// = DESCRIPTION
//    This test application tests a wide range of events that can be
//    demultiplexed using various ACE utilities.  Events used include ^C
//    events, reading from STDIN, vanilla Win32 events, thread exits,
//    ReactorEx notifications, proactive reads, and proactive writes.
//
//    The proactive I/O events are demultiplexed by the ACE_Proactor.
//    The thread exits, notications, and vanilla Win32 events are
//    demultiplexed by the ACE_ReactorEx.  To enable a single thread
//    to run all these events, the Proactor is integrated with the
//    ReactorEx.
//
// = AUTHOR
//    Tim Harrison
// 
// ============================================================================

#include "ace/ReactorEx.h"
#include "ace/Proactor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Task.h"

typedef ACE_Task<ACE_MT_SYNCH> MT_TASK;

class Peer_Handler : public MT_TASK, public ACE_Handler
  // = TITLE
  //     Connect to a server.  Receive messages from STDIN_Handler 
  //     and forward them to the server using proactive I/O.
{
public:
  // = Initialization methods.
  Peer_Handler (int argc, char *argv[]);
  ~Peer_Handler (void);

  int open (void * =0);
  // This method creates the network connection to the remote peer.
  // It does blocking connects and accepts depending on whether a
  // hostname was specified from the command line.

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

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
  // We've been removed from the ReactorEx.

  virtual int handle_output (ACE_HANDLE fd);
  // Called when output events should start.  Note that this is
  // automatically invoked by the
  // <ACE_ReactorEx_Notificiation_Strategy>.

private:
  ACE_SOCK_Stream stream_;
  // Socket that we have connected to the server.

  ACE_ReactorEx_Notification_Strategy strategy_;
  // The strategy object that the reactorEx uses to notify us when
  // something is added to the queue.

  // = Remote peer info.
  char *host_;
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

  virtual int open (void * = 0);
  // Activate object.

  virtual int close (u_long = 0);
  // Shut down.

  int svc (void);
  // Thread runs here as an active object.

private:
  static void handler (int signum);
  // Handle a ^C.  (Do nothing, this just illustrates how we can catch
  // signals along with the other things).

  void register_thread_exit_hook (void);
  // Helper function to register with the ReactorEx for thread exit.

  virtual int handle_signal (int index, siginfo_t *, ucontext_t *);
  // The STDIN thread has exited.  This means the user hit ^C.  We can
  // end the event loop.

  MT_TASK &ph_;
  // Send all input to ph_.

  ACE_HANDLE thr_handle_;
  // Handle of our thread.
};

Peer_Handler::Peer_Handler (int argc, char *argv[])
  : host_ (0),
    port_ (ACE_DEFAULT_SERVER_PORT),
    strategy_ (ACE_ReactorEx::instance (), 
	       this, 
	       ACE_Event_Handler::WRITE_MASK),
    mb_ (BUFSIZ)
{
  // This code sets up the message to notify us when a new message is
  // added to the queue.  Actually, the queue notifies ReactorEx which
  // then notifies us.
  this->msg_queue ()->notification_strategy (&this->strategy_);

  ACE_Get_Opt get_opt (argc, argv, "h:p:");
  int c;

  while ((c = get_opt ()) != EOF)
    {
    switch (c)
      {
      case 'h':
	host_ = get_opt.optarg;
	break;
      case 'p':
	port_ = ACE_OS::atoi (get_opt.optarg);
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
      ACE_ReactorEx::end_event_loop();
      return;
    }

  // Reset pointers 
  this->mb_.wr_ptr (-result.bytes_transferred ());

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

// We've been removed from the ReactorEx.
int 
Peer_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Peer_Handler closing down\n"));
  return 0;
}

// New stuff added to the message queue.  Try to dequeue a message.
int 
Peer_Handler::handle_output (ACE_HANDLE fd)
{
  ACE_Message_Block *mb;

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

  ACE_OS::signal (SIGINT, ACE_SignalHandler (STDIN_Handler::handler));
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
	  // notify the ReactorEx via the Notification_Strategy.  This
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

// Register an exit hook with the reactorEx.  

void
STDIN_Handler::register_thread_exit_hook (void)
{
  // Get a real handle to our thread.
  ACE_Thread_Manager::instance ()->thr_self (this->thr_handle_);

  // Register ourselves to get called back when our thread exits.

  if (ACE_ReactorEx::instance ()->
      register_handler (this, this->thr_handle_) == -1)
    ACE_ERROR ((LM_ERROR, "Exit_Hook Register failed.\n"));
}

// The STDIN thread has exited.  This means the user hit ^C.  We can
// end the event loop and delete ourself.

int 
STDIN_Handler::handle_signal (int, siginfo_t *si, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) STDIN thread has exited.\n"));
  ACE_ASSERT (this->thr_handle_ == si->si_handle_);
  ACE_ReactorEx::end_event_loop();
  return 0;
}

int
main (int argc, char *argv[])
{
  // Let the proactor know that it will be used with ReactorEx
  ACE_Proactor proactor (0, 0, 1);
  ACE_Proactor::instance (&proactor);

  // Open handler for remote peer communications this will run from
  // the main thread.
  Peer_Handler peer_handler (argc, argv);

  if (peer_handler.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "%p open failed, errno = %d.\n",
		       "peer_handler", errno), 0);

  // Open active object for reading from stdin.
  STDIN_Handler stdin_handler (peer_handler);

  // Spawn thread.
  if (stdin_handler.open () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "%p open failed, errno = %d.\n",
		       "stdin_handler", errno), 0);

  // Register proactor with ReactorEx so that we can demultiplex
  // "waitable" events and I/O operations from a single thread.
  if (ACE_ReactorEx::instance ()->register_handler 
      (ACE_Proactor::instance ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p failed to register Proactor.\n",
		       argv[0]), -1);

  // Run main event demultiplexor.
  ACE_ReactorEx::run_event_loop ();

  return 0;
}
