// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_structure.cpp
//
// = DESCRIPTION
//    Extension of class AST_Structure that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_structure.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_visitor.h"
#include "be_extern.h"
#include "ast_field.h"
#include "utl_identifier.h"
#include "idl_defines.h"

ACE_RCSID (be, 
           be_structure, 
           "$Id$")

be_structure::be_structure (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    UTL_Scope (),
    AST_Structure (),
    be_scope (),
    be_decl (),
    be_type ()
{
}

be_structure::be_structure (UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_struct,
              n),
    AST_Type (AST_Decl::NT_struct,
              n),
    AST_ConcreteType (AST_Decl::NT_struct,
                      n),
    UTL_Scope (AST_Decl::NT_struct),
    AST_Structure (n,
                   local,
                   abstract),
    be_scope (AST_Decl::NT_struct),
    be_decl (AST_Decl::NT_struct,
             n),
    be_type (AST_Decl::NT_struct,
             n)
{
}

void
be_structure::redefine (AST_Structure *from)
{
  be_structure *bs = be_structure::narrow_from_decl (from);
  this->common_varout_gen_ = bs->common_varout_gen_;
  AST_Structure::redefine (from);
}

void
be_structure::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
  AST_Decl::destroy ();
}

int
be_structure::accept (be_visitor *visitor)
{
  return visitor->visit_structure (this);
}

AST_Field *
be_structure::be_add_field (AST_Field *f)
{
  return this->fe_add_field (f);
}

// Narrowing.
IMPL_NARROW_METHODS3 (be_structure, AST_Structure, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_structure)
IMPL_NARROW_FROM_SCOPE (be_structure)
