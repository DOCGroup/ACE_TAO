// -*- C++ -*-
//
// $Id$

ACE_INLINE
CORBA::UserException::UserException (CORBA::UserException const & rhs)
  : CORBA::Exception (rhs)
{
}

ACE_INLINE CORBA::UserException *
CORBA::UserException::_downcast (CORBA::Exception * exception)
{
  return dynamic_cast<CORBA::UserException *> (exception);
}

ACE_INLINE const CORBA::UserException *
CORBA::UserException::_downcast (CORBA::Exception const * exception)
{
  return dynamic_cast<const CORBA::UserException *> (exception);
}

ACE_INLINE CORBA::TypeCode_ptr
CORBA::UserException::_tao_type (void) const
{
  return 0;
}
