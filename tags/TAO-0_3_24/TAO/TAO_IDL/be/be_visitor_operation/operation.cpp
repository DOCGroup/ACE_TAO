//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation, "$Id$")


// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
}


int
be_visitor_operation::void_return_type (be_type *bt)
{
  // is the operation return type void?

  if (bt->node_type () == AST_Decl::NT_pre_defined
      && (be_predefined_type::narrow_from_decl (bt)->pt ()
          == AST_PredefinedType::PT_void))
    return 1;
  else
    return 0;
}

int
be_visitor_operation::has_param_type (be_operation *node,
                                      AST_Argument::Direction dir)
{
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      0);
      // continue until each element is visited
      while (!si->is_done ())
	{
	  be_argument *bd = be_argument::narrow_from_decl (si->item ());
          if (bd && (bd->direction () == dir))
            return 1;

	  si->next ();
	} // end of while loop
      delete si;
    } // end of if

  // not of the type we are looking for
  return 0;
}

