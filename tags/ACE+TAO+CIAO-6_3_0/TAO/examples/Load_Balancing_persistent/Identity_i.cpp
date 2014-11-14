/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Identity_i.cpp
 *
 *  $Id$
 *
 *  Implements the interface in Identity.idl.
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

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
Identity_i::get_name (CORBA::String_out name)
{
  name = this->name_.in ();
}

PortableServer::POA_ptr
Identity_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
