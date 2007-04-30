// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::UserException::UserException (void)
{
}

ACE_INLINE
CORBA::UserException::UserException (char const * repository_id,
                                     char const * local_name)
  : CORBA::Exception (repository_id,
                      local_name)
{
}

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

TAO_END_VERSIONED_NAMESPACE_DECL
