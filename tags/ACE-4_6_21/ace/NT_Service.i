/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_NT_Service::ACE_NT_Service (DWORD start_timeout,
                                DWORD service_type,
                                DWORD controls_mask) :
                                 start_time_(start_timeout),
                                 svc_handle_(0),
                                 svc_sc_handle_(0),
                                 name_(0),
                                 desc_(0)
{
  svc_status_.dwServiceType = service_type;
  svc_status_.dwCurrentState = 0;
  svc_status_.dwControlsAccepted = controls_mask;
  svc_status_.dwWin32ExitCode = NO_ERROR;
  svc_status_.dwServiceSpecificExitCode = 0;
  svc_status_.dwCheckPoint = 0;
}


ACE_INLINE
ACE_NT_Service::ACE_NT_Service (LPCTSTR name,
                                LPCTSTR desc,
                                DWORD start_timeout,
                                DWORD service_type,
                                DWORD controls_mask) :
                                 start_time_(start_timeout),
                                 svc_handle_(0),
                                 svc_sc_handle_(0),
                                 name_(ACE::strnew(name)),
                                 desc_(ACE::strnew(desc))
{
  svc_status_.dwServiceType = service_type;
  svc_status_.dwCurrentState = 0;
  svc_status_.dwControlsAccepted = controls_mask;
  svc_status_.dwWin32ExitCode = NO_ERROR;
  svc_status_.dwServiceSpecificExitCode = 0;
  svc_status_.dwCheckPoint = 0;
}


ACE_INLINE int
ACE_NT_Service::svc (void)
{
  return -1;
}


ACE_INLINE
LPCTSTR
ACE_NT_Service::name (void) const
{
  return name_;
}

ACE_INLINE
LPCTSTR
ACE_NT_Service::desc (void) const
{
  return desc_;
}

ACE_INLINE void
ACE_NT_Service::svc_handle(const SERVICE_STATUS_HANDLE new_svc_handle)
{
  this->svc_handle_ = new_svc_handle;
  return;
}
