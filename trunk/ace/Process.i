/* -*- C++ -*- */
// $Id$

#if defined (ACE_WIN32)
ACE_INLINE PROCESS_INFORMATION 
ACE_Process::process_info (void)
{
  return process_info_;
}
#endif /* ACE_WIN32 */


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
#if defined (ACE_WIN32) || defined (CHORUS)
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#else
  return ACE_OS::kill (this->getpid (), signum);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
ACE_Process::terminate (void)
{
  return ACE::terminate_process (this->getpid ());
}

// ************************************************************

ACE_INLINE u_long
ACE_Process_Options::creation_flags (void) const
{
#if defined (UNICODE)  
  return creation_flags_ | CREATE_UNICODE_ENVIRONMENT;
#else
  return creation_flags_;
#endif /* UNICODE */
}

ACE_INLINE void
ACE_Process_Options::creation_flags (u_long cf)
{
  creation_flags_ = cf;
}

// ******************************
#if defined (ACE_WIN32)

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


ACE_INLINE int
ACE_Process_Options::handle_inheritence (void)
{
  return handle_inheritence_;
}

ACE_INLINE void
ACE_Process_Options::handle_inheritence (int hi)
{
  handle_inheritence_ = hi;
}

#else /* !defined (ACE_WIN32) */

ACE_INLINE ACE_HANDLE
ACE_Process_Options::get_stdin (void)
{
  return stdin_;
}

ACE_INLINE ACE_HANDLE 
ACE_Process_Options::get_stdout (void)
{
  return stdout_;
}

ACE_INLINE ACE_HANDLE 
ACE_Process_Options::get_stderr (void)
{
  return stderr_;
}

#endif /* ACE_WIN32 */

ACE_INLINE LPTSTR
ACE_Process_Options::command_line_buf (void)
{
  return command_line_buf_;
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
ACE_Process_Options::working_directory (LPCTSTR wd)
{
  ACE_OS::strcpy (working_directory_, wd);
}
