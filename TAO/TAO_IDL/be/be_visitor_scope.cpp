//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_scope.cpp
//
// = DESCRIPTION
//    Visitor for the base be_scope node. This serves to maintain the current
//    state (context) of code generation for the derived visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_scope.h"

// ******************************************************
//  Generic visitor for a scope.
//  All elements that give rise to a scope inherit from
//  this class
// ******************************************************

be_visitor_scope::be_visitor_scope (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_scope::~be_visitor_scope (void)
{
}

// visit the scope and its elements
int
be_visitor_scope::visit_scope (be_scope *node)
{
  // proceed if the number of members in our scope is greater than 0
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      // continue until each element is visited
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
          if (!d)
	    {
	      delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"), -1);

	    }
	  be_decl *bd = be_decl::narrow_from_decl (d);
          // set the scope node as "node" in which the code is being generated
          // so that elements in the node's scope can use it for code generation
          this->ctx_->scope (node->decl ());
          // set the node to be visited
          this->ctx_->node (bd);
	  if (bd == 0 || bd->accept (this) == -1)
	    {
	      delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"), -1);

	    }
	  si->next ();
          if (!si->is_done ())
            this->post_process (); // anything to be done after each element
                                   // except the last one is processed?

	} // end of while loop
      delete si;
    } // end of if

  return 0;
}

int
be_visitor_scope::post_process (void)
{
  return 0;
}
