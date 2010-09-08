// $Id$

#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"
#include "Options.h"
#include "Event_Analyzer.h"



int
Event_Analyzer::open (void *)
{
  // No-op for now...
  return 0;
}

int
Event_Analyzer::close (u_long)
{
  // No-op for now...
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
  if (Options::instance ()->debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) passing through Event_Analyser::put() (%s)\n"),
                this->is_reader () ? ACE_TEXT ("reader") : ACE_TEXT ("writer")));

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    this->control (mb);

  // Just pass the message along to the next Module in the stream...
  return this->put_next (mb);
}

int
Event_Analyzer::init (int, ACE_TCHAR *[])
{
  // No-op for now.
  return 0;
}

int
Event_Analyzer::fini (void)
{
  // No-op for now.
  return 0;
}

int
Event_Analyzer::info (ACE_TCHAR **strp, size_t length) const
{
  const ACE_TCHAR *mod_name = this->name ();

  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (mod_name));
}
