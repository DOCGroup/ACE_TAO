// $Id$

#include "Active_Process.h"

#if !defined (__CUTS_INLINE__)
#include "Active_Process.inl"
#endif

//
// CUTS_Active_Process
//
CUTS_Active_Process::CUTS_Active_Process (pid_t pid)
{
#if defined (ACE_WIN32)
  this->process_info_.dwProcessId = pid;
  this->process_info_.hProcess = ::OpenProcess (PROCESS_TERMINATE |
                                                PROCESS_QUERY_INFORMATION |
                                                SYNCHRONIZE,
                                                FALSE,
                                                pid);
#else
  this->child_id_ = pid;
#endif
}

//
// ~CUTS_Active_Process
//
CUTS_Active_Process::~CUTS_Active_Process (void)
{

}
