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

Identity_i::Identity_i (const char *name)
  : name_ (name)
{
}

Identity_i::~Identity_i (void)
{
}

void
Identity_i::get_name (CORBA::String_out name
 )
{
  name = this->name_.in ();
}
