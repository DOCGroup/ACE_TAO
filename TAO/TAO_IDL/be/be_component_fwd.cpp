
//=============================================================================
/**
 *  @file    be_component_fwd.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_ComponentFwd that provides additional means for C++
 *  mapping of a forward declared component.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_component_fwd.h"
#include "be_visitor.h"
#include "global_extern.h"
#include "utl_err.h"

be_component_fwd::be_component_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_component_fwd,
              n),
    AST_Type (AST_Decl::NT_component_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    AST_ComponentFwd (dummy,
                      n),
    be_decl (AST_Decl::NT_component_fwd,
             n),
    be_type (AST_Decl::NT_component_fwd,
             n),
    be_interface_fwd (dummy,
                      n)
{
  this->size_type (AST_Type::VARIABLE);
}

be_component_fwd::~be_component_fwd (void)
{
}

void
be_component_fwd::destroy (void)
{
  this->AST_ComponentFwd::destroy ();
}

int
be_component_fwd::accept (be_visitor *visitor)
{
  return (idl_global->ignore_idl3 ()
            ? 0
            : visitor->visit_component_fwd (this));
}



IMPL_NARROW_FROM_DECL (be_component_fwd)
