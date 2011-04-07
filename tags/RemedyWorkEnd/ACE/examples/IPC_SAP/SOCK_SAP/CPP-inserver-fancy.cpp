 // $Id$

// This example tests the features of the <ACE_SOCK_Acceptor>,
// <ACE_SOCK_Stream>, and <ACE_Svc_Handler> classes.  If the platform
// supports threads it uses a thread-per-connection concurrency model.
// Otherwise, it uses a single-threaded iterative server model.

#include "ace/OS_main.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Singleton.h"
#include "ace/Profile_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_select.h"

#include "CPP-inserver-fancy.h"



// Forward declaration.
class Handler;

class Handler_Factory
{
  // = TITLE
  //   Creates the oneway or twoway handlers.
public:
  Handler_Factory (void);
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

  ACE_SOCK_Acceptor twoway_acceptor_;
  // Twoway acceptor factory.

  ACE_SOCK_Acceptor oneway_acceptor_;
  // Oneway acceptor factory.
};

class Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
  // = TITLE
  //   Base class for the oneway and twoway handlers.

  friend class Handler_Factory;
  // The factory has special permission. (to access svc ()).

public:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Generic initialization method.

  virtual int close (u_long);
  // Close down and delete this.
  //FUZZ: enable check_for_lack_ACE_OS

protected:
  Handler (ACE_HANDLE handle);
  // Constructor.

  int parse_header_and_allocate_buffer (char *&buf,
                                        ACE_INT32 *len);
  // Implement the generic code that's called from any of the subclass
  // <run> methods to get the header and the buffer to read the data.
  // This method factors out common code.

  virtual int run (void) = 0;
  // Hook method called by the <svc> template method to do the actual
  // protocol.  Must be overridden by the subclass.

  virtual int svc (void);
  // Template method entry point into the handler task.

  virtual void print_results (void);
  // Print the results.

  size_t total_bytes_;
  // Total number of bytes received.

  size_t message_count_;
  // Number of messages received.

  ACE_Profile_Timer timer_;
  // Keeps track of how much time we're using.
};

class Twoway_Handler : public Handler
{
  // = TITLE
  //   Performs the twoway protocol.
public:
  Twoway_Handler (ACE_HANDLE handle);
  // Constructor.

private:
  virtual int run (void);
  // Template Method hook called by <svc>.
};

class Oneway_Handler : public Handler
{
  // = TITLE
public:
  Oneway_Handler (ACE_HANDLE handle);
  // Constructor.

private:
  virtual int run (void);
  // Template Method hook called by <svc>.

  virtual void print_results (void);
  // Print the results.
};

u_short
Options::port (void) const
{
  return this->port_;
}

int
Options::verbose (void) const
{
  return this->verbose_;
}

int
Options::reply_message_len (void) const
{
  return this->reply_message_len_;
}

Options::~Options (void)
{
}

Options::Options (void)
  : verbose_ (0),
    port_ (ACE_DEFAULT_SERVER_PORT),
    reply_message_len_ (24) // Default to the approximate size of an
                            // GIOP reply message.
{
}

int
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("p:r:v"), 1);

  for (int c; (c = getopt ()) != -1; )
  //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
      {
      case 'p':
        this->port_ = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'r':
        this->reply_message_len_ = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'v':
        this->verbose_ = 1;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) usage: %n [-p <port>] [-v]"),
                          -1);
      }

  return 0;
}

// Options Singleton.
typedef ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> OPTIONS;

Handler::Handler (ACE_HANDLE handle)
  : total_bytes_ (0),
    message_count_ (0)
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
  return 0;
}

int
Handler::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) closing down %x\n",
              this));
  delete this;
  return 0;
}

int
Handler::svc (void)
{
  // Timer logic.
  this->timer_.start ();

  // Invoke the hook method to run the specific test.
  int result = this->run ();

  this->timer_.stop ();

  this->print_results ();

  return result;
}

int
Handler::parse_header_and_allocate_buffer (char *&request,
                                           ACE_INT32 *len)
{
  ssize_t result = this->peer ().recv_n ((void *) len,
                                         sizeof (ACE_INT32));
  if (result == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) connected closed\n"));
      return -1;
    }
  else if (result == -1 || result != sizeof (ACE_INT32))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "recv_n failed"),
                      -1);
  else
    {
      *len = ntohl (*len);
      ACE_NEW_RETURN (request,
                      char[*len],
                      -1);
    }

  return 0;
}

void
Handler::print_results (void)
{
}

Twoway_Handler::Twoway_Handler (ACE_HANDLE handle)
  : Handler (handle)
{
}

// Function entry point into the twoway server task.

int
Twoway_Handler::run (void)
{
  // Read data from client (terminate on error).

  char *request = 0;

  for (;;)
    {
      ACE_INT32 len = 0;

      if (parse_header_and_allocate_buffer (request,
                                            &len) == -1)
        return -1;

      // Subtract off the sizeof the length prefix.
      ssize_t r_bytes = this->peer ().recv_n (request,
                                              len - sizeof (ACE_UINT32));

      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (OPTIONS::instance ()->verbose ()
               && ACE::write_n (ACE_STDOUT,
                                request,
                                r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));
      else
        {
          ssize_t s_bytes = (ssize_t) OPTIONS::instance ()->reply_message_len ();

          // Don't try to send more than is in the request buffer!
          if (s_bytes > r_bytes)
            s_bytes = r_bytes;

          if (this->peer ().send_n (request,
                                    s_bytes) != s_bytes)
            ACE_ERROR ((LM_ERROR,
                        "%p\n",
                        "send_n"));
        }
      this->total_bytes_ += size_t (r_bytes);
      this->message_count_++;

      delete [] request;
      request = 0;
    }

  delete [] request;
  return 0;
}

Oneway_Handler::Oneway_Handler (ACE_HANDLE handle)
  : Handler (handle)
{
}

void
Oneway_Handler::print_results (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  this->timer_.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\t\treal time = %f secs \n\t\tuser time = %f secs \n\t\tsystem time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\t\tmessages = %d\n\t\ttotal bytes = %d\n\t\tmbits/sec = %f\n\t\tusec-per-message = %f\n"),
              this->message_count_,
              this->total_bytes_,
              (((double) this->total_bytes_ * 8) / et.real_time) / (double) (1024 * 1024),
              ((et.user_time + et.system_time) / (double) this->message_count_) * ACE_ONE_SECOND_IN_USECS));
}

// Function entry point into the oneway server task.

int
Oneway_Handler::run (void)
{
  // Read data from client (terminate on error).

  char *request = 0;

  for (;;)
    {
      ACE_INT32 len = 0;

      if (parse_header_and_allocate_buffer (request,
                                            &len) == -1)
        return -1;

      // Subtract off the sizeof the length prefix.
      ssize_t r_bytes = this->peer ().recv_n (request,
                                              len - sizeof (ACE_UINT32));

      if (r_bytes == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "%p\n",
                      "recv"));
          break;
        }
      else if (r_bytes == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) reached end of input, connection closed by client\n"));
          break;
        }
      else if (OPTIONS::instance ()->verbose ()
               && ACE::write_n (ACE_STDOUT,
                                request,
                                r_bytes) != r_bytes)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "ACE::write_n"));

      this->total_bytes_ += size_t (r_bytes);
      this->message_count_++;
      delete [] request;
      request = 0;
    }

  delete [] request;
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
  ACE_INET_Addr twoway_server_addr (OPTIONS::instance ()->port ());
  ACE_INET_Addr oneway_server_addr (OPTIONS::instance ()->port () + 1);

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
  handler->close (0);
  return 0;
#endif /* ACE_HAS_THREADS */
}

Handler_Factory::Handler_Factory (void)
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
  FD_SET ((ACE_SOCKET) this->twoway_acceptor_.get_handle (),
          &handles);
  FD_SET ((ACE_SOCKET) this->oneway_acceptor_.get_handle (),
          &handles);

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
                    "(%P|%t) %p\n",
                    "select"));
      else if (result == 0 && OPTIONS::instance ()->verbose ())
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) select timed out\n"));
      else
        {
          if (FD_ISSET ((ACE_SOCKET) this->twoway_acceptor_.get_handle (),
                        &temp))
            this->create_handler (this->twoway_acceptor_,
                                  &Handler_Factory::make_twoway_handler,
                                  "twoway");
          if (FD_ISSET ((ACE_SOCKET) this->oneway_acceptor_.get_handle (),
                        &temp))
            this->create_handler (this->oneway_acceptor_,
                                  &Handler_Factory::make_oneway_handler,
                                  "oneway");
        }
    }

  ACE_NOTREACHED (return 0;)
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  OPTIONS::instance ()->parse_args (argc, argv);

  Handler_Factory server;

  return server.handle_events ();
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX> *
  ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
