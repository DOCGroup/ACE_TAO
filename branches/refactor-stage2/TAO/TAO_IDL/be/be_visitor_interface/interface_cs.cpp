//
// $Id$

//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface,
           interface_cs,
           "$Id$")


// ************************************************************
// Interface visitor for client stubs
// ************************************************************

be_visitor_interface_cs::be_visitor_interface_cs (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cs::~be_visitor_interface_cs (void)
{
}

int
be_visitor_interface_cs::visit_interface (be_interface *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  be_type *bt;

  // Set the right type.
  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Initialize the static narrrowing helper variable.
  *os << be_nl << be_nl
      << "int " << node->full_name () << "::_tao_class_id = 0;";

  const char *fhname = node->fwd_helper_name ();

  // Helper functions generated in case this interface was
  // forward declared in some other IDL file and not defined there.
  *os << be_nl << be_nl
      << node->full_name () << "_ptr" << be_nl
      << fhname << "_life::"
      << "tao_duplicate (" << be_idt << be_idt_nl
      << node->nested_type_name (this->ctx_->scope ()) 
      << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->nested_type_name (this->ctx_->scope ())
      << "::_duplicate (p);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "void" << be_nl
      << fhname << "_life::"
      << "tao_release (" << be_idt << be_idt_nl
      << node->nested_type_name (this->ctx_->scope ()) 
      << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::release (p);" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << node->full_name () <<  "_ptr" << be_nl
      << fhname << "_life::"
      << "tao_nil (" << be_idt << be_idt_nl
      << "void" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "CORBA::Boolean" << be_nl
      << fhname << "_life::"
      << "tao_marshal (" << be_idt << be_idt_nl
      << node->nested_type_name (this->ctx_->scope ()) 
      << "_ptr p," << be_nl
      << "TAO_OutputCDR &cdr" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return p->marshal (cdr);" << be_uidt_nl
      << "}";

  if (! node->is_abstract ())
    {
      *os << be_nl << be_nl
          << node->full_name () << "_ptr" << be_nl
          << fhname << "_cast::"
          << "tao_narrow (" << be_idt << be_idt_nl
          << "CORBA::Object *p" << be_nl
          << "ACE_ENV_ARG_DECL" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return " << node->nested_type_name (this->ctx_->scope ())
          << "::_narrow (p ACE_ENV_ARG_PARAMETER);"
          << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "CORBA::Object *" << be_nl
          << fhname << "_cast::"
          << "tao_upcast (" << be_idt << be_idt_nl
          << "void *src" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << node->nested_type_name (this->ctx_->scope ()) 
          << " **tmp =" << be_idt_nl
          << "ACE_static_cast (" 
          << node->nested_type_name (this->ctx_->scope ())
          << " **, src);" << be_uidt_nl
          << "return *tmp;" << be_uidt_nl
          << "}";
    }

  if (node->has_mixed_parentage ())
    {
      *os << be_nl << be_nl
          << "void" << be_nl
          << "CORBA::release (" 
          << node->nested_type_name (this->ctx_->scope ()) 
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "CORBA::AbstractBase_ptr abs = p;" << be_nl
          << "CORBA::release (abs);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "CORBA::Boolean" << be_nl
          << "CORBA::is_nil (" 
          << node->nested_type_name (this->ctx_->scope ()) 
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "CORBA::Object_ptr obj = p;" << be_nl
          << "return CORBA::is_nil (obj);" << be_uidt_nl
          << "}";
    }

  *os << be_nl
      << "\n#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)" << be_idt_nl
      << "template class" << be_idt_nl
      << "TAO_Objref_Var_T<" << be_idt << be_idt_nl
      << node->name () << "," << be_nl
      << node->fwd_helper_name () << "_life" << be_uidt_nl
      << ">;" << be_uidt << be_uidt_nl
      << "template class" << be_idt_nl
      << "TAO_Objref_Out_T<" << be_idt << be_idt_nl
      << node->name () << "," << be_nl
      << node->fwd_helper_name () << "_life" << be_uidt_nl
      << ">;" << be_uidt << be_uidt << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl
      << "# pragma instantiate \\" << be_idt << be_idt_nl
      << "TAO_Objref_Var_T< \\" << be_idt << be_idt_nl
      << node->name () << ", \\" << be_nl
      << node->fwd_helper_name () << "_life \\" << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt_nl
      << "# pragma instantiate \\" << be_idt << be_idt_nl
      << "TAO_Objref_Out_T< \\" << be_idt << be_idt_nl
      << node->name () << ", \\" << be_nl
      << node->fwd_helper_name () << "_life \\" << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */";

  be_visitor_context ctx = (*this->ctx_);

  // Interceptor classes.  The interceptors helper classes must be
  // defined before the interface operations because they are used in
  // the implementation of said operations.

  ctx.state (TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS);
  be_visitor_interface_interceptors_cs ii_visitor (&ctx);

  if (node->accept (&ii_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for interceptors classes failed\n"),
                        -1);
    }

  if (!node->is_local () && ! node->is_abstract ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS);
      be_visitor_interface_remote_proxy_impl_cs irpi_visitor (&ctx);

      if (node->accept (&irpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "visit_interface - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }

      ctx = *this->ctx_;
      be_visitor_interface_remote_proxy_broker_cs irpb_visitor (&ctx);

      if (node->accept (&irpb_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "visit_interface - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  if (node->is_local ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl
          << "{}";
    }

  if (! node->is_abstract () && ! node->is_local ())
    {
       // Generate the destructor and default constructor.
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name ()
          << " (int collocated)" << be_nl
          << "{" << be_idt_nl
          << "this->" << node->flat_name ()
          << "_setup_collocation (collocated);" << be_uidt_nl
          << be_uidt << "}";

      // Collocation setup method.
      *os << be_nl << be_nl
          << "void" << be_nl
          << node->name () << "::" << node->flat_name ()
          << "_setup_collocation (int collocated)" << be_nl
          << "{" << be_idt_nl
          << "if (collocated)" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer (this);"
          << be_uidt << be_uidt_nl
          << "else" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->full_remote_proxy_broker_name ()
          << "::the" << node->remote_proxy_broker_name ()
          << " ();" << be_uidt << be_uidt;

      // Now we setup the immediate parents.
      int n_parents = node->n_inherits ();
      int has_concrete_parent = 0;

      if (n_parents > 0)
        {
          for (int i = 0; i < n_parents; ++i)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);

              if (inherited->is_abstract ())
                {
                  continue;
                }

              if (has_concrete_parent == 0)
                {
                  *os << be_nl;
                }

              has_concrete_parent = 1;

              *os << be_nl
                  << "this->" << inherited->flat_name ()
                  << "_setup_collocation" << " (collocated);";
            }
        }

      *os << be_uidt_nl << "}";
    }

  *os << be_nl << be_nl << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  if (be_global->any_support ())
    {
      *os << "void " << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *tmp = ACE_static_cast ("
          << node->local_name () << " *, _tao_void_pointer);" << be_nl
          << "CORBA::release (tmp);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  if (node->has_mixed_parentage ())
    {
      *os << "void" << be_nl
          << node->name () << "::_add_ref (void)" << be_nl
          << "{" << be_idt_nl
          << "this->ACE_NESTED_CLASS (CORBA, Object)::_add_ref ();"
          << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // The _narrow method

  *os << node->full_name () << "_ptr" << be_nl << node->full_name ()
      << "::_narrow (" << be_idt << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "CORBA::AbstractBase_ptr obj" << be_nl;
    }
  else
    {
      *os << "CORBA::Object_ptr obj" << be_nl;
    }

  *os << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // Local interfaces have slightly different _narrow implementation.
  if (! node->is_local ())
    {
      // Remote _narrow implementation.
      *os << "if (CORBA::is_nil (obj))" << be_idt_nl
          << "{" << be_idt_nl
          << "return " << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      if (! node->is_abstract ())
        {
          *os << "if (! obj->_is_local ())" << be_idt_nl
              << "{" << be_idt_nl;
        }

      *os << "CORBA::Boolean is_a =" << be_idt_nl
          << "obj->_is_a (" << be_idt << be_idt_nl
          << "\"" << node->repoID () << "\"" << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "ACE_CHECK_RETURN (" << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ());" << be_nl << be_nl
          << "if (is_a == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "return " << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ();" << be_uidt_nl;

      if (node->is_abstract ())
        {
          *os << "}" << be_uidt_nl;
        }
      else
        {
          *os << "}" << be_uidt << be_uidt_nl;
          *os << "}" << be_uidt_nl;
        }

      *os << be_nl;
    }

  *os << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_unchecked_narrow (obj ACE_ENV_ARG_PARAMETER);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  if (node->is_abstract ())
    {
      this->gen_abstract_unchecked_narrow (node,
                                           bt,
                                           os);
    }
  else
    {
      this->gen_concrete_unchecked_narrow (node,
                                           bt,
                                           os);
    }

  if (node->is_abstract ())
    {
      *os << node->full_name () << "_ptr" << be_nl
          << node->full_name ()
          << "::_downcast (CORBA::AbstractBase_ptr abs)" << be_nl
          << "{" << be_idt_nl
          << "if (CORBA::is_nil (abs))" << be_idt_nl
          << "{" << be_idt_nl
          << "return " << node->local_name () << "::_nil ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "return (" << node->local_name ()
          << "_ptr) abs->_tao_obv_narrow ((ptrdiff_t) &_downcast);"
          << be_uidt_nl
          << "}" << be_nl << be_nl;

      *os << "\n#if defined (_MSC_VER)" << be_nl
          << "void *" << be_nl
          << node->name () << "::" << node->flat_name ()
          << "_tao_obv_narrow (ptrdiff_t type_id)" << be_nl
          << "{" << be_idt_nl
          << "return this->_tao_obv_narrow (type_id);" << be_uidt_nl
          << "}"
          << "\n#endif /* _MSC_VER */" << be_uidt_nl << be_nl
          << "void *" << be_nl
          << node->full_name ()
          << "::_tao_obv_narrow (ptrdiff_t type_id)" << be_nl
          << "{" << be_idt_nl
          << "if (type_id == (ptrdiff_t) &_downcast)" << be_idt_nl
          << "{" << be_idt_nl
          << "return this;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "void *rval = 0;" << be_nl;

      // Find the possible base classes.

      int n_inherits_downcastable = 0;
      AST_Interface *inherited = 0;

      for (int i = 0; i < node->n_inherits (); ++i)
        {
          inherited = node->inherits ()[i];

          ++n_inherits_downcastable;

          *os << be_nl
              << "if (rval == 0)" << be_idt_nl
              << "{"
              << "\n#if defined (_MSC_VER)" << be_idt_nl
              << "rval = this->" << inherited->flat_name ()
              << "_tao_obv_narrow (type_id);"
              << "\n#else" << be_nl
              << "rval = this->" << inherited->name () << "::"
              << "_tao_obv_narrow (type_id);"
              << "\n#endif /* _MSC_VER */" << be_uidt_nl
              << "}" << be_uidt_nl;
        }

      *os << be_nl << "return rval;" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (! CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate the is_a method (not supported on local objects).
  if (! node->is_local ())
    {
      *os << "CORBA::Boolean" << be_nl
          << node->full_name () << "::_is_a (" << be_idt << be_idt_nl
          << "const char *value" << be_nl
          << "ACE_ENV_ARG_DECL";

      if (node->is_abstract ())
        {
          *os << "_NOT_USED";
        }

      *os << be_uidt_nl << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "if (" << be_idt << be_idt_nl;

      int status =
        node->traverse_inheritance_graph (be_interface::is_a_helper,
                                          os);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "_is_a method codegen failed\n"),
                            -1);
        }

      if (node->is_abstract () || node->has_mixed_parentage ())
        {
          *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
              << "(char *)value," << be_nl
              << "\"IDL:omg.org/CORBA/AbstractBase:1.0\"" << be_uidt_nl
              << ")";
        }

      if (node->has_mixed_parentage ())
        {
          *os << " ||" << be_uidt_nl;
        }
      else if (node->is_abstract ())
        {
          *os << be_uidt << be_uidt_nl;
        }

      if (! node->is_abstract ())
        {
          *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
              << "(char *)value," << be_nl
              << "\"IDL:omg.org/CORBA/Object:1.0\"" << be_uidt_nl
              << ")" << be_uidt << be_uidt_nl;
        }

      *os << " )" << be_nl
          << "{" << be_idt_nl
          << "return 1; // success using local knowledge" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl;

      if (node->is_abstract ())
        {
          *os << "return 0;" << be_uidt_nl;
        }
      else
        {
          *os << "return this->ACE_NESTED_CLASS (CORBA, Object)::_is_a ("
              << be_idt << be_idt_nl
              << "value" << be_nl
              << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl;
        }

      *os << "}" << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // Generating _tao_QueryInterface method.
  *os << "void *" << node->full_name ()
      << "::_tao_QueryInterface (ptrdiff_t type)" << be_nl
      << "{" << be_idt_nl
      << "void *retv = 0;" << be_nl << be_nl
      << "if ";

  if (node->traverse_inheritance_graph (
          be_interface::queryinterface_helper,
          os
        ) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_tao_QueryInterface method codegen failed\n"),
                        -1);
    }

  *os << "(type == ACE_reinterpret_cast ("
      << be_idt << be_idt << be_idt << be_idt << be_idt << be_idt_nl
      << " ptrdiff_t," << be_nl;

  if (node->is_abstract ())
    {
      *os << " &CORBA::AbstractBase";
    }
  else
    {
      *os << " &CORBA::Object";
    }

  *os << "::_tao_class_id)" << be_uidt_nl
      << " )" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "retv =" << be_idt_nl
      << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << "void *," << be_nl
      << "ACE_static_cast (";

  if (node->is_abstract ())
    {
      *os << "CORBA::AbstractBase_ptr";
    }
  else
    {
      *os << "CORBA::Object_ptr";
    }

  *os << ", this)" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  *os << "if (retv != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "this->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retv;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "const char* " << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID ()
      << "\";" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "CORBA::Boolean" << be_nl;

  if (node->is_local ())
    {
      *os << node->name () << "::marshal (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}";
    }
  else
    {
      *os << node->name () << "::marshal (TAO_OutputCDR &cdr)" << be_nl
          << "{" << be_idt_nl
          << "return (cdr << this);" << be_uidt_nl
          << "}";
    }

  // Generate code for the elements of the interface.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  if (! node->is_abstract ())
    {
      // Smart Proxy classes.
      if (! node->is_local () && be_global->gen_smart_proxies ())
        {
          be_visitor_context ctx (*this->ctx_);

          ctx.state (TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS);
          be_visitor_interface_smart_proxy_cs isp_visitor (&ctx);

          if (node->accept (&isp_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_interface_cs::"
                                 "visit_interface - "
                                 "codegen for smart proxy classes failed\n"),
                                -1);
            }
        }
    }

  if (be_global->tc_support ())
    {
      ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      be_visitor_typecode_defn tc_visitor (&ctx);

      if (node->accept (&tc_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "visit_interface - "
                             "TypeCode definition failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_interface_cs::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
{
  if (node == base)
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_cs::"
                             "abstract_base_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      if (d->node_type () == AST_Decl::NT_op)
        {
          UTL_ScopedName *item_new_name  = 0;
          ACE_NEW_RETURN (item_new_name,
                          UTL_ScopedName (d->local_name ()->copy (),
                                          0),
                          -1);

          UTL_ScopedName *base = (UTL_ScopedName *)node->name ()->copy ();
          base->nconc (item_new_name);

          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               0,
                               op->is_local (),
                               op->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          new_op.set_name (base);
          ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
          be_visitor_operation_cs op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);

          base->destroy ();
          delete base;
          base = 0;
        }
    }

  return 0;
}

void
be_visitor_interface_cs::gen_concrete_unchecked_narrow (be_interface *node,
                                                        be_type *bt,
                                                        TAO_OutStream *os)
{
  *os << node->full_name () << "_ptr "  << be_nl
      << node->full_name () << "::_unchecked_narrow ("
      << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  if (! node->is_local ())
    {
      // Declare the default proxy.
      *os << bt->nested_type_name (this->ctx_->scope ())
          << "_ptr default_proxy = "
          << bt->nested_type_name (this->ctx_->scope ())
          <<"::_nil ();" << be_nl << be_nl;

      // Code for lzay evaluation..
      *os << "// Code for lazily evaluated IOR's" << be_nl;

      *os << "if (!obj->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "default_proxy," << be_nl
          << "::" <<  bt->name ()
          << " (" << be_idt << be_idt_nl
          << "obj->steal_ior ()," << be_nl
          << "obj->orb_core ()" << be_uidt_nl << ")," << be_uidt_nl
          << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ()" << be_uidt_nl << ");" << be_uidt_nl << be_nl
          << "return default_proxy;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      // Remote _unchecked_narrow implementation.
      *os << "if (! obj->_is_local ())" << be_idt_nl
          << "{" << be_idt_nl
          << "TAO_Stub* stub = obj->_stubobj ();" << be_nl << be_nl
          << "if (stub != 0)" << be_idt_nl
          << "{" << be_idt_nl
          << "stub->_incr_refcnt ();" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      // If the policy didtates that the proxy be collocated, use the
      // function to create one.
      *os << "if (" << be_idt << be_idt_nl
          << "!CORBA::is_nil (stub->servant_orb_var ().ptr ()) &&" << be_nl
          << "stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects () &&"
          << be_nl
          << "obj->_is_collocated () &&" << be_nl
          << node->flat_client_enclosing_scope () << node->base_proxy_broker_name ()
          << "_Factory_function_pointer != 0" << be_uidt_nl << ")"
          << be_nl << "{"
          << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "default_proxy," << be_nl
          << "::" <<  bt->name ()
          << " (" << be_idt << be_idt_nl
          << "stub," << be_nl
          << "1," << be_nl
          << "obj->_servant ()" << be_uidt_nl << ")," << be_uidt_nl
          <<  bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ()" << be_uidt_nl << ");"
          << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;


      // The default proxy will either be returned else be transformed to
      // a smart one!
      *os << "if (CORBA::is_nil (default_proxy))" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "default_proxy," << be_nl
          << "::" << bt->name () << " (" << be_idt << be_idt_nl
          << "stub," << be_nl
          << "0," << be_nl
          << "obj->_servant ()" << be_uidt_nl
          << ")," << be_uidt_nl
          << bt->nested_type_name (this->ctx_->scope ())
          << "::_nil ()" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      if (be_global->gen_smart_proxies ())
        {
          *os << "return TAO_" << node->flat_name ()
              << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (default_proxy);"
              << be_uidt_nl;
        }
      else
        {
          *os << "return default_proxy;" << be_uidt_nl;
        }

      *os << "}" << be_uidt_nl
          << "else " << be_idt_nl;
    }
  else
    {
      *os << be_idt;
    }

  // Local  & lazily evaluated _unchecked_narrow.
  *os << "return" << be_idt_nl
      << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << node->local_name () << "_ptr," << be_nl
      << "obj->_tao_QueryInterface (" << be_idt << be_idt_nl
      << "ACE_reinterpret_cast (" << be_idt << be_idt_nl
      << "ptrdiff_t," << be_nl
      << "&" << node->local_name () << "::_tao_class_id" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt << be_uidt_nl;

  *os << "}" << be_nl << be_nl;
}

void
be_visitor_interface_cs::gen_abstract_unchecked_narrow (be_interface *node,
                                                        be_type *bt,
                                                        TAO_OutStream *os)
{
  *os << node->full_name () << "_ptr "  << be_nl
      << node->full_name () << "::_unchecked_narrow ("
      << be_idt << be_idt_nl
      << "CORBA::AbstractBase_ptr obj" << be_nl
      << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "return " << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  // Declare the default proxy.
  *os << bt->nested_type_name (this->ctx_->scope ())
      << "_ptr default_proxy = "
      << bt->nested_type_name (this->ctx_->scope ())
      <<"::_nil ();" << be_nl << be_nl;

  *os << "if (obj->_is_objref ())" << be_idt_nl
      << "{" << be_idt_nl;

  *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "default_proxy," << be_nl
      << "::" << bt->name () << " (" << be_idt << be_idt_nl
      << "obj->_stubobj ()," << be_nl
      << "0," << be_nl
      << "obj->_servant ()" << be_uidt_nl
      << ")," << be_uidt_nl
      << bt->nested_type_name (this->ctx_->scope ())
      << "::_nil ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl;

  *os << "else" << be_idt_nl
      << "{" << be_idt_nl
      << "default_proxy = " << bt->name () << "::_downcast (obj);"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  *os << "return default_proxy;" << be_uidt_nl;

  *os << "}" << be_nl << be_nl;
}
