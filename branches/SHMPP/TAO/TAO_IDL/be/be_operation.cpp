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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_operation, "$Id$")

/*
 * BE_Operation
 */
be_operation::be_operation (void)
  : argument_count_ (-1),
    has_native_ (0),
    strategy_ (new be_operation_default_strategy (this))
{
}

be_operation::be_operation (AST_Type *rt, AST_Operation::Flags fl,
                            UTL_ScopedName *n, UTL_StrList *p)
  : AST_Operation (rt, fl, n, p),
    AST_Decl (AST_Decl::NT_op, n, p),
    UTL_Scope (AST_Decl::NT_op),
    argument_count_ (-1),
    has_native_ (0),
    strategy_ (new be_operation_default_strategy (this))
{
}


be_operation::~be_operation (void)
{
  // We know that it cannot be 0, but..
  if (!this->strategy_)
    delete this->strategy_;
}


// compute total number of members
int
be_operation::compute_argument_attr (void)
{
  if (this->argument_count_ != -1)
    return 0;

  this->argument_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      UTL_ScopeActiveIterator *si =
	      new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
        {
          // get the next AST decl node
	        AST_Decl *d = si->item ();

          if (d->node_type () == AST_Decl::NT_argument)
	          {
              this->argument_count_++;
              be_argument *arg = be_argument::narrow_from_decl (d);
              be_type* type =
                be_type::narrow_from_decl (arg->field_type ());

              if (type->base_node_type () == AST_Decl::NT_native)
                this->has_native_ = 1;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }

  be_type* type = be_type::narrow_from_decl (this->return_type ());
  if (type->base_node_type () == AST_Decl::NT_native)
    this->has_native_ = 1;

  return 0;
}


int 
be_operation::void_return_type ()
{
  be_type* type = be_type::narrow_from_decl (this->return_type ());
  if (type->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (type)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}
// return the member count
int
be_operation::argument_count (void)
{
  this->compute_argument_attr ();

  return this->argument_count_;
}

// return if any argument or the return type is a <native> type.
int
be_operation::has_native (void)
{
  this->compute_argument_attr ();

  return this->has_native_;
}

be_argument *
be_operation::add_argument_to_scope (be_argument *arg)
{
  this->add_to_scope (arg);
  this->add_to_referenced (arg, 0, 0);
  return arg;
}

// compute the size type of the node in question
int
be_operation::compute_size_type (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              // our sizetype depends on the sizetype of our members. Although
              // previous value of sizetype may get overwritten, we are
              // guaranteed by the "size_type" call that once the value reached
              // be_decl::VARIABLE, nothing else can overwrite it.
              this->size_type (bd->size_type ());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l) be_operation::compute_size_type - "
                          "narrow_from_decl returned 0\n"));
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
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
    this->strategy_ = new_strategy;

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
be_operation::marshaling ()
{
  return this->strategy_->marshaling ();
}

be_operation*
be_operation::arguments ()
{
  return this->strategy_->arguments ();
}


// Narrowing
IMPL_NARROW_METHODS3 (be_operation, AST_Operation, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_operation)
IMPL_NARROW_FROM_SCOPE (be_operation)
