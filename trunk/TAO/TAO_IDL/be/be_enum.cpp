// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum.cpp
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
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

ACE_RCSID(be, be_enum, "$Id$")

be_enum::be_enum (void)
{
  this->size_type (be_decl::FIXED);
}

be_enum::be_enum (UTL_ScopedName *n,
                  UTL_StrList *p,
                  idl_bool local,
                  idl_bool abstract)
  : AST_Enum (n, 
              p, 
              local, 
              abstract),
    AST_Decl (AST_Decl::NT_enum, 
              n, 
              p),
    UTL_Scope (AST_Decl::NT_enum),
    COMMON_Base (local, 
                 abstract),
    member_count_ (-1)
{
  this->size_type (be_decl::FIXED);
}

// Compute total number of members.
int
be_enum::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si = 0;

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // Instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this, 
                                               UTL_Scope::IK_decls),
                      -1);

      while (!(si->is_done ()))
	      {
	        // Get the next AST decl node.
          this->member_count_++;
          si->next ();
        } 

      delete si;
    }

  return 0;
}

// return the member count
int
be_enum::member_count (void)
{
  if (this->member_count_ == -1)
    {
      this->compute_member_count ();
    }

  return this->member_count_;
}

// Convert a numeric value to the string name
UTL_ScopedName *
be_enum::value_to_name (const unsigned long v)
{
  UTL_ScopeActiveIterator *iter = 0;
  AST_EnumVal *item = 0;
  AST_Decl *i = 0;

  ACE_NEW_RETURN (iter,
                  UTL_ScopeActiveIterator (this,
                                           IK_decls),
                  0);

  while (!iter->is_done ())
    {
      i = iter->item  ();
      item = AST_EnumVal::narrow_from_decl (i);

      if (item->constant_value ()->ev ()->u.ulval == v)
        {
          delete iter;
          return item->name ();
        }

      iter->next ();
    }

  delete iter;
  return 0;
}

void
be_enum::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
//  be_type::destroy ();
}

int
be_enum::accept (be_visitor *visitor)
{
  return visitor->visit_enum (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_enum, AST_Enum, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_enum)
IMPL_NARROW_FROM_SCOPE (be_enum)
