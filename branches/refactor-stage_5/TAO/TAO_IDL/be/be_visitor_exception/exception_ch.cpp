//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exception_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Exception in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_visitor_typecode/typecode_decl.h"

ACE_RCSID (be_visitor_exception, 
           exception_ch, 
           "$Id$")

// ******************************************************
// For client header.
// ******************************************************

be_visitor_exception_ch::be_visitor_exception_ch (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_ch::~be_visitor_exception_ch (void)
{
}

// Visit the Exception node and its scope.
int be_visitor_exception_ch::visit_exception (be_exception *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name ());

  *os << be_nl << be_nl << "class " << be_global->stub_export_macro ()
            << " " << node->local_name ()
            << " : public CORBA::UserException" << be_nl;
  *os << "{" << be_nl
      << "public:" << be_idt_nl;

  // Generate code for field members.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_ch::"
                         "visit_exception - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_nl;

  // Constructors and destructor.
  *os << node->local_name () << " (void);" << be_nl
      << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl
      << "~" << node->local_name () << " (void);\n" << be_nl;

  // Assignment operator.
  *os << node->local_name () << " &operator= (const "
      << node->local_name () << " &);" << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);" << be_nl << be_nl;
    }


  *os << "static " << node->local_name ()
      << " *_downcast (CORBA::Exception *);" << be_nl;

  *os << "static CORBA::Exception *_alloc (void);\n" << be_nl;

  *os << "virtual CORBA::Exception *"
      << "_tao_duplicate (void) const;\n" << be_nl
      << "virtual void _raise (void);\n" << be_nl
      << "virtual void _tao_encode (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ") const;" << be_uidt_nl << be_nl
      << "virtual void _tao_decode (" << be_idt << be_idt_nl
      << "TAO_InputCDR &" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ");" << be_uidt;

  // Generate constructor that takes each member as a parameter. We need a
  // new state. Such a constructor exists if we have members.
  if (node->member_count () > 0)
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CH);
      be_visitor_exception_ctor visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception::"
                             "visit_exception - "
                             "codegen for ctor failed\n"), 
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  if (be_global->tc_support ())
    {
      *os << be_nl << be_nl 
          << "virtual CORBA::TypeCode_ptr _type (void) const;";
    }

  *os << be_uidt_nl << "};";

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_typecode_decl visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_ch::"
                             "visit_exception - "
                             "TypeCode declaration failed\n"), 
                            -1);
        }
    }

  os->gen_endif ();
  node->cli_hdr_gen (1);
  return 0;
}
