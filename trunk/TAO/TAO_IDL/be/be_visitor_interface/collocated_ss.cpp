//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    collocated_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for collocated classes for an Interface in the
//    server skeleton file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"


// ************************************************************
//  be_visitor_interface_collacted_ss
// ************************************************************

be_visitor_interface_collocated_ss::be_visitor_interface_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_collocated_ss::~be_visitor_interface_collocated_ss (void)
{
}

int be_visitor_interface_collocated_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = tao_cg->server_skeletons ();
  long i;

  this->ctx_->node (node);

  *os << node->full_coll_name () << "::"
      << node->local_coll_name () << " (\n";

  os->incr_indent (0);
  os->incr_indent ();
  *os << node->full_skel_name () << "_ptr "
      << " servant," << be_nl;

  *os << "STUB_Object *stub\n";
  os->decr_indent ();
  *os << ")\n";
  os->decr_indent (0);

  os->incr_indent ();

  if (node->is_nested ())
    {
      be_decl* scope =
	be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << ": ACE_NESTED_CLASS ("
	  << scope->name () << ","
	  << node->local_name ()
	  << ") ()," << be_nl;
    }
  else
    {
      *os << ": " << node->name ()
	  << " ()," << be_nl;
    }

  // @@ We should call the constructor for all base classes, since we
  // are using multiple inheritance.

  if (node->traverse_inheritance_graph (be_interface::collocated_ctor_helper, os)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_ss::"
                         "visit_interface - "
                         "codegen for base class ctor init failed\n"),
                        -1);
    }

#if 0
  if (node->n_inherits () > 0)
    {
      for (i = 0; i < node->n_inherits (); ++i)
        {
          be_interface* parent =
            be_interface::narrow_from_decl (node->inherits()[i]);
	  if (parent->is_nested ())
	    {
	      be_decl* scope =
		be_scope::narrow_from_scope (parent->defined_in())->decl ();

	      *os << "  ACE_NESTED_CLASS ("
		  << "POA_" << scope->name () << ","
		  << parent->local_coll_name ()
		  << ") (servant, stub)," << be_nl;
	    }
	  else
	    {
	      *os << "  " << parent->full_coll_name ()
		  << " (servant, stub)," << be_nl;
	    }
        }
    }
#endif

  *os << "  CORBA_Object (stub, servant, CORBA::B_TRUE)," << be_nl
      << "  servant_ (servant)";

  *os << "\n";
  os->decr_indent ();
  *os << "{\n";
  *os << "}\n\n";

  os->indent ();
  *os << node->full_skel_name () << "_ptr "
      << node->full_coll_name () << "::"
      << "_get_servant (void) const\n"
      << "{\n";
  os->incr_indent ();
  *os << "return this->servant_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();
  *os << "CORBA::Boolean " << node->full_coll_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return this->servant_->_is_a (" << be_idt << be_idt_nl
      << "logical_type_id," << be_nl
      << "_tao_environment" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}\n\n";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}
