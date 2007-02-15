// -*- C++ -*-
//
// $Id$


ACE_BEGIN_VERSIONED_NAMESPACE_DECL


// This is the primary entry point into the ACE_Service_Config (the
// constructor just handles simple initializations).

ACE_INLINE int
ACE_Service_Gestalt::open (const ACE_TCHAR program_name[],
                          const ACE_TCHAR *logger_key,
                          bool ignore_static_svcs,
                          bool ignore_default_svc_conf,
                          bool ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Gestalt::open");
  this->no_static_svcs_ = ignore_static_svcs;

  return this->open_i (program_name,
                       logger_key,
                       ignore_static_svcs,
                       ignore_default_svc_conf,
                       ignore_debug_flag);
}

ACE_INLINE int
ACE_Service_Gestalt::open (int argc,
                          ACE_TCHAR *argv[],
                          const ACE_TCHAR *logger_key,
                          bool ignore_static_svcs,
                          bool ignore_default_svc_conf,
                          bool ignore_debug_flag)
{
  ACE_TRACE ("ACE_Service_Gestalt::open");
  this->no_static_svcs_ = ignore_static_svcs;

  if (this->parse_args_i (argc,
                          argv) == -1)
    return -1;
  else
    return this->open_i (argv == 0 ? 0 : argv[0],
                         logger_key,
                         ignore_static_svcs,
                         ignore_default_svc_conf,
                         ignore_debug_flag);
}

ACE_END_VERSIONED_NAMESPACE_DECL
