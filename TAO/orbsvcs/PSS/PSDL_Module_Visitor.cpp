// -*- C++ -*-
// $Id$

#include "PSDL_Module_Visitor.h"
#include "PSDL_Scope.h"
#include "PSDL_Stream.h"



TAO_PSDL_Module_Visitor::TAO_PSDL_Module_Visitor (void)
{
}

TAO_PSDL_Module_Visitor::~TAO_PSDL_Module_Visitor (void)
{
}

int
TAO_PSDL_Module_Visitor::visit_module (TAO_PSDL_Module *module)
{
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  // The visitor method for the modules. We invoke the accept method
  // of the TAO_PSDL_Module. This method is going to invoke the chain
  // of visitor methods for members in the module scope.
  *ps_sh << "TAO_NAMESPACE "
         << module->identifier_name ().c_str ()
         << " {";

  ps_sh->nl ();

  ps_sh->incr_indent ();
  TAO_PSDL_Scope::instance ()->set_name_space (module->identifier_name ());

  if (module->specification ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) PSDL_Module_Visitor::"
                         "visit_module - "
                         "codegen for scope failed\n"), -1);
    }

  ps_sh->decr_indent ();
  ps_sh->nl ();

  *ps_sh << "} TAO_NAMESPACE_CLOSE // module "
         << module->identifier_name ().c_str ()
         << "\n\n";

  return 0;
}
