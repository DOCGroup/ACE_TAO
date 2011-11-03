//=============================================================================
/**
*  @file   amh_si.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH generates code that is
*  specific to AMH interfaces.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

be_visitor_amh_interface_si::be_visitor_amh_interface_si (
    be_visitor_context *ctx)
  : be_visitor_interface_si (ctx)
{
}

be_visitor_amh_interface_si::~be_visitor_amh_interface_si (void)
{
}

int
be_visitor_amh_interface_si::visit_interface (be_interface *node)
{
  if (node->srv_inline_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  int status =
    node->traverse_inheritance_graph (
              be_visitor_amh_interface_si::gen_skel_helper, os);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_amh_interface_si::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_amh_interface_si::gen_skel_helper (be_interface *derived,
                                              be_interface *ancestor,
                                              TAO_OutStream *os)
{
  // If derived and ancestor are same, skip it.
  if (derived == ancestor)
    {
      return 0;
    }

  // If an operation or an attribute is abstract (declared in an
  // abstract interface), we will either generate the full
  // definition (if there are no concrete interfaces between the
  // abstract ancestor and us) or, if there is a concrete ancestor
  // in between, we will catch its definition elsewhere in this
  // traversal.
  if (ancestor->is_abstract ())
    {
      return 0;
    }

  // Else generate code that does the cast to the appropriate type.

  if (ancestor->nmembers () > 0)
    {
      // If there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting.

      ACE_CString ancestor_name ("POA_");
      char *buf = 0;
      ancestor->compute_full_name ("AMH_", "", buf);
      ancestor_name += buf;
      // buf was allocated by ACE_OS::strdup, so we need to use free instead
      // of delete.
      ACE_OS::free (buf);
      buf = 0;

      const char *ancestor_amh_name = ancestor_name.fast_rep ();

      ACE_CString derived_name ("POA_");
      derived->compute_full_name ("AMH_", "", buf);
      derived_name += buf;
      // buf was allocated by ACE_OS::strdup, so we need to use free instead
      // of delete.
      ACE_OS::free (buf);
      buf = 0;

      const char *derived_amh_name = derived_name.fast_rep ();

      for (UTL_ScopeActiveIterator si (ancestor, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // Get the next AST decl node
          AST_Decl *d = si.item ();
          AST_Decl::NodeType nt = d->node_type ();

          if (nt == AST_Decl::NT_op)
            {
              be_operation *op =
                be_operation::narrow_from_decl (d);

              /// These implied IDL operations are for stub-side only.
              if (op->is_sendc_ami ())
                {
                  continue;
                }

              *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
                  << "// " << __FILE__ << ":" << __LINE__;

              *os << be_nl_2;

              // Generate code in the inline file.
              // Generate the static method corresponding to this method.
              *os << "ACE_INLINE" << be_nl
                  << "void" << be_nl
                  << derived_amh_name << "::"
                  << d->local_name ()
                  << "_skel (" << be_idt << be_idt_nl
                  << "TAO_ServerRequest & req," << be_nl
                  << "void * context," << be_nl
                  << "void * obj)" << be_uidt
                  << be_uidt_nl
                  << "{" << be_idt_nl;
              *os << ancestor_amh_name
                  << "* const impl = static_cast<"
                  << derived_amh_name
                  << " *> (obj);" << be_nl;
              *os << ancestor_amh_name
                  << "::" << d->local_name ()
                  << "_skel (" << be_idt_nl
                  << "req," << be_nl
                  << "context," << be_nl
                  << "impl);" << be_uidt
                  << be_uidt_nl
                  << "}";
            }
          else if (nt == AST_Decl::NT_attr)
            {
              be_attribute *attr = be_attribute::narrow_from_decl (d);

              if (attr == 0)
                {
                  return -1;
                }

              *os << be_nl_2;

              // Generate code in the inline file.
              // Generate the static method corresponding to this method.
              *os << "ACE_INLINE" << be_nl
                  << "void" << be_nl
                  << derived_amh_name << "::_get_"
                  << d->local_name ()
                  << "_skel (" << be_idt << be_idt_nl
                  << "TAO_ServerRequest & req," << be_nl
                  << "void * context," << be_nl
                  << "void * obj)" << be_uidt
                  << be_uidt_nl
                  << "{" << be_idt_nl;

               *os << ancestor_amh_name
                  << "* const impl = static_cast<"
                  << derived_amh_name
                  << " *> (obj);" << be_nl;

              *os << ancestor_amh_name
                  << "::_get_" << d->local_name ()
                  << "_skel (" << be_idt << be_idt_nl
                  << "req," << be_nl
                  << "context," << be_nl
                  << "impl);" << be_uidt
                  << be_uidt << be_uidt_nl
                  << "}";

              if (!attr->readonly ())
                {
                  *os << be_nl_2;

                  // Generate code in the inline file.
                  // Generate the static method corresponding to
                  // this method.
                  *os << "ACE_INLINE" << be_nl
                      << "void" << be_nl
                      << derived_amh_name
                      << "::_set_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest & req," << be_nl
                      << "void * context," << be_nl
                      << "void * obj)" << be_uidt
                      << be_uidt_nl
                      << "{" << be_idt_nl;

                   *os << ancestor_amh_name
                      << "* const impl = static_cast<"
                      << derived_amh_name
                      << " *> (obj);" << be_nl;

                  *os << ancestor_amh_name
                      << "::_set_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "context," << be_nl
                      << "impl);" << be_uidt
                      << be_uidt << be_uidt_nl
                      << "}";
                }
            }
        } // End of FOR
    }

  return 0;
}

