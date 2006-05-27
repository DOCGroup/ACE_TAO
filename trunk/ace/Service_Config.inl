// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// This is the primary entry point into the ACE_Service_Config (the
// constructor just handles simple initializations).

ACE_INLINE int
ACE_Service_Config::open (const ACE_TCHAR program_name[],
                          const ACE_TCHAR *logger_key,
                          int ignore_static_svcs,
                          int ignore_default_svc_conf,
                          int ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Config::open");
  return ACE_Service_Config::current()->open (program_name,
                                              logger_key,
                                              ignore_static_svcs,
                                              ignore_default_svc_conf,
                                              ignore_debug_flag);
}


ACE_INLINE int
ACE_Service_Config::open (int argc,
                          ACE_TCHAR *argv[],
                          const ACE_TCHAR *logger_key,
                          int ignore_static_svcs,
                          int ignore_default_svc_conf,
                          int ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Config::open");
  return ACE_Service_Config::current()->open (argc,
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

// Compare two service descriptors for equality.

ACE_INLINE bool
ACE_Static_Svc_Descriptor::operator== (ACE_Static_Svc_Descriptor &d) const
{
  return ACE_OS::strcmp (name_, d.name_) == 0;
}

// Compare two service descriptors for inequality.

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

// Initialize and activate a statically linked service.

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_TCHAR *svc_name,
                                const ACE_TCHAR *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  return ACE_Service_Config::current ()->initialize (svc_name,
                                                     parameters);
}

// Dynamically link the shared object file and retrieve a pointer to
// the designated shared object in this file.

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_Service_Type *sr,
                                const ACE_TCHAR *parameters)
{
  ACE_TRACE ("ACE_Service_Config::initialize");
  return ACE_Service_Config::current ()->initialize (sr,
                                                     parameters);
}


ACE_INLINE int
ACE_Service_Config::process_directive (const ACE_TCHAR directive[])
{
  return ACE_Service_Config::current ()->process_directive (directive);
}

// Process service configuration requests as indicated in the queue of
// svc.conf files.
ACE_INLINE int
ACE_Service_Config::process_directives (void)
{
  return ACE_Service_Config::current ()->process_directives ();
}

ACE_INLINE int
ACE_Service_Config::process_directive (const ACE_Static_Svc_Descriptor &ssd,
                                       int force_replace)
{
  return ACE_Service_Config::current ()->process_directive (ssd,
                                                            force_replace);
}


#if defined (ACE_HAS_WINCE) && defined (ACE_USES_WCHAR)
// We must provide these function to bridge Svc_Conf parser with ACE.

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_Service_Type *sp, ACE_ANTI_TCHAR parameters[])
{
  return ACE_Service_Config::initialize (sp, ACE_TEXT_ANTI_TO_TCHAR (parameters));
}

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_ANTI_TCHAR svc_name[], ACE_ANTI_TCHAR parameters[])
{
  return ACE_Service_Config::initialize (ACE_TEXT_ANTI_TO_TCHAR (svc_name),
                                         ACE_TEXT_ANTI_TO_TCHAR (parameters));
}

ACE_INLINE int
ACE_Service_Config::resume (const ACE_ANTI_TCHAR svc_name[])
{
  return ACE_Service_Config::resume (ACE_TEXT_ANTI_TO_TCHAR (svc_name));
}

ACE_INLINE int
ACE_Service_Config::suspend (const ACE_ANTI_TCHAR svc_name[])
{
  return ACE_Service_Config::suspend (ACE_TEXT_ANTI_TO_TCHAR (svc_name));
}

ACE_INLINE int
ACE_Service_Config::remove (const ACE_ANTI_TCHAR svc_name[])
{
  return ACE_Service_Config::remove (ACE_TEXT_ANTI_TO_TCHAR (svc_name));
}
#endif /* ACE_HAS_WINCE && !ACE_USES_WCHAR */

ACE_END_VERSIONED_NAMESPACE_DECL
