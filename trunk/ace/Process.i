/* -*- C++ -*- */
// $Id$


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
