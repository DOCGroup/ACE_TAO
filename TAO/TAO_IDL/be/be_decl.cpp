// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_decl.cpp
//
// = DESCRIPTION
//    Extension of class AST_Decl that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_decl.h"
#include "be_scope.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_root.h"
#include "be_exception.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_enum.h"
#include "be_operation.h"
#include "be_factory.h"
#include "be_sequence.h"
#include "be_visitor.h"
#include "utl_identifier.h"

ACE_RCSID (be, 
           be_decl, 
           "$Id$")

// Default Constructor
be_decl::be_decl (void)
{
}

// Constructor
be_decl::be_decl (AST_Decl::NodeType type,
                  UTL_ScopedName *n)
  : AST_Decl (type,
              n)
{
}

// Destructor
be_decl::~be_decl (void)
{
}

void
be_decl::compute_full_name  (const char *prefix,
                             const char *suffix,
                             char *&name)
{
  if (prefix == 0 || suffix == 0)
    return;

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == 0)
    {
      // Global scope.

      // Prefix.
      result_str = prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl *parent = be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_flat_name - "
                      "scope name is nil\n"));
        }

      // Parent name.
      result_str = ACE_CString (parent->full_name ());

      // _
      if (ACE_OS::strcmp (parent->full_name (), "") != 0)
        result_str += ACE_CString ("::");

      // Prefix.
      result_str += prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = result_str.rep ();
}

void
be_decl::compute_flat_name  (const char *prefix,
                             const char *suffix,
                             char *&name)
{
  if (prefix == 0 || suffix == 0)
    {
      return;
    }

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == 0)
    {
      // Global scope.

      // Prefix.
      result_str = prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl *parent =
        be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) be_decl::"
                      "compute_flat_name - "
                      "scope name is nil\n"));
        }

      // Parent name.
      result_str = ACE_CString (parent->flat_name ());

      // _
      if (ACE_OS::strcmp (parent->flat_name (), "") != 0)
        result_str += "_";

      // Prefix.
      result_str += prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  name = result_str.rep ();
}

void
be_decl::destroy (void)
{
  this->AST_Decl::destroy ();
}

// Return the scope created by this node (if one exists, else NULL).
be_scope *
be_decl::scope (void)
{
  be_decl *d = this;

   switch (this->node_type ())
   {
     case AST_Decl::NT_interface_fwd:
        // Resolve forward declared interface by looking at full_definition()
        // field and iterating.
        d =
          be_interface::narrow_from_decl (
              (be_interface_fwd::narrow_from_decl (this))->full_definition ()
            );
     // Fall through
     case AST_Decl::NT_interface:
        return be_interface::narrow_from_decl (d);
     case AST_Decl::NT_module:
        return be_module::narrow_from_decl (d);
     case AST_Decl::NT_root:
        return be_root::narrow_from_decl (d);
     case AST_Decl::NT_except:
        return be_exception::narrow_from_decl (d);
     case AST_Decl::NT_union:
        return be_union::narrow_from_decl (d);
     case AST_Decl::NT_struct:
        return be_structure::narrow_from_decl (d);
     case AST_Decl::NT_enum:
        return be_enum::narrow_from_decl (d);
     case AST_Decl::NT_op:
        return be_operation::narrow_from_decl (d);
     case AST_Decl::NT_factory:
        return be_factory::narrow_from_decl (d);
     case AST_Decl::NT_sequence:
        return be_sequence::narrow_from_decl (d);
     default:
        return (be_scope *)0;
   }
}

int
be_decl::accept (be_visitor *visitor)
{
  return visitor->visit_decl (this);
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)
