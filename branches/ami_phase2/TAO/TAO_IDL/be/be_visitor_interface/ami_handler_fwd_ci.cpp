//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Ami_<Interface>_Handler node in the
//    client inline.  
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_ami_handler_fwd, ami_handler_fwd_ci, "$Id$")


// ********************************************************************
// Visitor implementation for the Ami_Handler_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_ami_handler_fwd_ci::be_visitor_ami_handler_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_ami_handler_fwd_ci::~be_visitor_ami_handler_fwd_ci (void)
{
}

// visit the Ami_Handler_Fwd_ci node and its scope
int
be_visitor_ami_handler_fwd_ci::visit_interface (be_interface *node)
{
  if (node->cli_inline_gen () || node->imported ())
    return 0;

  // Generate code.

  // Get the out stream.
  TAO_OutStream *os;
  os = this->ctx_->stream ();
  
  // Start from the current indentation level.
  os->indent ();

  // Create the full name and local name for the AMI_<Interface
  // name>_Handler interface.
  char *full_name = 0;
  char *local_name = 0;

  // Full name. 
  // If there exists a scope name for this, then generate
  // "Scope::AMI_<Local Name>_Handler". 

  size_t scope_len = 0;

  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  
  if (parent != 0 &&
      parent->fullname () != 0 &&
      ACE_OS::strlen (parent->fullname ()))
    scope_len = ACE_OS::strlen (parent->fullname ()) + ACE_OS::strlen ("::");
  
  ACE_NEW_RETURN (full_name,
                  char [scope_len +
                       ACE_OS::strlen ("AMI_") +
                       ACE_OS::strlen (node->local_name ()->get_string ()) +
                       ACE_OS::strlen ("_Handler") +
                       1],
                  -1);
  
  if (parent != 0 &&
      parent->fullname () != 0 &&
      ACE_OS::strlen (parent->fullname ()))
    ACE_OS::sprintf (full_name,
                     "%s::AMI_%s_Handler",
                     parent->fullname (),
                     node->local_name ()->get_string ());
  else
    ACE_OS::sprintf (full_name,
                     "AMI_%s_Handler",
                     node->local_name ()->get_string ());

  // Local name.

  ACE_NEW_RETURN (local_name,
                  char [ACE_OS::strlen ("AMI_") +
                       ACE_OS::strlen (node->local_name ()->get_string ()) +
                       ACE_OS::strlen ("_Handler") +
                       1],
                  -1);
  
  ACE_OS::sprintf (local_name,
                   "AMI_%s_Handler",
                   node->local_name ()->get_string ());

  // Generate the constructors and destructor.

  *os << "ACE_INLINE" << be_nl;
  *os << full_name << "::" << local_name 
      << " (void) // default constructor" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << full_name << "::" << local_name
      << " (TAO_Stub *objref, TAO_ServantBase *_tao_servant, "
      << "CORBA::Boolean _tao_collocated) // constructor"
      << be_nl
      << "  : CORBA_Object (objref, _tao_servant, _tao_collocated)"
      << be_nl
      << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << full_name << "::~" << local_name
      << " (void) // destructor" << be_nl
      << "{}\n\n";

  // _nil method.
  *os << "ACE_INLINE "
      << full_name << "_ptr" << be_nl
      << full_name << "::_nil (void)" << be_nl
      << "{" << be_idt_nl
      << "return (" << full_name << "_ptr)0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate the ifdefined macro for  the _var type.
  os->gen_ifdef_macro (full_name, "_var");
  if (node->gen_var_impl (full_name, local_name) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // Generate the ifdefined macro for  the _out type.
  os->gen_ifdef_macro (full_name, "_out");
  if (node->gen_out_impl (full_name,
                          local_name) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();

  return 0;
}
