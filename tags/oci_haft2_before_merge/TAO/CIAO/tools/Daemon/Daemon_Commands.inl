// -*- C++ -*-
// $Id$

ACE_INLINE
CIAO::Daemon_i::CMD_Install::CMD_Install (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}

ACE_INLINE
CIAO::Daemon_i::CMD_Uninstall::CMD_Uninstall (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}

ACE_INLINE
CIAO::Daemon_i::CMD_Query::CMD_Query (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}

ACE_INLINE
CIAO::Daemon_i::CMD_Replace::CMD_Replace (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}

ACE_INLINE
CIAO::Daemon_i::CMD_Get_IOR::CMD_Get_IOR (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}

ACE_INLINE
CIAO::Daemon_i::CMD_Shutdown::CMD_Shutdown (CIAO::Daemon_i::controller *c)
  : Command_Base (c)
{
}
