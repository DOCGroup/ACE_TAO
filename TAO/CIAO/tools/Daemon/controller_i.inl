// -*- C++ -*-
// $Id$

ACE_INLINE
CIAO::Daemon_i::controller::controller ()
  : cmd_ (0)
{
}

ACE_INLINE
CIAO::Daemon_i::controller::~controller ()
{
  //  this->fini ();
}

ACE_INLINE CIAO::Daemon_ptr
CIAO::Daemon_i::controller::daemon (void)
{
  return this->daemon_.in ();
}

ACE_INLINE CORBA::ORB_ptr
CIAO::Daemon_i::controller::orb (void)
{
  return this->orb_.in ();
}

ACE_INLINE
CIAO::Daemon_i::Command_Base::Command_Base (CIAO::Daemon_i::controller *c)
  : controller_ (c)
{
}
