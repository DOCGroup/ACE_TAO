//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, cdr_op_ci, "$Id$")

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_cdr_op_ci::be_visitor_interface_cdr_op_ci
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_ci::~be_visitor_interface_cdr_op_ci (void)
{
}

int
be_visitor_interface_cdr_op_ci::visit_interface (be_interface *node)
{
  // No CDR operations for locality constraint interfaces.
  if (idl_global->gen_locality_constraint ())
    return 0;

  // already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

#if 0
  int i;
  int to_do;

  // Decide how much code to produce
  if (idl_global->ami_call_back () == I_TRUE)
    to_do = 3;
  else
    to_do = 1;
  be_interface_type_strategy *old_strategy = 0;

  for (i = 0; i < to_do; i++)
    {
      switch (i)
        {
        case 0:
          break;
        case 1:
          old_strategy =
            node->set_strategy (new be_interface_ami_handler_strategy (node));
          break;
        case 2:
          delete node->set_strategy (old_strategy);
            {
              be_valuetype *excep_holder = new be_valuetype (node->name (),
                                                             0,
                                                             0,
                                                             0,
                                                             0);
              excep_holder->set_name (node->name ());

              old_strategy = excep_holder->set_strategy (
                new be_interface_ami_exception_holder_strategy (excep_holder));

              be_type *bt = be_type::narrow_from_decl (excep_holder);

              // grab the right visitor to generate the code
              be_visitor_context ctx (*this->ctx_);
              ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI);
              be_visitor *visitor = tao_cg->make_visitor (&ctx);

              if (!visitor)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_operation_cdr_op_ci::"
                                     "visit_operation - "
                                     "Bad visitor to return type\n"),
                                    -1);
                }

              if (bt->accept (visitor) == -1)
                {
                  delete visitor;
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_operation_cdr_op_ci::"
                                     "visit_interface - "
                                     "codegen for return type failed\n"),
                                    -1);
                }
              delete visitor;
              //delete excep_holder;
              delete node->set_strategy (old_strategy);
            }            
          return 0;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cdr_op_ci::visit_interface - "
                             "codegen failed - invalid state\n"), -1);
        }

#endif /* 0 */ 

      // First generate code for our children. The reason we do this first is
      // because the inlined code for our children must be available before we use
      // it in our parent, but we must forward declare the parent
      // operators, so code like this:
      //
      // // IDL
      // interface Foo {
      //   exception Bar {Foo foo_member; };
      // };
      //
      // can work properly (the Foo::Bar operators need the Foo operators
      // defined).
      //

      // generate the CDR << and >> operator declarations
      os->indent ();
      *os << "ACE_INLINE CORBA::Boolean" << be_nl
          << "operator<< (" << be_idt << be_idt_nl
          << "TAO_OutputCDR &," << be_nl
          << "const " << node->full_name () << "_ptr" << be_uidt_nl
          << ");" << be_uidt_nl;
      *os << "ACE_INLINE CORBA::Boolean" << be_nl
          << "operator>> (" << be_idt << be_idt_nl
          << "TAO_InputCDR &," << be_nl
          << node->full_name () << "_ptr &" << be_uidt_nl
          << ");" << be_uidt << "\n\n";

      // Now it really the type to generate the operators for the members
      // of the interface...

      // set the substate as generating code for the types defined in our scope
      this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
      // all we have to do is to visit the scope and generate code
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cdr_op_ci"
                             "::visit_interface - "
                             "codegen for scope failed\n"), -1);
        }

      //  set the sub state as generating code for the output operator
      this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
      // @@ TODO don't use the IIOP interpreter here, we should use << and
      // >> operators, or even the operator for the base CORBA::Object!!
      *os << "ACE_INLINE CORBA::Boolean" << be_nl
          << "operator<< (" << be_idt << be_idt_nl
          << "TAO_OutputCDR &strm," << be_nl
          << "const " << node->full_name () << "_ptr _tao_objref" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl;
      *os << "CORBA::Object_ptr _tao_corba_obj = _tao_objref;" << be_nl;
      *os << "return (strm << _tao_corba_obj);" << be_uidt_nl
          << "}\n\n";

      // set the substate as generating code for the input operator
      this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
      // @@ TODO don't use the IIOP interpreter here, we should use << and
      // >> operators, or even the operator for the base CORBA::Object!!
      *os << "ACE_INLINE CORBA::Boolean" << be_nl
          << "operator>> (" << be_idt << be_idt_nl
          << "TAO_InputCDR &strm," << be_nl
          << node->full_name () << "_ptr &_tao_objref" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl;
      // hand over to the TAO's internal marshaling engine
      //  *os << "CORBA::Environment env;" << be_nl;
      *os << "ACE_TRY_NEW_ENV" << be_nl
          << "{" << be_idt_nl;
      *os << "CORBA::Object_var obj;" << be_nl;
      *os << "if ((strm >> obj.inout ()) == 0)" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "// narrow to the right type" << be_nl;
      *os << "_tao_objref =" << be_idt_nl
          << node->full_name () << "::_narrow (" << be_idt << be_idt_nl
          << "obj.in ()," << be_nl
          << "ACE_TRY_ENV" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
      *os << "ACE_TRY_CHECK;" << be_nl;
      *os << "return 1;" << be_uidt_nl;
      *os << "}" << be_nl
          << "ACE_CATCHANY" << be_nl
          << "{" << be_idt_nl
          << "// do nothing" << be_uidt_nl
          << "}" << be_nl
          << "ACE_ENDTRY;" << be_nl
          << "return 0;" << be_uidt_nl;
      *os << "}\n\n";

      node->cli_inline_cdr_op_gen (1);

#if 0
      switch (i)
        {
        case 0:
        case 1:
        case 2:
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cdr_op_ci::visit_interface - "
                             "codegen failed - invalid state\n"), -1);
        }
    } // for 
#endif /* 0 */

  return 0;
}
