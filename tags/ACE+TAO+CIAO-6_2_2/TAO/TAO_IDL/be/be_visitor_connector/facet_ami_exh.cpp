
//=============================================================================
/**
 *  @file    facet_ami_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for facets in the AMI connector
 *  impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "connector.h"
#include "ast_generator.h"
#include "be_predefined_type.h"

be_visitor_facet_ami_exh::be_visitor_facet_ami_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    iface_ (0),
    callback_iface_ (0),
    scope_name_ (0),
    iface_name_ (0),
    sync_ (false)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
}

be_visitor_facet_ami_exh::~be_visitor_facet_ami_exh (void)
{
}

int
be_visitor_facet_ami_exh::visit_component (be_component *node)
{
  this->node_ = node;

  /// Not sure if this bulletproof. The contents of *A.idl come
  /// from iteration over the AMI4CCM interfaces list, which is
  /// in turn populated by the corresponding pragmas. We may
  /// eventually have to do the same for this code generation,
  /// as well as that of be_visitor_connector_ami_exh.
  return this->visit_scope (node);
}

int
be_visitor_facet_ami_exh::visit_provides (be_provides *node)
{
  this->iface_ =
    be_interface::narrow_from_decl (node->provides_type ());

  if (this->gen_reply_handler_class () == -1)
    {
    ACE_ERROR_RETURN ((LM_ERROR,
             ACE_TEXT ("be_visitor_facet_ami_exh")
             ACE_TEXT ("::visit_provides - ")
             ACE_TEXT ("gen_reply_handler_class() ")
             ACE_TEXT ("failed\n")),
            -1);
   }


  if (this->gen_facet_executor_class () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_executor_class() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}
int
be_visitor_facet_ami_exh::visit_attribute (be_attribute *node)
{

  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       node->name (),
                       0,
                       0);

  get_op.set_name ((UTL_IdList *) node->name ()->copy ());
  if (this->visit_operation (&get_op) == -1)
    {
       ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_facet_ami_exh::"
                             "visit_attribute - "
                             "codegen for get_attribute failed\n"),
                            -1);
    }
  get_op.destroy ();

  if (node->readonly ())
    {
      // Nothing else to do.
      return 0;
    }
  Identifier id ("void");
  UTL_ScopedName sn (&id, 0);

  // Create the return type, which is "void"
  be_predefined_type rt (AST_PredefinedType::PT_void, &sn);

  // Argument type is the same as the attribute type.
  AST_Argument *arg =
    idl_global->gen ()->create_argument (AST_Argument::dir_IN,
                                         node->field_type (),
                                         node->name ());

  arg->set_name ((UTL_IdList *) node->name ()->copy ());

  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       node->name (),
                       0,
                       0);

  set_op.set_name ((UTL_IdList *) node->name ()->copy ());
  set_op.be_add_argument (arg);

  if (this->visit_operation (&set_op) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_facet_ami_exh::"
                        "visit_attribute - "
                        "codegen for set_attribute failed\n"),
                        -1);
    }

    set_op.destroy ();
    rt.destroy ();

  return 0;
}

int
be_visitor_facet_ami_exh::visit_operation (be_operation *node)
{
  AST_Decl *d =
    ScopeAsDecl (node->defined_in ());

  /// We end up here also from the visit_scope() call on the
  /// connector. We want to skip the CCM-related operations
  /// that were added to the connector since it's a component.
  /// We want only the facet interface operations.
  /// In case of sync. attribute operations we have a node_type NT_root
  if ((d->node_type () != AST_Decl::NT_interface) &&
      (d->node_type () != AST_Decl::NT_root))
    {
      return  0;
    }

  /// We're generating implementation operation declarations,
  /// so we can just use this visitor.
  be_visitor_operation_ih v (this->ctx_);

  if (v.visit_operation (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_ami_exh")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("be_visitor_operation_ih ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }

  return 0;
}

void
be_visitor_facet_ami_exh::init (bool for_impl)
{
  UTL_Scope *s = this->iface_->defined_in ();
  AST_Decl *scope = ScopeAsDecl (s);
  this->scope_name_ = scope->full_name ();
  bool global = (scope->node_type () == AST_Decl::NT_root);
  this->smart_scope_ = (global ? "" : "::");
  this->iface_name_ = this->iface_->local_name ();

  /// The reply handler class we are generating inherits from the
  /// CORBA AMI skeleton class, not the AMI_xxxCallback class
  /// generated from the corresponding interface in this IDL file.
  /// So to get the correct *_excep operation signatures, we
  /// visit the scope of the AMI_xxxHandler interface generated
  /// by -GC, which must be applied to this IDL file.
  this->handler_str_ = this->scope_name_;
  this->handler_str_ += this->smart_scope_;

  this->handler_str_ += (for_impl ? "" : "AMI_");
  ACE_CString tmp (this->iface_name_);
  this->handler_str_ +=
    (for_impl ? tmp : tmp.substr (ACE_OS::strlen ("AMI4CCM_")));
  this->handler_str_ += (for_impl ? "Reply" : "");
  this->handler_str_ += "Handler";

  /// Look up the AMI_xxxCallback class (see comment above)
  /// so we can traverse its inheritance graph below.
  UTL_ScopedName *sn =
    FE_Utils::string_to_scoped_name (this->handler_str_.c_str ());
  AST_Decl *d = s->lookup_by_name (sn, true, false);
  this->callback_iface_ = be_interface::narrow_from_decl (d);

  if (this->callback_iface_ == 0)
    this->sync_  = true;

  sn->destroy ();
  delete sn;
  sn = 0;
}

int
be_visitor_facet_ami_exh::gen_reply_handler_class (void)
{
  os_ << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;


  const char *suffix = "_reply_handler";
  this->init (false);
  if (this->sync_)
    return 0;
  os_ << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << this->iface_name_ << suffix << be_idt_nl
      << ": public ::POA_" << this->handler_str_.c_str ()
      << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->iface_name_ << suffix << " (" << be_idt_nl
      << "::" << this->scope_name_ << this->smart_scope_
      << this->iface_name_
      << "ReplyHandler_ptr callback," << be_nl
      << "::PortableServer::POA_ptr poa);" << be_uidt_nl << be_nl
      << "virtual ~" << this->iface_name_ << suffix << " (void);";

  int const status =
    this->callback_iface_->traverse_inheritance_graph (
      be_interface::op_attr_decl_helper,
      &os_,
      false,
      false);

  if (status == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_facet_ami_exh")
                  ACE_TEXT ("::gen_reply_handler_class - ")
                  ACE_TEXT ("traverse_inheritance_graph() on ")
                  ACE_TEXT ("callback interface failed\n")));
    }

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << this->scope_name_ << this->smart_scope_
      << this->iface_name_
      << "ReplyHandler_var callback_;" << be_nl
      << "::PortableServer::POA_var poa_;" << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_facet_ami_exh::gen_facet_executor_class (void)
{
    os_ << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  const char *suffix = "_exec_i";
  const char *scope_name =
    ScopeAsDecl (this->iface_->defined_in ())->full_name ();
  const char *iface_name = this->iface_->local_name ();

  os_ << be_nl_2
      << "class " << export_macro_.c_str () << " "
      << iface_name << suffix << be_idt_nl
      << ": public virtual ::" << scope_name << "::CCM_"
      << iface_name << "," << be_idt_nl
      << "public virtual ::CORBA::LocalObject"
      << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << iface_name << suffix << " (void);" << be_nl_2
      << "virtual ~" << iface_name << suffix
      << " (void);";


  ACE_CString handler_str (
  ScopeAsDecl (this->iface_->defined_in ())->full_name ());
  ACE_CString tmp (this->iface_->local_name ());
  handler_str += "::";
  handler_str += tmp;

  if (ACE_OS::strstr (tmp.c_str(), "AMI4CCM") != 0)
    this->sync_ = false;
  else
    this->sync_ = true;
  if (this->sync_)
   {
     UTL_Scope *ss = this->iface_->defined_in();
     UTL_ScopedName *sn =
     FE_Utils::string_to_scoped_name (handler_str.c_str ());
     AST_Decl *d = ss->lookup_by_name (sn, true);

     sn->destroy ();
     delete sn;
     sn = 0;

     be_interface *sync_iface =
     be_interface::narrow_from_decl (d);

     /// The overload of traverse_inheritance_graph() used here
     /// doesn't automatically prime the queues.
     sync_iface->get_insert_queue ().reset ();
     sync_iface->get_del_queue ().reset ();
     sync_iface->get_insert_queue ().enqueue_tail (sync_iface);



     Facet_AMI_ExecH_Op_Attr_Generator op_attr_gen (this);
     int status =
         sync_iface->traverse_inheritance_graph(
             op_attr_gen,
             &os_,
             false,
             false);

     if (status == -1)
       {
         ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("be_visitor_facet_ami_exh")
                      ACE_TEXT ("::gen_facet_executor_class - ")
                      ACE_TEXT ("traverse_inheritance_graph() on ")
                      ACE_TEXT ("interface failed\n")));

       }
   }
 else
   {
     if (this->visit_scope (this->iface_) == -1)
       {
         ACE_ERROR_RETURN ((LM_ERROR,
                                ACE_TEXT ("be_visitor_connector_ami_exh")
                                ACE_TEXT ("::gen_facet_executor_class - ")
                                ACE_TEXT ("visit_scope() on sendc ")
                                ACE_TEXT ("interface failed\n")),
                               -1);
       }
   }

  const char *container_type = be_global->ciao_container_type ();

  os_ << be_nl_2
      << "virtual void set_"
      << tao_cg->downcase (container_type)
      << "_context ("
      << "::Components::"
      << be_global->ciao_container_type () << "Context_ptr ctx);";

  AST_Decl *s = ScopeAsDecl (this->node_->defined_in ());
  bool is_global =
   (s->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");

  os_ << be_nl_2
      << "virtual ::CORBA::Object_ptr _get_component (void);";

  os_ << be_nl_2
      << "virtual void _set_component (" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_ptr);" << be_uidt;

  os_ << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_Context_var context_;" << be_nl
      << "::" << s->name () << smart_scope
      << "CCM_" << this->node_->local_name ()
      << "_var component_;" << be_uidt_nl
      << "};";

  return 0;
}
// ==================================================

Facet_AMI_ExecH_Op_Attr_Generator::Facet_AMI_ExecH_Op_Attr_Generator (
      be_visitor_scope * visitor)
  : visitor_ (visitor)
{
}

int
Facet_AMI_ExecH_Op_Attr_Generator::emit (be_interface * /*derived_interface*/,
                                        TAO_OutStream *  /*os*/,
                                        be_interface * base_interface)
{
  return visitor_->visit_scope (base_interface);
}
