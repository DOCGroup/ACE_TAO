// $Id$

// This example tests the features of the <ACE_SOCK_Acceptor>,
// <ACE_SOCK_Stream>, and <ACE_Svc_Handler> classes.  If the platform
// supports threads it uses a thread-per-connection concurrency model;
// otherwise it uses a single-threaded iterative server model.

#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Profile_Timer.h"

// Forward declaration.
class Handler;

class Handler_Factory
{
  // = TITLE
  //   Creates oneway or twoway handlers.
public:
  Handler_Factory (u_short port);
  // Constructor.

  ~Handler_Factory (void);
  // Destructor.

  int handle_events (void);
  // Run the main event loop.

private:
  int init_acceptors (void);
  // Initialize the acceptors.

  int create_handler (ACE_SOCK_Acceptor &acceptor,
                      Handler *(*handler_factory) (ACE_HANDLE),
                      const char *handler_type);
  // Factory that creates the right kind of <Handler>.

  // = Factory functions.
  static Handler *make_twoway_handler (ACE_HANDLE);
  // Create a twoway handler.

  static Handler *make_oneway_handler (ACE_HANDLE);
  // Create a oneway handler.

  u_short port_;
  // Port number we're listening on.

  ACE_SOCK_Acceptor twoway_acceptor_;
  // Twoway acceptor factory.

  ACE_SOCK_Acceptor oneway_acceptor_;
  // Oneway acceptor factory.
};

class Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //   Base class for the oneway and twoway handlers.
public:
  virtual int open (void * = 0);
  // Generic initialization method.

protected:
  Handler (ACE_HANDLE handle, int verbose = 0);
  // Constructor.

  virtual int run (void) = 0;
  // Hook method called by the <svc> template method to do the actual
  // protocol.  Must be overridden by the subclass.

  virtual int svc (void);
  // Template method entry point into the handler task.

  void print_results (void);
  // Print the results.

  int verbose_;
  // Are we running verbosely?

  size_t total_bytes_;
  // Total number of bytes received.

  size_t message_count_;
  // Number of messages received.

  ACE_INT32 len_;
  // Max length of buffers sent by the user.

  void *buf_;
  // Pointer to the buffer that we read into.

  ACE_Profile_Timer timer_;
  // Keeps track of how much time we're using.
};

class Twoway_Handler : public Handler
{
  // = TITLE
  //   Performs the twoway protocol.
public:
  Twoway_Handler (ACE_HANDLE handle, int verbose = 0);
  // Constructor.

  virtual int run (void);
  // Template Method hook called by <svc>.
};

class Oneway_Handler : public Handler
{
  // = TITLE
public:
  Oneway_Handler (ACE_HANDLE handle, int verbose = 0);
  // Constructor.

  virtual int run (void);
  // Template Method hook called by <svc>.
};

Handler::Handler (ACE_HANDLE handle,
                  int verbose)
  : verbose_ (verbose),
    total_bytes_ (0),
    message_count_ (0),
    len_ (0),
    buf_ (0)
{
  this->peer ().set_handle (handle);
}

int
Handler::open (void *)
{
  ACE_INET_Addr cli_addr;

  // Make sure we're not in non-blocking mode.
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "disable"),
                       0);
                       
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) client %s connected from %d on handle %d\n",
              cli_addr.get_host_name (),
              cli_addr.get_port_number (),
              this->peer ().get_handle ()));

  if (this->peer ().recv_n ((void *) &this->len_,
                            sizeof (ACE_INT32)) != sizeof (ACE_INT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      -1);
  else
    {
      this->len_ = ntohl (this->len_);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) reading messages of size %d\n",
                  this->len_));
      ACE_ALLOCATOR_RETURN (this->buf_,
                            ACE_OS::malloc (this->len_),
                            -1);
    }
  return 0;
}

void
Handler::print_results (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  this->timer_.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n"),
	      et.real_time,
	      et.user_time,
	      et.system_time));

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\t\t messages = %d\n\t\t total bytes = %d\n\t\tmbits/sec = %f\n\t\tusec-per-message = %f\n"),
	      this->message_count_,
              this->total_bytes_,
	      (((double) this->total_bytes_ * 8) / et.real_time) / (double) (1024 * 1024),
	      ((et.user_time + et.system_time) / (double) this->message_count_) * ACE_ONE_SECOND_IN_USECS));
}

int
Handler::svc (void)
{
  // Timer logic.
  this->timer_.start ();

  if (this->run () == -1)
    return -1;

  this->timer_.stop ();

  this->print_results ();
  ACE_OS::free (this->buf_);
  return 0;
}

Twoway_Handler::Twoway_Handler (ACE_HANDLE handle,
                                int verbose)
  : Handler (handle, verbose)
{
}

// Function entry point into the twoway server task.

int
Twoway_Handler::run (void)
{
  // Read data from client (terminate on error).

  for (;;)
    {
      ssize_t r_bytes = this->peer ().recv (this->buf_, this->len_);
      
      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (this->verbose_ 
               && ACE::write_n (ACE_STDOUT, this->buf_, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));
      else if (this->peer ().send_n (this->buf_, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "send_n"));

      this->total_bytes_ += size_t (r_bytes);
      this->message_count_++;
    }
  return 0;
}

Oneway_Handler::Oneway_Handler (ACE_HANDLE handle,
                                int verbose)
  : Handler (handle, verbose)
{
}

// Function entry point into the oneway server task.

int
Oneway_Handler::run (void)
{
  // Read data from client (terminate on error).

  for (;;)
    {
      ssize_t r_bytes = this->peer ().recv (this->buf_, this->len_);
      
      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (this->verbose_ 
               && ACE::write_n (ACE_STDOUT, this->buf_, r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));

      this->total_bytes_ += size_t (r_bytes);
      this->message_count_++;
    }
  return 0;
}

// Create a twoway handler.

Handler *
Handler_Factory::make_twoway_handler (ACE_HANDLE handle)
{
  return new Twoway_Handler (handle);
}

// Create a oneway handler.

Handler *
Handler_Factory::make_oneway_handler (ACE_HANDLE handle)
{
  return new Oneway_Handler (handle);
}

int
Handler_Factory::init_acceptors (void)
{
  // Create the oneway and twoway server addresses.
  ACE_INET_Addr twoway_server_addr (this->port_);
  ACE_INET_Addr oneway_server_addr (this->port_ + 1);

  // Create acceptors, reuse the address.
  if (this->twoway_acceptor_.open (twoway_server_addr, 1) == -1
      || this->oneway_acceptor_.open (oneway_server_addr, 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);
  else if (this->twoway_acceptor_.get_local_addr (twoway_server_addr) == -1
           || this->oneway_acceptor_.get_local_addr (oneway_server_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "get_local_addr"),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting twoway server at port %d and oneway server at port %d\n",
              twoway_server_addr.get_port_number (),
              oneway_server_addr.get_port_number ()));
  return 0;
}

int
Handler_Factory::create_handler (ACE_SOCK_Acceptor &acceptor,
                                 Handler * (*handler_factory) (ACE_HANDLE),
                                 const char *handler_type)
{
  ACE_SOCK_Stream new_stream;

  if (acceptor.accept (new_stream) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "accept"),
                       -1);

  Handler *handler;
  
  ACE_ALLOCATOR_RETURN (handler,
                        (*handler_factory) (new_stream.get_handle ()),
                        -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) spawning %s handler\n",
              handler_type));

  if (handler->open () == -1)
    return -1;

#if defined (ACE_MT_SAFE)
  // Spawn a new thread and run the new connection in that thread of
  // control using the <server> function as the entry point.
  return handler->activate ();
#else
  handler->svc ();
  handler->close ();
#endif /* ACE_HAS_THREADS */
}

Handler_Factory::Handler_Factory (u_short port)
  : port_ (port)

{
}

Handler_Factory::~Handler_Factory (void)
{
  this->twoway_acceptor_.close ();
  this->oneway_acceptor_.close ();
}

// Run the main event loop.

int
Handler_Factory::handle_events (void)
{
  if (this->init_acceptors () == -1)
    return -1;

  fd_set handles;
  
  FD_ZERO (&handles);
  FD_SET (this->twoway_acceptor_.get_handle (), &handles);
  FD_SET (this->oneway_acceptor_.get_handle (), &handles);

  // Performs the iterative server activities.

  for (;;)
    {
      ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
      fd_set temp = handles;

      int result = ACE_OS::select (int (this->oneway_acceptor_.get_handle ()) + 1,
                                   (fd_set *) &temp,
                                   0,
                                   0,
                                   timeout);
      if (result == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n", "select"));
      else if (result == 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) select timed out\n"));
      else 
        {
          if (FD_ISSET (this->twoway_acceptor_.get_handle (), &temp))
            this->create_handler (this->twoway_acceptor_,
                                  &Handler_Factory::make_twoway_handler,
                                  "twoway");
          if (FD_ISSET (this->oneway_acceptor_.get_handle (), &temp))
            this->create_handler (this->oneway_acceptor_,
                                  &Handler_Factory::make_oneway_handler,
                                  "oneway");
        }
    }

  /* NOTREACHED */
  return 0;
}

int
main (int argc, char *argv[])
{
  u_short port = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_SERVER_PORT;

  Handler_Factory server (port);

  return server.handle_events ();
}
