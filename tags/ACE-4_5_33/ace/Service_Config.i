/* -*- C++ -*- */
// $Id$

// Service_Config.i

// This is the primary entry point into the ACE_Service_Config (the
// constructor just handles simple initializations).

ACE_INLINE int
ACE_Service_Config::open (int argc,
                          ASYS_TCHAR *argv[], 
                          LPCTSTR logger_key)
{
  ACE_TRACE ("ACE_Service_Config::open");
  ACE_Service_Config::parse_args (argc, argv);
  return ACE_Service_Config::open (argv[0], logger_key);
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

ACE_INLINE char
ACE_Service_Config::debug (void)
{
  return ACE_Service_Config::debug_;
}

#if defined (ACE_HAS_WINCE)
  // We must provide these function to bridge Svc_Conf parser with ACE.

ACE_INLINE int
ACE_Service_Config::initialize (const ACE_Service_Type *sp, char parameters[])
{
  wchar_t *wparameters = ACE_const_cast (wchar_t *, ACE_WIDE_STRING (parameters));
  return ACE_Service_Config::initialize (sp, wparameters);
}

ACE_INLINE int
ACE_Service_Config::initialize (const char svc_name[], char parameters[])
{
  const wchar_t *wsvc_name = ACE_WIDE_STRING (svc_name);
  wchar_t *wparameters = ACE_const_cast (wchar_t *, ACE_WIDE_STRING (parameters));
  return ACE_Service_Config::initialize (wsvc_name, wparameters);
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
