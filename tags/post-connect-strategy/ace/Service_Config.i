/* -*- C++ -*- */
// $Id$

// Service_Config.i

// This is the primary entry point into the ACE_Service_Config (the
// constructor just handles simple initializations).

ACE_INLINE int
ACE_Service_Config::open (const ASYS_TCHAR program_name[],
                          LPCTSTR logger_key,
                          int ignore_static_svcs,
                          int ignore_default_svc_conf)
{
  ACE_TRACE ("ACE_Service_Config::open");
  ACE_Service_Config::no_static_svcs_ = ignore_static_svcs;

  return ACE_Service_Config::open_i (program_name,
                                     logger_key,
                                     ignore_default_svc_conf);
}

ACE_INLINE int
ACE_Service_Config::open (int argc,
                          ASYS_TCHAR *argv[],
                          LPCTSTR logger_key,
                          int ignore_static_svcs,
                          int ignore_default_svc_conf)
{
  ACE_TRACE ("ACE_Service_Config::open");
  ACE_Service_Config::no_static_svcs_ = ignore_static_svcs;

  if (ACE_Service_Config::parse_args (argc, argv) == -1)
    return -1;
  else
    return ACE_Service_Config::open_i (argv[0],
                                       logger_key,
                                       ignore_default_svc_conf);
}

// Compare two service descriptors for equality.

ACE_INLINE int
ACE_Static_Svc_Descriptor::operator== (ACE_Static_Svc_Descriptor &d) const
{
  return ACE_OS::strcmp (name_, d.name_) == 0;
}

ACE_INLINE void
ACE_Service_Config::signal_handler (ACE_Sig_Adapter *signal_handler)
{
  signal_handler_ = signal_handler;
}

#if defined (ACE_HAS_WINCE)
  // We must provide these function to bridge Svc_Conf parser with ACE.

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_Service_Type *sp, char parameters[])
{
  return ACE_Service_Config::initialize (sp, ACE_WIDE_STRING (parameters));
}

ACE_INLINE int
ACE_Service_Config::initialize (const char svc_name[], char parameters[])
{
  return ACE_Service_Config::initialize (ACE_WIDE_STRING (svc_name), ACE_WIDE_STRING (parameters));
}

ACE_INLINE int
ACE_Service_Config::resume (const char svc_name[])
{
  return ACE_Service_Config::resume (ACE_WIDE_STRING (svc_name));
}

ACE_INLINE int
ACE_Service_Config::suspend (const char svc_name[])
{
  return ACE_Service_Config::suspend (ACE_WIDE_STRING (svc_name));
}

ACE_INLINE int
ACE_Service_Config::remove (const char svc_name[])
{
  return ACE_Service_Config::remove (ACE_WIDE_STRING (svc_name));
}
#endif /* ACE_HAS_WINCE */
