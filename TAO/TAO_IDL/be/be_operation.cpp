// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation.cpp
//
// = DESCRIPTION
//    Extension of class AST_Operation that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_operation.h"
#include "be_operation_strategy.h"
#include "be_predefined_type.h"
#include "be_argument.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_operation, 
           "$Id$")

be_operation::be_operation (void)
{
  ACE_NEW (this->strategy_,
           be_operation_default_strategy (this));
}

be_operation::be_operation (AST_Type *rt,
                            AST_Operation::Flags fl,
                            UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
  : AST_Operation (rt,
                   fl,
                   n,
                   local,
                   abstract),
    AST_Decl (AST_Decl::NT_op,
              n),
    UTL_Scope (AST_Decl::NT_op),
    COMMON_Base (local,
                 abstract)
{
  ACE_NEW (this->strategy_,
           be_operation_default_strategy (this));

  be_type *bt = be_type::narrow_from_decl (rt);
  bt->seen_in_operation (I_TRUE);
}


be_operation::~be_operation (void)
{
}

void
be_operation::destroy (void)
{
  delete this->strategy_;
  this->strategy_ = 0;

  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_decl::destroy ();
  this->AST_Operation::destroy ();
}

int
be_operation::accept (be_visitor *visitor)
{
  return visitor->visit_operation (this);
}

be_operation_strategy *
be_operation::set_strategy (be_operation_strategy *new_strategy)
{
  be_operation_strategy *old = this->strategy_;

  if (new_strategy != 0)
    {
      this->strategy_ = new_strategy;
    }

  return old;
}


TAO_CodeGen::CG_STATE
be_operation::next_state (TAO_CodeGen::CG_STATE current_state,
                          int is_extra_state)
{
  return this->strategy_->next_state (current_state, is_extra_state);
}

int
be_operation::has_extra_code_generation (TAO_CodeGen::CG_STATE current_state)
{
  return this->strategy_->has_extra_code_generation (current_state);
}

be_operation*
be_operation::marshaling (void)
{
  return this->strategy_->marshaling ();
}

be_operation*
be_operation::arguments (void)
{
  return this->strategy_->arguments ();
}


// Narrowing
IMPL_NARROW_METHODS3 (be_operation, AST_Operation, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_operation)
IMPL_NARROW_FROM_SCOPE (be_operation)
