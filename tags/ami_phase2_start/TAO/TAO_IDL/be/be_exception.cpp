// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_exception.cpp
//
// = DESCRIPTION
//    Extension of class AST_Exception that provides additional means for C++
//    mapping of an interface.
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

ACE_RCSID(be, be_exception, "$Id$")

/*
 * BE_Exception
 */
be_exception::be_exception (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_exception::be_exception (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (AST_Decl::NT_except, n, p),
    AST_Structure (AST_Decl::NT_except, n, p),
    UTL_Scope (AST_Decl::NT_except),
    member_count_ (-1)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// compute total number of members
int
be_exception::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si;  // iterator

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          this->member_count_++;
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the member count
int
be_exception::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
}

// Are we or the parameter node involved in any recursion
idl_bool
be_exception::in_recursion (be_type *node)
{
  if (!node)
    {
      // we are determining the recursive status for ourselves
      node = this;
    }

  // proceed if the number of members in our scope is greater than 0
  if (this->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      -1);
      // continue until each element is visited
      while (!si->is_done ())
        {
          be_field *field = be_field::narrow_from_decl (si->item ());
          if (!field)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("(%N:%l) be_exception::")
                                 ASYS_TEXT ("in_recursion - ")
                                 ASYS_TEXT ("bad field node\n")),
                                0);
            }
          be_type *type = be_type::narrow_from_decl (field->field_type ());
          if (!type)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("(%N:%l) be_exception::")
                                 ASYS_TEXT ("in_recursion - ")
                                 ASYS_TEXT ("bad field type\n")),
                                0);
            }
          if (type->in_recursion (node))
            {
              delete si;
              return 1;
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  // not in recursion
  return 0;
}

int
be_exception::accept (be_visitor *visitor)
{
  return visitor->visit_exception (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_exception)
IMPL_NARROW_FROM_SCOPE (be_exception)
