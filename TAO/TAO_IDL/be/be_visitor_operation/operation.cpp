
//=============================================================================
/**
 *  @file    operation.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the stubs file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
}

// Is the operation return type void?
int
be_visitor_operation::void_return_type (be_type *bt)
{
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      be_predefined_type * const bpd =
        be_predefined_type::narrow_from_decl (bt);

      if (!bpd)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_IDL (%N:%l) "
                           "be_predefined_type::narrow_from_decl "
                           "failed\n"),
                          0);

      AST_PredefinedType::PredefinedType const pdt = bpd->pt ();

      if (pdt == AST_PredefinedType::PT_void)
        {
          return 1;
        }
    }

  return 0;
}

int
be_visitor_operation::has_param_type (be_operation *node,
                                      AST_Argument::Direction dir)
{
  return (node->count_arguments_with_direction (dir) != 0);
}

size_t
be_visitor_operation::count_non_out_parameters (be_operation *node)
{
  // @@ Once the valuetype issue discussed below is fixed we can
  //    replace this routine with:
  //
  // return node->count_arguments_with_direction (AST_Argument::dir_IN
  //                                              | AST_Argument::dir_INOUT);
  //
  size_t count = 0;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_argument *bd =
        be_argument::narrow_from_decl (si.item ());

      // We do not generate insertion operators for valuetypes
      // yet. Do not include them in the count.
      be_valuetype *vt =
        be_valuetype::narrow_from_decl (bd->field_type ());

      if (bd && (bd->direction () != AST_Argument::dir_OUT) && !vt)
        {
          ++count;
        }
    }

  return count;
}

int
be_visitor_operation::is_amh_exception_holder (be_interface *node)
{
  int is_an_amh_exception_holder = 0;
  const char *amh_underbar = "AMH_";
  const char *node_name = node->local_name ();

  if( amh_underbar[0] == node_name[0] &&
      amh_underbar[1] == node_name[1] &&
      amh_underbar[2] == node_name[2] &&
      amh_underbar[3] == node_name[3]
      ) // node name starts with "AMH_"
    {
      //ACE_DEBUG ((LM_DEBUG, "Passed first test of amh_excepholder\n"));
      const char *last_E = ACE_OS::strrchr (node->full_name (), 'E');
      if (last_E != 0
          && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
        {
          //ACE_DEBUG ((LM_DEBUG, "be_visitor_operation: Passed second test of amh_excepholder\n"));
          is_an_amh_exception_holder = 1;
        }
    }

  return is_an_amh_exception_holder;
}

int
be_visitor_operation::gen_raise_exception (const char *exception_name,
                                           const char *exception_arguments)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "throw "
      << exception_name << "(" << exception_arguments << ");";

  return 0;
}

int
be_visitor_operation::gen_stub_operation_body (
    be_operation *node,
    be_type *return_type
  )
{
  UTL_Scope *s =
    this->ctx_->attribute ()
      ? this->ctx_->attribute ()->defined_in ()
      : node->defined_in ();

  be_interface *intf = be_interface::narrow_from_scope (s);

  if (intf == 0)
    {
      be_porttype *pt = be_porttype::narrow_from_scope (s);

      if (pt == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation::")
                             ACE_TEXT ("gen_stub_operation_body - ")
                             ACE_TEXT ("bad scope\n")),
                            -1);
        }
      else
        {
          intf = this->ctx_->interface ();
        }
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  *os << be_nl << "{" << be_idt_nl;

  if (node->has_native ()) // native exists => no stub
    {
      if (this->gen_raise_exception ("::CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_cs::"
              "visit_operation - "
              "codegen for native exception failed\n"
            ),
            -1
          );
        }

      *os << be_uidt_nl << "}";

      return 0;
    }

  if (!node->is_abstract ())
    {
      // If the object is lazily evaluated the proxy broker might well
      // be null.  Initialize it now.
      *os << "if (!this->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "::CORBA::Object::tao_object_initialize (this);"
          << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
            *os << "if (this->the" << intf->base_proxy_broker_name () << "_ == 0)"
                << be_idt_nl
                << "{" << be_idt_nl
                << intf->flat_name () << "_setup_collocation ();"
                << be_uidt_nl
                << "}" << be_uidt_nl << be_nl;
        }
    }

  // Declare return type helper class.
  *os << "TAO::Arg_Traits< ";

  this->gen_arg_template_param_name (node,
                                     return_type,
                                     os);

  *os << ">::"
      << (node->flags () == AST_Operation::OP_oneway &&
          be_global->use_clonable_in_args() ? "clonable_" : "")
      << "ret_val _tao_retval;";

  // Declare the argument helper classes.
  this->gen_stub_body_arglist (node, os);

  *os << be_nl << be_nl
      << "TAO::Argument *_the_tao_operation_signature [] =" << be_idt_nl
      << "{" << be_idt_nl
      << "&_tao_retval";

  AST_Argument *arg = 0;

  for (UTL_ScopeActiveIterator arg_list_iter (node, UTL_Scope::IK_decls);
       ! arg_list_iter.is_done ();
       arg_list_iter.next ())
    {
      arg = AST_Argument::narrow_from_decl (arg_list_iter.item ());

      *os << "," << be_nl
          << "&_tao_" << arg->local_name ();
    }

  *os << be_uidt_nl
      << "};" << be_uidt;

  if (this->gen_pre_stub_info (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for exceptiondata failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "TAO::" << (node->is_abstract () ? "AbstractBase_" : "" )
      << "Invocation_Adapter _tao_call (" << be_idt << be_idt_nl
      << "this," << be_nl
      << "_the_tao_operation_signature," << be_nl
      << node->argument_count () + 1 << "," << be_nl
      << "\"";

  /// This logic handles the implied IDL for attributes.
  /// The AMI ReplyHandler version of generated get/set ops
  /// for attributes doesn't have the leading underscore.
  bool const escape = (node->is_attr_op () && !intf->is_ami_rh ());
  ACE_CString opname (escape ? "_" : "");

  /// This logic handles regular IDL for attributes. The AMI
  /// backend preprocessing visitor doesn't set the context
  /// member for attributes, but sets flags in the interface
  /// and operation nodes instead.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          opname += "_set_";
        }
      else
        {
          opname += "_get_";
        }
    }

  opname += node->original_local_name ()->get_string ();

  /// Some compilers can't resolve the stream operator overload.
  const char *lname = opname.c_str ();
  ACE_CDR::ULong len = opname.length ();

  *os << lname << "\"," << be_nl
      << len << "," << be_nl;

  if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
    {
      *os << "this->the" << intf->base_proxy_broker_name () << "_";
    }
  else
    {
      *os << "0";
    }

  if (node->flags () == AST_Operation::OP_oneway)
    {
      *os << "," << be_nl
          << "TAO::TAO_ONEWAY_INVOCATION";
    }

  *os << be_uidt_nl
      << ");" << be_uidt;

  *os << be_nl << be_nl;

  // Since oneways cannot raise user exceptions, we have that
  // case covered as well.
  if (node->exceptions ())
    {
      *os << "_tao_call.invoke (" << be_idt << be_idt_nl
          << "_tao_" << node->flat_name ()
          << "_exceptiondata," << be_nl
          << node->exceptions ()->length () << be_uidt_nl
          << ");" << be_uidt;
    }
  else
    {
      *os << "_tao_call.invoke (0, 0);";
    }

  if (!this->void_return_type (return_type))
    {
      *os << be_nl << be_nl
          << "return _tao_retval.retn ();";
    }

  *os << be_uidt_nl << "}";

  return 0;
}

int
be_visitor_operation::gen_pre_stub_info (
    be_operation *node
  )
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_exceptlist_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_cs::"
                         "gen_pre_stub_info - "
                         "Exceptionlist generation error\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation::gen_raise_interceptor_exception (
    be_type *bt,
    const char *excep,
    const char *completion_status
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->void_return_type (bt))
    {
      *os << "throw " << excep << "(" << completion_status << ");";
    }
  else
    {
      if (bt->size_type () == AST_Type::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "0" << be_uidt_nl
              << ");" << be_uidt;
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "_tao_retval" << be_uidt_nl
              << ");" << be_uidt;
        }
    }

  return 0;
}

void
be_visitor_operation::gen_stub_body_arglist (be_operation *node,
                                             TAO_OutStream *os,
                                             bool ami)
{
  AST_Argument *arg = 0;
  UTL_ScopeActiveIterator arg_decl_iter (node, UTL_Scope::IK_decls);

  if (ami)
    {
      // Skip the reply handler (first argument).
      arg_decl_iter.next ();
    }

  for (; ! arg_decl_iter.is_done (); arg_decl_iter.next ())
    {
      arg = AST_Argument::narrow_from_decl (arg_decl_iter.item ());

      *os << be_nl
          << "TAO::Arg_Traits< ";

      this->gen_arg_template_param_name (arg,
                                         arg->field_type (),
                                         os);

      *os << ">::";

      switch (arg->direction ())
        {
          case AST_Argument::dir_IN:
            *os << "in";
            if (be_global->use_clonable_in_args() &&
                node->flags () == AST_Operation::OP_oneway)
              {
                *os << "_clonable";
              }
            break;
          case AST_Argument::dir_INOUT:
            *os << "inout";
            break;
          case AST_Argument::dir_OUT:
            *os << "out";
          default:
            break;
        }

      *os << "_arg_val _tao_" << arg->local_name () << " ("
          << arg->local_name () << ");";
    }
}

void
be_visitor_operation::gen_arg_template_param_name (AST_Decl *scope,
                                                   AST_Type *bt,
                                                   TAO_OutStream *os)
{
  AST_Typedef *alias = 0;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      alias = AST_Typedef::narrow_from_decl (bt);
    }

  AST_Decl::NodeType nt = bt->unaliased_type ()->node_type ();
  ACE_CDR::ULong bound = 0;

  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      AST_String *s =
        AST_String::narrow_from_decl (bt->unaliased_type  ());
      bound = s->max_size ()->ev ()->u.ulval;

      // If the (w)string is unbounded, code is generated below by the
      // last line of this method, whether bt is a typedef or not.
      if (bound > 0)
        {
          *os << "::TAO::";

          if (alias != 0)
            {
              *os << alias->local_name () << "_" << bound;
            }
           else
            {
              // If we have an unaliased, bounded (w)string parameter,
              // we know that be_visitor_arg_traits has used an empty
              // struct of a type constructed from the flat name of
              // the parameter's 'enclosing scope' (the parameter
              // identifier for parameters, and the operation identifier
              // for return types) to get a unique type for the
              // Arg_Traits<> template parameter. So we generate it
              // the same way here, using the 'scope' argument's
              // flat name.
              *os << scope->flat_name ();
            }

          return;
        }
    }

  // We need the unaliased type name to make the code block below
  // work correctly.
  AST_Type *ut = bt->unaliased_type ();
  nt = ut->node_type ();

  // For the four predefined types below, we use the helper struct
  // type, in order to disambiguate the template parameter.
  if (nt == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt =
        AST_PredefinedType::narrow_from_decl (ut);

      switch (pdt->pt ())
        {
          case AST_PredefinedType::PT_boolean:
            *os << "::ACE_InputCDR::to_boolean";
            return;
          case AST_PredefinedType::PT_octet:
            *os << "::ACE_InputCDR::to_octet";
            return;
          case AST_PredefinedType::PT_char:
            *os << "::ACE_InputCDR::to_char";
            return;
          case AST_PredefinedType::PT_wchar:
            *os << "::ACE_InputCDR::to_wchar";
            return;
          case AST_PredefinedType::PT_void:
            break;
          default:
            *os << "::";
            break;
        }
    }
  else if (nt != AST_Decl::NT_string)
    {
      // We are unbounded, since the bounded case is handled
      // above. In this case, we want to generate 'char *'
      // without the leading double colon.
      *os << "::";
    }

  /// For now, keep a list of system operation or arg names
  /// that may not be remapped. May decide later to regnerate
  /// ORB code for alt mapping as well.
  ACE_CString repo_id (scope->repoID ());
  bool sys_val = (repo_id == "IDL:repository_id:1.0");

  // For types other than the 4 above, don't unalias the type name
  // in case it is a sequence or array.
  if (nt == AST_Decl::NT_string && bound == 0)
    {
      if (be_global->alt_mapping () && !sys_val)
        {
          *os << "std::string";
        }
      else
        {
          *os << "char *";
        }
    }
  else
    {
      *os << bt->name ();
    }

  if (nt == AST_Decl::NT_array)
    {
      *os << "_tag";
    }
}
