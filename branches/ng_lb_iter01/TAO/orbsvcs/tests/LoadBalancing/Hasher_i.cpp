// -*- C++ -*-
//
// $Id$

#include "Hasher_i.h"

#include "ace/ACE.h"


CORBA::ULong
Hasher_i::hash_string (const char *str,
                       CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Hash the given string, and return the computed hash.
  return ACE::hash_pjw (str);
}
