// $Id$

#define ACE_BUILD_DLL
#include "ace/Svc_Conf.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/Malloc.h"
#include "ace/Service_Manager.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Containers.h"
#include "ace/Auto_Ptr.h"
#if !defined (ACE_HAS_WINCE)
#  include "ace/Proactor.h"
#endif /* !ACE_HAS_WINCE */
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Config.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Config)

void
ACE_Service_Config::dump (void) const
{
  ACE_TRACE ("ACE_Service_Config::dump");
}

// All the factory functions that allocate default statically linked
// services should be placed below.

// Allocate a Service Manager.

ACE_FACTORY_DEFINE (ACE, ACE_Service_Manager)

// ----------------------------------------

// Set the signal handler to point to the handle_signal() function.
ACE_Sig_Adapter *ACE_Service_Config::signal_handler_ = 0;

// Trigger a reconfiguration.
sig_atomic_t ACE_Service_Config::reconfig_occurred_ = 0;

  // = Set by command-line options.
char ACE_Service_Config::debug_ = 0;
char ACE_Service_Config::be_a_daemon_ = 0;
char ACE_Service_Config::no_static_svcs_ = 0;

// Number of the signal used to trigger reconfiguration.
int ACE_Service_Config::signum_ = SIGHUP;

// Name of file used to store messages.
LPCTSTR ACE_Service_Config::logger_key_ = ACE_DEFAULT_LOGGER_KEY;

// Name of the service configuration file.
const ASYS_TCHAR *ACE_Service_Config::service_config_file_ = ASYS_TEXT (ACE_DEFAULT_SVC_CONF);

// The ACE_Service_Manager static service object is now defined
// by the ACE_Object_Manager, in Object_Manager.cpp.

// List of statically configured services.

ACE_STATIC_SVCS *ACE_Service_Config::static_svcs_ = 0;

ACE_STATIC_SVCS *
ACE_Service_Config::static_svcs (void)
{
  if (ACE_Service_Config::static_svcs_ == 0)
    ACE_NEW_RETURN (ACE_Service_Config::static_svcs_, ACE_STATIC_SVCS, 0);

  return ACE_Service_Config::static_svcs_;
}

ACE_Allocator *
ACE_Service_Config::alloc (void)
{
  ACE_TRACE ("ACE_Service_Config::allocator");
  return ACE_Allocator::instance ();
}

ACE_Allocator *
ACE_Service_Config::alloc (ACE_Allocator *r)
{
  ACE_TRACE ("ACE_Service_Config::allocator");
  return ACE_Allocator::instance (r);
}

ACE_Reactor *
ACE_Service_Config::reactor (void)
{
  ACE_TRACE ("ACE_Service_Config::reactor");
  return ACE_Reactor::instance ();
}

ACE_Reactor *
ACE_Service_Config::reactor (ACE_Reactor *r)
{
  ACE_TRACE ("ACE_Service_Config::reactor");
  return ACE_Reactor::instance (r);
}

ACE_Service_Repository *
ACE_Service_Config::svc_rep ()
{
  ACE_TRACE ("ACE_Service_Config::svc_rep");
  return ACE_Service_Repository::instance ();
}

ACE_Service_Repository *
ACE_Service_Config::svc_rep (ACE_Service_Repository *s)
{
  ACE_TRACE ("ACE_Service_Config::svc_rep");
  return ACE_Service_Repository::instance (s);
}

ACE_Thread_Manager *
ACE_Service_Config::thr_mgr (void)
{
  ACE_TRACE ("ACE_Service_Config::thr_mgr");
  return ACE_Thread_Manager::instance ();
}

ACE_Thread_Manager *
ACE_Service_Config::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Service_Config::thr_mgr");
  return ACE_Thread_Manager::instance (tm);
}

// Totally remove <svc_name> from the daemon by removing it from the
// ACE_Reactor, and unlinking it if necessary.

int
ACE_Service_Config::remove (const ASYS_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::remove");
  return ACE_Service_Repository::instance ()->remove (svc_name);
}

// Suspend SVC_NAME.  Note that this will not unlink the service from
// the daemon if it was dynamically linked, it will mark it as being
// suspended in the Service Repository and call the suspend() member
// function on the appropriate ACE_Service_Object.  A service can be
// resumed later on by calling the RESUME() member function...

int
ACE_Service_Config::suspend (const ASYS_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::suspend");
  return ACE_Service_Repository::instance ()->suspend (svc_name);
}

// Resume a SVC_NAME that was previously suspended or has not yet
// been resumed (e.g., a static service).

int
ACE_Service_Config::resume (const ASYS_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::resume");
  return ACE_Service_Repository::instance ()->resume (svc_name);
}

// Initialize the Service Repository.  Note that this *must* be
// performed in the constructor (rather than open()) since otherwise
// the repository will not be properly initialized to allow static
// configuration of services...

ACE_Service_Config::ACE_Service_Config (int ignore_static_svcs,
                                        size_t size,
                                        int signum)
{
  ACE_TRACE ("ACE_Service_Config::ACE_Service_Config");
  ACE_Service_Config::no_static_svcs_ = (char) ignore_static_svcs;
  ACE_Service_Config::signum_ = signum;

  // Initialize the Service Repository.
  ACE_Service_Repository::instance (size);

  // Initialize the ACE_Reactor (the ACE_Reactor should be the same
  // size as the ACE_Service_Repository).
  ACE_Reactor::instance ();

  // There's no point in dealing with this on NT since it doesn't really
  // support signals very well...
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  // This really ought to be a Singleton I suspect...

  if (ACE_Reactor::instance ()->register_handler (ACE_Service_Config::signum_,
                                                  ACE_Service_Config::signal_handler_) == -1)
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("can't register signal handler\n")));
#endif /* ACE_LACKS_UNIX_SIGNALS */
}

// Handle the command-line options intended for the
// ACE_Service_Config.

void
ACE_Service_Config::parse_args (int argc, ASYS_TCHAR *argv[])
{
  ACE_TRACE ("ACE_Service_Config::parse_args");
  ACE_Get_Opt getopt (argc,
                      argv,
                      ASYS_TEXT ("bdf:k:ns:S:"),
                      1); // Start at argv[1].

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'b':
        ACE_Service_Config::be_a_daemon_ = 1;
        break;
      case 'd':
        ACE_Service_Config::debug_ = 1;
        break;
      case 'f':
        ACE_Service_Config::service_config_file_ = getopt.optarg;
        break;
      case 'k':
        ACE_Service_Config::logger_key_ = ACE_WIDE_STRING (getopt.optarg);
        break;
      case 'n':
        ACE_Service_Config::no_static_svcs_ = 1;
        break;
      case 's':
        {
          // There's no point in dealing with this on NT since it doesn't really
          // support signals very well...
#if !defined (ACE_LACKS_UNIX_SIGNALS)
          ACE_Service_Config::signum_ = ACE_OS::atoi (getopt.optarg);

          if (ACE_Reactor::instance ()->register_handler
              (ACE_Service_Config::signum_,
               ACE_Service_Config::signal_handler_) == -1)
            ACE_ERROR ((LM_ERROR,
                        ASYS_TEXT ("cannot obtain signal handler\n")));
#endif /* ACE_LACKS_UNIX_SIGNALS */
          break;
        }
      case 'S':
        // Process just a single directive.
        if (ACE_Service_Config::process_directive (getopt.optarg) == -1)
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("process_directives")));
        break;
      default:
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("%c is not a ACE_Service_Config option\n"),
                    c));
        break;
      }
}

// Initialize and activate a statically linked service.

int
ACE_Service_Config::initialize (const ASYS_TCHAR svc_name[],
                                ASYS_TCHAR *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  ACE_ARGV args (parameters);
  ACE_Service_Type  *srp = 0;

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("opening static service %s\n"), svc_name));

  if (ACE_Service_Repository::instance ()->find (svc_name,
                                                 (const ACE_Service_Type **) &srp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("%s not found\n"), svc_name), -1);

  else if (srp->type ()->init (args.argc (), args.argv ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("static initialization failed, %p\n"),
                       svc_name), -1);
  else
    {
      srp->active (1);
      return 0;
    }
}

// Dynamically link the shared object file and retrieve a pointer to
// the designated shared object in this file.

int
ACE_Service_Config::initialize (const ACE_Service_Type *sr,
                                ASYS_TCHAR parameters[])
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  ACE_ARGV args (parameters);

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("opening dynamic service %s\n"), sr->name ()));

  if (ACE_Service_Repository::instance ()->insert (sr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("insertion failed, %p\n"), sr->name ()), -1);

  else if (sr->type ()->init (args.argc (), args.argv ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("dynamic initialization failed for %s\n"),
                      sr->name ()), -1);
  else
    return 0;
}

int
ACE_Service_Config::process_directives_i (void)
{

  // AC 970827 Skip the heap check because yacc allocates a buffer
  // here which will be reported as a memory leak for some reason.
  ACE_NO_HEAP_CHECK

  // The fact that these are all global variables means that we really
  // can't be doing this processing in multiple threads
  // simultaneously...
  ace_yyerrno = 0;
  ace_yylineno = 1;

  // Use an auto_ptr to make sure that we release this memory
  // regardless of how we exit...
  ACE_NEW_RETURN (ace_obstack, ACE_Obstack, -1);

  auto_ptr<ACE_Obstack> holder (ace_obstack);

  ace_yyparse ();

  if (ace_yyerrno > 0)
    {
      errno = EINVAL; // This is a hack, better errors should be provided...
      return ace_yyerrno;
    }
  else
    return 0;
}

int
ACE_Service_Config::process_directive (ASYS_TCHAR directive[])
{
  ACE_TRACE ("ACE_Service_Config::process_directives");
  ACE_UNUSED_ARG (directive);

  // @@ What needs to happen at this point is for the <directive> to
  // be placed into a buffer that the YY_INPUT macro knows how to
  // process correctly.
  return ACE_Service_Config::process_directives_i ();
}

// Process service configuration requests as indicated in the
// <service_config_file>.

int
ACE_Service_Config::process_directives (void)
{
  ACE_TRACE ("ACE_Service_Config::process_directives");

  FILE *fp = ACE_OS::fopen (ACE_Service_Config::service_config_file_,
                            ASYS_TEXT ("r"));

  if (fp == 0)
    {
      errno = ENOENT;
      return -1; // No service configuration file
    }
  else
    {
      ace_yyrestart (fp);
      return ACE_Service_Config::process_directives_i ();
    }
}

// Add the default statically-linked services to the Service
// Repository.

int
ACE_Service_Config::load_static_svcs (void)
{
  ACE_TRACE ("ACE_Service_Config::load_static_svcs");

  ACE_Static_Svc_Descriptor **ssdp = 0;
  ACE_STATIC_SVCS &svcs = *ACE_Service_Config::static_svcs ();

  for (ACE_STATIC_SVCS_ITERATOR iter (svcs);
       iter.next (ssdp) != 0;
       iter.advance ())
    {
      ACE_Static_Svc_Descriptor *ssd = *ssdp;

      ACE_Service_Object_Exterminator gobbler;
      void *sym = (*ssd->alloc_)(&gobbler);

      ACE_Service_Type_Impl *stp =
        ace_create_service_type (ssd->name_,
                                 ssd->type_,
                                 sym,
                                 ssd->flags_,
                                 gobbler);
      if (stp == 0)
        continue;

      ACE_Service_Type *sr;

      ACE_NEW_RETURN (sr, ACE_Service_Type (ssd->name_, stp,
                                              0, ssd->active_), -1);

      if (ACE_Service_Repository::instance ()->insert (sr) == -1)
        return -1;
    }
  return 0;
}

// Performs an open without parsing command-line arguments.

int
ACE_Service_Config::open (const ASYS_TCHAR program_name[],
                          LPCTSTR logger_key)
{
  ACE_TRACE ("ACE_Service_Config::open");

  // Become a daemon before doing anything else.
  if (ACE_Service_Config::be_a_daemon_)
    ACE_Service_Config::start_daemon ();

  u_long flags = ACE_LOG_MSG->flags ();

  if (flags == 0)
    // Only use STDERR if the caller hasn't already set the flags.
    flags = (u_long) ACE_Log_Msg::STDERR;

  LPCTSTR key = logger_key;

  if (key == 0 || ACE_OS::strcmp (key, ACE_DEFAULT_LOGGER_KEY))
    // Only use the static <logger_key_> if the caller doesn't
    // override it in the parameter list.
    key = ACE_Service_Config::logger_key_;

  if (ACE_LOG_MSG->open (program_name,
                         flags,
                         key) == -1)
    return -1;
  else
    {
      ACE_DEBUG ((LM_STARTUP, ASYS_TEXT ("starting up daemon %n\n")));

      // Initialize the Service Repository (this will still work if
      // user forgets to define an object of type ACE_Service_Config).
      ACE_Service_Repository::instance (ACE_Service_Config::MAX_SERVICES);

      // Initialize the ACE_Reactor (the ACE_Reactor should be the
      // same size as the ACE_Service_Repository).
      ACE_Reactor::instance ();

      // Register ourselves to receive reconfiguration requests via
      // signals!

      if (ACE_Service_Config::no_static_svcs_ == 0
          && ACE_Service_Config::load_static_svcs () == -1)
        return -1;
      else
        return ACE_Service_Config::process_directives ();
    }
}

ACE_Service_Config::ACE_Service_Config (const ASYS_TCHAR program_name[],
                                        LPCTSTR logger_key)
{
  ACE_TRACE ("ACE_Service_Config::ACE_Service_Config");

  if (this->open (program_name, logger_key) == -1
      && errno != ENOENT)
    // Only print out an error if it wasn't the svc.conf file that was
    // missing.
    ACE_ERROR ((LM_ERROR, ASYS_TEXT ("%p\n"), program_name));
}

// Signal handling API to trigger dynamic reconfiguration.

void
ACE_Service_Config::handle_signal (int sig, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Service_Config::handle_signal");

  if (ACE_Service_Config::signum_ != sig)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("error, signal %S does match %S\n"),
                sig, ACE_Service_Config::signum_));

  if (ACE_Service_Config::debug_)
    ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("signal %S occurred\n"), sig));

  ACE_Service_Config::reconfig_occurred_ = 1;
}

// Trigger the reconfiguration process.

void
ACE_Service_Config::reconfigure (void)
{
  ACE_TRACE ("ACE_Service_Config::reconfigure");

  ACE_Service_Config::reconfig_occurred_ = 0;

  if (ACE_Service_Config::debug_)
    {
      time_t t = ACE_OS::time (0);
      ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("beginning reconfiguration at %s"), ACE_OS::ctime (&t)));
    }

  if (ACE_Service_Config::process_directives () == -1)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("process_directives")));
}

// Run the event loop until the <ACE_Reactor::handle_events>
// method returns -1 or the <end_reactor_event_loop> method
// is invoked.

int
ACE_Service_Config::run_reactor_event_loop (void)
{
  ACE_TRACE ("ACE_Service_Config::run_reactor_event_loop");

  return ACE_Reactor::run_event_loop ();
}

// Run the event loop until the <ACE_Reactor::handle_events>
// method returns -1, the <end_reactor_event_loop> method
// is invoked, or the <ACE_Time_Value> expires.

int
ACE_Service_Config::run_reactor_event_loop (ACE_Time_Value &tv)
{
  ACE_TRACE ("ACE_Service_Config::run_reactor_event_loop");

  return ACE_Reactor::run_event_loop (tv);
}

/* static */
int
ACE_Service_Config::end_reactor_event_loop (void)
{
  ACE_TRACE ("ACE_Service_Config::end_reactor_event_loop");
  return ACE_Reactor::end_event_loop ();
}

/* static */
int
ACE_Service_Config::reactor_event_loop_done (void)
{
  ACE_TRACE ("ACE_Service_Config::reactor_event_loop_done");
  return ACE_Reactor::event_loop_done ();
}

// Tidy up and perform last rites on a terminating ACE_Service_Config.
int
ACE_Service_Config::close (void)
{
  ACE_TRACE ("ACE_Service_Config::close");

  // ACE_Service_Config must be deleted before the Singletons are
  // closed so that an object's fini() method may reference a
  // valid ACE_Reactor.
  ACE_Service_Config::close_svcs ();

  // The Singletons can be used independently of the services.
  // Therefore, this call must go out here.
  ACE_Service_Config::close_singletons ();

  // Delete the dynamically allocated static_svcs instance.
  delete ACE_Service_Config::static_svcs_;
  ACE_Service_Config::static_svcs_ = 0;

  return 0;
}

int
ACE_Service_Config::close_svcs (void)
{
  ACE_TRACE ("ACE_Service_Config::close_svcs");

  ACE_Service_Repository::close_singleton ();

  return 0;
}

int
ACE_Service_Config::close_singletons (void)
{
  ACE_TRACE ("ACE_Service_Config::close_singletons");

  ACE_Reactor::close_singleton ();
#if !defined (ACE_HAS_WINCE)
  ACE_Proactor::close_singleton ();
#endif /* !ACE_HAS_WINCE */
  ACE_Thread_Manager::close_singleton ();

  return 0;
}

// Perform user-specified close activities and remove dynamic memory.


ACE_Service_Config::~ACE_Service_Config (void)
{
  ACE_TRACE ("ACE_Service_Config::~ACE_Service_Config");
  ACE_Service_Config::close ();
}

// ************************************************************

/* static */
int
ACE_Service_Config::reconfig_occurred (void)
{
  ACE_TRACE ("ACE_Service_Config::reconfig_occurred");
  return ACE_Service_Config::reconfig_occurred_ != 0;
}

void
ACE_Service_Config::reconfig_occurred (int config_occurred)
{
  ACE_TRACE ("ACE_Service_Config::reconfig_occurred");
  ACE_Service_Config::reconfig_occurred_ = config_occurred;
}

// Become a daemon (i.e., run as a ASYS_TEXT ("background") process).

int
ACE_Service_Config::start_daemon (void)
{
  ACE_TRACE ("ACE_Service_Config::start_daemon");
  return ACE::daemonize ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>;
template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >;
template class auto_ptr<ACE_Obstack>;
template class ACE_Auto_Basic_Ptr<ACE_Obstack>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >
#pragma instantiate auto_ptr<ACE_Obstack>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Obstack>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
