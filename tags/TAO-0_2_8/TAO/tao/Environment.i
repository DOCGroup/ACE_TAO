// This may look like C, but it's really -*- C++ -*-
// $Id$

ACE_INLINE CORBA_Exception*
CORBA_Environment::exception (void) const
{
  return this->exception_;
}

