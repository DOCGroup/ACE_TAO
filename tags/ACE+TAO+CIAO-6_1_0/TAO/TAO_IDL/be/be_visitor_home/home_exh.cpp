
//=============================================================================
/**
 *  @file    home_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for homes in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "home.h"

be_visitor_home_exh::be_visitor_home_exh (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    comp_ (0),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->exec_export_macro ())
{
}

be_visitor_home_exh::~be_visitor_home_exh (void)
{
}

int
be_visitor_home_exh::visit_home (be_home *node)
{
  if (node->imported ())
    {
      return 0;
    }

  node_ = node;
  comp_ = node_->managed_component ();

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "namespace CIAO_" << comp_->flat_name () << "_Impl" << be_nl
      << "{" << be_idt;

  if (this->gen_exec_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
                         ACE_TEXT ("visit_home - ")
                         ACE_TEXT ("gen_servant_class() failed\n")),
                        -1);
    }

  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_home_exh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_home_exh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}

int
be_visitor_home_exh::visit_factory (be_factory *node)
{
  os_ << be_nl_2
      << "virtual ::Components::EnterpriseComponent_ptr" << be_nl
      << node->local_name ();

  // We can reuse this visitor.
  be_visitor_valuetype_init_arglist_ch v (this->ctx_);

  if (v.visit_factory (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_exh::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("codegen for argument ")
                         ACE_TEXT ("list failed\n")),
                        -1);
    }

  os_ << ";";

  return 0;
}

int
be_visitor_home_exh::gen_exec_class (void)
{
  // We don't want a '_cxx_' prefix here.
  const char *lname =
    node_->original_local_name ()->get_string ();

  os_ << be_nl
      << "class " << export_macro_.c_str () << " " << lname
      << "_exec_i" << be_idt_nl
      << ": public virtual " << lname << "_Exec," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt;

  os_ << be_nl
      << lname << "_exec_i (void);";

  os_ << be_nl_2
      << "virtual ~" << lname << "_exec_i (void);";

  be_home *h = node_;

  while (h != 0)
    {
      if (this->visit_scope (h) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_home_exh::")
                             ACE_TEXT ("gen_exec_class - ")
                             ACE_TEXT ("visit_scope() failed\n")),
                            -1);
        }

      for (long i = 0; i < h->n_inherits (); ++i)
        {
          // A closure of all the supported interfaces is stored
          // in the base class 'pd_inherits_flat' member.
          be_interface *bi =
            be_interface::narrow_from_decl (h->inherits ()[i]);

          int status =
            bi->traverse_inheritance_graph (
              be_visitor_home_exh::op_attr_decl_helper,
              &os_);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_home_exh::")
                                 ACE_TEXT ("gen_exec_class - ")
                                 ACE_TEXT ("traverse_inheritance_graph() ")
                                 ACE_TEXT ("failed on %s\n"),
                                 bi->full_name ()),
                                -1);
            }
        }

      h = be_home::narrow_from_decl (h->base_home ());
    }

  os_ << be_nl_2
      << "// Implicit operations.";

  os_ << be_nl_2
      << "virtual ::Components::EnterpriseComponent_ptr" << be_nl
      << "create (void);";

  os_ << be_uidt_nl
      << "};";

  return 0;
}

void
be_visitor_home_exh::gen_entrypoint (void)
{
  os_ << be_nl_2
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::Components::HomeExecutorBase_ptr" << be_nl
      << "create_" << node_->flat_name ()
      << "_Impl (void);";
}

int
be_visitor_home_exh::op_attr_decl_helper (be_interface * /* derived */,
                                          be_interface *ancestor,
                                          TAO_OutStream *os)
{
  /// We're in a static method, so we have to instantiate a temporary
  /// visitor and context.
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_ROOT_EXH);
  ctx.stream (os);
  be_visitor_home_exh visitor (&ctx);

  /// Since this visitor overriddes only visit_operation() and
  /// visit_attribute(), we can get away with this for the declarations.
  return visitor.visit_scope (ancestor);
}
