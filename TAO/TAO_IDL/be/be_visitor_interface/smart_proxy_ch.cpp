//=============================================================================
/**
 *  @file    smart_proxy_ch.cpp
 *
 *  This provides code generation for smart proxy classes for an
 *  interface in the client header.
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_smart_proxy_ch::be_visitor_interface_smart_proxy_ch (
    be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_smart_proxy_ch::~be_visitor_interface_smart_proxy_ch () = default;

int be_visitor_interface_smart_proxy_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = nullptr;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2
      << "class " << be_global->stub_export_macro () << " "
      << "TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "/**" << be_nl
      << " * @param permanent If true, this factory stays registered until the" << be_nl
      << " * program terminates. If false, it is unregistered after its first" << be_nl
      << " * invocation, allowing a different smart proxy per object." << be_nl
      << " */" << be_nl
      << "explicit TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory (bool permanent = true);"
      << be_nl_2
      << "virtual ~TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory ();" << be_nl_2
      << "virtual " << node->local_name () << "_ptr create_proxy ("
      << node->local_name () << "_ptr proxy);" << be_uidt_nl
      << "};";

  *os << be_nl_2
      << "class " << be_global->stub_export_macro () << " "
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "friend class TAO_Singleton<TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter, TAO_SYNCH_RECURSIVE_MUTEX>;" << be_nl_2
      << "void register_proxy_factory (TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory *df, bool one_shot_factory = true);" << be_nl
      << "void unregister_proxy_factory ();" << be_nl_2
      << node->local_name () << "_ptr create_proxy ("
      << node->local_name () << "_ptr proxy);" << be_uidt_nl
      << "protected:" << be_idt_nl
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter () = default;" << be_nl
      << "~TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter ();" << be_nl
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter (TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter const &) = delete;" << be_nl
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter (TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter &&) = delete;" << be_nl
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter &operator= (TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter const &) = delete;" << be_nl
      << "TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter &operator= (TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter &&) = delete;" << be_nl
      << "TAO_" << node->flat_name ()
      << "_Default_Proxy_Factory *proxy_factory_ {};" << be_nl
      << "bool one_shot_factory_ {};" << be_nl
      << "bool disable_factory_ {};" << be_nl
      << "TAO_SYNCH_RECURSIVE_MUTEX lock_;" << be_uidt_nl
      << "};";

  *os << be_nl_2
      << "using TAO_" << node->flat_name () << "_PROXY_FACTORY_ADAPTER = "
      << "TAO_Singleton<TAO_" << node->flat_name ()
      << "_Proxy_Factory_Adapter, TAO_SYNCH_RECURSIVE_MUTEX>;" << be_nl_2;

  *os << be_nl_2
      << "class " << be_global->stub_export_macro () << " "
      << "TAO_"<< node->flat_name ()
      << "_Smart_Proxy_Base" << be_idt_nl
      << ": public virtual "
      << bt->nested_type_name (this->ctx_->scope ()->decl ());


  if (node->n_inherits () > 0)
    {
      long i;

      for (i = 0; i < node->n_inherits (); i++)
        {
          be_interface *inherited =
            dynamic_cast<be_interface*> (node->inherits ()[i]);
          be_decl *scope = nullptr;

          if (inherited->is_nested ())
            {
              // Inherited node is used in the scope of "node" node.
              scope =
                dynamic_cast<be_scope*> (node->defined_in ())->decl ();
            }

          *os << "," << be_nl << "  public virtual ";
          *os << inherited->nested_sp_type_name (scope,
                                                 "_Smart_Proxy_Base");
        }
    }
  else
    {
      *os << "," << be_nl << "  public virtual TAO_Smart_Proxy_Base";
    }

  *os << be_uidt_nl;

  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << "TAO_"<< node->flat_name () << "_Smart_Proxy_Base () = default;"
      << be_nl
      << "~TAO_" << node->flat_name ()
      << "_Smart_Proxy_Base () override;" << be_nl
      // This method will delegate this method to the <base_proxy_>
      // member of the smart proxy and so the smart proxy's (nil)
      // stubobj will not be returned.
      << "TAO_Stub *_stubobj () const override;" << be_nl
      << "TAO_Stub *_stubobj () override;"
      << be_nl;

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "protected:" << be_idt_nl
      << "::" << node->full_name ()
      << "_ptr get_proxy ();" << be_nl
      << "::" << node->full_name () << "_var proxy_ {};"
      << be_uidt_nl
      << "};\n\n";

  return 0;
}

int be_visitor_interface_smart_proxy_ch::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

