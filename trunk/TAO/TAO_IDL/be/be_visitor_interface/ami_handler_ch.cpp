//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_ch.cpp
//
// = DESCRIPTION
//    Visitor generating AMI Reply Handler code for Interfaces in the
//    client header. 
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>  
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, ami_handler_ch, "$Id$")


// ************************************************************
// Interface visitor for server header
// ************************************************************

be_visitor_interface_ami_handler_ch::be_visitor_interface_ami_handler_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ami_handler_ch::~be_visitor_interface_ami_handler_ch (void)
{
}

int
be_visitor_interface_ami_handler_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  static char namebuf [NAMEBUFSIZE]; // holds the class name

  if (node->srv_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();
  
  // Generate the skeleton class name.

  // Start with whatever indentation level we are at.
  os->indent (); 

  // We shall have a POA_ prefix only if we are at the topmost level. 
  if (!node->is_nested ())
    {
      // we are outermost
      ACE_OS::sprintf (namebuf,
                       "POA_AMI_%s_Handler",
                       node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf,
                       "AMI_%s_Handler",
                       node->local_name ()->get_string ());
    }

  *os << "class " << namebuf << ";" << be_nl;

  // generate the _ptr declaration
  *os << "typedef " << namebuf << " *" << namebuf
      << "_ptr;" << be_nl;

  // Now generate the class definition.
  // This class will inherit from the Messaging::ReplyHandler class. 
  *os << "class " << idl_global->export_macro ()
      << " " << namebuf
      << " : public POA_Messaging::ReplyHandler" 
      << be_nl;

  // Body of the class definition.
  *os << be_nl
      << "{" << be_nl;

  // Default constructor.
  *os << "protected:" << be_idt_nl
      << namebuf << " (void);\n" << be_uidt_nl;
  
  // Public portion starts. 
  *os << "public:" << be_idt_nl
    
    // Copy constructor.
      << namebuf << " (const " << namebuf << "& rhs);" << be_nl
    
    // Destructor.
      << "virtual ~" << namebuf << " (void);\n\n"
      
      << be_nl;
    
    // Methods.
  
  // _is_a.
  *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;
  
  // _down_cast.
  *os << "virtual void* _downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ");\n" << be_uidt_nl;
  
  // add a skeleton for our _is_a method
  *os << "static void _is_a_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &req," << be_nl
      << "void *obj," << be_nl
      << "void *context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // add a skeleton for our _non_existent method
  *os << "static void _non_existent_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &req," << be_nl
      << "void *obj," << be_nl
      << "void *context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // add the dispatch method
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_req," << be_nl
      << "void *_tao_context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // Print out the _this() method.
  *os << node->name () << " *_this (" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // The _interface_repository_id method.
  *os << "virtual const char* _interface_repository_id "
      << "(void) const;\n\n";

  // The _create_collocated_objref method.
  *os << "virtual void* _create_collocated_objref"
      << "(const char*, CORBA::ULong type, TAO_Stub *sobj);"
      << be_nl << be_nl;

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ami_handler_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the call.
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ami_handler_ch::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};\n\n";

#if 0
  //
  // @@ Alex: Do this:
  // 
  // Generate the collocated class
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ami_handler_ch::"
                         "visit_interface - "
                         "codegen for collocated class failed\n"),
                        -1);
    }

  // generate the TIE class.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ami_handler_ch::"
                         "visit_interface - "
                         "codegen for TIE class failed\n"),
                        -1);
    }
#endif /* 0 */

  *os << "\n";

#if 0
  ctx.stream (tao_cg->server_template_header ());
#endif /* 0 */

  return 0;
}
