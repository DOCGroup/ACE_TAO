// $Id$

#include "ace/Svc_Conf.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/Malloc.h"
#include "ace/Service_Manager.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Containers.h"
#include "ace/Auto_Ptr.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"

#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Config.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace,
           Service_Config,
           "$Id$")

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
int ACE_Service_Config::be_a_daemon_ = 0;
int ACE_Service_Config::no_static_svcs_ = 1;

// Number of the signal used to trigger reconfiguration.
int ACE_Service_Config::signum_ = SIGHUP;

// Indicates where to write the logging output.  This is typically
// either a STREAM pipe or a socket address.
const ACE_TCHAR *ACE_Service_Config::logger_key_ = ACE_DEFAULT_LOGGER_KEY;

// The ACE_Service_Manager static service object is now defined by the
// ACE_Object_Manager, in Object_Manager.cpp.

// Are we initialized already?
int ACE_Service_Config::is_initialized_ = 0;

// List of statically configured services.

ACE_STATIC_SVCS *ACE_Service_Config::static_svcs_ = 0;
ACE_SVC_QUEUE *ACE_Service_Config::svc_queue_ = 0;
ACE_SVC_QUEUE *ACE_Service_Config::svc_conf_file_queue_ = 0;

ACE_STATIC_SVCS *
ACE_Service_Config::static_svcs (void)
{
  if (ACE_Service_Config::static_svcs_ == 0)
    ACE_NEW_RETURN (ACE_Service_Config::static_svcs_,
                    ACE_STATIC_SVCS,
                    0);
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

#if defined (ACE_THREAD_MANAGER_LACKS_STATICS)
  return ACE_THREAD_MANAGER_SINGLETON::instance ();
#else /* ! ACE_THREAD_MANAGER_LACKS_STATICS */
  return ACE_Thread_Manager::instance ();
#endif /* ACE_THREAD_MANAGER_LACKS_STATICS */
}

#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
ACE_Thread_Manager *
ACE_Service_Config::thr_mgr (ACE_Thread_Manager *tm)
{
  ACE_TRACE ("ACE_Service_Config::thr_mgr");
  return ACE_Thread_Manager::instance (tm);
}
#endif /* ! ACE_THREAD_MANAGER_LACKS_STATICS */

// Totally remove <svc_name> from the daemon by removing it from the
// ACE_Reactor, and unlinking it if necessary.

int
ACE_Service_Config::remove (const ACE_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::remove");
  return ACE_Service_Repository::instance ()->remove (svc_name);
}

// Suspend <svc_name>.  Note that this will not unlink the service
// from the daemon if it was dynamically linked, it will mark it as
// being suspended in the Service Repository and call the <suspend>
// member function on the appropriate <ACE_Service_Object>.  A service
// can be resumed later on by calling the <resume> method...

int
ACE_Service_Config::suspend (const ACE_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::suspend");
  return ACE_Service_Repository::instance ()->suspend (svc_name);
}

// Resume a SVC_NAME that was previously suspended or has not yet
// been resumed (e.g., a static service).

int
ACE_Service_Config::resume (const ACE_TCHAR svc_name[])
{
  ACE_TRACE ("ACE_Service_Config::resume");
  return ACE_Service_Repository::instance ()->resume (svc_name);
}

// Initialize the Service Repository.  Note that this *must* be
// performed in the constructor (rather than <open>) since otherwise
// the repository will not be properly initialized to allow static
// configuration of services...

ACE_Service_Config::ACE_Service_Config (int ignore_static_svcs,
                                        size_t size,
                                        int signum)
{
  ACE_TRACE ("ACE_Service_Config::ACE_Service_Config");
  ACE_Service_Config::no_static_svcs_ = ignore_static_svcs;
  ACE_Service_Config::signum_ = signum;

  // Initialize the Service Repository.
  ACE_Service_Repository::instance (size);

  // Initialize the ACE_Reactor (the ACE_Reactor should be the same
  // size as the ACE_Service_Repository).
  ACE_Reactor::instance ();
}

int
ACE_Service_Config::init_svc_conf_file_queue (void)
{
  if (ACE_Service_Config::svc_conf_file_queue_ == 0)
      ACE_NEW_RETURN (ACE_Service_Config::svc_conf_file_queue_,
                      ACE_SVC_QUEUE,
                      -1);
  return 0;
}

// Handle the command-line options intended for the
// ACE_Service_Config.

int
ACE_Service_Config::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_TRACE ("ACE_Service_Config::parse_args");
  ACE_Get_Opt getopt (argc,
                      argv,
                      ACE_LIB_TEXT ("bdf:k:nys:S:"),
                      1); // Start at argv[1].

  if (ACE_Service_Config::init_svc_conf_file_queue () == -1)
    return -1;

  for (int c; (c = getopt ()) != -1; )
    switch (c)
      {
      case 'b':
        ACE_Service_Config::be_a_daemon_ = 1;
        break;
      case 'd':
        ACE::debug (1);
        break;
      case 'f':
        if (ACE_Service_Config::svc_conf_file_queue_->enqueue_tail
            (ACE_TString (getopt.optarg)) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             "enqueue_tail"),
                            -1);
        break;
      case 'k':
        ACE_Service_Config::logger_key_ = getopt.optarg;
        break;
      case 'n':
        ACE_Service_Config::no_static_svcs_ = 1;
        break;
      case 'y':
        ACE_Service_Config::no_static_svcs_ = 0;
        break;
      case 's':
        {
          // There's no point in dealing with this on NT since it
          // doesn't really support signals very well...
#if !defined (ACE_LACKS_UNIX_SIGNALS)
          ACE_Service_Config::signum_ =
            ACE_OS::atoi (getopt.optarg);

          if (ACE_Reactor::instance ()->register_handler
              (ACE_Service_Config::signum_,
               ACE_Service_Config::signal_handler_) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_LIB_TEXT ("cannot obtain signal handler\n")),
                              -1);
#endif /* ACE_LACKS_UNIX_SIGNALS */
          break;
        }
      case 'S':
        if (ACE_Service_Config::svc_queue_ == 0)
          ACE_NEW_RETURN (ACE_Service_Config::svc_queue_,
                          ACE_SVC_QUEUE,
                          -1);
        if (ACE_Service_Config::svc_queue_->enqueue_tail
            (ACE_TString (getopt.optarg)) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             "enqueue_tail"),
                            -1);
        break;
      default:
        if (ACE::debug () > 0)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_LIB_TEXT ("%c is not a ACE_Service_Config option\n"),
                      c));
      }

  return 0;
}

// Initialize and activate a statically linked service.

int
ACE_Service_Config::initialize (const ACE_TCHAR svc_name[],
                                ACE_TCHAR *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  ACE_ARGV args (parameters);
  ACE_Service_Type *srp = 0;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("opening static service %s\n"),
                svc_name));

  if (ACE_Service_Repository::instance ()->find
      (svc_name,
       (const ACE_Service_Type **) &srp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%s not found\n"),
                       svc_name),
                      -1);
  else if (srp->type ()->init (args.argc (),
                               args.argv ()) == -1)
    {
      // Remove this entry.
      ACE_ERROR ((LM_ERROR,
                         ACE_LIB_TEXT ("static initialization failed, %p\n"),
                         svc_name));
      ACE_Service_Repository::instance ()->remove (svc_name);
      return -1;
    }
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
                                ACE_TCHAR parameters[])
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  ACE_ARGV args (parameters);

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("opening dynamic service %s\n"),
                sr->name ()));

  if (sr->type ()->init (args.argc (),
                         args.argv ()) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("dynamic initialization failed for %s\n"),
                  sr->name ()));
      return -1;
    }
  else
    {
      if (ACE_Service_Repository::instance ()->insert (sr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("insertion failed, %p\n"),
                           sr->name ()),
                          -1);
      return 0;
    }
}

int
ACE_Service_Config::process_directives_i (ACE_Svc_Conf_Param *param)
{
  // AC 970827 Skip the heap check because yacc allocates a buffer
  // here which will be reported as a memory leak for some reason.
  ACE_NO_HEAP_CHECK

  // The fact that these are global variables means that we really
  // can't track the number of errors in multiple threads
  // simultaneously.
  ace_yyerrno = 0;
  ace_yylineno = 1;

  ace_yyparse (param);

  if (param->yyerrno > 0)
    {
      // This is a hack, better errors should be provided...
      errno = EINVAL;
      return param->yyerrno;
    }
  else
    return 0;
}

int
ACE_Service_Config::process_directive (const ACE_TCHAR directive[])
{
  ACE_TRACE ("ACE_Service_Config::process_directive");

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("Service_Config::process_directive - %s\n"),
                directive));

  ACE_UNUSED_ARG (directive);

  ACE_Svc_Conf_Param d (directive);

  int result = ACE_Service_Config::process_directives_i (&d);

  return result;
}

// Process service configuration requests as indicated in the queue of
// svc.conf files.

int
ACE_Service_Config::process_directives (void)
{
  ACE_TRACE ("ACE_Service_Config::process_directives");

  int result = 0;

  if (ACE_Service_Config::svc_conf_file_queue_ != 0)
    {
      ACE_TString *sptr = 0;
      ACE_SVC_QUEUE &queue = *ACE_Service_Config::svc_conf_file_queue_;

      // Iterate through all the svc.conf files.
      for (ACE_SVC_QUEUE_ITERATOR iter (queue);
           iter.next (sptr) != 0;
           iter.advance ())
        {
          FILE *fp = ACE_OS::fopen (sptr->fast_rep (),
                                    ACE_LIB_TEXT ("r"));
          if (fp == 0)
            {
              // Invalid svc.conf file.  We'll report it here and
              // break out of the method.
              if (ACE::debug ())
                ACE_DEBUG ((LM_DEBUG,
                            ACE_LIB_TEXT ("%p\n"),
                            sptr->fast_rep ()));
              errno = ENOENT;
              result = -1;
              break;
            }
          else
            {
              ACE_Svc_Conf_Param f (fp);

              // Keep track of the number of errors.
              result += ACE_Service_Config::process_directives_i (&f);
            }
          ACE_OS::fclose (fp);
        }
    }

  return result;
}

int
ACE_Service_Config::process_commandline_directives (void)
{
  int result = 0;

  if (ACE_Service_Config::svc_queue_ != 0)
    {
      ACE_TString *sptr = 0;
      ACE_SVC_QUEUE &queue = *ACE_Service_Config::svc_queue_;

      for (ACE_SVC_QUEUE_ITERATOR iter (queue);
           iter.next (sptr) != 0;
           iter.advance ())
        {
          // Process just a single directive.
          if (ACE_Service_Config::process_directive (sptr->fast_rep ()) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_LIB_TEXT ("%p\n"),
                          ACE_LIB_TEXT ("process_directive")));
              result = -1;
            }
        }

      delete ACE_Service_Config::svc_queue_;
      ACE_Service_Config::svc_queue_ = 0;
    }

  return result;
}

int
ACE_Service_Config::process_directive (const ACE_Static_Svc_Descriptor &ssd,
                                       int force_replace)
{
  if (!force_replace)
    {
      if (ACE_Service_Repository::instance ()->find (ssd.name_,
                                                     0, 0) >= 0)
        {
          // The service is already there, just return
          return 0;
        }
    }

  ACE_Service_Object_Exterminator gobbler;
  void *sym = (ssd.alloc_)(&gobbler);

  ACE_Service_Type_Impl *stp =
    ace_create_service_type (ssd.name_,
                             ssd.type_,
                             sym,
                             ssd.flags_,
                             gobbler);
  if (stp == 0)
    return 0;


  ACE_Service_Type *service_type;
  ACE_NEW_RETURN (service_type,
                  ACE_Service_Type (ssd.name_,
                                    stp,
                                    0,
                                    ssd.active_),
                  -1);

  return ACE_Service_Repository::instance ()->insert (service_type);
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

      if (ACE_Service_Config::process_directive (*ssd, 1) == -1)
        return -1;
    }
  return 0;
}

// Performs an open without parsing command-line arguments.

int
ACE_Service_Config::open_i (const ACE_TCHAR program_name[],
                            const ACE_TCHAR *logger_key,
                            int ignore_default_svc_conf_file,
                            int ignore_debug_flag)
{
  int result = 0;
  ACE_TRACE ("ACE_Service_Config::open_i");
  ACE_Log_Msg *log_msg = ACE_LOG_MSG;

  // Record the current log setting upon entering this thread.
  u_long old_process_mask = log_msg->priority_mask
    (ACE_Log_Msg::PROCESS);
  u_long old_thread_mask = log_msg->priority_mask
    (ACE_Log_Msg::THREAD);

  if (ACE_Service_Config::is_initialized_ != 0)
    // Guard against reentrant processing!
    return 0;
  else
    ACE_Service_Config::is_initialized_++;

  if (ACE_Service_Config::init_svc_conf_file_queue () == -1)
    return -1;
  else if (!ignore_default_svc_conf_file
      && ACE_Service_Config::svc_conf_file_queue_->is_empty ()
      // Load the default "svc.conf" entry here if there weren't
      // overriding -f arguments in <parse_args>.
      && ACE_Service_Config::svc_conf_file_queue_->enqueue_tail
           (ACE_TString (ACE_DEFAULT_SVC_CONF)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       "enqueue_tail"),
                      -1);

  if (ignore_debug_flag == 0)
    {
      // If -d was included as a startup parameter, the user wants debug
      // information printed during service initialization.
      if (ACE::debug ())
        ACE_Log_Msg::enable_debug_messages ();
      else
        // The user has requested no debugging info.
        ACE_Log_Msg::disable_debug_messages ();
    }

  // Become a daemon before doing anything else.
  if (ACE_Service_Config::be_a_daemon_)
    ACE_Service_Config::start_daemon ();

  u_long flags = log_msg->flags ();

  if (flags == 0)
    // Only use STDERR if the caller hasn't already set the flags.
    flags = (u_long) ACE_Log_Msg::STDERR;

  const ACE_TCHAR *key = logger_key;

  if (key == 0 || ACE_OS::strcmp (key, ACE_DEFAULT_LOGGER_KEY) == 0)
    // Only use the static <logger_key_> if the caller doesn't
    // override it in the parameter list or if the key supplied is
    // equal to the default static logger key.
    key = ACE_Service_Config::logger_key_;
  else
    ACE_SET_BITS (flags, ACE_Log_Msg::LOGGER);

  if (log_msg->open (program_name,
                     flags,
                     key) == -1)
    result = -1;
  else
    {
      if (ACE::debug ())
        ACE_DEBUG ((LM_STARTUP,
                    ACE_LIB_TEXT ("starting up daemon %n\n")));

      // Initialize the Service Repository (this will still work if
      // user forgets to define an object of type ACE_Service_Config).
      ACE_Service_Repository::instance (ACE_Service_Config::MAX_SERVICES);

      // Initialize the ACE_Reactor (the ACE_Reactor should be the
      // same size as the ACE_Service_Repository).
      ACE_Reactor::instance ();

      // There's no point in dealing with this on NT since it doesn't
      // really support signals very well...
#if !defined (ACE_LACKS_UNIX_SIGNALS)
      // @@ This really ought to be a Singleton.
      if (ACE_Reactor::instance ()->register_handler
          (ACE_Service_Config::signum_,
           ACE_Service_Config::signal_handler_) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("can't register signal handler\n")));
#endif /* ACE_LACKS_UNIX_SIGNALS */

      // See if we need to load the static services.
      if (ACE_Service_Config::no_static_svcs_ == 0
          && ACE_Service_Config::load_static_svcs () == -1)
        result = -1;
      else
        {
          if (ACE_Service_Config::process_commandline_directives () == -1)
            result = -1;
          else
            result = ACE_Service_Config::process_directives ();
        }
    }

  {
    // Make sure to save/restore errno properly.
    ACE_Errno_Guard error (errno);

    if (ignore_debug_flag == 0)
      {
        // Reset debugging back to the way it was when we came into
        // into <open_i>.
        log_msg->priority_mask (old_process_mask, ACE_Log_Msg::PROCESS);
        log_msg->priority_mask (old_thread_mask, ACE_Log_Msg::THREAD);
      }
  }

  return result;
}

ACE_Service_Config::ACE_Service_Config (const ACE_TCHAR program_name[],
                                        const ACE_TCHAR *logger_key)
{
  ACE_TRACE ("ACE_Service_Config::ACE_Service_Config");

  if (this->open (program_name,
                  logger_key) == -1
      && errno != ENOENT)
    // Only print out an error if it wasn't the svc.conf file that was
    // missing.
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                program_name));
}

// Signal handling API to trigger dynamic reconfiguration.

void
ACE_Service_Config::handle_signal (int sig,
                                   siginfo_t *,
                                   ucontext_t *)
{
#if defined (ACE_NDEBUG)
  ACE_UNUSED_ARG (sig);
#else  /* ! ACE_NDEBUG */
  ACE_ASSERT (ACE_Service_Config::signum_ == sig);
#endif /* ! ACE_NDEBUG */

  ACE_Service_Config::reconfig_occurred_ = 1;
}

// Trigger the reconfiguration process.

void
ACE_Service_Config::reconfigure (void)
{
  ACE_TRACE ("ACE_Service_Config::reconfigure");

  ACE_Service_Config::reconfig_occurred_ = 0;

  if (ACE::debug ())
    {
#if !defined (ACE_NLOGGING)
      time_t t = ACE_OS::time (0);
#endif /* ! ACE_NLOGGING */
      if (ACE::debug ())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_LIB_TEXT ("beginning reconfiguration at %s"),
                    ACE_OS::ctime (&t)));
    }
  if (ACE_Service_Config::process_directives () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("process_directives")));
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

// Run the event loop until the <ACE_Reactor::handle_events> method
// returns -1, the <end_reactor_event_loop> method is invoked, or the
// <ACE_Time_Value> expires.

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

  ACE_Service_Config::is_initialized_--;
  if (ACE_Service_Config::is_initialized_ > 0)
    return 0;

  // Delete the service repository.  All the objects inside the
  // service repository should already have been finalized.
  ACE_Service_Config::close_svcs ();

  // Delete the list fo svc.conf files
  delete ACE_Service_Config::svc_conf_file_queue_;
  ACE_Service_Config::svc_conf_file_queue_ = 0;

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
ACE_Service_Config::fini_svcs (void)
{
  ACE_TRACE ("ACE_Service_Config::fini_svcs");

  // Clear the LM_DEBUG bit from log messages if appropriate
  if (ACE::debug ())
    ACE_Log_Msg::disable_debug_messages ();

  int result = 0;
  if (ACE_Service_Repository::instance () != 0)
    result = ACE_Service_Repository::instance ()->fini ();

  // Since the fini() method of the objects inside the service
  // repository may reference the ACE singletons, they must be
  // destroyed after the objects have been finalized.
  ACE_Service_Config::close_singletons ();

  if (ACE::debug ())
    ACE_Log_Msg::enable_debug_messages ();

  return result;
}

int
ACE_Service_Config::close_singletons (void)
{
  ACE_TRACE ("ACE_Service_Config::close_singletons");

  ACE_Reactor::close_singleton ();

#if ! defined (ACE_THREAD_MANAGER_LACKS_STATICS)
  ACE_Thread_Manager::close_singleton ();
#endif /* ! ACE_THREAD_MANAGER_LACKS_STATICS */

  return 0;
}

// Perform user-specified close activities and remove dynamic memory.

ACE_Service_Config::~ACE_Service_Config (void)
{
  ACE_TRACE ("ACE_Service_Config::~ACE_Service_Config");
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

// Become a daemon (i.e., run as a "background" process).

int
ACE_Service_Config::start_daemon (void)
{
  ACE_TRACE ("ACE_Service_Config::start_daemon");
  return ACE::daemonize ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array<ACE_Get_Opt::ACE_Get_Opt_Long_Option *>;
template class ACE_Array_Base<ACE_Get_Opt::ACE_Get_Opt_Long_Option *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Array<ACE_Get_Opt::ACE_Get_Opt_Long_Option *>
#pragma instantiate ACE_Array_Base<ACE_Get_Opt::ACE_Get_Opt_Long_Option *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
