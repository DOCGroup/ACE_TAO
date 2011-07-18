// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_home.cpp
//
// = DESCRIPTION
//    Extension of class AST_Home that provides additional means for C++
//    mapping of a component home.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_home.h"
#include "be_component.h"
#include "be_interface_strategy.h"
#include "be_visitor.h"
#include "global_extern.h"
#include "utl_err.h"

ACE_RCSID (be,
           be_home,
           "$Id$")

be_home::be_home (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    AST_Home (),
    be_scope (),
    be_type (),
    be_interface ()
{
  this->size_type (AST_Type::VARIABLE);
}

be_home::be_home (UTL_ScopedName *n,
                  AST_Home *base_home,
                  AST_Component *managed_component,
                  AST_ValueType *primary_key,
                  AST_Interface **supports,
                  long n_supports,
                  AST_Interface **supports_flat,
                  long n_supports_flat)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_home,
              n),
    AST_Type (AST_Decl::NT_home,
              n),
    UTL_Scope (AST_Decl::NT_home),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   false,
                   false),
    AST_Home (n,
              base_home,
              managed_component,
              primary_key,
              supports,
              n_supports,
              supports_flat,
              n_supports_flat),
    be_scope (AST_Decl::NT_home),
    be_type (AST_Decl::NT_home,
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

  // Some previous error may have caused a lookup failure, in which
  // case we'll crash if we do the narrow below.
  if (managed_component == 0)
    {
      idl_global->set_err_count (idl_global->err_count () + 1);
      return;
    }

  be_component *bt =
     be_component::narrow_from_decl (managed_component);

  bt->seen_in_operation (true);

  idl_global->object_arg_seen_ = true;
}

be_home::~be_home (void)
{
}

void
be_home::destroy (void)
{
  // Skip be_interface, since our decls were added to
  // the equivalent interface and will get destroyed
  // there.
  if (this->strategy_ != 0)
    {
      this->strategy_->destroy ();
      delete this->strategy_;
      this->strategy_ = 0;
    }
    
  this->be_scope::destroy ();
  this->be_type::destroy ();
  
  // This skips AST_Interface, for the reason above.
  this->AST_Home::destroy ();
}

int
be_home::accept (be_visitor *visitor)
{
  if (idl_global->ignore_idl3 ())
    {
      idl_global->err ()->ignore_idl3_error (this);
      return -1;
    }

  return visitor->visit_home (this);
}



IMPL_NARROW_FROM_DECL (be_home)
IMPL_NARROW_FROM_SCOPE (be_home)
