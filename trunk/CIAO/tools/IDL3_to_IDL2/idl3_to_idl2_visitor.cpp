/* -*- c++ -*- */
// $Id$

#include "idl3_to_idl2_visitor.h"
#include "identifier_helper.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_connector.h"
#include "ast_porttype.h"
#include "ast_mirror_port.h"
#include "ast_component_fwd.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_home.h"
#include "ast_finder.h"
#include "ast_operation.h"
#include "ast_root.h"

#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "nr_extern.h"

idl3_to_idl2_visitor::idl3_to_idl2_visitor (void)
  : basic_visitor (),
    home_ (0)
{
}

idl3_to_idl2_visitor::~idl3_to_idl2_visitor (void)
{
}

int
idl3_to_idl2_visitor::visit_module (AST_Module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  ACE_CString name =
    IdentifierHelper::try_escape (node->original_local_name ());

  *os << "module " << name.c_str () << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);
  this->check_prefix (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_module - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}; // end of module " << name.c_str ();

  return 0;
}

int
idl3_to_idl2_visitor::visit_interface (AST_Interface *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl;

  if (node->is_local ())
    {
      *os << "local ";
    }

  if (node->is_abstract ())
    {
      *os << "abstract ";
    }

  *os << "interface "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  AST_Type **parents = node->inherits ();

  for (long i = 0; i < node->n_inherits (); ++i)
    {
      if (i == 0)
        {
          *os << " : ";
        }
      else
        {
          *os << ", ";
        }

      *os << IdentifierHelper::orig_sn (parents[i]->name ()).c_str ();
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);
  this->check_prefix (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_interface - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_component (AST_Component *node)
{
  if (node->imported ())
    {
      return 0;
    }
    
  *os << be_nl << be_nl
      << "interface "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

  AST_Component *base = node->base_component ();
  long nsupports = node->n_supports ();

  *os << " : "
      << (base != 0
            ? IdentifierHelper::orig_sn (base->name ()).c_str ()
            : "Components::CCMObject");

  for (long i = 0; i < nsupports; ++i)
    {
      *os << ", "
          << IdentifierHelper::orig_sn (node->supports ()[i]->name ()).c_str ();
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_component - "
                          "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  *os << be_nl << be_nl
      << "component "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

  return 0;
}

int
idl3_to_idl2_visitor::visit_provides (AST_Provides *node)
{
  Identifier *orig_id =
    IdentifierHelper::original_local_name (node->local_name ());

  UTL_ScopedName *n = node->provides_type ()->name ();
  ACE_CString impl_name =
    IdentifierHelper::orig_sn (n);

  *os << be_nl << be_nl
      << impl_name.c_str () << " provide_"
      << this->port_prefix_.c_str () << orig_id << " ();";

  orig_id->destroy ();
  delete orig_id;
  orig_id = 0;

  return 0;
}

int
idl3_to_idl2_visitor::visit_uses (AST_Uses *node)
{
  *os << be_nl << be_nl;

  Identifier *orig_id =
    IdentifierHelper::original_local_name (node->local_name ());

  UTL_ScopedName *n = node->uses_type ()->name ();
  ACE_CString impl_name =
    IdentifierHelper::orig_sn (n);
    
  ACE_CString port_name (this->port_prefix_);
  port_name += orig_id->get_string ();

  if (node->is_multiple ())
    {
      *os << "struct " << port_name.c_str ()
          << "Connection" << be_nl
          << "{" << be_idt_nl
          << impl_name.c_str () << " objref;" << be_nl
          << "Components::Cookie ck;" << be_uidt_nl
          << "};" << be_nl << be_nl
          << "typedef sequence<" << orig_id << "Connection> "
          << port_name.c_str () << "Connections;"
          << be_nl << be_nl
          << "Components::Cookie connect_" << port_name.c_str ()
          << " (in " << impl_name.c_str () << " connection)"
          << be_idt_nl
          << "raises (Components::ExceededConnectionLimit, "
          << "Components::InvalidConnection);"
          << be_uidt_nl << be_nl
          << impl_name.c_str () << " disconnect_"
          << port_name.c_str () << " (in Components::Cookie ck)"
          << be_idt_nl
          << "raises (Components::InvalidConnection);"
          << be_uidt_nl << be_nl
          << port_name.c_str () << "Connections get_connections_"
          << port_name.c_str () << " ();";
    }
  else
    {
      *os << "void connect_" << port_name.c_str () << " (in "
          << impl_name.c_str () << " conxn)" << be_idt_nl
          << "raises (Components::AlreadyConnected, "
          << "Components::InvalidConnection);"
          << be_uidt_nl << be_nl
          << impl_name.c_str () << " disconnect_"
          << port_name.c_str () << " ()" << be_idt_nl
          << "raises (Components::NoConnection);"
          << be_uidt_nl << be_nl
          << impl_name.c_str () << " get_connection_"
          << port_name.c_str () << " ();";
    }

  orig_id->destroy ();
  delete orig_id;
  orig_id = 0;

  return 0;
}

int
idl3_to_idl2_visitor::visit_publishes (AST_Publishes *node)
{
  Identifier *orig_id =
    IdentifierHelper::original_local_name (node->local_name ());

  UTL_ScopedName *n = node->publishes_type ()->name ();
  ACE_CString impl_name =
    IdentifierHelper::orig_sn (n, true);

  *os << be_nl << be_nl
      << "Components::Cookie subscribe_" << orig_id
      << " (in "
      << impl_name.c_str () << "Consumer consumer)"
      << be_idt_nl
      << "raises (Components::ExceededConnectionLimit);"
      << be_uidt_nl << be_nl
      << impl_name.c_str () << "Consumer unsubscribe_" << orig_id
      << " (in Components::Cookie ck)" << be_idt_nl
      << "raises (Components::InvalidConnection);" << be_uidt;

  orig_id->destroy ();
  delete orig_id;
  orig_id = 0;

  return 0;
}

int
idl3_to_idl2_visitor::visit_emits (AST_Emits *node)
{
  Identifier *orig_id =
    IdentifierHelper::original_local_name (node->local_name ());

  UTL_ScopedName *n = node->emits_type ()->name ();
  ACE_CString impl_name =
    IdentifierHelper::orig_sn (n, true);

  *os << be_nl << be_nl
      << "void connect_" << orig_id
      << " (in "
      << impl_name.c_str () << "Consumer consumer)" << be_idt_nl
      << "raises (Components::AlreadyConnected);"
      << be_uidt_nl << be_nl
      << impl_name.c_str () << "Consumer disconnect_" << orig_id
      << " ()" << be_idt_nl
      << "raises (Components::NoConnection);" << be_uidt;

  orig_id->destroy ();
  delete orig_id;
  orig_id = 0;

  return 0;
}

int
idl3_to_idl2_visitor::visit_consumes (AST_Consumes *node)
{
  Identifier *orig_id =
    IdentifierHelper::original_local_name (node->local_name ());

  UTL_ScopedName *n = node->consumes_type ()->name ();
  ACE_CString impl_name =
    IdentifierHelper::orig_sn (n, true);

  *os << be_nl << be_nl
      << impl_name.c_str () << "Consumer get_consumer_" << orig_id
      << " ();";

  orig_id->destroy ();
  delete orig_id;
  orig_id = 0;

 return 0;
}

int
idl3_to_idl2_visitor::visit_porttype (AST_PortType *node)
{
  /// We want to visit these nodes only by navigating from an
  /// extended port or a mirror port.
  return 0;
}

int
idl3_to_idl2_visitor::visit_extended_port (AST_Extended_Port *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// Skip if we are defined inside a porttype.
  /// Depends on nested ports not being allowed.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->port_prefix_ = node->local_name ()->get_string ();
      this->port_prefix_ += '_';
    }

  if (this->visit_porttype_scope (node->port_type ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("idl3_to_idl2_visitor")
                         ACE_TEXT ("::visit_extended_port - ")
                         ACE_TEXT ("visit porttype scope failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->port_prefix_ = "";
  return 0;
}

int
idl3_to_idl2_visitor::visit_mirror_port (AST_Mirror_Port *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// Skip if we are defined inside a porttype.
  /// Depends on nested ports not being allowed.
  if (nt == AST_Decl::NT_component || nt == AST_Decl::NT_connector)
    {
      this->port_prefix_ = node->local_name ()->get_string ();
      this->port_prefix_ += '_';
    }
    
  int status =
    this->visit_porttype_scope_mirror (node->port_type ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("idl3_to_idl2_visitor")
                         ACE_TEXT ("::visit_mirror_port - ")
                         ACE_TEXT ("visit_porttype_scope")
                         ACE_TEXT ("_mirror failed\n")),
                        -1);
    }

  /// Reset port prefix string.
  this->port_prefix_ = "";
  return 0;
}

int
idl3_to_idl2_visitor::visit_connector (AST_Connector *node)
{
  return this->visit_component (node);
}

int
idl3_to_idl2_visitor::visit_param_holder (AST_Param_Holder *)
{
  return 0;
}

int
idl3_to_idl2_visitor::visit_eventtype (AST_EventType *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_valuetype (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_eventtype - "
                          "codegen for valuetype failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "interface " << node->original_local_name () << "Consumer : ";

  AST_Type *parent = 0;
  AST_Decl::NodeType nt = AST_Decl::NT_native;

  if (node->n_inherits () > 0)
    {
      parent = node->inherits ()[0];
      AST_Type *ut = parent->unaliased_type ();
      nt = ut->node_type ();
    }

  if (node->n_inherits () == 0 || nt == AST_Decl::NT_valuetype)
    {
      *os << "Components::EventConsumerBase";
    }
  else
    {
      *os << IdentifierHelper::orig_sn (node->inherits ()[0]->name (), true).c_str ()
          << "Consumer";
    }

  *os << be_nl
      << "{" << be_idt_nl
      << "void push_" << node->original_local_name () << " (in "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " the_"
      << node->original_local_name () << ");" << be_uidt_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_valuetype_fwd (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl3_to_idl2_visitor::visit_eventtype_fwd - "
                          "codegen for valuetype_fwd failed\n"),
                        -1);
    }

  *os << be_nl
      << "interface " << node->original_local_name () << "Consumer;";

  return 0;
}

int
idl3_to_idl2_visitor::visit_home (AST_Home *node)
{
  if (node->imported ())
    {
      return 0;
    }
    
  this->home_ = node;

  ACE_CString explicit_name = node->original_local_name ()->get_string ();
  explicit_name += "Explicit";

  *os << be_nl << be_nl
      << "interface " << explicit_name.c_str () << " : ";

  AST_Home *base = node->base_home ();

  if (base == 0)
    {
      *os << "Components::CCMHome";
    }
  else
    {
      *os << IdentifierHelper::orig_sn (base->name (), true).c_str ()
          << "Explicit";
    }

  *os << be_nl
      << "{" << be_idt;

  this->check_id_and_version (node);

  // Create a temporary interface node corresponding to the one we
  // just generated above for the home explicit interface.
  UTL_ScopedName *sn =
    this->create_scoped_name (0,
                              explicit_name.c_str (),
                              0,
                              ScopeAsDecl (node->defined_in ()));

  AST_Interface xplicit (sn,
                         0,
                         0,
                         0,
                         0,
                         false,
                         false);
  xplicit.set_defined_in (node->defined_in ());

  // Reset the home's decls to be defined in the explicit home interface.
  this->tranfer_scope_elements (node, xplicit);

  *os << be_uidt_nl
      << "};" << be_nl << be_nl;

  xplicit.destroy ();
  sn->destroy ();
  delete sn;
  sn = 0;

  AST_Type *key = node->primary_key ();
  ACE_CString mng_name =
    IdentifierHelper::orig_sn (node->managed_component ()->name ());
  ACE_CString key_name;

  // Generate the implicit home interface and its operations.
  *os << "interface " << node->original_local_name () << "Implicit"
      << (key == 0 ? " : Components::KeylessCCMHome" : "") << be_nl
      << "{" << be_idt_nl
      << mng_name.c_str () << " create (";

  if (key != 0)
    {
      key_name =
        IdentifierHelper::orig_sn (key->name ());

      *os << "in " << key_name.c_str () << " key";
    }

  *os << ")" << be_idt_nl
      << "raises (Components::CreateFailure";

  if (key != 0)
    {
      *os << ", Components::InvalidKey, Components::DuplicateKeyValue";
    }

  *os << ");" << be_uidt;

  if (key != 0)
    {
      *os << be_nl << be_nl
          << mng_name.c_str ()
          << " find_by_primary_key (in " << key_name.c_str ()
          << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::FinderFailure);" << be_uidt;

      *os << be_nl << be_nl
          << "void remove (in " << key_name.c_str () << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::RemoveFailure);" << be_uidt;

      *os << be_nl << be_nl
          << key_name.c_str () << " get_primary_key (in "
          << mng_name.c_str () << " comp);";
    }

  *os << be_uidt_nl
      << "};";

  // Create equivalent interface.
  *os << be_nl << be_nl
      << "interface "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " : "
      << node->original_local_name () << "Explicit, "
      << node->original_local_name () << "Implicit" << be_nl
      << "{" << be_nl
      << "};";

  return 0;
}

int
idl3_to_idl2_visitor::visit_factory (AST_Factory *node)
{
  Identifier *id = node->original_local_name ();
  
  *os << be_nl << be_nl;
  
  if (this->home_ == 0)
    {
      *os << "factory ";
    }
  else
    {
      AST_Component *c = this->home_->managed_component ();
      
      *os << IdentifierHelper::orig_sn (c->name ()).c_str ()
          << " ";
    }
    
  *os << IdentifierHelper::try_escape (id).c_str ()
      << " (";
      
  this->gen_params (node, node->argument_count ());
  
  *os << ")";
  
  this->gen_exception_list (node->exceptions ());
  
  *os << ";";
          
  return 0;
}

int
idl3_to_idl2_visitor::visit_finder (AST_Finder *node)
{
  return this->visit_factory (node);
}

int
idl3_to_idl2_visitor::visit_root (AST_Root *node)
{
  int status = be_global->outfile_init (this->os,
                                        "",
                                        "_IDL2.idl",
                                        "_TAO_IDL_",
                                        "_IDL_");

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("idl3_to_idl2_visitor::visit_root - ")
                         ACE_TEXT ("failed to initialize output file\n")),
                        -1);
    }

  ACE_CString filename;

  for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      if (i == 0)
        {
          *os << be_nl;
        }

      ACE_CString raw_filename = idl_global->included_idl_files ()[i];
      bool excluded_file_found =
        this->match_excluded_file (raw_filename.c_str ());

      if (raw_filename.find (".pidl") != ACE_CString::npos
          || raw_filename == "orb.idl"
          || raw_filename == "Components.idl"
          || excluded_file_found)
        {
          filename = raw_filename;
        }
      else
        {
          filename =
            raw_filename.substr (0, raw_filename.rfind ('.')) + "_IDL2.idl";
        }

      *os << be_nl
          << "#include \"" << filename.c_str () << "\"";
    }

  const char *pfix = node->prefix ();

  if (ACE_OS::strcmp (pfix, "") != 0)
    {
      *os << be_nl << be_nl
          << "#pragma prefix \"" << pfix << "\"";
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("idl3_to_idl2_visitor::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  *os << be_nl << be_nl
      << "#endif /* ifndef */" << be_nl << be_nl;

  return 0;
}

UTL_ScopedName *
idl3_to_idl2_visitor::create_scoped_name (const char *prefix,
                                          const char *local_name,
                                          const char *suffix,
                                          AST_Decl *parent)
{
  ACE_CString local_string (prefix,
                            0,
                            false);
  local_string += local_name;
  local_string += suffix;
  Identifier *local_id = 0;
  ACE_NEW_RETURN (local_id,
                  Identifier (local_string.fast_rep ()),
                  0);
  UTL_ScopedName *last_segment = 0;
  ACE_NEW_RETURN (last_segment,
                  UTL_ScopedName (local_id,
                                  0),
                  0);
  UTL_ScopedName *full_name =
    static_cast<UTL_ScopedName *> (parent->name ()->copy ());
  full_name->nconc (last_segment);
  return full_name;
}

void
idl3_to_idl2_visitor::tranfer_scope_elements (AST_Home *src,
                                              AST_Interface &dst)
{
  // Transfer the elements of the home's scope to the temporary
  // explicit home interface.
  for (UTL_ScopeActiveIterator src_iter (src, UTL_Scope::IK_decls);
       ! src_iter.is_done ();
       src_iter.next ())
    {
      AST_Decl *d = src_iter.item ();
      d->set_defined_in (&dst);
      UTL_ScopedName *new_name =
        this->create_scoped_name (0,
                                  d->local_name ()->get_string (),
                                  0,
                                  &dst);
      d->set_name (new_name);
      dst.add_to_scope (d);
    }

  // Visit the transferred scope elements normally to generate the IDL.
  // This way referenced items will have the interface's name in the
  // scoped name instead of the home's name.
  for (UTL_ScopeActiveIterator dst_iter (&dst, UTL_Scope::IK_decls);
       ! dst_iter.is_done ();
       dst_iter.next ())
    {
      if (dst_iter.item ()->ast_accept (this) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "idl3_to_idl2_visitor::tranfer_scope_elements - "
                      "codegen for destination scope failed\n"));
        }
    }
}

int
idl3_to_idl2_visitor::visit_porttype_scope (AST_PortType *node)
{
  return this->visit_scope (node);
}

int
idl3_to_idl2_visitor::visit_porttype_scope_mirror (
  AST_PortType *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            {
              AST_Provides *p =
                AST_Provides::narrow_from_decl (d);

              AST_Uses mirror_node (p->name (),
                                    p->provides_type (),
                                    false);

              if (this->visit_uses (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("idl3_to_idl2_visitor")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_uses() failed\n")),
                                    -1);
                }

              mirror_node.destroy ();
              break;
            }
          case AST_Decl::NT_uses:
            {
              AST_Uses *u =
                AST_Uses::narrow_from_decl (d);

              AST_Provides mirror_node (u->name (),
                                        u->uses_type ());

              if (this->visit_provides (&mirror_node) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     ACE_TEXT ("idl3_to_idl2_visitor")
                                     ACE_TEXT ("::visit_porttype_mirror - ")
                                     ACE_TEXT ("visit_provides() failed\n")),
                                    -1);
                }

              mirror_node.destroy ();
              break;
            }
          default:
            return d->ast_accept (this);
        }
    }

  return 0;
}