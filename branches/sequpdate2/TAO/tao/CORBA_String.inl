// -*- C++ -*-
//
// $Id$

// ----------------------------------------------------------------------
// String_var type
// ----------------------------------------------------------------------

#include "ace/OS_NS_string.h"
// #include "ace/OS_NS_wchar.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO::String_Var_Equal_To::operator() (CORBA::String_var const & lhs,
                                      CORBA::String_var const & rhs) const
{
  return (ACE_OS::strcmp (lhs.in (), rhs.in ()) == 0);
}

ACE_INLINE bool
TAO::String_Var_Equal_To::operator() (CORBA::WString_var const & lhs,
                                      CORBA::WString_var const & rhs) const
{
  return (ACE_OS::strcmp (lhs.in (), rhs.in ()) == 0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
