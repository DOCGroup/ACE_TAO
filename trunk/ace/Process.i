/* -*- C++ -*- */
// $Id$

#if defined (ACE_WIN32)
ACE_INLINE PROCESS_INFORMATION 
ACE_ProcessEx::process_info (void)
{
  return process_info_;
}
#endif /* ACE_WIN32 */


ACE_INLINE pid_t
ACE_ProcessEx::getpid (void)
{
#if defined (ACE_WIN32)
  return process_info_.dwProcessId;
#else /* ACE_WIN32 */
  return child_id_;
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_ProcessEx::kill (int signum)
{
  return ACE_OS::kill (this->getpid (), signum);
}

// ************************************************************

ACE_INLINE pid_t
ACE_Process::getpid (void)
{
#if defined (ACE_WIN32)
  return process_info_.dwProcessId;
#else /* ACE_WIN32 */
  return child_id_;
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_Process::kill (int signum)
{
  return ACE_OS::kill (this->getpid (), signum);
}

// ************************************************************

#if defined (ACE_WIN32)

ACE_INLINE BOOL
ACE_Process_Options::new_console (void) const
{
  return new_console_;
}

ACE_INLINE void
ACE_Process_Options::new_console (BOOL nc)
{
  new_console_ = nc;
}

ACE_INLINE STARTUPINFO *
ACE_Process_Options::startup_info (void)
{
  return &startup_info_;
}

ACE_INLINE const LPSECURITY_ATTRIBUTES 
ACE_Process_Options::get_process_attributes (void) const
{  
  return process_attributes_;
}
 
ACE_INLINE LPSECURITY_ATTRIBUTES 
ACE_Process_Options::set_process_attributes (void)
{
  process_attributes_ = &security_buf1_;
  return process_attributes_;
}
 
ACE_INLINE const LPSECURITY_ATTRIBUTES 
ACE_Process_Options::get_thread_attributes (void) const
{
  return thread_attributes_;
}

ACE_INLINE LPSECURITY_ATTRIBUTES 
ACE_Process_Options::set_thread_attributes (void)
{
  thread_attributes_ = &security_buf2_;
  return thread_attributes_;
}

#else /* !defined (ACE_WIN32) */

ACE_INLINE ACE_HANDLE
ACE_Process_Options::std_in (void)
{
  return stdin_;
}

ACE_INLINE ACE_HANDLE 
ACE_Process_Options::std_out (void)
{
  return stdout_;
}

ACE_INLINE ACE_HANDLE 
ACE_Process_Options::std_err (void)
{
  return stderr_;
}

#endif /* ACE_WIN32 */

ACE_INLINE LPTSTR
ACE_Process_Options::cl_options_buf (void)
{
  return cl_options_;
}

ACE_INLINE LPTSTR 
ACE_Process_Options::working_directory (void)
{
  if (working_directory_[0] == '\0')
    return 0;
  else
    return working_directory_;
}

ACE_INLINE void
ACE_Process_Options::working_directory (const TCHAR *wd)
{
  ACE_OS::strcpy (working_directory_, wd);
}

ACE_INLINE LPCTSTR
ACE_Process_Options::path (void) const
{
  return path_;
}

ACE_INLINE void
ACE_Process_Options::path (LPCTSTR p)
{
  ACE_OS::strcpy (path_, p);
}

#if defined (ACE_WIN32)
ACE_INLINE BOOL
ACE_Process_Options::handle_inheritence (void)
{
  return handle_inheritence_;
}

ACE_INLINE void
ACE_Process_Options::handle_inheritence (BOOL hi)
{
  handle_inheritence_ = hi;
}
#endif /* ACE_WIN32 */
