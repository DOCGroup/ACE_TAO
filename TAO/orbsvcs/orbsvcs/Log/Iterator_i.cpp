// $Id$

#include "Iterator_i.h"

Iterator_i::Iterator_i (void)
{
}

Iterator_i::~Iterator_i (void)
{
  // Do nothing
}

void
Iterator_i::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

DsLogAdmin::RecordList*
Iterator_i::get (CORBA::ULong position,
                 CORBA::ULong how_many,
                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidParam))
{
  return 0;
}
