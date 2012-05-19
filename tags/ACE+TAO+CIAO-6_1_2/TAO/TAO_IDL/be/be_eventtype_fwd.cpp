
//=============================================================================
/**
 *  @file    be_eventtype_fwd.cpp
 *
 *  $Id$
 *
 *  Extension of class be_valuetype_fwd that provides additional
 *  means for C++ mapping of an eventtype.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_eventtype_fwd.h"
#include "be_visitor.h"

#include "ast_interface.h"

#include "global_extern.h"

be_eventtype_fwd::be_eventtype_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (dummy->is_local (),
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_eventtype_fwd,
              n),
    AST_Type (AST_Decl::NT_eventtype_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    be_decl (AST_Decl::NT_eventtype_fwd,
             n),
    be_type (AST_Decl::NT_eventtype_fwd,
             n),
    be_interface_fwd (dummy,
                      n),
    AST_ValueTypeFwd (dummy,
                      n),
    be_valuetype_fwd (dummy,
                      n),
    AST_EventTypeFwd (dummy,
                      n)
{
}

be_eventtype_fwd::~be_eventtype_fwd (void)
{
}

int
be_eventtype_fwd::accept (be_visitor *visitor)
{
  return (idl_global->ignore_idl3 ()
            ? 0
            : visitor->visit_eventtype_fwd (this));
}

void
be_eventtype_fwd::destroy (void)
{
  this->be_valuetype_fwd::destroy ();
}



IMPL_NARROW_FROM_DECL (be_eventtype_fwd)

