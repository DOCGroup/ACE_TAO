//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_stub_ch.cpp
//
// = DESCRIPTION
//    Visitor generating default stub class for the AMI Handler in the
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


// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_interface_ami_handler_stub_ch::be_visitor_interface_ami_handler_stub_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ami_handler_stub_ch::~be_visitor_interface_ami_handler_stub_ch (void)
{
}

int
be_visitor_interface_ami_handler_stub_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  // Not already generated and not imported.
  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // Grab the stream.
      os = this->ctx_->stream ();
      
      // Ifdef guard.
      
      os->gen_ifdef_macro (node->flat_name ());

      // Now generate the class definition.
      
      os->indent ();

      *os << "class " << idl_global->export_macro () << " "
          << node->local_name ();
      
      // Node interface inherits from the Messaging::ReplyHandler
      // interface. 
      *os << " : public virtual Messaging::ReplyHandler"  << be_nl; 
      
      // Generate the body.
  
      *os << "{" << be_nl
          << "public:" << be_nl
    
        // generate the _ptr_type and _var_type typedefs
        // but we must protect against certain versions of g++
          << "#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8"
          << be_idt_nl
          << "typedef " << node->local_name () << "_ptr _ptr_type;" << be_nl
          << "typedef " << node->local_name () << "_var _var_type;" << be_uidt_nl
          << "#endif /* __GNUC__ */\n" << be_idt_nl
    
        // generate the static _duplicate, _narrow, and _nil operations
          << "// the static operations" << be_nl
          << "static " << node->local_name () << "_ptr " << "_duplicate ("
          << node->local_name () << "_ptr obj);" << be_nl
          << "static " << node->local_name () << "_ptr "
          << "_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
          << "CORBA::Environment &env = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl
          << "static " << node->local_name () << "_ptr "
          << "_unchecked_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object_ptr obj," << be_nl
          << "CORBA::Environment &env = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl
          << "static " << node->local_name ()
          << "_ptr _nil (void)" << be_nl
          << "{" << be_idt_nl
          << "return (" << node->local_name () << "_ptr)0;" << be_uidt_nl
          << "}\n\n";

      // Visit the scope to generate the stubs for the call back
      // methods.   
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ami_handler_stub_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"),
                            -1);
        }

      
      // the _is_a method
      os->indent ();
      *os << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
          << "const CORBA::Char *type_id, " << be_nl
          << "CORBA::Environment &env = " << be_idt_nl
          << "TAO_default_environment ()"
          << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl
          << "virtual const char* "
          << "_interface_repository_id (void) const;\n" << be_uidt_nl;
  
      // generate the "protected" constructor so that users cannot instantiate
      // us
      *os << "protected:" << be_idt_nl
          << node->local_name () << " (void);" << be_nl
          << node->local_name ()
          << " (TAO_Stub *objref, " << be_idt << be_idt_nl
          << "TAO_ServantBase *_tao_servant = 0, " << be_nl
          << "CORBA::Boolean _tao_collocated = 0" << be_uidt_nl
          << ");" << be_uidt_nl
          << "virtual ~" << node->local_name () << " (void);" << be_uidt_nl;
      
      // private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *os << "private:\n";
      os->incr_indent ();
      *os << node->local_name () << " (const " << node->local_name () << " &);"
          << be_nl
          << "void operator= (const " << node->local_name () << " &);\n";
  
      os->decr_indent ();
      *os << "};\n\n";
      os->gen_endif ();
  
      // by using a visitor to declare and define the TypeCode, we have the
      // added advantage to conditionally not generate any code. This will be
      // based on the command line options. This is still TO-DO
      be_visitor *visitor;
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_TYPECODE_DECL);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ami_handler_stub_ch::"
                             "visit_interface - "
                             "TypeCode declaration failed\n"),
                            -1);
        }
    }
  return 0;
}
