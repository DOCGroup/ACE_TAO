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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_operation, "$Id$")

be_operation::be_operation (void)
  : argument_count_ (-1),
    has_native_ (0)
{
  ACE_NEW (this->strategy_,
           be_operation_default_strategy (this));
}

be_operation::be_operation (AST_Type *rt,
                            AST_Operation::Flags fl,
                            UTL_ScopedName *n,
                            UTL_StrList *p,
                            idl_bool local,
                            idl_bool abstract)
  : AST_Operation (rt, 
                   fl, 
                   n, 
                   p, 
                   local, 
                   abstract),
    AST_Decl (AST_Decl::NT_op, 
              n, 
              p),
    UTL_Scope (AST_Decl::NT_op),
    COMMON_Base (local, 
                 abstract),
    argument_count_ (-1),
    has_native_ (0)
{
  ACE_NEW (this->strategy_,
           be_operation_default_strategy (this));
}


be_operation::~be_operation (void)
{
  // We know that it cannot be 0, but..
  if (!this->strategy_)
    {
      delete this->strategy_;
    }
}


// Compute total number of members.
int
be_operation::compute_argument_attr (void)
{
  if (this->argument_count_ != -1)
    {
      return 0;
    }

  this->argument_count_ = 0;

  // If there are elements in this scope.
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      UTL_ScopeActiveIterator *si = 0;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this, 
                                               UTL_Scope::IK_decls),
                      -1);

      while (!si->is_done ())
        {
          // Get the next AST decl node.
          AST_Decl *d = si->item ();

          if (d->node_type () == AST_Decl::NT_argument)
            {
              this->argument_count_++;
              be_argument *arg = be_argument::narrow_from_decl (d);
              be_type* type =
                be_type::narrow_from_decl (arg->field_type ());

              if (type->base_node_type () == AST_Decl::NT_native)
                {
                  this->has_native_ = 1;
                }
            }

          si->next ();
        }

      delete si;
    }

  be_type* type = be_type::narrow_from_decl (this->return_type ());

  if (type->base_node_type () == AST_Decl::NT_native)
    {
      this->has_native_ = 1;
    }

  return 0;
}


int
be_operation::void_return_type (void)
{
  be_type* type = be_type::narrow_from_decl (this->return_type ());

  if (type->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (type)->pt ()
            == AST_PredefinedType::PT_void))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}
// Return the member count.
int
be_operation::argument_count (void)
{
  this->compute_argument_attr ();

  return this->argument_count_;
}

// Return if any argument or the return type is a <native> type.
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
  this->add_to_referenced (arg, 
                           0, 
                           0);
  return arg;
}

// Compute the size type of the node in question.
int
be_operation::compute_size_type (void)
{
  UTL_ScopeActiveIterator *si = 0;
  AST_Decl *d = 0;
  be_decl *bd = 0;

  if (this->nmembers () > 0)
    {
      // If there are elements in this scope,
      // instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this, 
                                               UTL_Scope::IK_decls),
                      -1);

      while (!si->is_done ())
        {
          // Get the next AST decl node
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              // Our sizetype depends on the sizetype of our members. Although
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
        }
      delete si;
    }

  return 0;
}

void
be_operation::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_decl::destroy ();
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
