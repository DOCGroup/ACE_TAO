// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_root.cpp
//
// = DESCRIPTION
//    Extension of class AST_Root that provides additional means for C++
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

ACE_RCSID(be, be_root, "$Id$")

// Default Constructor
be_root::be_root (void)
{
}

// Constructor used to build the root of the abstract syntax tree (AST)
be_root::be_root (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Root (n, p),
    AST_Decl (AST_Decl::NT_root, n, p),
    UTL_Scope (AST_Decl::NT_root)

{
}

// We had to provide these since the AST_Root::fe_* method was setting the
// names of these three to "local type"

/*
 * Add this AST_Sequence to the locally defined types in this scope
 */
AST_Sequence *
be_root::fe_add_sequence (AST_Sequence *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types(t);
  return t;
}

/*
 * Add this AST_String to the locally defined types in this scope
 */
AST_String *
be_root::fe_add_string (AST_String *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types (t);

  return t;
}

/*
 * Add this AST_Array to the locally defined types in this scope
 */
AST_Array *
be_root::fe_add_array (AST_Array *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types (t);

  return t;
}

int
be_root::accept (be_visitor *visitor)
{
  return visitor->visit_root (this);
}

/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS3 (be_root, AST_Root, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_root)
IMPL_NARROW_FROM_SCOPE (be_root)
