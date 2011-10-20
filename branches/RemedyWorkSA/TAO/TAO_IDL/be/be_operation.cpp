
//=============================================================================
/**
 *  @file    be_operation.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Operation that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_operation.h"
#include "be_predefined_type.h"
#include "be_argument.h"
#include "be_visitor.h"
#include "be_util.h"

#include "ast_exception.h"

#include "utl_err.h"
#include "utl_exceptlist.h"

#include "global_extern.h"

be_operation::be_operation (AST_Type *rt,
                            AST_Operation::Flags fl,
                            UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_op,
              n),
    UTL_Scope (AST_Decl::NT_op),
    AST_Operation (rt,
                   fl,
                   n,
                   local,
                   abstract),
    be_scope (AST_Decl::NT_op),
    be_decl (AST_Decl::NT_op,
             n),
    is_sendc_ami_ (false),
    is_excep_ami_ (false),
    is_attr_op_ (false)
{
  if (this->imported ())
    {
      return;
    }

  idl_global->operation_seen_ = true;

  if (!this->is_local ())
    {
      be_type *bt = be_type::narrow_from_decl (rt);
      bt->seen_in_operation (true);
      be_util::set_arg_seen_bit (bt);
      idl_global->non_local_op_seen_ = true;
    }
}

be_operation::~be_operation (void)
{
}

void
be_operation::destroy (void)
{
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

AST_Argument *
be_operation::be_add_argument (AST_Argument *arg)
{
  this->add_to_scope (arg);
  this->add_to_referenced (arg,
                           0,
                           0);
  return arg;
}

int
be_operation::be_insert_exception (AST_Exception *ex)
{
  UTL_ExceptList *new_list = 0;
  ACE_NEW_RETURN (new_list,
                  UTL_ExceptList (ex,
                                  this->pd_exceptions),
                  -1);
  this->pd_exceptions = new_list;
  return 0;
}

bool
be_operation::is_sendc_ami (void) const
{
  return this->is_sendc_ami_;
}

void
be_operation::is_sendc_ami (bool val)
{
  this->is_sendc_ami_ = val;
}

bool
be_operation::is_excep_ami (void) const
{
  return this->is_excep_ami_;
}

void
be_operation::is_excep_ami (bool val)
{
  this->is_excep_ami_ = val;
}

bool
be_operation::is_attr_op (void) const
{
  return this->is_attr_op_;
}

void
be_operation::is_attr_op (bool val)
{
  this->is_attr_op_ = val;
}

IMPL_NARROW_FROM_DECL (be_operation)
IMPL_NARROW_FROM_SCOPE (be_operation)
