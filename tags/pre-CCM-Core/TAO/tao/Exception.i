// This may look like C, but it's really -*- C++ -*-
// $Id$

ACE_INLINE CORBA_Exception*
CORBA_Exception::_downcast (CORBA_Exception* x)
{
  return x;
}

ACE_INLINE
CORBA_UserException::CORBA_UserException (const CORBA_UserException &src)
  : CORBA_Exception (src)
{
}

ACE_INLINE CORBA::ULong
CORBA_SystemException::minor (void) const
{
  return this->minor_;
}

ACE_INLINE void
CORBA_SystemException::minor (CORBA::ULong m)
{
  this->minor_ = m;
}

ACE_INLINE CORBA::CompletionStatus
CORBA_SystemException::completed (void) const
{
  return this->completed_;
}

ACE_INLINE void
CORBA_SystemException::completed (CORBA::CompletionStatus c)
{
  this->completed_ = c;
}

