
//=============================================================================
/**
 *  @file    ami_handler_reply_stub_operation_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================

// ************************************************************
// Operation visitor for client stubs.
// ************************************************************

be_visitor_operation_ami_handler_reply_stub_operation_cs::
be_visitor_operation_ami_handler_reply_stub_operation_cs (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_reply_stub_operation_cs::
~be_visitor_operation_ami_handler_reply_stub_operation_cs (void)
{
}

// Processing to be done after every element in the scope is processed
int
be_visitor_operation_ami_handler_reply_stub_operation_cs::post_process (
    be_decl *bd
  )
{
  // All we do here is to insert a comma and a newline.
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << ",\n";
    }

  return 0;
}

int
be_visitor_operation_ami_handler_reply_stub_operation_cs::visit_operation (
  be_operation *node)
{
  be_type *bt = 0;
  be_visitor_context ctx;

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Initialize the return type variable.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_ami_handler_reply_stub_operation_cs::"
          "visit_operation - "
          "Bad return type\n"
        ),
        -1
      );
    }

  // Start with the current indentation level.
  os->indent ();

  // Generate the return type. Return type is simply void.
  *os << be_nl_2
      << "void" << be_nl;

  // Get the scope name.
  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  if (!parent)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_ami_")
                         ACE_TEXT ("handler_reply_stub_operation_cs::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("node information not sufficient :-<\n")),
                        -1);
    }

  // Genereate scope name.
  *os << parent->full_name ();

  // Generate the operation name.
  *os << "::";

  *os << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << "_reply_stub (" << be_idt_nl
      << "TAO_InputCDR &_tao_in, " << be_nl
      << "::Messaging::ReplyHandler_ptr _tao_reply_handler," << be_nl
      << "::CORBA::ULong reply_status"
      << ")" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl;

  *os << "// Retrieve Reply Handler object." << be_nl;
  *os << parent->full_name () << "_var "
      << "_tao_reply_handler_object =" << be_idt_nl;

  *os << parent->full_name ();
  *os << "::_narrow (_tao_reply_handler);" << be_uidt
      << be_nl_2
      << "// Exception handling" << be_nl
      << "switch (reply_status)" << be_nl
      << "{" << be_idt_nl
      << "case TAO_AMI_REPLY_OK:" << be_nl
      << "{" << be_idt << "\n";

  // declare variables for arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
  be_visitor_operation_argument oad_visitor (&ctx);

  if (node->accept (&oad_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  // Demarshal parameters
  if (this->gen_marshal_and_invoke (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%1) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  *os << "break;" << be_uidt_nl
      << "}" << be_nl
      << "case TAO_AMI_REPLY_USER_EXCEPTION:" << be_nl
      << "case TAO_AMI_REPLY_SYSTEM_EXCEPTION:" << be_nl
      << "{" << be_idt_nl
      << "const ACE_Message_Block* cdr = _tao_in.start ();" << be_nl ;

  const char *exception_data_arg = "0";
  const char *exception_count_arg = "0";

  // Don't do anything if the exception list is empty.
  if (node->exceptions ())
    {
      *os << be_nl << "static TAO::Exception_Data " << "exceptions_data [] = " << be_nl;
      *os << "{" << be_idt_nl;

      int excep_count = 0;

      be_exception *ex = 0;

      // Initialize an iterator to iterate thru the exception list.
      // Continue until each element is visited.
      // Iterator must be explicitly advanced inside the loop.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();)
        {
          ex = be_exception::narrow_from_decl (ei.item ());

          *os << "{" << be_idt_nl
              << "\"" << ex->repoID () << "\"," << be_nl;
          // Allocator method.
          *os << ex->name () << "::_alloc"
              << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl;

          if (be_global->tc_support ())
            {
              *os << ", " << ex->tc_name ();
            }
          else
            {
              *os << ", 0";
            }

          *os << "\n#endif /* TAO_HAS_INTERCEPTORS */" << be_uidt_nl
              << "}";

          ++excep_count;
          ei.next ();

          if (!ei.is_done ())
            {
              *os << "," << be_nl;
            }

        }

      *os << be_uidt_nl << "};" << be_nl_2;

      *os << "::CORBA::ULong const exceptions_count = "
          << excep_count << ";\n" << be_nl;

      exception_data_arg = "exceptions_data";
      exception_count_arg = "exceptions_count";
    }

  *os << "::CORBA::OctetSeq "
      << "_tao_marshaled_exception (" << be_idt << be_idt_nl
      << "static_cast <CORBA::ULong> (cdr->length ())," << be_nl
      << "static_cast <CORBA::ULong> (cdr->length ())," << be_nl
      << "reinterpret_cast <unsigned char*> (cdr->rd_ptr ())," << be_nl
      << "0" << be_uidt_nl
      << ");" << be_uidt_nl;

 *os  << "::Messaging::ExceptionHolder* exception_holder_ptr = 0;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "exception_holder_ptr," << be_nl
      << "::TAO::ExceptionHolder (" << be_idt_nl
      << "(reply_status == TAO_AMI_REPLY_SYSTEM_EXCEPTION)," << be_nl
      << "_tao_in.byte_order ()," << be_nl
      << "_tao_marshaled_exception," << be_nl
      << exception_data_arg << "," << be_nl
      << exception_count_arg  << "," << be_nl
      << "_tao_in.char_translator ()," << be_nl
      << "_tao_in.wchar_translator ()" << ")" << be_uidt_nl
      << ");" << be_uidt_nl << be_uidt_nl;

  *os << "::Messaging::ExceptionHolder_var exception_holder_var = "
      << "exception_holder_ptr;" << be_nl;

  *os << "_tao_reply_handler_object->"
      << node->local_name () << "_excep (" << be_idt << be_idt_nl
      << "exception_holder_var";

  *os << be_uidt_nl << ");" << be_uidt_nl;

  *os << "break;" << be_uidt_nl
      << "}" << be_nl;

  *os << "case TAO_AMI_REPLY_NOT_OK:" << be_idt_nl
      << "// @@ Michael: Not even the spec mentions this case." << be_nl
      << "//             We have to think about this case." << be_nl
      << "break;" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;
  *os << "}";

  return 0;
}

// This method is used to generate the ParamData table entry.
int
be_visitor_operation_ami_handler_reply_stub_operation_cs::visit_argument (
    be_argument *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  // Retrieve the type for this argument.
  bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        "(%N:%l) be_visitor_operation_ami_handler_reply_stub_operation_cs::"
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
be_visitor_operation_ami_handler_reply_stub_operation_cs::gen_pre_stub_info (
    be_operation *node,
    be_type *
  )
{
  // Check if this operation raises any exceptions. In that case, we must
  // generate a list of exception typecodes. This is not valid for
  // attributes
  if (!this->ctx_->attribute ())
    {
      be_visitor_context ctx = *this->ctx_;
      be_visitor_operation_exceptlist_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((
            LM_ERROR,
            "(%N:%l) "
            "be_visitor_operation_ami_handler_reply_stub_operation_cs::"
            "gen_pre_stub_info - "
            "Exceptionlist generation error\n"),
          -1);
        }
    }

  return 0;
}

int
be_visitor_operation_ami_handler_reply_stub_operation_cs::gen_marshal_and_invoke (
    be_operation *node,
    be_type *
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  os->indent ();

  *os << "// Demarshall all the arguments." << be_nl;

  if (this->has_param_type (node, AST_Argument::dir_IN))
    {
      *os << "if (!(\n" << be_idt << be_idt << be_idt;

      // demarshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      be_visitor_operation_argument_marshal visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l)  be_visitor_operation_ami_handler_reply_stub_operation_cs::"
                             "gen_marshal_params - "
                             "codegen for args failed\n"),
                            -1);
        }

      *os << be_uidt << be_uidt_nl
          << " ))" << be_nl;

      *os << "throw ::CORBA::MARSHAL ();" << be_uidt_nl << be_nl;
    }


  // Invoke the callback method
  *os << "// Invoke the call back method." << be_nl
      << "_tao_reply_handler_object->";

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "set_";
        }
      else
        {
          *os << "get_";
        }
    }

  *os << node->local_name () << " (" << be_idt << be_idt_nl;

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
  be_visitor_operation_argument oau_visitor (&ctx);

  if (node->accept (&oau_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  *os << be_uidt_nl << ");" << be_uidt
      << be_nl;

  return 0;
}
