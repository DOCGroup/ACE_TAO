//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ci, "$Id$")


// **************************************************
// Interface visitor for client inline
// **************************************************
be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  if (node->imported ())
    return 0;

  // Generate inline methods for elements of our scope.
  // This will always be done here for any non-imported node,
  // whether the _var and _out code has been generated already
  // at a forward declaration or not.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // A forward declared interface may have set this flag.
  if (node->cli_inline_gen ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent (); // start from the current indentation level

  // Generate the constructor from stub and servant.
  if (! node->is_local ())
    {
      os->gen_ifdef_macro (node->flat_name (), "");
      *os << "ACE_INLINE" << be_nl;
      *os << node->name () << "::" << node->local_name () <<
        " (" << be_idt_nl << "TAO_Stub *objref," << be_nl
          << "CORBA::Boolean _tao_collocated," << be_nl 
	  << "TAO_Abstract_ServantBase *servant" << be_nl
	  << ")" // constructor 
	  << be_nl;
      *os << "  : CORBA_Object (objref, _tao_collocated, servant)" << be_nl;
      *os << "{" << be_idt_nl
	  << be_idt_nl // idt = 1
	  << "this->setup_collocation (_tao_collocated);";
      /*
      if (node->n_inherits () > 0)
	{
	  for (int i = 0; i < node->n_inherits (); i++)
	    {
	      be_interface *inherited =
		be_interface::narrow_from_decl (node->inherits ()[i]);
	      be_decl *scope = 0;
	      if (inherited->is_nested ())
		{
		  // inherited node is used in the scope of "node" node
		  scope =
		    be_scope::narrow_from_scope (node->defined_in ())->decl ();
		}
	      
	      *os << "this->" << inherited->local_name ()<< "::setup_collocation" << " (_tao_collocated);" << be_nl;
	   } 
	    
	}
      */
      *os << be_uidt_nl // idt = 0
	  <<"}" << be_nl;
      os->gen_endif ();
    }

  // generate the ifdefined macro for  the _var type
  os->gen_ifdef_macro (node->flat_name (), "_var");
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // generate the ifdefined macro for  the _out type
  os->gen_ifdef_macro (node->flat_name (), "_out");
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();

  return 0;
}
