//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, cdr_op_ch, "$Id$")

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_interface_cdr_op_ch::be_visitor_interface_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_ch::~be_visitor_interface_cdr_op_ch (void)
{
}

int
be_visitor_interface_cdr_op_ch::visit_interface (be_interface *node)
{
  // No CDR operations for locality constraint interfaces.
  if (idl_global->gen_locality_constraint ())
    return 0;
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
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
              ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH);
              be_visitor *visitor = tao_cg->make_visitor (&ctx);

              if (!visitor)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "be_visitor_operation_cdr_op_ch::"
                                     "visit_operation - "
                                     "Bad visitor to return type\n"),
                                    -1);
                }

              if (bt->accept (visitor) == -1)
                {
                  delete visitor;
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_operation_cdr_op_ch::"
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
                             "(%N:%l) be_visitor_interface_cdr_op_ch::visit_interface - "
                             "codegen failed - invalid state\n"), -1);
        }

#endif 0
      // generate the CDR << and >> operator declarations
      os->indent ();
      *os << "CORBA::Boolean " << idl_global->stub_export_macro () << be_nl
          << "operator<< (TAO_OutputCDR &, const " << node->full_name ()
          << "_ptr );" << be_nl;
      *os << "CORBA::Boolean " << idl_global->stub_export_macro () << be_nl
          << "operator>> (TAO_InputCDR &, "
          << node->full_name () << "_ptr &);\n";


      // set the substate as generating code for the types defined in our scope
      this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
      // all we have to do is to visit the scope and generate code
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cdr_op_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"), -1);
        }


      node->cli_hdr_cdr_op_gen (1);
#if 0
      switch (i)
        {
        case 0:
        case 1:
        case 2:
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cdr_op_ch::visit_interface - "
                             "codegen failed - invalid state\n"), -1);
        }
    } // for 
#endif /* 0 */
  return 0;
}
