//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale & Angelo Corsaro
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation_cs,
           "$Id$")

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_cs::be_visitor_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
//    operation_name_ (0)
{
}

be_visitor_operation_cs::~be_visitor_operation_cs (void)
{
//  delete[] operation_name_;
}

// Processing to be done after every element in the scope is processed.
int
be_visitor_operation_cs::post_process (be_decl *bd)
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
be_visitor_operation_cs::visit_operation (be_operation *node)
{
  be_interface *intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_thru_poa_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  if (node->is_local ())
    {
      return 0;
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype rt_visitor = (&ctx);

  if (bt->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // Generate the operation name
  *os << " " << node->name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist al_visitor (&ctx);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << be_nl;

  // @@@ (JP) No collocation for abstract interface operations yet. We
  // are generating the code as if there were no proxies, and using
  // the (concrete interface's) stub info that we got over the wire.
  if (intf->is_abstract ())
    {
      return this->gen_stub_operation_body (node,
                                            bt);
    }
  else
    {
      *os << "{" << be_idt_nl;
      *os << this->gen_environment_var ();

      if (be_global->exception_support ())
        {
          *os << be_nl << be_nl;
        }

      // For what follows, the return type node nust be unaliased.
      if (bt->node_type () == AST_Decl::NT_typedef)
        {
          be_typedef *btd = be_typedef::narrow_from_decl (bt);
          bt = btd->primitive_base_type ();
        }

      AST_Decl::NodeType bnt = bt->base_node_type ();
      be_predefined_type *bpt = 0;
      AST_PredefinedType::PredefinedType pdt = AST_PredefinedType::PT_void;

      if (bnt == AST_Decl::NT_pre_defined)
        {
          bpt = be_predefined_type::narrow_from_decl (bt);
          pdt = bpt->pt ();

          if (pdt == AST_PredefinedType::PT_longlong)
            {
              *os << "CORBA::LongLong _tao_check_retval = "
                  << "ACE_CDR_LONGLONG_INITIALIZER;" << be_nl << be_nl;
            }
          else if (pdt == AST_PredefinedType::PT_longdouble)
            {
              *os << "CORBA::LongDouble _tao_check_retval = "
                  << "ACE_CDR_LONG_DOUBLE_INITIALIZER;" << be_nl << be_nl;
            }
        }

      // If the object is lazily evaluated the proxy brker might well
      // be null.  Initialize it now
      *os << "if (!this->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NESTED_CLASS (CORBA, Object)::tao_object_initialize (this);" << be_nl
          << "this->" << intf->flat_name ()
          << "_setup_collocation (" << be_idt << be_idt_nl
          << "this->ACE_NESTED_CLASS (CORBA, Object)::_is_collocated ()" 
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      // Generate code that retrieves the proper proxy implementation
      // using the proxy broker available, and perform the call
      // using the proxy implementation provided by the broker.

      *os << intf->base_proxy_impl_name () << " &proxy = " << be_idt_nl
          << "this->the" << intf->base_proxy_broker_name ()
          << "_->select_proxy (this ACE_ENV_ARG_PARAMETER);" << be_uidt_nl;

      if (!this->void_return_type (bt))
        {
          *os << "ACE_CHECK_RETURN (";

          if (bnt == AST_Decl::NT_enum)
            {
              // The enum is a unique type, so we must cast.
              *os << "(" << bt->name () << ")0);";
            }
          else if (bnt == AST_Decl::NT_struct || bnt == AST_Decl::NT_union)
            {
              if (bt->size_type () == AST_Type::FIXED)
                {
                  // For a fixed size struct or union the return value
                  // is not a pointer, so we call the default constructor
                  // and return the result.
                  *os << bt->name () << " ());";
                }
              else
                {
                  *os << "0);";
                }
            }
          else if (bnt == AST_Decl::NT_pre_defined)
            {
              if (pdt == AST_PredefinedType::PT_longlong
                  || pdt == AST_PredefinedType::PT_longdouble)
                {
                  *os << "_tao_check_retval);" << be_nl
                      << "ACE_UNUSED_ARG (_tao_check_retval);";
                }
              else
                {
                  *os << "0);";
                }
            }
          else
            {
              *os << "0);";
            }

          *os << be_nl << be_nl
              << "return ";
        }
      else
        {
          *os << "ACE_CHECK;" << be_nl << be_nl;
        }

      *os << "proxy." << node->local_name ()
          << " (" << be_idt << be_idt_nl << "this";

      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (d == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"),
                                -1);
            }

          be_decl *decl = be_decl::narrow_from_decl (d);

          *os << "," << be_nl
              << decl->local_name ();
        }

      if (!be_global->exception_support ())
        {
          *os << be_nl << "ACE_ENV_ARG_PARAMETER";
        }

      *os << be_uidt_nl << ");" << be_uidt << be_uidt_nl << "}";
    }

  return 0;
}

int
be_visitor_operation_cs::visit_argument (be_argument *node)
{
  // This method is used to generate the ParamData table entry.

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
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
