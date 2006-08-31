// $Id$

//
// spawn
//
CUTS_INLINE
pid_t CUTS_Active_Process::spawn (ACE_Process_Options & options)
{
  return this->getpid ();
  ACE_UNUSED_ARG (options);
}
