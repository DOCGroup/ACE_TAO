
//=============================================================================
/**
 *  @file    exception_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Exception in the client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "exception.h"
#include "be_visitor_typecode/typecode_decl.h"
#include "global_extern.h"
#include "utl_err.h"

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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2 << "class " << be_global->stub_export_macro ()
            << " " << node->local_name ()
            << " : public ::CORBA::UserException" << be_nl;
  *os << "{" << be_nl
      << "public:" << be_idt;

  // Generate code for field members.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_exception_ch::")
                         ACE_TEXT ("visit_exception - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  // Constructors and destructor.
  *os << be_nl_2
      << node->local_name () << " (void);" << be_nl
      << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl
      << "~" << node->local_name () << " (void);\n" << be_nl;

  // Assignment operator.
  *os << node->local_name () << " &operator= (const "
      << node->local_name () << " &);" << be_nl_2;

  if (be_global->any_support ())
    {
      *os << "static void _tao_any_destructor (void *);" << be_nl_2;
    }

  *os << "static " << node->local_name ()
      << " *_downcast ( ::CORBA::Exception *);" << be_nl
      << "static const " << node->local_name ()
      << " *_downcast ( ::CORBA::Exception const *);" << be_nl_2;

  *os << "static ::CORBA::Exception *_alloc (void);" << be_nl_2;

  *os << "virtual ::CORBA::Exception *"
      << "_tao_duplicate (void) const;\n" << be_nl
      << "virtual void _raise (void) const;\n" << be_nl
      << "virtual void _tao_encode (TAO_OutputCDR &cdr) const;" << be_nl
      << "virtual void _tao_decode (TAO_InputCDR &cdr);";

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
                             ACE_TEXT ("be_visitor_exception::")
                             ACE_TEXT ("visit_exception - ")
                             ACE_TEXT ("codegen for ctor failed\n")),
                            -1);
        }
    }

  if (be_global->tc_support ())
    {
      *os << be_nl_2
          << "virtual ::CORBA::TypeCode_ptr _tao_type (void) const;";
    }

  *os << be_uidt_nl << "};";

  if (be_global->tc_support ())
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_typecode_decl visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              ACE_TEXT ("be_visitor_exception_ch::")
                              ACE_TEXT ("visit_exception - ")
                              ACE_TEXT ("TypeCode declaration failed\n")),
                            -1);
        }
    }

  node->cli_hdr_gen (1);
  return 0;
}
