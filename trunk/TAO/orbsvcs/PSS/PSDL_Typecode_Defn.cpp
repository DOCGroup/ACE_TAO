// $Id$

#include "PSDL_Typecode_Defn.h"
#include "PSDL_Stream.h"




TAO_PSDL_Typecode_Defn::TAO_PSDL_Typecode_Defn ()
{
  // Constructor
}

TAO_PSDL_Typecode_Defn::~TAO_PSDL_Typecode_Defn ()
{
  // Destructor
}

int
TAO_PSDL_Typecode_Defn::visit_type_declarator (TAO_PSDL_Type_Declarator *type_declarator)
{
  ACE_UNUSED_ARG (type_declarator);
  // Get a pointer to PSDL_Stream of the stub implementation file
  TAO_PSDL_Stream *ps_si = TAO_PSDL_Scope::instance ()->get_si ();

  ps_si->indent (); // start from current indentation level

  // Generate the typecode information here
  *ps_si << "static const CORBA::Long _oc_";

  // Flat name generation.
  return 0;
}
