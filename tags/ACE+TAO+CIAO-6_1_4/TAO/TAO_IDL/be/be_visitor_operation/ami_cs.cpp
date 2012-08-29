
//=============================================================================
/**
 *  @file    ami_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu> Michael Kircher
 */
//=============================================================================

#include "operation.h"

be_visitor_operation_ami_cs::be_visitor_operation_ami_cs (
    be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_cs::~be_visitor_operation_ami_cs (void)
{
}

// Processing to be done after every element in the scope is
// processed.
int
be_visitor_operation_ami_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << ",\n";
    }

  return 0;
}

int
be_visitor_operation_ami_cs::visit_operation (be_operation *node)
{
  // No sendc method for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  be_visitor_context ctx;
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the return type mapping. Return type is simply void.
  *os << be_nl_2
      << "void" << be_nl;

  // Generate the operation name.

  // Grab the scope name.
  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  if (parent == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "scope name is nil\n"),
                        -1);
    }

  // Generate the scope::operation name.
  *os << parent->full_name ()
      << "::" << this->ctx_->port_prefix ().c_str ()
      << node->local_name ()->get_string ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist oa_visitor (&ctx);

  // Get the AMI version from the strategy class.
  be_operation *ami_op = node;

  if (ami_op->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument
  *os << be_nl << "{" << be_idt;

  if (node->has_native ()) // native exists => no stub
    {
      be_predefined_type bpt (AST_PredefinedType::PT_void,
                              0);

      int const status = this->gen_raise_exception ("::CORBA::MARSHAL",
                                                    "");

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for has-native exception failed\n"),
                            -1);
        }
    }
  else
    {
      *os << be_nl
          << "if (!this->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "::CORBA::Object::tao_object_initialize (this);"
          << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;
    }

  // Includes the reply handler, but we have to add 1 for the retval anyway.
  int nargs = ami_op->argument_count ();

  if (nargs == 1)
    {
      // No arguments other than the reply handler, and the return
      // type is void.  No need to generate argument list.

      *os << be_nl_2
          << "TAO::Argument ** _the_tao_operation_signature = 0;";
      nargs = 0; // Don't count the reply handler.
    }
  else
    {
      *os << be_nl<< be_nl
          << "TAO::Arg_Traits<void>::"
          << (node->flags () == AST_Operation::OP_oneway &&
              be_global->use_clonable_in_args() ? "clonable_" : "")
          << "ret_val _tao_retval;";

      // Declare the argument helper classes.
      this->gen_stub_body_arglist (ami_op, os, true);

      // Assemble the arg helper class pointer array.
      *os << be_nl_2
          << "TAO::Argument *_the_tao_operation_signature[] =" << be_idt_nl
          << "{" << be_idt_nl
          << "&_tao_retval";

      AST_Argument *arg = 0;
      UTL_ScopeActiveIterator arg_list_iter (ami_op,
                                             UTL_Scope::IK_decls);

      // For a sendc_* operation, skip the reply handler (first argument).
      arg_list_iter.next ();

      for (; ! arg_list_iter.is_done (); arg_list_iter.next ())
        {
          arg = AST_Argument::narrow_from_decl (arg_list_iter.item ());

          *os << "," << be_nl
              << "&_tao_" << arg->local_name ();
        }

      *os << be_uidt_nl
          << "};" << be_uidt;
    }

  ACE_CString base (node->local_name ()->get_string ());

  /// The sendc_* operation makes the invocation with the
  /// original operation name.
  ACE_CString lname_str (base.substr (ACE_OS::strlen ("sendc_")));
  const char *lname = lname_str.c_str ();

  ACE_CString opname (node->is_attr_op () ? "_" : "");
  opname += lname;

  /// Some compilers can't resolve the stream operator overload.
  const char *op_name = opname.c_str ();
  ACE_CDR::ULong len = opname.length ();

  *os << be_nl_2
      << "TAO::Asynch_Invocation_Adapter _tao_call (" << be_idt << be_idt_nl
      << "this," << be_nl
      << "_the_tao_operation_signature," << be_nl
      << nargs << "," << be_nl
      << "\"" << op_name << "\"," << be_nl
      << len << "," << be_nl;

  *os << "TAO::TAO_CO_NONE";
  if (be_global->gen_direct_collocation())
    {
      *os << " | TAO::TAO_CO_DIRECT_STRATEGY";
    }
  if (be_global->gen_thru_poa_collocation())
    {
      *os << " | TAO::TAO_CO_THRU_POA_STRATEGY";
    }

  *os << be_uidt_nl
      << ");" << be_uidt;

  *os << be_nl_2
      << "_tao_call.invoke (" << be_idt << be_idt_nl
      << "ami_handler," << be_nl
      << "&";

  if (parent->is_nested ())
    {
      be_decl *gparent =
        be_scope::narrow_from_scope (parent->defined_in ())->decl ();

      *os << gparent->name () << "::";
    }

  *os << "AMI_"  << parent->local_name () << "Handler::"
      << lname << "_reply_stub" << be_uidt_nl
      << ");" << be_uidt;

  *os << be_uidt_nl
      << "}";

  return 0;
}

// This method is used to generate the ParamData table entry.
int
be_visitor_operation_ami_cs::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0; // argument type

  // Retrieve the type for this argument.
  bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";

  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }

  *os << "0}";

  return 0;
}

int
be_visitor_operation_ami_cs::gen_pre_stub_info (be_operation *,
                                                be_type *)
{
  // Nothing to be done here, we do not throw any exceptions
  // besides system exceptions, so we do not need an user exception table.
  return 0;
}

