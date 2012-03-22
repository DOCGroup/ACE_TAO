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
//    increment by 1 up to 5 (default) groups.  Both of these values, as well
//    as others, can be overridden via command-line options.  Use the -?
//    option to display the usage message...
//
// = AUTHOR
//    Don Hinton <dhinton@dresystems.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/Vector_T.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/ACE.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"
#include "ace/SString.h"
#include "ace/Signal.h"
#include "ace/Min_Max.h"



#if defined (ACE_HAS_IP_MULTICAST) && defined (ACE_HAS_THREADS)

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

static const int MCT_ITERATIONS = 10;
static const int MCT_GROUPS = 5;
static const int MCT_MIN_GROUPS = 2;

static const char MCT_START_GROUP[] = "239.255.0.1";
static const int  MCT_START_PORT = 16000;

static const size_t MAX_STRING_SIZE = 200;

int advance_addr (ACE_INET_Addr &addr);

// Keep track of errors so we can report them on exit.
static sig_atomic_t error = 0;

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
      role_ (BOTH),
      sdm_opts_ (ACE_SOCK_Dgram_Mcast::DEFOPTS),
      iterations_ (MCT_ITERATIONS),
      ttl_ (1),
      wait_ (2)
    {
      if (IP_MAX_MEMBERSHIPS == 0)
        this->groups_ = MCT_GROUPS;
      else
        this->groups_ = ACE_MIN (IP_MAX_MEMBERSHIPS, MCT_GROUPS);
    }
  ~MCT_Config (void)
    {}

  //FUZZ: disable check_for_lack_ACE_OS
  int open (int argc, ACE_TCHAR *argv[]);
  //FUZZ: enable check_for_lack_ACE_OS

  int debug (void) const { return this->debug_;}
  void dump (void) const;
  int groups (void) const { return this->groups_;}
  const ACE_INET_Addr group_start (void) const { return this->group_start_;}
  u_long role (void) const { return this->role_;}
  int iterations (void) const { return this->iterations_;}
  int ttl (void) const { return this->ttl_;}

  //FUZZ: disable check_for_lack_ACE_OS
  int wait (void) const { return this->wait_;}
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_SOCK_Dgram_Mcast::options options (void) const
  {
    return static_cast<ACE_SOCK_Dgram_Mcast::options> (this->sdm_opts_);
  }

private:
  // Starting group address. (only IPv4 capable right now...)
  ACE_INET_Addr group_start_;

  // Number of groups we will try to use in the test.
  int groups_;

  // Debug flag.
  int debug_;

  // Role, i.e., PRODUCER, CONSUMER, BOTH: defaults to BOTH
  u_long role_;

  // ACE_SOCK_Dgram_Mcast ctor options
  u_long sdm_opts_;

  // Producer iterations
  int iterations_;

  // TTL, time to live, for use over routers.
  int ttl_;

  // Time to wait on CONSUMER threads to end before killing test.
  int wait_;
};

int
MCT_Config::open (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  int help = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT (":?"), 1, 1);
  //FUZZ: enable check_for_lack_ACE_OS

  if (getopt.long_option (ACE_TEXT ("GroupStart"),
                          'g',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" Unable to add GroupStart option.\n")),
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

  if (getopt.long_option (ACE_TEXT ("Role"),
                          'r',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" Unable to add Role option.\n")), 1);

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

  if (getopt.long_option (ACE_TEXT ("TTL"),
                          't',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" Unable to add TTL option.\n")),
                      1);

  if (getopt.long_option (ACE_TEXT ("Wait"),
                          'w',
                          ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" Unable to add wait option.\n")),
                      1);

  if (getopt.long_option (ACE_TEXT ("help"),
                          'h',
                          ACE_Get_Opt::NO_ARG) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT (" Unable to add help option.\n")),
                      1);

  //FUZZ: disable check_for_lack_ACE_OS
  // Now, let's parse it...
  int c = 0;
  while ((c = getopt ()) != EOF)
    {
      //FUZZ: enable check_for_lack_ACE_OS
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
            ACE_TCHAR *group = getopt.opt_arg ();
            if (this->group_start_.set (group) != 0)
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("Bad group address:%s\n"),
                            group));
              }
          }
          break;
        case 'i':
          this->iterations_ = ACE_OS::atoi (getopt.opt_arg ());
          break;
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
        case 'r':
          {
            ACE_TCHAR *c = getopt.opt_arg ();
            if (ACE_OS::strcasecmp (c, ACE_TEXT ("CONSUMER")) == 0)
              this->role_ = CONSUMER;
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("PRODUCER")) == 0)
              this->role_ = PRODUCER;
            else
              {
                help = 1;
                retval = -1;
              }
          }
          break;
        case 'm':
          {
            //@todo add back OPT_BINDADDR_NO...
            ACE_TCHAR *c = getopt.opt_arg ();
            if (ACE_OS::strcasecmp (c, ACE_TEXT ("OPT_BINDADDR_YES")) == 0)
              ACE_SET_BITS (this->sdm_opts_,
                            ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES);
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("OPT_BINDADDR_NO")) == 0)
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
            else if (ACE_OS::strcasecmp (c, ACE_TEXT ("OPT_NULLIFACE_ONE")) == 0)
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
        case 't':
          this->ttl_ = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'w':
          this->wait_ = ACE_OS::atoi (getopt.opt_arg ());
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
              && getopt.opt_opt () != 'h')
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
                    ACE_TEXT ("starting multicast group address\n")
                    ACE_TEXT ("                                      ")
                    ACE_TEXT ("(default=239.255.0.1:16000)\n")
                    ACE_TEXT ("  -n {#}       --Groups={#}           ")
                    ACE_TEXT ("number of groups (default=5)\n")
                    ACE_TEXT ("  -d           --Debug                ")
                    ACE_TEXT ("debug flag (default=off)\n")
                    ACE_TEXT ("  -r {STRING}  --Role={STRING}        ")
                    ACE_TEXT ("role {PRODUCER|CONSUMER|BOTH}\n")
                    ACE_TEXT ("                                      ")
                    ACE_TEXT ("(default=BOTH)\n")
                    ACE_TEXT ("  -m {STRING}  --SDM_options={STRING} ")
                    ACE_TEXT ("ACE_SOCK_Dgram_Mcast ctor options\n")
                    ACE_TEXT ("                                      ")
                    ACE_TEXT ("(default=DEFOPTS)\n")
                    ACE_TEXT ("  -i {#}       --Iterations={#}       ")
                    ACE_TEXT ("number of iterations (default=100)\n")
                    ACE_TEXT ("  -t {#}       --TTL={#}              ")
                    ACE_TEXT ("time to live (default=1)\n")
                    ACE_TEXT ("  -w {#}       --Wait={#}             ")
                    ACE_TEXT ("number of seconds to wait on CONSUMER\n")
                    ACE_TEXT ("                                      ")
                    ACE_TEXT ("(default=2)\n")
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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" Dumping MCT_Config\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tIP_MAX_MEMBERSHIPS = %d\n"),
              IP_MAX_MEMBERSHIPS));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tgroups_ = %d\n"),
              this->groups_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\trole_ = %s\n"),
              (ACE_BIT_ENABLED (this->role_, PRODUCER)
               && ACE_BIT_ENABLED (this->role_, CONSUMER))
                 ? ACE_TEXT ("PRODUCER/CONSUMER")
                 : ACE_BIT_ENABLED (this->role_, PRODUCER)
                     ? ACE_TEXT ("PRODUCER")
                     : ACE_TEXT ("CONSUMER")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tsdm_options_ = %d\n"),
              this->sdm_opts_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\titerations_ = %d\n"),
              this->iterations_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tttl_ = %d\n"),
              this->ttl_));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\twait_ = %d\n"),
              this->wait_));
  // Note that this call to get_host_addr is the non-reentrant
  // version, but it's okay for us.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\tgroups_start_ = %s:%d\n"),
              this->group_start_.get_host_addr (),
              this->group_start_.get_port_number ()));

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
  static ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> active_handlers_;
};

ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> MCT_Event_Handler::active_handlers_ = 0;

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
  mcast_.close ();
}


ACE_SOCK_Dgram_Mcast *
MCT_Event_Handler::mcast (void)
{
  return &this->mcast_;
}

int
MCT_Event_Handler::find (const char *buf)
{
  size_t const size = this->address_vec_.size ();
  size_t i = 0;
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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%C not in:\n%C"),
              buf, local.c_str ()));

  return -1;
}


int
MCT_Event_Handler::join (const ACE_INET_Addr &mcast_addr,
                         int reuse_addr,
                         const ACE_TCHAR *net_if)
{
  char buf[MAX_STRING_SIZE];
  ACE_OS::sprintf (buf, "%s/%d",
                   mcast_addr.get_host_addr (),
                   mcast_addr.get_port_number ());

  if (this->mcast_.join (mcast_addr, reuse_addr, net_if) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("MCT_Event_Handler::join %C %p\n"),
                       buf,
                       ACE_TEXT ("failed")),
                      -1);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Joined %C\n"), buf));

  ACE_CString *str = 0;
  ACE_NEW_RETURN (str, ACE_CString (buf), -1);
  this->address_vec_.push_back (str);
  return 0;
}

int
MCT_Event_Handler::leave (const ACE_INET_Addr &mcast_addr,
                          const ACE_TCHAR *net_if)
{
  if (this->mcast_.leave (mcast_addr, net_if) == 0)
    {
      char buf[MAX_STRING_SIZE];
      size_t size = this->address_vec_.size ();
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
    {
      ++error;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("MCT_Event_Handler::handle_input - ")
                         ACE_TEXT ("calling recv\n")), -1);
    }

  // Zero length buffer means we are done.
  if (ACE_OS::strlen (buf) == 0)
    return -1;
  else if (this->find (buf) == -1)
    {
      ++error;
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("MCT_Event_Handler::handle_input - ")
                  ACE_TEXT ("Received dgram for a group we didn't join ")
                  ACE_TEXT ("(%s)\n"),
                  buf));
    }
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

ACE_HANDLE
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

  //FUZZ: disable check_for_lack_ACE_OS
  // = Task hooks.
  virtual int open (void *args = 0);
  //FUZZ: enable check_for_lack_ACE_OS

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
  MCT_Event_Handler *handler = 0;

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
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("MCT_Task::open - join error\n")),
                                  -1);
              advance_addr (addr);
            }
        }
      else
        {
          if (handler->join (addr) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("MCT_Task::open - join error\n")),
                              -1);
        }

      advance_addr (addr);

      if (this->reactor ()->register_handler (handler, READ_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("MCT_Task::open - cannot register ")
                           ACE_TEXT ("handler\n")),
                          -1);
    }

  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("MCT_TASK:open - activate failed")),
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

      if (socket.send (buf, ACE_OS::strlen (buf),addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Send to %C, %p\n"),
                           address,
                           ACE_TEXT ("send_dgram - error calling send on ")
                           ACE_TEXT ("ACE_SOCK_Dgram.")), -1);
      addr.set_port_number (++port);
    }
  return 0;
}

int producer (MCT_Config &config)
{
  int retval = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Starting producer...\n")));
  ACE_SOCK_Dgram socket (ACE_sap_any_cast (ACE_INET_Addr &), PF_INET);
  //FUZZ: enable check_for_lack_ACE_OS
#if defined (ACE_LINUX)
  socket.set_nic (ACE_TEXT("lo"));
#endif
  // Note that is is IPv4 specific and needs to be changed once
  //
  if (config.ttl () > 1)
    {
      int ttl = config.ttl ();
      if (socket.set_option (IPPROTO_IP,
                             IP_MULTICAST_TTL,
                             (void*) &ttl,
                             sizeof ttl) != 0)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("could net set socket option IP_MULTICAST_TTL ")
                    ACE_TEXT ("= %d\n"),
                    ttl));
      else
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("set IP_MULTICAST_TTL = %d\n"), ttl));
    }

  int iterations = config.iterations ();
  // we add an extra 5 groups for noise.
  int groups = config.groups () + 5;
  for (int i = 0; (i < iterations || iterations == 0) && !finished; ++i)
    {
      ACE_INET_Addr addr = config.group_start ();
      for (int j = 0; j < groups && !finished; ++j)
        {
          if ((retval += send_dgram (socket, addr,
                                     ((i + 1) == iterations))) == -1)
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Calling send_dgram.\n")));
          if ((retval += advance_addr (addr)) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Calling advance_addr.\n")));
        }
      // Give the task thread a chance to run.
      ACE_Thread::yield ();
    }
  socket.close ();
  return retval;
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
                       ACE_TEXT ("advance_addr - Cannot advance multicast ")
                       ACE_TEXT ("group address past %s\n"),
                       addr.get_host_addr ()),
                      -1);

  ACE_TCHAR buf[MAX_STRING_SIZE];
  ACE_OS::sprintf (buf, ACE_TEXT ("%d.%d.%d.%d:%d"),
                   a, b, c, d, addr.get_port_number ());
  addr.set (buf);
  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  int retval = 0;
  MCT_Config config;
  retval = config.open (argc, argv);
  if (retval != 0)
    return 1;

  const ACE_TCHAR *temp = ACE_TEXT ("Multicast_Test");
  ACE_TString test = temp;

  u_long role = config.role ();
  if (ACE_BIT_DISABLED (role, MCT_Config::PRODUCER)
      || ACE_BIT_DISABLED (role, MCT_Config::CONSUMER))
    {
      if (ACE_BIT_ENABLED (role, MCT_Config::PRODUCER))
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

  ACE_Reactor *reactor = ACE_Reactor::instance ();

  MCT_Task *task = new MCT_Task (config, reactor);

  if (ACE_BIT_ENABLED (role, MCT_Config::CONSUMER))
    {
      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Starting consumer...\n")));
      // Open makes it an active object.
      retval += task->open ();
    }

  // now produce the datagrams...
  if (ACE_BIT_ENABLED (role, MCT_Config::PRODUCER))
    retval += producer (config);

  if (ACE_BIT_ENABLED (role, MCT_Config::CONSUMER))
    {
      // and wait for everything to finish
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("start waiting for consumer to finish...\n")));
      // Wait for the threads to exit.
      // But, wait for a limited time since we could hang if the last udp
      // message isn't received.
      ACE_Time_Value max_wait ( config.wait ()/* seconds */);
      ACE_Time_Value wait_time (ACE_OS::gettimeofday () + max_wait);
      ACE_Time_Value *ptime = ACE_BIT_ENABLED (role, MCT_Config::PRODUCER)
                                ? &wait_time : 0;
      if (ACE_Thread_Manager::instance ()->wait (ptime) == -1)
        {
          // We will no longer wait for this thread, so we must
          // force it to exit otherwise the thread will be referencing
          // deleted memory.
          finished = 1;
          reactor->end_reactor_event_loop ();

          if (errno == ETIME)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("maximum wait time of %d msec exceeded\n"),
                        max_wait.msec ()));
          else
            ACE_OS::perror (ACE_TEXT ("wait"));

          ++error;

          // This should exit now that we ended the reactor loop.
          task->wait ();
        }
    }

  delete task;
  ACE_END_TEST;
  return (retval == 0 && error == 0) ? 0 : 1;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Multicast_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("This test must be run on a platform ")
              ACE_TEXT ("that support IP multicast.\n")));

  ACE_END_TEST;
  return 1;
}
#endif /* ACE_HAS_IP_MULTICAST && ACE_HAS_THREADS */
