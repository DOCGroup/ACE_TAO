//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    collocated_ami_handler_ch.cpp
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

ACE_RCSID(be_visitor_interface, collocated_ami_handler_ch, "$Id$")


// ************************************************************
//  collocated class in header
// ************************************************************

be_visitor_interface_collocated_ami_handler_ch::be_visitor_interface_collocated_ami_handler_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_collocated_ami_handler_ch::~be_visitor_interface_collocated_ami_handler_ch (void)
{
}

int
be_visitor_interface_collocated_ami_handler_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate AMI_...name..._Handler string.
  char *ifdef_string = 0;
  ACE_NEW_RETURN (ifdef_string,
                  char [ACE_OS::strlen ("AMI_") +
                       ACE_OS::strlen (node->flatname ()) +
                       ACE_OS::strlen ("_Handler") +
                       // end of string
                       1],
                  0);
  ACE_OS::sprintf (ifdef_string,
                   "AMI_%s_Handler",
                   node->flatname ());

  os->gen_ifdef_macro (ifdef_string, "_collocated");

  // Prepare the local name for the class.
  char *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  char [ACE_OS::strlen ("AMI_") +
                       ACE_OS::strlen (node->local_name ()->get_string ()) +
                       ACE_OS::strlen ("_Handler") +
                       // end of string
                       1],
                  0);
  ACE_OS::sprintf (local_name,
                   "AMI_%s_Handler",
                   node->local_name ()->get_string ());

  // Get the coll names.
  char *coll_local_name, *coll_full_name;
  node->compute_coll_names (local_name, coll_local_name, coll_full_name);

  // Output the class defn.
  os->indent ();
  *os << "class " << idl_global->stub_export_macro ()
      << " " << coll_local_name;
  os->incr_indent ();
  *os << " : public virtual " << local_name;

  *os << "\n";
  os->decr_indent ();
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();

  *os << coll_local_name << " (\n";

  os->incr_indent (0);
  os->incr_indent ();

  // XXXASG - can we make this a method to return the right name ??
  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }

  *os << local_name << "_ptr "
      << " servant," << be_nl;

  *os << "TAO_Stub *stub\n";
  os->decr_indent ();
  *os << ");\n";
  os->decr_indent (0);

  os->indent ();

  // _is_a method.
  *os << "virtual CORBA::Boolean _is_a" << be_idt
      << "(" << be_idt_nl
      << "const CORBA::Char *logical_type_id," << be_nl
      << "CORBA_Environment &TAO_IN_ENV = TAO_default_environment ()" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // _get_servant method.

  if (!node->is_nested ())
    {
      // The skeleton name is the outermost, we need to printout the
      // POA_ prefix that goes with it.
      *os << "POA_";
    }

  *os << local_name
      << "_ptr _get_servant (void) const;" << be_nl << be_nl;

  // _non_existent method.
  *os << "virtual CORBA::Boolean _non_existent" << be_idt
      << "(" << be_idt_nl
      << "CORBA_Environment &TAO_IN_ENV = TAO_default_environment ()" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  // Visit the scope to generate all the AMI call back hood methods.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_collocated_ami_handler_ch::"
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
  *os << local_name << "_ptr servant_;\n";
  os->decr_indent ();
  *os << "};\n\n";

  os->gen_endif ();

  delete ifdef_string;
  delete local_name;
  delete coll_local_name;
  delete coll_full_name;
  return 0;
}
