TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
CORBA::UserException::UserException () = default;

ACE_INLINE
CORBA::UserException::UserException (char const * repository_id,
                                     char const * local_name)
  : CORBA::Exception (repository_id,
                      local_name)
{
}

ACE_INLINE
CORBA::UserException::UserException (CORBA::UserException const &rhs) = default;

ACE_INLINE CORBA::UserException *
CORBA::UserException::_downcast (CORBA::Exception * exception)
{
  return dynamic_cast<CORBA::UserException *> (exception);
}

ACE_INLINE CORBA::UserException const *
CORBA::UserException::_downcast (CORBA::Exception const * exception)
{
  return dynamic_cast<CORBA::UserException const *> (exception);
}

TAO_END_VERSIONED_NAMESPACE_DECL
