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

/*
 * BE_Enum
 */
be_enum::be_enum (void)
{
  this->size_type (be_decl::FIXED);
}

be_enum::be_enum (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Enum (n, p),
    AST_Decl (AST_Decl::NT_enum, n, p),
    UTL_Scope (AST_Decl::NT_enum),
    member_count_ (-1)
{
  this->size_type (be_decl::FIXED);
}

// compute total number of members
int
be_enum::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si;  // iterator
  AST_Decl *d;  // temp node

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
	{
	  // get the next AST decl node
	  d = si->item ();
	  if (!d->imported ())
	    {
              this->member_count_++;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the member count
int
be_enum::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
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
