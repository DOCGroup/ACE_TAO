/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Identity_i.cpp
//
// = DESCRIPTION
//    Implements the interface in Identity.idl.
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================
#include "Identity_i.h"

Identity_i::Identity_i (const char *name,
                        PortableServer::POA_ptr poa)
  : name_ (name),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

Identity_i::~Identity_i (void)
{
}

void
Identity_i::get_name (CORBA::String_out name
                      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  name = this->name_.in ();
}

PortableServer::POA_ptr
Identity_i::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
