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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, exception_cs, "$Id$")


// ***************************************************************************
// For client stubs.
// ***************************************************************************
be_visitor_exception_cs::be_visitor_exception_cs (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cs::~be_visitor_exception_cs (void)
{
}

// Visit the Exception_Cs node and its scope.
int be_visitor_exception_cs::visit_exception (be_exception *node)
{
  TAO_OutStream *os = 0;

  if (!node->cli_stub_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      os->indent ();

      // Generate stub code  required of any anonymous types of members.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception::"
                             "visit_exception -"
                             "code for stub failed\n"),
                            -1);
        }

      // Default constructor.
      *os << "// Default constructor." << be_nl;
      *os << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl;
      *os << "  : CORBA_UserException (\""
          << node->repoID () << "\")\n";
      *os << "{" << be_nl;
      *os << "}" << be_nl << be_nl;

      // Destructor.
      *os << "// Destructor - all members are of self managing types."
          << be_nl;
      *os << node->name () << "::~" << node->local_name ()
          << " (void)" << be_nl;
      *os << "{" << be_nl;
      *os << "}" << be_nl << be_nl;

      if (!node->is_local ())
        {
          *os << "void "
              << node->name ()
              << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
              << "{" << be_idt_nl
              << node->local_name () << " *tmp = ACE_static_cast ("
              << node->local_name () << "*, _tao_void_pointer);" << be_nl
              << "delete tmp;" << be_uidt_nl
              << "}" << be_nl << be_nl;
        }

      // Copy constructor.
      *os << "// Copy constructor." << be_nl;
      *os << node->name () << "::" << node->local_name () << " (const ::"
          << node->name () << " &_tao_excp)" << be_nl;
      *os << "  : CORBA_UserException ("
          << "_tao_excp._id ())" << be_nl;
      *os << "{\n";

      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      be_visitor *visitor = 0;

      if (node->nmembers () > 0)
        {
          *os << be_idt;

          // Assign each individual member.
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception -"
                                 "codegen for scope failed\n"),
                                -1);
            }

          delete visitor;
          *os << be_uidt;
          os->decr_indent ();
        }

      *os << "}" << be_nl << be_nl;

      // Assignment operator.
      *os << "// Assignment operator." << be_nl;
      *os << node->name () << "&" << be_nl;
      *os << node->name () << "::operator= (const ::"
          << node->name () << " &_tao_excp)" << be_nl
          << "{" << be_idt_nl
          << "this->CORBA_UserException::operator= "
          << "(_tao_excp);\n";

      // Assign each individual member.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cs::"
                             "visit_exception -"
                             "codegen for scope failed\n"),
                            -1);
        }

      delete visitor;
      os->indent ();
      *os << "return *this;" << be_uidt_nl
          << "}" << be_nl << be_nl;

      // Narrow method.
      *os << "// Narrow." << be_nl;
      *os << node->name () << " *" << be_nl;
      *os << node->name () << "::_downcast (CORBA::Exception *exc)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "if (!ACE_OS::strcmp (\"" << node->repoID ()
          << "\", exc->_id ()))" << be_idt_nl;
      *os << "{" << be_idt_nl;
      *os << "return ACE_dynamic_cast (" << node->local_name ()
          << " *, exc);" << be_uidt_nl;
      *os << "}" << be_uidt_nl;
      *os << "else" << be_idt_nl;
      *os << "{" << be_idt_nl;
      *os << "return 0;" << be_uidt_nl;
      *os << "}" << be_uidt << be_uidt_nl;
      *os << "}" << be_nl << be_nl;

      *os << "void " << node->name () << "::_raise ()" << be_nl
          << "{" << be_idt_nl
          << "TAO_RAISE (*this);" << be_uidt_nl
          << "}" << be_nl << be_nl;

      *os << "void " << node->name ()
          << "::_tao_encode (" << be_idt << be_idt_nl;

      if (!node->is_local ())
        {
          *os << "TAO_OutputCDR &cdr" << be_nl
              << "ACE_ENV_ARG_DECL" << be_uidt_nl
              << ") const" << be_uidt_nl
              << "{" << be_idt_nl
              << "if (cdr << *this)" << be_idt_nl
              << "{" << be_idt_nl
              << "return;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;

          if (be_global->use_raw_throw ())
            {
              *os << "throw CORBA::MARSHAL ();" << be_uidt_nl;
            }
          else
            {
              *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl;
            }

          *os << "}" << be_nl << be_nl;
        }
      else
        {
          *os << "TAO_OutputCDR &" << be_nl
              << "ACE_ENV_ARG_DECL" << be_uidt_nl
              << ") const" << be_uidt_nl
              << "{" << be_idt_nl;

          if (be_global->use_raw_throw ())
            {
              *os << "throw CORBA::MARSHAL ();" << be_uidt_nl;
            }
          else
            {
              *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl;
            }

          *os << "}" << be_nl << be_nl;
        }

      *os << "void " << node->name ()
          << "::_tao_decode (" << be_idt << be_idt_nl;

      if (!node->is_local ())
        {
          *os << "TAO_InputCDR &cdr" << be_nl
              << "ACE_ENV_ARG_DECL" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "if (cdr >> *this)" << be_idt_nl
              << "{" << be_idt_nl
              << "return;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;

          if (be_global->use_raw_throw ())
            {
              *os << "throw CORBA::MARSHAL ();" << be_uidt_nl;
            }
          else
            {
              *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl;
            }

          *os << "}" << be_nl << be_nl;
        }
      else
        {
          *os << "TAO_InputCDR &" << be_nl
              << "ACE_ENV_ARG_DECL" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl;

          if (be_global->use_raw_throw ())
            {
              *os << "throw CORBA::MARSHAL ();" << be_uidt_nl;
            }
          else
            {
              *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl;
            }

          *os << "}" << be_nl << be_nl;
        }

      // Generate the _alloc method.
      *os << "// TAO extension - the _alloc method." << be_nl;
      *os << "CORBA::Exception *" << node->name ()
          << "::_alloc (void)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "CORBA::Exception *retval = 0;" << be_nl
          << "ACE_NEW_RETURN (retval, ::" << node->name ()
          << ", 0);" << be_nl
          << "return retval;" << be_uidt_nl;
      *os << "}" << be_nl << be_nl;

      // Constructor taking all members. It exists only if there are any
      // members.
      if (node->member_count () > 0)
        {
          // Generate the signature.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_CS);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception::"
                                 "visit_exception - "
                                 "codegen for ctor failed\n"),
                                -1);
            }

          delete visitor;

          *os << "  : CORBA_UserException "
              << "(\"" << node->repoID () << "\")" << be_uidt_nl;
          *os << "{\n";

          // Assign each individual member. We need yet another state.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS);

          // Indicate that the special ctor is being generated.
          ctx.exception (1);

          visitor = tao_cg->make_visitor (&ctx);

          *os << be_idt;

          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception -"
                                 "codegen for scope failed\n"),
                                -1);
            }

          delete visitor;
          os->decr_indent ();
          *os << "}\n\n";
        }

      if (!node->is_local ())
        {
          // By using a visitor to declare and define the TypeCode, we
          // have the added advantage to conditionally not generate
          // any code. This will be based on the command line
          // options. This is still TO-DO.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_TYPECODE_DEFN);
          ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
          visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (node->accept (visitor) == -1))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_exception_cs::"
                                 "visit_exception - "
                                 "TypeCode definition failed\n"),
                                -1);
            }
        }

      if (!node->is_local () && be_global->tc_support ())
        {
          *os << "\n// TAO extension - the virtual _type method." << be_nl;
          *os << "CORBA::TypeCode_ptr " << node->name ()
              << "::_type (void) const" << be_nl;
          *os << "{" << be_idt_nl;
          *os << "return ::" << node->tc_name () << ";" << be_uidt_nl;
          *os << "}" << be_nl << be_nl;
        }

      node->cli_stub_gen (I_TRUE);
    }

  return 0;
}
