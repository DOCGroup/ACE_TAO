//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    collocated_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for collocated classes for the Interface node
//    inside the server header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, collocated_sh, "$Id$")


// ************************************************************
//  collocated class in header
// ************************************************************

be_visitor_interface_collocated_sh::be_visitor_interface_collocated_sh
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_collocated_sh::~be_visitor_interface_collocated_sh (void)
{
}

int be_visitor_interface_collocated_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flatname (), "_collocated");

  // output the class defn
  os->indent ();
  *os << "class " << idl_global->export_macro ()
      << " " << node->local_coll_name ();
  os->incr_indent ();
  *os << " : public virtual " << node->name ();

  // generate base classes if any
  if (node->n_inherits () > 0)
    {
      for (int i = 0; i < node->n_inherits (); ++i)
        {
          *os << "," << be_nl;
          be_interface* parent =
            be_interface::narrow_from_decl (node->inherits()[i]);
          *os << "  public virtual "
              << parent->relative_coll_name (node->full_coll_name ());
        }
    }
  *os << "\n";
  os->decr_indent ();
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();

  *os << node->local_coll_name () << " (\n";

  os->incr_indent (0);
  os->incr_indent ();

  // XXXASG - can we make this a method to return the right name ??
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }

  *os << node->local_name () << "_ptr "
      << " servant," << be_nl;

  *os << "STUB_Object *stub\n";
  os->decr_indent ();
  *os << ");\n";
  os->decr_indent (0);

  os->indent ();
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name ()
      << "_ptr _get_servant (void) const;" << be_nl;

  *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char *logical_type_id," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");" << be_uidt << "\n";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  os->decr_indent ();

  *os << be_nl << "private:\n";
  os->incr_indent ();
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }
  *os << node->local_name () << "_ptr servant_;\n";
  os->decr_indent ();
  *os << "};\n\n";

  os->gen_endif ();

  return 0;
}
