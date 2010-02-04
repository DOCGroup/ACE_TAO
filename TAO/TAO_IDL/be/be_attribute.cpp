
//=============================================================================
/**
 *  @file    be_attribute.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Attribute that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================


#include "be_attribute.h"
#include "be_type.h"
#include "be_visitor.h"

#include "global_extern.h"

be_attribute::be_attribute (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Field (),
    AST_Attribute (),
    be_decl (),
    original_attribute_ (0)
{
  be_operation_default_strategy *bods = 0;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->get_strategy_ = bods;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->set_strategy_ = bods;
}

be_attribute::be_attribute (bool ro,
                            AST_Type *ft,
                            UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_attr,
              n),
    AST_Field (AST_Decl::NT_attr,
               ft,
               n),
    AST_Attribute (ro,
                   ft,
                   n,
                   local,
                   abstract),
    be_decl (AST_Decl::NT_attr,
             n),
    original_attribute_ (0)
{
  be_operation_default_strategy *bods = 0;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->get_strategy_ = bods;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->set_strategy_ = bods;

  if (!this->imported () && !this->is_local ())
    {
      // For the return types of the two operations
      // generated from this attribute.
      this->set_arg_seen_bit (be_type::narrow_from_decl (ft));
      idl_global->basic_arg_seen_ = true;
    }
}

be_type *
be_attribute::field_type (void) const
{
  return
    be_type::narrow_from_decl  (
      this->AST_Attribute::field_type ());
}

int
be_attribute::accept (be_visitor *visitor)
{
  return visitor->visit_attribute (this);
}


be_operation_strategy *
be_attribute::set_set_strategy (be_operation_strategy *new_strategy)
{
  be_operation_strategy *old = this->set_strategy_;

  if (new_strategy != 0)
    {
      this->set_strategy_ = new_strategy;
    }

  return old;
}

be_operation_strategy *
be_attribute::set_get_strategy (be_operation_strategy *new_strategy)
{
  be_operation_strategy *old = this->get_strategy_;

  if (new_strategy != 0)
    {
      this->get_strategy_ = new_strategy;
    }

  return old;
}

be_operation_strategy *
be_attribute::get_set_strategy ()
{
  return this->set_strategy_;
}

be_operation_strategy *
be_attribute::get_get_strategy ()
{
  return this->get_strategy_;
}

void
be_attribute::destroy (void)
{
  if (0 != this->get_strategy_)
    {
      this->get_strategy_->destroy ();
      delete this->get_strategy_;
      this->get_strategy_ = 0;
    }

  if (0 != this->set_strategy_)
    {
      this->set_strategy_->destroy ();
      delete this->set_strategy_;
      this->set_strategy_ = 0;
    }

  this->be_decl::destroy ();
  this->AST_Attribute::destroy ();
}

void
be_attribute::original_attribute (be_attribute *original_attribute)
{
  this->original_attribute_ = original_attribute;
}

be_attribute *
be_attribute::original_attribute (void)
{
  return this->original_attribute_;
}

IMPL_NARROW_FROM_DECL (be_attribute)
