//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_sth.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the server template header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_sth,
           "$Id$")

// ****************************************
// Root visitor for server template header
// ****************************************

be_visitor_root_sth::be_visitor_root_sth (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_sth::~be_visitor_root_sth (void)
{
}

int
be_visitor_root_sth::init (void)
{
  // Open the file.
  if (tao_cg->start_server_template_header (
          be_global->be_get_server_template_hdr_fname ()
        )
      == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          "(%N:%l) be_visitor_root_sth::init - "
          "Error:Unable to open server template header file : %s\n",
          be_global->be_get_server_template_hdr_fname ()
        ),
        -1
      );
    }

  // Set the stream and the next state.
  this->ctx_->stream (tao_cg->server_template_header ());
  return 0;
}

int
be_visitor_root_sth::visit_scope (be_scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root_sth::visit_scope - "
                             "bad node in this scope\n"),
                            -1);
        }

      be_decl *bd = be_decl::narrow_from_decl (d);

      // Set the scope node as "node" in which the code is being
      // generated so that elements in the node's scope can use it
      // for code generation.
      this->ctx_->scope (node->decl ());

      // Set the node to be visited.
      this->ctx_->node (bd);

      // Send the visitor.
      if (bd == 0 || bd->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root_sth::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);

        }
    }

  return 0;
}

int
be_visitor_root_sth::visit_module (be_module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = tao_cg->server_template_header ();

  // Generate the skeleton class name.

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
               << "// " __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Now generate the class definition. The prefix POA_ is prepended to our
  // name only if we are the outermost module.
  *os << "namespace ";

  if (node->is_nested ())
    {
      // We are inside another module.
      *os << node->local_name () << be_nl;
    }
  else
    {
      // We are outermost module.
      *os << "POA_" << node->local_name () << be_nl;
    }

  *os << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_sth::"
                         "visit_module - "
                         "codegen for scope failed\n"),
                         -1);
    }

  *os << be_uidt_nl << "} // module " << node->name ();

  return 0;
}

int
be_visitor_root_sth::visit_interface (be_interface *node)
{
  if (node->imported () || node->is_local ())
    {
      return 0;
    }

  // Generate the TIE class.

  this->ctx_->node (node);
  be_visitor_interface_tie_sh visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_root_sth::"
                         "visit_interface - "
                         "codegen for TIE class failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root_sth::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

