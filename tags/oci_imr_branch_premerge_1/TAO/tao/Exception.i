// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Exception*
CORBA::Exception::_downcast (CORBA::Exception* x)
{
  return x;
}

ACE_INLINE
CORBA::UserException::UserException (const CORBA::UserException &src)
  : CORBA::Exception (src)
{
}

ACE_INLINE CORBA::ULong
CORBA::SystemException::minor (void) const
{
  return this->minor_;
}

ACE_INLINE void
CORBA::SystemException::minor (CORBA::ULong m)
{
  this->minor_ = m;
}

ACE_INLINE CORBA::CompletionStatus
CORBA::SystemException::completed (void) const
{
  return this->completed_;
}

ACE_INLINE void
CORBA::SystemException::completed (CORBA::CompletionStatus c)
{
  this->completed_ = c;
}
