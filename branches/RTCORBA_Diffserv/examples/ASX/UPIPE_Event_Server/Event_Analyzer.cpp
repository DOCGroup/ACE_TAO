// $Id$

#include "Event_Analyzer.h"

ACE_RCSID(UPIPE_Event_Server, Event_Analyzer, "$Id$")

#if defined (ACE_HAS_THREADS)

int
Event_Analyzer::open (void *)
{
  return 0;
}

int
Event_Analyzer::close (u_long)
{
  return 0;
}

int
Event_Analyzer::control (ACE_Message_Block *mb)
{
  ACE_IO_Cntl_Msg *ioc = (ACE_IO_Cntl_Msg *) mb->rd_ptr ();
  ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds cmd;

  switch (cmd = ioc->cmd ())
    {
    case ACE_IO_Cntl_Msg::SET_LWM: 
    case ACE_IO_Cntl_Msg::SET_HWM:
      this->water_marks (cmd, *(size_t *) mb->cont ()->rd_ptr ());
      break;
    default:
      break;
    }
  return 0;
}

int 
Event_Analyzer::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    this->control (mb);

  return this->put_next (mb);
}

int 
Event_Analyzer::init (int, char *[])
{
  return 0;
}

int 
Event_Analyzer::fini (void)
{
  return 0;
}

int 
Event_Analyzer::info (char **strp, size_t length) const
{
  const char *mod_name = this->name ();
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);
  return ACE_OS::strlen (mod_name);
}

#endif /* ACE_HAS_THREADS */
