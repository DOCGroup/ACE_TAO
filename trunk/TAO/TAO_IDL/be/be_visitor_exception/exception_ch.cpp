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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, exception_ch, "$Id$")


// ******************************************************
// for client header
// ******************************************************

be_visitor_exception_ch::be_visitor_exception_ch (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_ch::~be_visitor_exception_ch (void)
{
}

// visit the Exception node and its scope
int be_visitor_exception_ch::visit_exception (be_exception *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      os = this->ctx_->stream ();

      os->gen_ifdef_macro (node->flat_name ());

      os->indent ();
      *os << "class " << be_global->stub_export_macro ()
                << " " << node->local_name ()
                << " : public CORBA::UserException" << be_nl;
      *os << "{" << be_nl << "public:" << be_idt_nl;

      // generate code for field members
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_ch::"
                             "visit_exception - "
                             "codegen for scope failed\n"), -1);
        }

      *os << be_nl;

      // constructors and destructor
      *os << node->local_name () << " (void); // default ctor" << be_nl;
      *os << node->local_name () << " (const " << node->local_name ()
          << " &); // copy ctor" << be_nl;
      *os << "~" << node->local_name () << " (void);" << be_nl;

      if (!node->is_local ())
        *os << "static void _tao_any_destructor (void*);" << be_nl;

      // assignment operator
      *os << node->local_name () << " &operator= (const "
          << node->local_name () << " &);\n\n";

      *os << be_nl
          << "virtual void _raise (void);\n" << be_nl
          << "virtual void _tao_encode (" << be_idt << be_idt_nl
          << "TAO_OutputCDR &," << be_nl
          << "CORBA::Environment &) const;" << be_uidt << be_uidt_nl
          << "virtual void _tao_decode (" << be_idt << be_idt_nl
          << "TAO_InputCDR &," << be_nl
          << "CORBA::Environment &);" << be_uidt << be_uidt << "\n"
          << be_nl
          << "static " << node->local_name ()
          << " *_downcast (CORBA::Exception *);\n\n";

      // generate constructor that takes each member as a parameter. We need a
      // new state. Such a constructor exists if we have members
      if (node->member_count () > 0)
        {
          be_visitor_context ctx (*this->ctx_);
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CH);
          be_visitor *visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception::"
                                 "visit_exception - "
                                 "codegen for ctor failed\n"), -1);
            }
          delete visitor;
        }


      *os << be_nl
          << "// = TAO extension" << be_nl
          << "static CORBA::Exception *_alloc (void);";

      if (!node->is_local () && be_global->tc_support ())
        *os << be_nl <<"virtual CORBA::TypeCode_ptr _type (void) const;";

      *os << be_uidt_nl << "}; // exception " << node->name ()
          << "\n" << be_nl;

      if (!node->is_local ())
        {
          // by using a visitor to declare and define the TypeCode, we
          // have the added advantage to conditionally not generate
          // any code. This will be based on the command line
          // options. This is still TO-DO
          be_visitor *visitor;
          be_visitor_context ctx (*this->ctx_);
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DECL);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_ch::"
                                 "visit_exception - "
                                 "TypeCode declaration failed\n"
                                 ), -1);
            }
        }

      os->gen_endif ();

      node->cli_hdr_gen (1);

    }
  return 0;
}
