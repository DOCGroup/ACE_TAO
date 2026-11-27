#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/// This is the primary entry point into the ACE_Service_Config (the
/// constructor just handles simple initializations).
ACE_INLINE int
ACE_Service_Config::open (ACE_TCHAR const program_name[],
                          ACE_TCHAR const *logger_key,
                          bool ignore_static_svcs,
                          bool ignore_default_svc_conf,
                          bool ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Config::open");
  if (singleton()->open_i (program_name,
                           logger_key,
                           ignore_static_svcs,
                           ignore_default_svc_conf,
                           ignore_debug_flag) == -1)
    {
      return -1;
    }

  return current()->open (program_name,
                          logger_key,
                          ignore_static_svcs,
                          ignore_default_svc_conf,
                          ignore_debug_flag);
}

ACE_INLINE int
ACE_Service_Config::open (int argc,
                          ACE_TCHAR *argv[],
                          ACE_TCHAR const *logger_key,
                          bool ignore_static_svcs,
                          bool ignore_default_svc_conf,
                          bool ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Config::open");

  if (singleton()->parse_args_i(argc, argv) == -1)
    {
      return -1;
    }

  if (singleton()->open_i (argv[0],
                           logger_key,
                           ignore_static_svcs,
                           ignore_default_svc_conf,
                           ignore_debug_flag) == -1)
    {
      return -1;
    }

  return current()->open (argc,
                          argv,
                          logger_key,
                          ignore_static_svcs,
                          ignore_default_svc_conf,
                          ignore_debug_flag);
}

// Handle the command-line options intended for the
// ACE_Service_Config.
ACE_INLINE int
ACE_Service_Config::parse_args (int argc, ACE_TCHAR *argv[])
{
  return ACE_Service_Config::current ()->parse_args (argc, argv);
}

/// Return the global configuration instance. Allways returns the same
/// instance
ACE_INLINE ACE_Service_Gestalt *
ACE_Service_Config::global ()
{
  return ACE_Service_Config::singleton()->instance_.get ();
}

/// Return the configuration instance, considered "global" in the
/// current thread. This may be the same as instance(), but on some
/// occasions, it may be a different one. For example,
/// ACE_Service_Config_Guard provides a way of temporarily replacing
/// the "current" configuration instance in the context of a thread.
ACE_INLINE ACE_Service_Gestalt *
ACE_Service_Config::instance ()
{
  return ACE_Service_Config::current ();
}

/// This method now returns the service gestalt instead of exposing the
/// underlying repository container. Existing code can continue to use
/// ACE_Service_Config::static_svcs() through
/// ACE_Service_Gestalt::insert (ACE_Static_Svc_Descriptor*), without
/// exposing repository storage. This also makes service registration
/// problems easier to debug.

ACE_INLINE ACE_Service_Gestalt*
ACE_Service_Config::static_svcs ()
{
  return ACE_Service_Config::current ();
}

/// Compare two service descriptors for equality.
ACE_INLINE bool
ACE_Static_Svc_Descriptor::operator== (ACE_Static_Svc_Descriptor &d) const
{
  return ACE_OS::strcmp (name_, d.name_) == 0;
}

/// Compare two service descriptors for inequality.
ACE_INLINE bool
ACE_Static_Svc_Descriptor::operator!= (ACE_Static_Svc_Descriptor &d) const
{
  return !(*this == d);
}

ACE_INLINE void
ACE_Service_Config::signal_handler (ACE_Sig_Adapter *signal_handler)
{
  signal_handler_ = signal_handler;
}

/// Initialize and activate a statically linked service.
ACE_INLINE int
ACE_Service_Config::initialize (ACE_TCHAR const *svc_name,
                                ACE_TCHAR const *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  return ACE_Service_Config::current ()->initialize (svc_name,
                                                     parameters);
}

/// Dynamically link the shared object file and retrieve a pointer to
/// the designated shared object in this file.
ACE_INLINE int
ACE_Service_Config::initialize (ACE_Service_Type const *sr,
                                ACE_TCHAR const *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  return ACE_Service_Config::current ()->initialize (sr, parameters);
}

/// Process a file containing a list of service configuration
/// directives.
ACE_INLINE int ACE_Service_Config::process_file (ACE_TCHAR const file[])
{
  return ACE_Service_Config::current ()->process_file (file);
}

///
ACE_INLINE int
ACE_Service_Config::process_directive (ACE_TCHAR const directive[])
{
  return ACE_Service_Config::current ()->process_directive (directive);
}

/// Process service configuration requests as indicated in the queue of
/// svc.conf files.
ACE_INLINE int
ACE_Service_Config::process_directives ()
{
  return ACE_Service_Config::current ()->process_directives (false);
}

ACE_INLINE int
ACE_Service_Config::process_directive (ACE_Static_Svc_Descriptor const &ssd,
                                       bool force_replace)
{
  return ACE_Service_Config::current ()->process_directive (ssd, force_replace);
}

ACE_END_VERSIONED_NAMESPACE_DECL
