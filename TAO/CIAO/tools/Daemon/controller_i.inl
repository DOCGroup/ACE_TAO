// -*- C++ -*-
// $Id$

ACE_INLINE
CIAO::controller::controller ()
  : cmd_ (0)
{
}

ACE_INLINE
CIAO::controller::~controller ()
{
  //  this->fini ();
}

ACE_INLINE CIAO::Daemon_ptr
CIAO::controller::daemon (void)
{
  return this->daemon_.in ();
}

ACE_INLINE CORBA::ORB_ptr
CIAO::controller::orb (void)
{
  return this->orb_.in ();
}

ACE_INLINE
CIAO::Command::Command (CIAO::controller *c)
  : controller_ (c)
{
}

ACE_INLINE
CIAO::CMD_Install::CMD_Install (CIAO::controller *c)
  : Command (c)
{
}

ACE_INLINE
CIAO::CMD_Uninstall::CMD_Uninstall (CIAO::controller *c)
  : Command (c)
{
}

ACE_INLINE
CIAO::CMD_Query::CMD_Query (CIAO::controller *c)
  : Command (c)
{
}

ACE_INLINE
CIAO::CMD_Replace::CMD_Replace (CIAO::controller *c)
  : Command (c)
{
}

ACE_INLINE
CIAO::CMD_Get_IOR::CMD_Get_IOR (CIAO::controller *c)
  : Command (c)
{
}

ACE_INLINE
CIAO::CMD_Shutdown::CMD_Shutdown (CIAO::controller *c)
  : Command (c)
{
}
