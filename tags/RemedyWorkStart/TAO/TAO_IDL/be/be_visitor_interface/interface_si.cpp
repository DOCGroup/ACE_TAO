
//=============================================================================
/**
 *  @file    interface_si.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server inline file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************************
// Interface visitor for server inline
// ************************************************************************

be_visitor_interface_si::be_visitor_interface_si (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_si::~be_visitor_interface_si (void)
{
}

int
be_visitor_interface_si::visit_interface (be_interface *node)
{
  if (node->srv_inline_gen ()
      || node->imported ()
      || node->is_local ()
      || node->is_abstract ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Determine if we are in some form of a multiple inheritance.
  int status =
    node->traverse_inheritance_graph (be_interface::in_mult_inheritance_helper,
                                      0);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_si::")
                         ACE_TEXT ("visit_interface ")
                         ACE_TEXT ("error determining mult ")
                         ACE_TEXT ("inheritance\n")),
                        -1);
    }

  // Generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the
  // call. Hence we generate these in the inline file.
  status = node->traverse_inheritance_graph (be_interface::gen_skel_helper,
                                             os);
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_si::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for base ")
                         ACE_TEXT ("class skeletons failed\n")),
                        -1);
    }

  if (this->generate_amh_classes (node) == -1)
    {
      return -1;
    }

  if (be_global->gen_direct_collocation ())
    {
      status =
        node->traverse_inheritance_graph (
            be_interface::gen_colloc_op_defn_helper,
            os);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_si::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("codegen for collocated base ")
                             ACE_TEXT ("class skeletons failed\n")),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_interface_si::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_si::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_si::generate_amh_classes (be_interface *node)
{
  // We have to check for an abstract ancestor until AMH is integrated
  // with abstract interfaces. If the node itself is abstract, this
  // visitor would not be created.
  if (be_global->gen_amh_classes () && !node->has_mixed_parentage ())
    {
      be_visitor_amh_interface_si amh_intf (this->ctx_);
      return amh_intf.visit_interface (node);
    }

  return 0;
}
