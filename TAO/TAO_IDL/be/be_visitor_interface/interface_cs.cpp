//
// $Id$
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
  if (node->imported () || node->cli_stub_gen ())
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

  if (node->is_defined ())
    {
      *os << be_nl << be_nl
          << "// Traits specializations for " << node->name () << ".";

      *os << be_nl << be_nl
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::duplicate ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_duplicate (p);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "void" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::release ("
          << be_idt << be_idt_nl
          << node->name () << "_ptr p)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::release (p);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << node->name () << "_ptr" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::nil (void)"
          << be_nl
          << "{" << be_idt_nl
          << "return " << node->name () << "::_nil ();" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "::CORBA::Boolean" << be_nl
          << "TAO::Objref_Traits<" << node->name () << ">::marshal ("
          << be_idt << be_idt_nl
          << "const " << node->name () << "_ptr p," << be_nl
          << "TAO_OutputCDR & cdr)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "return ";

          if (node->is_abstract ())
            {
              *os << "cdr << p;";
            }
          else
            {
              *os << "::CORBA::Object::marshal (p, cdr);";
            }

      *os << be_uidt_nl
          << "}";
    }

  if (be_global->gen_ostream_operators ())
    {
      *os << be_nl << be_nl
          << "std::ostream &" << be_nl
          << node->name () << "::_tao_stream_v (std::ostream &strm) const"
          << be_nl
          << "{" << be_idt_nl
          << "return strm << \"\\\"" << node->repoID () << "\\\"\";"
          << be_uidt_nl
          << "}";
    }

  if (node->has_mixed_parentage ())
    {
      *os << be_nl << be_nl
          << "void" << be_nl
          << "CORBA::release ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::AbstractBase_ptr abs = p;" << be_nl
          << "::CORBA::release (abs);" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "::CORBA::Boolean" << be_nl
          << "CORBA::is_nil ("
          << node->name ()
          << "_ptr p)" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::Object_ptr obj = p;" << be_nl
          << "return ::CORBA::is_nil (obj);" << be_uidt_nl
          << "}";
    }

  if (!node->is_local () &&
      (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ()))
    {
      // Generate the proxy broker factory function pointer definition.
      *os << be_nl << be_nl
          << "// Function pointer for collocation factory initialization."
          << be_nl
          << "TAO::Collocation_Proxy_Broker * " << be_nl
          << "(*" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer) ("
          << be_idt << be_idt_nl
          << "::CORBA::Object_ptr obj" << be_uidt_nl
          << ") = 0;" << be_uidt;
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

  if (node->is_local ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl
          << "{}";
    }

  if (! node->is_abstract () && ! node->is_local ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl;

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
          *os << " : the" << node->base_proxy_broker_name () << "_ (0)" << be_nl;
        }

      *os << "{" << be_idt_nl;

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
          *os << "this->" << node->flat_name ()
              << "_setup_collocation ();" << be_uidt_nl;
        }
      *os << be_uidt << "}";
    }

  if (! node->is_local () &&
     (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ()))
    {
      *os << be_nl << be_nl
          << "void" << be_nl
          << node->name () << "::" << node->flat_name ()
          << "_setup_collocation ()" << be_nl
          << "{" << be_idt_nl
          << "if (" << "::"
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer";

       // Right now (29-01-04) we don't support collocation for
       // abstract interfaces, and the 'collocated' arg will always
       // be 0. However, just to be safe, we add a
       // check for non-zero collocation factory function pointer
       // (which at present is also 0 for abstract interfaces),
       // in case the logic is changed in the future.
       if (node->is_abstract ())
        {
          *os << " && " << node->flat_client_enclosing_scope ()
              << node->base_proxy_broker_name ()
              << "_Factory_function_pointer";
        }

      *os << ")" << be_idt_nl
          << "{" << be_idt_nl
          << "this->the" << node->base_proxy_broker_name ()
          << "_ =" << be_idt_nl
          << "::" << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer (";

       if (node->is_abstract ())
          {
            *os << be_idt << be_idt_nl
                << "this->equivalent_objref ()" << be_uidt_nl
                << ");" << be_uidt;
          }
        else
          {
            *os << "this);";
          }

      *os << be_uidt << be_uidt_nl
          << "}" << be_uidt;

      // Now we setup the immediate parents.
      long n_parents = node->n_inherits ();
      bool has_parent = false;

      if (n_parents > 0)
        {
          for (long i = 0; i < n_parents; ++i)
            {
              be_interface *inherited =
                be_interface::narrow_from_decl (node->inherits ()[i]);

              if (has_parent == 0)
                {
                  *os << be_nl;
                }

              has_parent = true;

              *os << be_nl
                  << "this->" << inherited->flat_name ()
                  << "_setup_collocation" << " ();";
            }
        }

      *os << be_uidt_nl << "}";
    }

  *os << be_nl << be_nl
      << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  bool gen_any_destructor =
    be_global->any_support ()
    && (!node->is_local () || be_global->gen_local_iface_anyops ());

  if (gen_any_destructor)
    {
      *os << "void " << be_nl
          << node->name ()
          << "::_tao_any_destructor (void *_tao_void_pointer)" << be_nl
          << "{" << be_idt_nl
          << node->local_name () << " *_tao_tmp_pointer =" << be_idt_nl
          << "static_cast<"
          << node->local_name () << " *> (_tao_void_pointer);" << be_uidt_nl
          << "::CORBA::release (_tao_tmp_pointer);" << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  if (node->has_mixed_parentage ())
    {
      *os << "void" << be_nl
          << node->name () << "::_add_ref (void)" << be_nl
          << "{" << be_idt_nl
          << "this->::CORBA::Object::_add_ref ();"
          << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  // The _narrow method
  if (this->gen_xxx_narrow ("narrow",
                            node) == false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_narrow () method codegen failed\n"),
                        -1);
    }

  // The _unchecked_narrow method
  if (this->gen_xxx_narrow ("unchecked_narrow",
                            node) == false)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cs::"
                         "visit_interface - "
                         "_unchecked_narrow () method codegen"
                         " failed\n"),
                        -1);
    }


  // The _duplicate method
  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_duplicate ("
      << bt->local_name ()
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (obj))" << be_idt_nl
      << "{" << be_idt_nl
      << "obj->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return obj;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // The _tao_release method
  *os << "void" << be_nl
      << node->full_name () << "::_tao_release ("
      << bt->local_name ()
      << "_ptr obj)" << be_nl
      << "{" << be_idt_nl
      << "::CORBA::release (obj);" << be_uidt_nl
      << "}" << be_nl << be_nl;


  // Empty implementations so the application can override or not.
  if (node->session_component_child () == 1)
    {
      *os << "// These two are inherited from SessionComponent."
          << be_nl << be_nl
          << "void" << be_nl
          << node->full_name () << "::ciao_preactivate (void)" << be_nl
          << "{" << be_nl
          << "}" << be_nl << be_nl
          << "void" << be_nl
          << node->full_name () << "::ciao_postactivate (void)" << be_nl
          << "{" << be_nl
          << "}" << be_nl << be_nl;
    }

  *os << "::CORBA::Boolean" << be_nl
      << node->full_name () << "::_is_a (const char *value)" << be_nl;

  *os << "{" << be_idt_nl
      << "if (" << be_idt << be_idt_nl;

  int const status =
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
          << "value," << be_nl
          << "\"IDL:omg.org/CORBA/AbstractBase:1.0\"" << be_uidt_nl
          << ")";
    }
  else if (node->is_local ())
    {
      *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
          << "value," << be_nl
          << "\"IDL:omg.org/CORBA/LocalObject:1.0\"" << be_uidt_nl
          << ")";
    }

  if (node->has_mixed_parentage () || node->is_local ())
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
          << "value," << be_nl
          << "\"IDL:omg.org/CORBA/Object:1.0\"" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl;
    }

  *os << ")" << be_nl
      << "{" << be_idt_nl
      << "return true; // success using local knowledge" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl;

  if (node->is_abstract () || node->is_local ())
    {
      *os << "return false;" << be_uidt_nl;
    }
  else
    {
      *os << "return this->::CORBA::Object::_is_a (value);" << be_uidt_nl;
    }

  *os << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "const char* " << node->full_name ()
      << "::_interface_repository_id (void) const"
      << be_nl
      << "{" << be_idt_nl
      << "return \"" << node->repoID ()
      << "\";" << be_uidt_nl
      << "}";

  *os << be_nl << be_nl
      << "::CORBA::Boolean" << be_nl;

  if (node->is_local ())
    {
      *os << node->name () << "::marshal (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}";
    }
  else
    {
      *os << node->name () << "::marshal (TAO_OutputCDR &cdr)" << be_nl
          << "{" << be_idt_nl
          << "return (cdr << this);" << be_uidt_nl
          << "}";
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

  if (be_global->tc_support () && !node->home_equiv ())
    {

      be_visitor_context ctx = *this->ctx_;
      //       ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
      TAO::be_visitor_objref_typecode tc_visitor (&ctx);

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

bool
be_visitor_interface_cs::gen_xxx_narrow (const char *pre,
                                         be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << node->full_name () << "_ptr" << be_nl
      << node->full_name () << "::_" << pre << " ("
      << be_idt << be_idt_nl;

  if (node->is_abstract ())
    {
      *os << "::CORBA::AbstractBase_ptr";
    }
  else
    {
      *os << "::CORBA::Object_ptr";
    }

  *os << " _tao_objref"
      << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_local ())
    {
      *os << "return " << node->local_name () << "::_duplicate ("
          << be_idt << be_idt_nl
          << "dynamic_cast<" << node->local_name () << "_ptr> (_tao_objref)"
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;

      return true;
    }
  else if (be_global->gen_smart_proxies () &&
           !node->is_abstract ())
    {
      *os << node->full_name () << " *proxy = 0;" << be_nl << be_nl
          << "proxy = TAO::Narrow_Utils<"
          << node->local_name () << ">::" << pre << " (";

      *os << be_idt << be_idt_nl
          << "_tao_objref," << be_nl
          << "\"" << node->repoID () << "\"," << be_nl;

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
          *os << node->flat_client_enclosing_scope ()
              << node->base_proxy_broker_name ()
              << "_Factory_function_pointer" << be_uidt_nl;
        }
      else
        {
          *os << "0" << be_uidt_nl;
        }
      *os << ");" << be_uidt << be_nl
          << "return TAO_" << node->flat_name ()
          << "_PROXY_FACTORY_ADAPTER::instance ()->create_proxy (proxy);"
          << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;
    }
  else
    {
      *os << "return" << be_idt_nl;

      if (!node->is_abstract ())
        {
          *os << "TAO::Narrow_Utils<"
              << node->local_name () << ">::" << pre << " (";
        }
      else
        {
          *os << "TAO::AbstractBase_Narrow_Utils<"
              << node->local_name () << ">::" << pre <<" (";
        }

      *os << be_idt << be_idt_nl
          << "_tao_objref," << be_nl
          << "\"" << node->repoID () << "\"," << be_nl;

      if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
        {
          *os << node->flat_client_enclosing_scope ()
              << node->base_proxy_broker_name ()
              << "_Factory_function_pointer" << be_uidt_nl;
        }
      else
        {
          *os << "0" << be_uidt_nl;
        }
      *os << ");" << be_uidt << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;
    }

  return true;
}

int
be_visitor_interface_cs::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
{
  // If the derived interface is local, the abstract parent's operation
  // was generated as pure virtual.
  if (!base->is_abstract () || node->is_local ())
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
                             "gen_abstract_ops_helper - "
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

          UTL_ScopedName *new_op_name =
            (UTL_ScopedName *)node->name ()->copy ();
          new_op_name->nconc (item_new_name);

          be_operation *op = be_operation::narrow_from_decl (d);
          UTL_ScopedName *old_op_name =
            (UTL_ScopedName *) op->name ()->copy ();
          op->set_name (new_op_name);
          op->set_defined_in (node);
          op->is_abstract (node->is_abstract ());

          ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
          be_visitor_operation_cs op_visitor (&ctx);
          op_visitor.visit_operation (op);

          op->set_name (old_op_name);
          op->set_defined_in (base);
          op->is_abstract (base->is_abstract ());
        }
    }

  return 0;
}
