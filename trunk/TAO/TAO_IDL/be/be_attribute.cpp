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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_attribute, "$Id$")

/*
 * BE_Attribute
 */
be_attribute::be_attribute (void)
: get_strategy_ (new be_operation_default_strategy (0)),
  set_strategy_ (new be_operation_default_strategy (0))
{
}

be_attribute::be_attribute (idl_bool ro, AST_Type *ft, UTL_ScopedName *n,
                            UTL_StrList *p)
  : AST_Attribute (ro, ft, n, p),
    AST_Field (AST_Decl::NT_attr, ft, n, p),
    AST_Decl (AST_Decl::NT_attr, n, p),
    get_strategy_ (new be_operation_default_strategy (0)),
    set_strategy_ (new be_operation_default_strategy (0))
{

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
    this->set_strategy_ = new_strategy;

  return old;
}

be_operation_strategy *
be_attribute::set_get_strategy (be_operation_strategy *new_strategy)
{
  be_operation_strategy *old = this->get_strategy_;

  if (new_strategy != 0)
    this->get_strategy_ = new_strategy;

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
