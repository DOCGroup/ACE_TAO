// $Id$

#include "tao/UserException.h"

#include "ace/SString.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/UserException.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Virtual.  Do not inline.
CORBA::UserException::~UserException (void)
{
}

CORBA::UserException &
CORBA::UserException::operator= (CORBA::UserException const & rhs)
{
  this->Exception::operator= (rhs);
  return *this;
}

// Virtual.  Do not inline.
CORBA::TypeCode_ptr
CORBA::UserException::_tao_type (void) const
{
  return 0;
}

ACE_CString
CORBA::UserException::_info (void) const
{
  // @@ we can use the exception's typecode to dump all the data held
  // within it ...

  ACE_CString user_exception_info = "user exception, ID '";
  user_exception_info += this->_rep_id ();
  user_exception_info += "'";
  return user_exception_info;
}

TAO_END_VERSIONED_NAMESPACE_DECL
