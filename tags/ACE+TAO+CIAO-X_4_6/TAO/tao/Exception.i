// -*- C++ -*-
//
// $Id$

ACE_INLINE CORBA::Exception *
CORBA::Exception::_downcast (CORBA::Exception * x)
{
  return x;
}

ACE_INLINE CORBA::Exception const *
CORBA::Exception::_downcast (CORBA::Exception const * x)
{
  return x;
}
