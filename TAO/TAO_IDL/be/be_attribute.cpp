// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_attribute.cpp
//
// = DESCRIPTION
//    Extension of class AST_Attribute that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_attribute.h"
#include "be_type.h"
#include "be_visitor.h"

#include "global_extern.h"

ACE_RCSID (be, 
           be_attribute, 
           "$Id$")

be_attribute::be_attribute (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Field (),
    AST_Attribute (),
    be_decl ()
{
  be_operation_default_strategy *bods = 0;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->get_strategy_ = bods;

  ACE_NEW (bods,
           be_operation_default_strategy (0));
  this->set_strategy_ = bods;
}

be_attribute::be_attribute (idl_bool ro,
                            AST_Type *ft,
                            UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
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
             n)
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
      ACE_SET_BITS (idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.basic_arg_seen_);
    }
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

// Narrowing
IMPL_NARROW_METHODS2 (be_attribute, AST_Attribute, be_decl)
IMPL_NARROW_FROM_DECL (be_attribute)
