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
#if defined (UNICODE) && !defined (ACE_HAS_WINCE)
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
#if !defined (ACE_HAS_WINCE)
  return &startup_info_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_Process_Options::get_process_attributes (void) const
{
#if !defined (ACE_HAS_WINCE)
  return process_attributes_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_Process_Options::set_process_attributes (void)
{
#if !defined (ACE_HAS_WINCE)
  process_attributes_ = &security_buf1_;
  return process_attributes_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_Process_Options::get_thread_attributes (void) const
{
#if !defined (ACE_HAS_WINCE)
  return thread_attributes_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE LPSECURITY_ATTRIBUTES
ACE_Process_Options::set_thread_attributes (void)
{
#if !defined (ACE_HAS_WINCE)
  thread_attributes_ = &security_buf2_;
  return thread_attributes_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}


ACE_INLINE int
ACE_Process_Options::handle_inheritence (void)
{
#if !defined (ACE_HAS_WINCE)
  return handle_inheritence_;
#else
  return FALSE;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE void
ACE_Process_Options::handle_inheritence (int hi)
{
#if !defined (ACE_HAS_WINCE)
  handle_inheritence_ = hi;
#else
  ACE_UNUSED_ARG (hi);
#endif /* !ACE_HAS_WINCE */
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

ACE_INLINE int
ACE_Process_Options::avoid_zombies (void)
{
  return avoid_zombies_;
}
ACE_INLINE void
ACE_Process_Options::avoid_zombies (int avoid_zombies)
{
  avoid_zombies_ = avoid_zombies;
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
#if !defined (ACE_HAS_WINCE)
  if (working_directory_[0] == '\0')
    return 0;
  else
    return working_directory_;
#else
  return 0;
#endif /* !ACE_HAS_WINCE */
}

ACE_INLINE void
ACE_Process_Options::working_directory (LPCTSTR wd)
{
#if !defined(ACE_HAS_WINCE)
  ACE_OS::strcpy (working_directory_, wd);
#else
  ACE_UNUSED_ARG (wd);
#endif /* !ACE_HAS_WINCE */
}

#if defined (ACE_HAS_WINCE)
// Here is a collection of inline functions which are defined only
// under CE.  They are not empty on most other platforms.

ACE_INLINE int
ACE_Process_Options::setenv (LPTSTR envp[])
{
  ACE_UNUSED_ARG (envp);
  return -1;
}

ACE_INLINE int
ACE_Process_Options::setenv (LPCTSTR format, ...)
{
  return -1;
}

ACE_INLINE int
ACE_Process_Options::setenv (LPCTSTR variable_name,
			     LPCTSTR format, ...)
{
  return -1;
}

ACE_INLINE int
ACE_Process_Options::set_handles (ACE_HANDLE std_in,
				  ACE_HANDLE std_out,
				  ACE_HANDLE std_err)
{
  ACE_UNUSED_ARG (std_in);
  ACE_UNUSED_ARG (std_out);
  ACE_UNUSED_ARG (std_err);
  return -1;
}

#endif /* ACE_HAS_WINCE */
