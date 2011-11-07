// -*- C++ -*-
// $Id$

#include "tao/GIOP_Message_Generator_Parser_11.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Octet
TAO_GIOP_Message_Generator_Parser_11::minor_version (void) const
{
  return static_cast<CORBA::Octet> (1);
}

TAO_END_VERSIONED_NAMESPACE_DECL
