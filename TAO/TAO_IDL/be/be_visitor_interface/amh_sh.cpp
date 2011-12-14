//=============================================================================
/**
*  @file   amh_sh.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH generates code that is
*  specific to AMH interfaces.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

be_visitor_amh_interface_sh::be_visitor_amh_interface_sh (
    be_visitor_context *ctx
  )
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_interface_sh::~be_visitor_amh_interface_sh (void)
{
}

// The node is the original interface node but we 'tweak' with the
// local_name and the the operation signatures to generate the AMH
// skeleton on the 'fly'.

int
be_visitor_amh_interface_sh::visit_interface (be_interface *node)
{
  if (node->srv_hdr_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  ACE_CString class_name;

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // We shall have a POA_ prefix only if we are at the topmost level.
  if (!node->is_nested ())
    {
      // We are outermost.
      class_name += "POA_AMH_";
      class_name += node->local_name ();
    }
  else
    {
      class_name += "AMH_";
      class_name +=  node->local_name ();
    }

  // Generate the skeleton class name.
  *os << "class " << class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << class_name.c_str () << " *" << class_name.c_str ()
      << "_ptr;" << be_nl_2;

  // Now generate the class definition.
  *os << "class " << be_global->skel_export_macro ()
      << " " << class_name.c_str () << be_idt_nl << ": " << be_idt;

  long n_parents = node->n_inherits ();

  if (n_parents > 0)
    {
      for (int i = 0; i < n_parents; ++i)
        {
          ACE_CString amh_name ("POA_");

          // @@ The following code is *NOT* exception-safe.
          char *buf = 0;
          be_interface *base =
            be_interface::narrow_from_decl (node->inherits ()[i]);
          base->compute_full_name ("AMH_", "", buf);
          amh_name += buf;
          // buf was allocated by ACE_OS::strdup, so we need to use free
          // instead of delete.
          ACE_OS::free (buf);

          if (i != 0)
            {
              *os << ", ";
            }

          *os << "public virtual "
              << amh_name.c_str ()
              << be_nl;
        }
    }
  else
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *os << "public virtual PortableServer::ServantBase";
    }

  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << class_name.c_str () << " (void);" << be_uidt_nl << be_nl
      << "public:" << be_idt_nl;

  // No copy constructor for locality constraint interface.
  *os << class_name.c_str () << " (const " << class_name.c_str ()
      << "& rhs);" << be_nl
      << "virtual ~" << class_name.c_str () << " (void);\n\n"
      << be_nl
      << "virtual ::CORBA::Boolean _is_a (const char* logical_type_id);" << be_nl_2;

  // Add the dispatch method.
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "TAO::Portable_Server::Servant_Upcall *_servant_upcall);" << be_uidt
      << be_uidt_nl << be_nl;

  this->this_method (node);

  // The _interface_repository_id method.
  *os << be_nl
      << "virtual const char* _interface_repository_id "
      << "(void) const;";

  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_amh_interface_sh::visit_operation (be_operation *node)
{
  be_visitor_amh_operation_sh visitor (this->ctx_);
  return visitor.visit_operation (node);
}

int
be_visitor_amh_interface_sh::visit_attribute (be_attribute *node)
{
  be_visitor_amh_operation_sh visitor (this->ctx_);
  return visitor.visit_attribute (node);
}

int
be_visitor_amh_interface_sh::add_original_members (be_interface *node,
                                                   be_interface *amh_node)
{
  if (!node || !amh_node)
    {
      return -1;
    }

  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_amh_pre_proc::visit_interface - "
              "bad node in this scope\n"
            ),
            0
          );
        }

      if (d->node_type () == AST_Decl::NT_attr)
        {
          be_attribute *attribute = be_attribute::narrow_from_decl (d);

          if (!attribute)
            {
              return 0;
            }
        }
      else
        {
          be_operation* operation = be_operation::narrow_from_decl (d);

          if (operation)
            {
              this->add_amh_operation (operation, amh_node);
            }
        }
    }

  return 0;
}


int
be_visitor_amh_interface_sh::add_amh_operation (be_operation *node,
                                            be_interface *amh_node)
{
  if (!node || !amh_node)
    {
      return -1;
    }

  // We do nothing for oneways!
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  // Create the return type, which is "void"
  be_predefined_type *rt = 0;
  ACE_NEW_RETURN (rt,
                  be_predefined_type (AST_PredefinedType::PT_void,
                                      sn),
                  -1);

  ACE_CString original_op_name (
                  node->name ()->last_component ()->get_string ()
                );

  UTL_ScopedName *op_name =
    static_cast<UTL_ScopedName *> (amh_node->name ()->copy ());

  ACE_NEW_RETURN (id,
                  Identifier (original_op_name.rep ()),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  op_name->nconc (sn);

  // Create the operation
  be_operation *operation = 0;
  ACE_NEW_RETURN (operation,
                  be_operation (rt, //node->return_type (),
                                AST_Operation::OP_noflags,
                                op_name,
                                0,
                                0),
                  -1);

  operation->set_name (op_name);

  // Iterate over the arguments and put all the in and inout
  // into the new method.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope.
      for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Decl *d = si.item ();

          if (!d)
            {
              operation->destroy ();
              delete operation;
              operation = 0;

              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_amh_pre_proc::")
                                 ACE_TEXT ("create_response_handler_operation - ")
                                 ACE_TEXT ("bad node in this scope\n")),
                                -1);

            }

          AST_Argument *original_arg =
            AST_Argument::narrow_from_decl (d);

          if (original_arg->direction () == AST_Argument::dir_INOUT ||
              original_arg->direction () == AST_Argument::dir_IN)
            {
              // Create the argument.
              be_argument *arg = 0;
              ACE_NEW_RETURN (arg,
                              be_argument (original_arg->direction (),
                                           original_arg->field_type (),
                                           original_arg->name ()),
                              -1);

              operation->be_add_argument (arg);
            }
        }
    }

  operation->set_defined_in (amh_node);

  // After having generated the operation we insert it into the
  // AMH node interface.
  if (0 == amh_node->be_add_operation (operation))
    {
      return -1;
    }

  return 0;
}


be_interface *
be_visitor_amh_interface_sh::create_amh_class (ACE_CString name)
{
  Identifier *id = 0;
  ACE_NEW_RETURN (id,
                  Identifier (name.c_str ()),
                  0);

  UTL_ScopedName *amh_class_name = 0;
  ACE_NEW_RETURN (amh_class_name,
                  UTL_ScopedName (id,
                                  0),
                  0);

  be_interface *amh_class = 0;
  ACE_NEW_RETURN (amh_class,
                  be_interface (amh_class_name, // name
                                0,              // list of inherited
                                0,              // number of inherited
                                0,              // list of ancestors
                                0,              // number of ancestors
                                0,              // non-local
                                0),             // non-abstract
                  0);

  amh_class->set_name (amh_class_name);
  return amh_class;
}

void
be_visitor_amh_interface_sh::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  ACE_CString non_amh_name = node->client_enclosing_scope ();
  non_amh_name += node->local_name ();

  // Print out the _this() method.  The _this() method for AMH
  // interfaces is "special", because the returned type is not exactly
  // the type of the class, but the original class that "implied" the
  // AMH one.
  *os << non_amh_name.c_str () << " *_this (void);\n" << be_uidt;
}
