// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests ACE_SOCK_Dgram_Mcast class.
//    It specifically tests subscribing to multiple groups on the same socket
//    on one or more physical interfaces (if available).
//
//    The test can be run as a producer, consumer, or both producer/consumer 
//    (default).  The test requires at least two (2) multicast groups which can
//    be configured as command line options.  The consumer subscribes to a 
//    single group per instance and an additional instance tries to subscribe
//    to all groups on a single socket (if the ACE_SOCK_Dgram_Mcast instance
//    bind()'s the first address to the socket, additional joins will fail).  
//    The producer iterates through the list of group addresses and sends a 
//    single message containing the destination address and port to each one. 
//    It also sends messages to five (5) additional groups and a message to an
//    additional port for each group in order to produce a bit of "noise" in
//    order to help validate how well the multicast filtering works on a 
//    particular platform. 
//
//    The list of destination groups start at 239.255.0.1 (default) and 
//    increment by 1 up to 5 (default) groups.  Both of these values can be
//    overridden via command-line options.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#include "tests/test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/ACE.h"
#include "ace/Reactor.h"
#include "ace/OS_String.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"

ACE_RCSID(tests, Multicast_Test, "$Id$")

#if defined (ACE_HAS_IP_MULTICAST)

/* 
 *  The 'finished' flag is used to break out of an infninite loop in the 
 *  task::svc () method.  The 'handler' will set the flag in respose to 
 *  SIGINT (CTRL-C).
 */
static sig_atomic_t finished = 0;
extern "C" void handler (int)
{
  finished = 1;
}

static const int MCT_ITERATIONS = 100;
static const int MCT_GROUPS = 5;
static const int MCT_MIN_GROUPS = 2;

static const char MCT_START_GROUP[] = "239.255.0.1";
static const int  MCT_START_PORT = 16000;

static const size_t MAX_STRING_SIZE = 200;

int advance_addr (ACE_INET_Addr &addr);

/*
 * MCast_Config holds configuration data for this test.
 */ 
class MCT_Config
{
public:

  enum 
    {
      PRODUCER = 1,
      CONSUMER = 2,
      BOTH = PRODUCER | CONSUMER
    };

  MCT_Config (void) 
    : group_start_ (MCT_START_PORT, MCT_START_GROUP),
      groups_ (0),
      debug_ (0),
      type_ (BOTH),
      sdm_opts_ (ACE_SOCK_Dgram_Mcast::DEFOPTS),
      iterations_ (MCT_ITERATIONS)
    {
      if (IP_MAX_MEMBERSHIPS == 0)
        this->groups_ = MCT_GROUPS;
      else
        this->groups_ = ACE_MIN (IP_MAX_MEMBERSHIPS, MCT_GROUPS);
    }
  ~MCT_Config (void) 
    {}

  int open (int argc, ACE_TCHAR *argv[]);
  int debug (void) const { return this->debug_;}
  void dump (void) const;
  int groups (void) const { return this->groups_;}
  const ACE_INET_Addr group_start (void) const { return this->group_start_;}
  u_long type (void) const { return this->type_;}
  int iterations (void) const { return this->iterations_;}
  ACE_SOCK_Dgram_Mcast::options options (void) const 
  { 
    return ACE_static_cast (ACE_SOCK_Dgram_Mcast::options, this->sdm_opts_);
  }

private:
  // Starting group address. (only IPv4 capable right now...)
  ACE_INET_Addr group_start_;

  // Number of groups we will try to use in the test.  
  int groups_;

  // Debug flag.
  int debug_;

  // Type, i.e., PRODUCER, CONSUMER, BOTH: defaults to BOTH
  u_long type_;

  // ACE_SOCK_Dgram_Mcast ctor options
  u_long sdm_opts_;

  // Producer iterations
  int iterations_;
};

int
MCT_Config::open (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  int help = 0;

  ACE_Get_Opt getopt (argc, argv, ACE_TEXT (":?"), 1, 1);

  if (getopt.long_option (ACE_TEXT ("GroupStart"),
                          'g',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add MCastGroupStart option.\n")),
                      1);

  if (getopt.long_option (ACE_TEXT ("Groups"),
                          'n',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add Groups option.\n")), 1);

  if (getopt.long_option (ACE_TEXT ("Debug"),
                          'd',
                          ACE_Get_Opt::NO_ARG) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add Debug option.\n")), 1);

  if (getopt.long_option (ACE_TEXT ("Type"),
                          't',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add Type option.\n")), 1);

  if (getopt.long_option (ACE_TEXT ("SDM_options"),
                          'm',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add Multicast_Options option.\n")),
                      1);

  if (getopt.long_option (ACE_TEXT ("Iterations"),
                          'i',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add iterations option.\n")), 
                      1);

  if (getopt.long_option (ACE_TEXT ("help"),
                          'h',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT (" Unable to add help option.\n")), 
                      1);

  // Now, let's parse it...
  int c = 0;
  while ((c = getopt ()) != EOF)
    {
      switch (c)
        {
        case 0:
          // Long Option. This should never happen.
          retval = -1;
           break;
        case 'g':
          {
            // @todo validate all these, i.e., must be within range 
            // 224.255.0.0 to 238.255.255.255, but we only allow the 
            // administrative "site local" range, 239.255.0.0 to 
            // 239.255.255.255.
            char *group = getopt.opt_arg ();
            if (this->group_start_.set (group) != 0)
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("Bad group address:%s\n"), 
                            group));
              }
          }
          break;
        case 'i':
          {
            this->iterations_ = ACE_OS::atoi (getopt.opt_arg ());
            break;
          }
        case 'n':
          {
            int n = ACE_OS::atoi (getopt.opt_arg ());
            // I'm assuming 0 means unlimited, so just use whatever the
            // user provides.  Seems to work okay on Solaris 5.8.
            if (IP_MAX_MEMBERSHIPS == 0)
              this->groups_ = n;
            else
              this->groups_ = ACE_MIN (ACE_MAX (n, MCT_MIN_GROUPS), 
                                       IP_MAX_MEMBERSHIPS);
            break;
          }
        case 'd':
          this->debug_ = 1;
          break;
        case 't':
          {
            ACE_TCHAR *c = getopt.opt_arg ();
            if (ACE_OS::strcasecmp (c, ACE_TEXT ("CONSUMER")) == 0)
              this->type_ = CONSUMER;
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("PRODUCER")) == 0)
              this->type_ = PRODUCER;
            else
              {
                help = 1;
                retval = -1;
              }
          }
          break;
        case 'm':
          {
            ACE_TCHAR *c = getopt.opt_arg ();
            if (ACE_OS::strcasecmp (c, ACE_TEXT ("OPT_BINDADDR_YES")) == 0)
              ACE_SET_BITS (this->sdm_opts_, 
                            ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES);
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("~OPT_BINDADDR_YES")) == 0)
              ACE_CLR_BITS (this->sdm_opts_, 
                            ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES);
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("DEFOPT_BINDADDR")) == 0)
              {
                ACE_CLR_BITS (this->sdm_opts_, 
                              ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES);
                ACE_SET_BITS (this->sdm_opts_,
                              ACE_SOCK_Dgram_Mcast::DEFOPT_BINDADDR);
              }
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("OPT_NULLIFACE_ALL")) == 0)
              ACE_SET_BITS (this->sdm_opts_,
                            ACE_SOCK_Dgram_Mcast::OPT_NULLIFACE_ALL);
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("~OPT_NULLIFACE_ALL")) == 0)
              ACE_CLR_BITS (this->sdm_opts_,
                            ACE_SOCK_Dgram_Mcast::OPT_NULLIFACE_ALL);
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("DEFOPT_NULLIFACE")) == 0)
              {
                ACE_CLR_BITS (this->sdm_opts_, 
                              ACE_SOCK_Dgram_Mcast::OPT_NULLIFACE_ALL);
                ACE_SET_BITS (this->sdm_opts_,
                              ACE_SOCK_Dgram_Mcast::DEFOPT_NULLIFACE);
              }
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("DEFOPTS")) == 0)
              this->sdm_opts_ = ACE_SOCK_Dgram_Mcast::DEFOPTS;
            else
              {
                help = 1;
                retval = -1;
              }
          }
          break;
        case ':':
          // This means an option requiring an argument didn't have one.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT (" Option '%c' requires an argument but ")
                      ACE_TEXT ("none was supplied\n"),
                      getopt.opt_opt ()));
          help = 1;
          retval = -1;
          break;
        case '?':
        case 'h':
        default:
          if (ACE_OS::strcmp (argv[getopt.opt_ind () - 1], ACE_TEXT ("-?")) != 0
              && ACE_OS::strcmp (argv[getopt.opt_ind () - 1], 
                                 ACE_TEXT ("-h")) != 0)
            // Don't allow unknown options.
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT (" Found an unknown option (%c) ")
                        ACE_TEXT ("we couldn't handle.\n"),
                        getopt.opt_opt ()));
                        // getopt.last_option ())); //readd with "%s" when 
                        // last_option() is available.
          help = 1;
          retval = -1;
          break;
        }
    }

  if (retval == -1)
    {
      if (help)
        // print usage here
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("usage: %s [options]\n")
                    ACE_TEXT ("Options:\n")
                    ACE_TEXT ("  -g {STRING}  --GroupStart={STRING}  ")
                    ACE_TEXT ("starting multicast group address ")
                    ACE_TEXT ("(default=239.255.0.1)\n")
                    ACE_TEXT ("  -n {#}       --Groups={#}           ")
                    ACE_TEXT ("number of groups (default=5)\n")
                    ACE_TEXT ("  -d           --debug                ")
                    ACE_TEXT ("debug flag (default=off)\n")
                    ACE_TEXT ("  -t {STRING}  --Type={STRING}        ")
                    ACE_TEXT ("type {PRODUCER|CONSUMER|BOTH} (default=BOTH)\n")
                    ACE_TEXT ("  -m {STRING}  --SDM_options={STRING} ")
                    ACE_TEXT ("ACE_SOCK_Dgram_Mcast ctor options ")
                    ACE_TEXT ("(default=DEFOPTS)\n")
                    ACE_TEXT ("  -i {#}       --Iterations={#}       ")
                    ACE_TEXT ("number of iterations (default=100)\n")
                    ACE_TEXT ("  -h/?         --help                 ")
                    ACE_TEXT ("show this message\n"),
                    argv[0]));
        
      return -1;
    }

  return 0;
}

void
MCT_Config::dump (void) const
{
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" Dumping MCast_Config\n")));
  ACE_DEBUG ((LM_DEBUG, 
              ACE_TEXT ("\tIP_MAX_MEMBERSHIPS = %d\n"),
              IP_MAX_MEMBERSHIPS));
  ACE_DEBUG ((LM_DEBUG, 
              ACE_TEXT ("\tgroups_ = %d\n"),
              this->groups_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\ttype_ = %s\n"),
              (ACE_BIT_ENABLED (this->type_, PRODUCER)
               && ACE_BIT_ENABLED (this->type_, CONSUMER)) 
                 ? ACE_TEXT ("PRODUCER/CONSUMER")
                 : ACE_BIT_ENABLED (this->type_, PRODUCER) 
                     ? ACE_TEXT ("PRODUCER")
                     : ACE_TEXT ("CONSUMER")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tsdm_options_ = %d\n"),
              this->sdm_opts_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\titerations_ = %d\n"),
              this->iterations_));
  // Note that this call to get_host_addr is the non-reentrant
  // version, but it's okay for us.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tgroups_start_ = %s\n"),
              this->group_start_.get_host_addr ()));

  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

/******************************************************************************/

class MCT_Event_Handler : public ACE_Event_Handler
{
public:
  MCT_Event_Handler (ACE_SOCK_Dgram_Mcast::options options 
                      = ACE_SOCK_Dgram_Mcast::DEFOPTS);
  virtual ~MCT_Event_Handler (void);

  int join (const ACE_INET_Addr &mcast_addr,
            int reuse_addr = 1,
            const ACE_TCHAR *net_if = 0);
  int leave (const ACE_INET_Addr &mcast_addr,
             const ACE_TCHAR *net_if = 0);

  // = Event Handler hooks.
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask close_mask);

  virtual ACE_HANDLE get_handle (void) const;

protected:
  ACE_SOCK_Dgram_Mcast *mcast (void);
  int find (const char *buf);

private:
  ACE_SOCK_Dgram_Mcast mcast_;
  
  // List of groups we've joined
  ACE_Vector<ACE_CString*> address_vec_;

  // Flag used to set the 'finished' flag when the last event handler
  // gets removed from the reactor.
  static ACE_Atomic_Op<ACE_Thread_Mutex, long> active_handlers_;
};

ACE_Atomic_Op<ACE_Thread_Mutex, long> MCT_Event_Handler::active_handlers_ = 0;

MCT_Event_Handler::MCT_Event_Handler (ACE_SOCK_Dgram_Mcast::options options)
  : mcast_ (options)
{
  // Increment the number of active handlers in the reactor.  Note this isn't
  // really correct, but it should work for our simple example.
  ++MCT_Event_Handler::active_handlers_;
}

MCT_Event_Handler::~MCT_Event_Handler (void)
{
  size_t size = this->address_vec_.size ();
  for (size_t i = 0; i < size; ++i)
    {
      delete this->address_vec_[i];
      this->address_vec_[i] = 0;
    }
}


ACE_SOCK_Dgram_Mcast *
MCT_Event_Handler::mcast (void)
{
  return &this->mcast_;
}

int 
MCT_Event_Handler::find (const char *buf)
{
  size_t size = this->address_vec_.size ();
  size_t i;
  for (i = 0; i < size; ++i)
    {
      if (ACE_OS::strcasecmp (buf, this->address_vec_[i]->c_str ()) == 0)
        return 0;
    }

  // Not found, so output message we received along with a list of groups
  // we've joined for debugging.
  ACE_CString local;
  for (i = 0; i < size; ++i)
    {
      local += "\t";
      local += this->address_vec_[i]->c_str ();
      local += "\n";
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s not in:\n%s"),
              buf, local.c_str ()));

  return -1;
}


int
MCT_Event_Handler::join (const ACE_INET_Addr &mcast_addr,
                         int reuse_addr,
                         const ACE_TCHAR *net_if)
{
  if (this->mcast_.join (mcast_addr, reuse_addr, net_if) == 0)
    {
      char buf[MAX_STRING_SIZE];
      ACE_OS::sprintf (buf, "%s/%d", 
                       mcast_addr.get_host_addr (), 
                       mcast_addr.get_port_number ());
      ACE_CString *str;
      ACE_NEW_RETURN (str, ACE_CString (ACE::strnew (buf)), -1);
      this->address_vec_.push_back (str);
      return 0;
    }
  return -1;
}

int 
MCT_Event_Handler::leave (const ACE_INET_Addr &mcast_addr,
                          const ACE_TCHAR *net_if)
{
  if (this->mcast_.leave (mcast_addr, net_if) == 0)
    {
      char buf[MAX_STRING_SIZE];
      size_t size = 0;
      for (size_t i = 0; i < size; ++i)
        {
          ACE_OS::sprintf (buf, "%s/%d", 
                           mcast_addr.get_host_addr (), 
                           mcast_addr.get_port_number ());
          if (ACE_OS::strcasecmp (buf, this->address_vec_[i]->c_str ()) == 0)
          {
            this->address_vec_[i]->set ("");
            break;
          }
        }
      return 0;
    }
  return -1;
}

int
MCT_Event_Handler::handle_input (ACE_HANDLE /*handle*/)
{
  char buf[MAX_STRING_SIZE];
  ACE_OS::memset (buf, 0, sizeof buf);
  ACE_INET_Addr addr;

  if (this->mcast ()->recv (buf, sizeof buf, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("error calling recv\n")), -1);

  // Zero length buffer means we are done.
  if (ACE_OS::strlen (buf) == 0)
    return -1;
  else if (this->find (buf) == -1)
    ACE_DEBUG ((LM_DEBUG, 
                ACE_TEXT ("error: received dgram for a group we didn't joing ")
                ACE_TEXT ("(%s) \n"), 
                buf)); 
 
  return 0;
}

int
MCT_Event_Handler::handle_close (ACE_HANDLE /*fd*/,
                                 ACE_Reactor_Mask /*close_mask*/)
{
  // If this is the last handler, use the finished flag to signal
  // the task to exit.
  if (--MCT_Event_Handler::active_handlers_ == 0)
    finished = 1;

  // The DONT_CALL flag keeps the reactor from calling handle_close ()
  // again, since we commit suicide below.
  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::ALL_EVENTS_MASK |
                                    ACE_Event_Handler::DONT_CALL);  
  this->reactor (0);
  delete this;
  return 0;
}

int
MCT_Event_Handler::get_handle (void) const
{
  return this->mcast_.get_handle ();
}

/******************************************************************************/

/*
 * Our MCT_Task object will be an Active Object if we are running the Consumer
 * side of the test.  open() calls active() which creates a thread and calls
 * the svc() method that calls runs the reactor event loop.
 */
class MCT_Task : public ACE_Task<ACE_NULL_SYNCH>
{
public: 
  MCT_Task (const MCT_Config &config,
            ACE_Reactor *reactor = ACE_Reactor::instance ());
  ~MCT_Task (void);

  // = Task hooks.
  virtual int open (void *args = 0);
  virtual int svc (void);

private:
  const MCT_Config &config_;
  int iterations_;
};

MCT_Task::MCT_Task (const MCT_Config &config,
                    ACE_Reactor *reactor)
  : config_ (config)
{
  this->reactor (reactor);
}

MCT_Task::~MCT_Task (void)
{}

int
MCT_Task::open (void *)
{ 
  MCT_Event_Handler *handler;

  ACE_INET_Addr addr = this->config_.group_start ();
  int groups = this->config_.groups ();
  for (int i = 0; i < groups; ++i)
    {
      ACE_NEW_RETURN (handler, 
                      MCT_Event_Handler (this->config_.options ()), -1);
      // We subscribe to all groups for the first one and one each for
      // all the others.
      if (i == 0)
        {
          // go ahead and hide the other one since we want our own.
          ACE_INET_Addr addr = this->config_.group_start ();
          for (int j = 0; j < groups; ++j)
            {
              // If OPT_BINDADDR_YES is set, this will fail after the first
              // join, so just break and keep on going, otherwise it's a 
              // real error.
              if (j > 0
                  && ACE_BIT_ENABLED (ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES, 
                                      this->config_.options ()))
                break;

              if (handler->join (addr) == -1)
                ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("join error\n")),
                                  -1);
              advance_addr (addr);
            }
        }
      else
        {
          if (handler->join (addr) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("join error\n")), -1);
        }

      advance_addr (addr);

      if (this->reactor ()->register_handler (handler, READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("cannot register handler\n")));
    }

  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT ("%p\n"), ACE_TEXT ("activate failed")),
                      -1);
  return 0;
}

int
MCT_Task::svc (void) 
{
  // make sure this thread owns the reactor or handle_events () won't do
  // anything.
  this->reactor ()->owner (ACE_Thread::self ());

  // loop and call handle_events...
  while (!finished)
    this->reactor ()->handle_events ();
         
  return 0;
}

/******************************************************************************/

int send_dgram (ACE_SOCK_Dgram &socket, ACE_INET_Addr addr, int done = 0)
{
  
  // Send each message twice, once to the right port, and once to the "wrong"
  // port.  This helps generate noise and lets us see if port filtering is
  // working properly.
  const char *address = addr.get_host_addr ();
  int port = addr.get_port_number ();
  
  for (int i = 0; i < 2; ++i)
    {
      char buf[MAX_STRING_SIZE];
      if (done)
        buf[0] = 0;
      else
        ACE_OS::sprintf (buf, "%s/%d", address, port);
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sending (%s)\n"), buf));
      if (socket.send (buf, ACE_OS_String::strlen (buf),addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("error in send_dgram")), -1);
      addr.set_port_number (++port);
    }
  return 0;
}

/*
 * Advance the address by 1, e.g., 239.255.0.1 => 239.255.0.2
 * Note that the algorithm is somewhat simplistic, but sufficient for our
 * purpose.
 */
int advance_addr (ACE_INET_Addr &addr)
{
  int a, b, c, d;
  ::sscanf (addr.get_host_addr (), "%d.%d.%d.%d", &a, &b, &c, &d);
  if (d < 255)
    ++d;
  else if (c < 255)
    {
      d = 1;
      ++c;
    }
  else if (b < 255)
    {
      d = 1;
      c = 0;
      ++b;
    }
  else if (a < 239)
    {
      d = 1;
      c = 0;
      b = 0;
      ++a;
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ACE_TEXT ("Cannot advance multicast group address ")
                       ACE_TEXT ("past %s\n"),
                       addr.get_host_addr ()),
                      -1);
  
  ACE_TCHAR buf[MAX_STRING_SIZE];
  ACE_OS::sprintf (buf, ACE_TEXT ("%d.%d.%d.%d:%d"),
                   a, b, c, d, addr.get_port_number ());
  addr.set (buf);            
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  MCT_Config config;
  retval = config.open (argc, argv);
  if (retval != 0)
    return 1;

  const ACE_TCHAR *temp = ACE_TEXT ("Multicast_Test");
  ACE_TString test = temp;

  if (ACE_BIT_DISABLED (config.type (), MCT_Config::PRODUCER)
      || ACE_BIT_DISABLED (config.type (), MCT_Config::CONSUMER))
    {
      if (ACE_BIT_ENABLED (config.type (), MCT_Config::PRODUCER))
        test += ACE_TEXT ("-PRODUCER");
      else
        test += ACE_TEXT ("-CONSUMER");
    }

  // Start test only if options are valid.
  ACE_START_TEST (test.c_str ());

  // Register a signal handler to close down application gracefully.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  // Dump the configuration info to the log if caller passed debug option.
  if (config.debug ())
    config.dump ();
  
  // not sure if this should be here are 
  ACE_Reactor *reactor = ACE_Reactor::instance ();

  MCT_Task *task = new MCT_Task (config, reactor);

  if (ACE_BIT_ENABLED (config.type (), MCT_Config::CONSUMER))
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Starting consumer...\n")));
      // Open makes it an active object.
      task->open ();
    }

  // now produce the datagrams...
  if (ACE_BIT_ENABLED (config.type (), MCT_Config::PRODUCER))
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Starting producer...\n")));
      ACE_SOCK_Dgram socket (ACE_sap_any_cast (ACE_INET_Addr &));
      int iterations = config.iterations ();
      // we add an extra 5 groups for noise.
      int groups = config.groups () + 5;
      for (int i = 0; i < iterations && !finished; ++i)
        {
          ACE_INET_Addr addr = config.group_start ();
          for (int j = 0; j < groups && !finished; ++j)
            {
              if (send_dgram (socket, addr, ((i + 1) == iterations)) == -1)
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("some sort of error\n")));
              if (advance_addr (addr) == -1)
                ACE_ERROR ((LM_ERROR, 
                            ACE_TEXT ("error calling advance addr\n")));
            }
          // Give the task thread a chance to run.
          ACE_Thread::yield ();
        }
    }

  if (ACE_BIT_ENABLED (config.type (), MCT_Config::CONSUMER))
    {
      // and wait for everything to finish
      ACE_DEBUG ((LM_INFO, 
                  ACE_TEXT ("start waiting for consumer to finish...\n")));
      ACE_Thread_Manager::instance ()->wait ();
    }

  delete task;
  ACE_END_TEST;
  return retval == 0 ? 0 : 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Vector<ACE_CString *>;
template class ACE_Array_Base<ACE_String_Base<char> *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Vector<ACE_CString>
#pragma instantiate ACE_Array_Base<ACE_String_Base<char> *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Multicast_Test"));

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("error: %s must be run on a platform ")
              ACE_TEXT ("that support IP multicast.\n"),
              argv[0]));
  ACE_END_TEST;
  return 1;
}
#endif /* ACE_HAS_IP_MULTICAST */
