//=============================================================================
/**
*  @file   amh_ss.cpp
*
*  $Id$
*
*  Creates code for AMH operations.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include "ace/SString.h"

be_visitor_amh_operation_ss::be_visitor_amh_operation_ss (
      be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_operation_ss::~be_visitor_amh_operation_ss (void)
{
}

int
be_visitor_amh_operation_ss::visit_operation (be_operation *node)
{
  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    {
      return 0;
    }

  /// These are not for the server side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  if (this->generate_shared_prologue (node, os, "") == -1)
    {
      return -1;
    }

  int argument_count =
    node->count_arguments_with_direction (AST_Argument::dir_IN
                                          | AST_Argument::dir_INOUT);

  if (argument_count != 0)
    {
      // Declare variables for arguments.
      be_visitor_context vardecl_ctx = *this->ctx_;
      vardecl_ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);

      be_visitor_args_vardecl_ss vardecl_visitor (&vardecl_ctx);
      vardecl_visitor.set_fixed_direction (AST_Argument::dir_IN);

      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          be_argument *argument =
            be_argument::narrow_from_decl (si.item ());

          if (argument == 0
              || argument->direction () == AST_Argument::dir_OUT)
            {
              continue;
            }

          if (vardecl_visitor.visit_argument (argument) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_amh_operation_ss::"
                                 "visit_operation - "
                                 "codegen for return var decl failed\n"),
                                -1);
            }
        }

      *os << be_nl
          << "TAO_InputCDR & _tao_in ="
          << " *_tao_server_request.incoming ();" << be_nl_2
          << "if (!(" << be_idt << be_idt;

      // Marshal each in and inout argument.
      be_visitor_context marshal_ctx = *this->ctx_;
      marshal_ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
      marshal_ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);

      be_visitor_args_marshal_ss marshal_visitor (&marshal_ctx);
      marshal_visitor.set_fixed_direction (AST_Argument::dir_IN);
      int i = 0;

      for (UTL_ScopeActiveIterator sj (node, UTL_Scope::IK_decls);
           !sj.is_done ();
           sj.next ())
        {
          be_argument *argument =
            be_argument::narrow_from_decl (sj.item ());

          if (argument == 0
              || argument->direction () == AST_Argument::dir_OUT)
            {
              continue;
            }

          if (i++ != 0)
            {
              *os << " &&";
            }

          if (marshal_visitor.visit_argument (argument) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_ss::"
                                 "visit_operation - "
                                 "codegen for demarshal failed\n"),
                                -1);
            }
        }

      *os << be_uidt_nl << "))" << be_nl;

      // If marshaling fails, raise exception.
      if (this->gen_raise_exception ("::CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) gen_raise_exception failed\n"),
                            -1);
        }

      *os << be_uidt_nl;
    }

  if (this->generate_shared_section (node, os) == -1)
    {
      return -1;
    }

  {
    be_visitor_context ctx (*this->ctx_);
    ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
    be_visitor_args_upcall_ss visitor (&ctx);
    visitor.set_fixed_direction (AST_Argument::dir_IN);

    for (UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
         !i.is_done ();)
      {
        be_argument *argument =
          be_argument::narrow_from_decl (i.item ());

        i.next ();

        if (argument == 0
            || argument->direction () == AST_Argument::dir_OUT)
          {
            continue;
          }

        *os << ",";

        if (argument->accept (&visitor) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_amh_operation_ss::"
                               "visit_operation - "
                               "codegen for upcall args failed\n"),
                              -1);
          }
      }
  }

  if (this->generate_shared_epilogue (os) == -1)
    {
      return -1;
    }

  return 0;

}

int
be_visitor_amh_operation_ss::visit_attribute (be_attribute *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  if (this->generate_shared_prologue (node, os, "_get_") == -1)
    {
      return -1;
    }

  if (this->generate_shared_section (node, os) == -1)
    {
      return -1;
    }

  if (this->generate_shared_epilogue (os) == -1)
    {
      return -1;
    }

  if (node->readonly ())
    {
      return 0;
    }

  if (this->generate_shared_prologue (node, os, "_set_") == -1)
    {
      return -1;
    }

  be_argument the_argument (AST_Argument::dir_IN,
                            node->field_type (),
                            node->name ());

  int status = 0;

  {
    be_visitor_context ctx (*this->ctx_);
    be_visitor_args_vardecl_ss vardecl_visitor (&ctx);

    status = vardecl_visitor.visit_argument (&the_argument);

    if (-1 == status)
      {
        the_argument.destroy ();
        return -1;
      }
  }

  *os << be_nl
      << "TAO_InputCDR & _tao_in ="
      << " *_tao_server_request.incoming ();"
      << be_nl_2
      << "if (!(" << be_idt << be_idt;

  {
    be_visitor_context ctx (*this->ctx_);
    ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
    ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
    be_visitor_args_marshal_ss marshal_visitor (&ctx);

    status = marshal_visitor.visit_argument (&the_argument);

    if (-1 == status)
      {
        the_argument.destroy ();
        return -1;
      }
  }

  *os << be_uidt_nl << "))" << be_nl
      << "{" << be_idt_nl;

  // If marshaling fails, raise exception.
  status = this->gen_raise_exception ("::CORBA::MARSHAL",
                                      "");
  if (-1 == status)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) gen_raise_exception failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl;

  if (-1 == this->generate_shared_section (node, os))
    {
      return -1;
    }

  *os << ",";

  {
    be_visitor_args_upcall_ss upcall_visitor (this->ctx_);
    status = upcall_visitor.visit_argument (&the_argument);
    the_argument.destroy ();

    if (-1 == status)
      {
        return -1;
      }
  }

  if (-1 == this->generate_shared_epilogue (os))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_prologue (be_decl *node,
                                                       TAO_OutStream *os,
                                                       const char *skel_prefix)
{
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());

  if (intf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }


  char *buf;
  intf->compute_full_name ("AMH_", "", buf);
  ACE_CString amh_skel_name ("POA_");
  amh_skel_name += buf;
  // buf was allocated by ACE_OS::strdup, so we need to use free instead
  // of delete.
  ACE_OS::free (buf);
  buf = 0;

  *os << "void" << be_nl
      << amh_skel_name.c_str () << "::"
      << skel_prefix << this->ctx_->port_prefix ().c_str ()
      << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest & _tao_server_request," << be_nl
      << "void * /* context */," << be_nl
      << "void * _tao_servant" << be_uidt_nl
      << ")" << be_uidt_nl;

  // Generate the actual code for the skeleton.
  // last argument
  *os << "{" << be_idt_nl;

  // Get the right object implementation.
  *os << amh_skel_name.c_str () << " * const _tao_impl =" << be_idt_nl
      << "static_cast<" << amh_skel_name.c_str () << " *> ("
      << "_tao_servant" << ");" << be_uidt_nl;

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_section (be_decl *node,
                                                      TAO_OutStream *os)
{
  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) generate_shared_section - "
                         "bad interface scope\n"),
                        -1);
    }

  // Create the response handler
  char *buf;
  intf->compute_full_name ("TAO_AMH_", "ResponseHandler", buf);
  ACE_CString response_handler_implementation_name ("POA_");
  response_handler_implementation_name += buf;
  // buf was allocated by ACE_OS::strdup, so we need to use free instead
  // of delete.
  ACE_OS::free (buf);
  buf = 0;

  *os << be_nl
      << "TAO_ORB_Core *orb_core =" << be_idt_nl
      << "_tao_server_request.orb ()->orb_core ();" << be_uidt_nl << be_nl
      << "TAO_AMH_BUFFER_ALLOCATOR* amh_allocator =" << be_idt_nl
      << "orb_core->lane_resources ().amh_response_handler_allocator ();"
      << be_uidt_nl << be_nl
      << "TAO::TAO_Buffer_Allocator<" << be_idt << be_idt_nl
      << response_handler_implementation_name.c_str () << "," << be_nl
      << "TAO_AMH_BUFFER_ALLOCATOR" << be_uidt_nl
      << "> buffer_allocator (amh_allocator);"
      << be_uidt_nl << be_nl
      << response_handler_implementation_name.c_str ()
      << "_ptr _tao_rh_ptr = "
      << be_idt_nl
      << "buffer_allocator.allocate();"
      << be_uidt_nl << be_nl
      << "if (!_tao_rh_ptr) " << be_idt_nl << "throw ::CORBA::NO_MEMORY ();"
      << be_uidt_nl;

  // Initialize amh rh
  *os << be_nl << "_tao_rh_ptr->init (_tao_server_request, amh_allocator);" << be_nl
      << be_nl;

  *os <<  "ACE_Utils::Auto_Functor <"
      << response_handler_implementation_name.c_str ()
      << ", TAO::ARH_Refcount_Functor> safe_rd_(_tao_rh_ptr);"
      << be_nl;

  // Make the upcall.
  *os << be_nl << "_tao_impl->"
      << node->local_name () << " (" << be_idt << be_idt_nl
      << "safe_rd_.get ()";

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_epilogue (TAO_OutStream *os)
{
  *os << be_uidt_nl << ");"
      << be_uidt << be_uidt_nl
      << "}";

  return 0;
}
