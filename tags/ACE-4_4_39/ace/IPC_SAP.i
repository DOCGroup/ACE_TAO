/* -*- C++ -*- */
// $Id$

// IPC_SAP.i

// Used to return the underlying handle_. 

inline ACE_HANDLE
ACE_IPC_SAP::get_handle (void) const
{
  ACE_TRACE ("ACE_IPC_SAP::get_handle");
  return this->handle_;
}

// Used to set the underlying handle_. 

inline void
ACE_IPC_SAP::set_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_IPC_SAP::set_handle");
  this->handle_ = handle;
}

// Provides access to the ACE_OS::ioctl system call. 

inline int 
ACE_IPC_SAP::control (int cmd, void *arg) const
{
  ACE_TRACE ("ACE_IPC_SAP::control");
  return ACE_OS::ioctl (this->handle_, cmd, arg);
}
