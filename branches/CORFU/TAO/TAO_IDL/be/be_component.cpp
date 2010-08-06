// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_component.cpp
//
// = DESCRIPTION
//    Extension of class AST_Component that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_component.h"
#include "be_visitor.h"

#include "global_extern.h"
#include "utl_err.h"

ACE_RCSID (be,
           be_component,
           "$Id$")

be_component::be_component (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    AST_Component (),
    be_scope (),
    be_type (),
    be_interface ()
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (true);
}

be_component::be_component (UTL_ScopedName *n,
                            AST_Component *base_component,
                            AST_Interface **supports,
                            long n_supports,
                            AST_Interface **supports_flat,
                            long n_supports_flat)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_component,
              n),
    AST_Type (AST_Decl::NT_component,
              n),
    UTL_Scope (AST_Decl::NT_component),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   false,
                   false),
    AST_Component (n,
                   base_component,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat),
    be_scope (AST_Decl::NT_component),
    be_type (AST_Decl::NT_component,
             n),
    be_interface (n,
                  supports,
                  n_supports,
                  supports_flat,
                  n_supports_flat,
                  false,
                  false)
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (true);
}

be_component::~be_component (void)
{
}

void
be_component::redefine (AST_Interface *from)
{
  be_component *bc = be_component::narrow_from_decl (from);
  this->var_out_seq_decls_gen_ = bc->var_out_seq_decls_gen_;
  AST_Component::redefine (from);
}

void
be_component::destroy ()
{
  this->be_interface::destroy ();
  this->AST_Component::destroy ();
}

int
be_component::accept (be_visitor *visitor)
{
  if (idl_global->ignore_idl3 ())
    {
      idl_global->err ()->ignore_idl3_error (this);
      return -1;
    }

  return visitor->visit_component (this);
}

AST_Structure *
be_component::be_add_structure (AST_Structure *t)
{
  return this->fe_add_structure (t);
}

AST_Typedef *
be_component::be_add_typedef (AST_Typedef *t)
{
  return this->fe_add_typedef (t);
}



IMPL_NARROW_FROM_DECL (be_component)
IMPL_NARROW_FROM_SCOPE (be_component)
