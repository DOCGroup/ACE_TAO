//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exception_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Exception in the stubs file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, exception_cs, "$Id$")


// ***************************************************************************
// for client stubs
// ***************************************************************************
be_visitor_exception_cs::be_visitor_exception_cs (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cs::~be_visitor_exception_cs (void)
{
}

// visit the Exception_Cs node and its scope
int be_visitor_exception_cs::visit_exception (be_exception *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      os->indent ();

      // generate stub code  required of any anonymous types of members
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception::"
                             "visit_exception -"
                             "code for stub failed\n"),
                            -1);
        }

      // default constructor
      *os << "// default constructor" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (void)" << be_nl;
      if (!node->is_local ())
        *os << "  : CORBA_UserException ("
            << "::" << node->tc_name () << ")\n";
      *os << "{" << be_nl;
      *os << "}\n\n";

      // destructor
      os->indent ();
      *os << "// destructor - all members are of self managing types" << be_nl;
      *os << node->name () << "::~" << node->local_name () << " (void)" << be_nl;
      *os << "{" << be_nl;
      *os << "}\n\n";

      *os << "void "
          << node->name () << "::_tao_any_destructor (void *x)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << "*,x);" << be_nl
          << "delete tmp;" << be_uidt_nl
          << "}\n\n";

      // copy constructor
      os->indent ();
      *os << "// copy constructor" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (const ::"
          << node->name () << " &_tao_excp)" << be_nl;
      *os << "  : CORBA_UserException ("
          << "_tao_excp._type ())" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      // assign each individual member
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cs::"
                             "visit_exception -"
                             "codegen for scope failed\n"), -1);
        }
      delete visitor;
      os->decr_indent ();
      *os << "}\n\n";

      // assignment operator
      os->indent ();
      *os << "// assignment operator" << be_nl;
      *os << node->name () << "&" << be_nl;
      *os << node->name () << "::operator= (const ::"
                << node->name () << " &_tao_excp)" << be_nl
                << "{\n" << be_idt_nl
                << "this->CORBA_UserException::operator= "
                << "(_tao_excp);\n";
      // assign each individual member
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cs::"
                             "visit_exception -"
                             "codegen for scope failed\n"), -1);
        }
      delete visitor;
      os->indent ();
      *os << "return *this;" << be_uidt_nl
                << "}\n\n";

      // narrow method
      os->indent ();
      *os << "// narrow" << be_nl;
      *os << node->name () << " *" << be_nl;
      *os << node->name () << "::_narrow (CORBA::Exception *exc)" << be_nl;
      *os << "{\n";
      os->incr_indent ();
      *os << "if (!ACE_OS::strcmp (\"" << node->repoID ()
          << "\", exc->_id ())) // same type" << be_nl;
      *os << "  return ACE_dynamic_cast (" << node->local_name ()
          << " *, exc);" << be_nl;
      *os << "else" << be_nl;
      *os << "  return 0;\n";
      os->decr_indent ();
      *os << "}\n\n";

      *os << be_nl
          << "void " << node->name () << "::_raise ()" << be_nl
          << "{" << be_idt_nl
          << "TAO_RAISE(*this);" << be_uidt_nl
          << "}\n\n";

      *os << be_nl
          << "void " << node->name ()
          << "::_tao_encode (" << be_idt << be_idt_nl
          << "TAO_OutputCDR &cdr," << be_nl
          << "CORBA::Environment &ACE_TRY_ENV) const"
          << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "if (cdr << *this)" << be_nl
          << "  return;" << be_nl
          << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl
          << "}\n\n";

      *os << be_nl
          << "void " << node->name ()
          << "::_tao_decode (" << be_idt << be_idt_nl
          << "TAO_InputCDR &cdr," << be_nl
          << "CORBA::Environment &ACE_TRY_ENV)"
          << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "if (cdr >> *this)" << be_nl
          << "  return;" << be_nl
          << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl
          << "}\n\n";

      // generate the _alloc method
      os->indent ();
      *os << "// TAO extension - the _alloc method" << be_nl;
      *os << "CORBA::Exception *" << node->name ()
          << "::_alloc (void)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "CORBA::Exception *retval = 0;" << be_nl
          << "ACE_NEW_RETURN (retval, ::" << node->name ()
          << ", 0);" << be_nl
          << "return retval;" << be_uidt_nl;
      *os << "}\n\n";

      // constructor taking all members. It exists only if there are any
      // members
      if (node->member_count () > 0)
        {
          // generate the signature
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CS);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception::"
                                 "visit_exception - "
                                 "codegen for ctor failed\n"), -1);
            }
          delete visitor;

          if (!node->is_local ())
            *os << "  : CORBA_UserException "
                << " (CORBA::TypeCode::_duplicate (" << node->tc_name ()
                << "))" << be_nl;
          *os << "{\n";
          os->incr_indent ();
          // assign each individual member. We need yet another state
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
          ctx.exception (1); // indicate that the special ctor is being
                             // generated
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception -"
                                 "codegen for scope failed\n"), -1);
            }
          delete visitor;
          os->decr_indent ();
          *os << "}\n\n";
        }

      if (!node->is_local ())
        {
          // by using a visitor to declare and define the TypeCode, we
          // have the added advantage to conditionally not generate
          // any code. This will be based on the command line
          // options. This is still TO-DO
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
          ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception - "
                                 "TypeCode definition failed\n"
                                 ), -1);
            }
        }

      node->cli_stub_gen (I_TRUE);
    }

  return 0;
}
