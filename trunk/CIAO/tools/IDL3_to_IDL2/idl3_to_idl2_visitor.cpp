/* -*- c++ -*- */
// $Id$

#include "idl3_to_idl2_visitor.h"
#include "identifier_helper.h"
#include "be_sunsoft.h"
#include "be_extern.h"

#include "ast_component_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "utl_exceptlist.h"
#include "global_extern.h"
#include "nr_extern.h"

idl3_to_idl2_visitor::idl3_to_idl2_visitor (void)
  : basic_visitor ()
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

  AST_Interface **parents = node->inherits ();

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

      *os << parents[i]->name ();
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
      << (base != 0 ? base->full_name () : "Components::CCMObject");

  for (long i = 0; i < nsupports; ++i)
    {
      *os << ", " << node->supports ()[i]->name ();
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

  this->gen_provides (node);
  this->gen_uses (node);
  this->gen_publishes (node);
  this->gen_emits (node);
  this->gen_consumes (node);

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

  *os << be_nl << be_nl;

  *os << "component "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << ";";

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

  AST_Interface *parent = 0;
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
      *os << node->inherits ()[0]->name () << "Consumer";
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

  ACE_CString explicit_name = node->local_name ()->get_string ();
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
      *os << base->name () << "Explicit";
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

  this->gen_factories (node, xplicit);
  this->gen_finders (node, xplicit);

  *os << be_uidt_nl
      << "};" << be_nl << be_nl;

  xplicit.destroy ();
  sn->destroy ();
  delete sn;
  sn = 0;

  AST_ValueType *key = node->primary_key ();

  // Generate the implicit home interface and its operations.
  *os << "interface " << node->local_name () << "Implicit"
      << (key == 0 ? " : Components::KeylessCCMHome" : "")
      << be_nl
      << "{" << be_idt_nl
      << node->managed_component ()->name () << " create (";

  if (key != 0)
    {
      *os << "in " << key->name () << " key";
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
          << node->managed_component ()->name ()
          << " find_by_primary_key (in " << key->name ()
          << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::FinderFailure);" << be_uidt;

      *os << be_nl << be_nl
          << "void remove (in " << key->name () << " key)" << be_idt_nl
          << "raises (Components::InvalidKey, Components::UnknownKeyValue, "
          << "Components::RemoveFailure);" << be_uidt;

      *os << be_nl << be_nl
          << key->name () << " get_primary_key (in "
          << node->managed_component ()->name () << " comp);";
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
idl3_to_idl2_visitor::visit_root (AST_Root *node)
{
  if (be_global->outfile_init (this->os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("idl3_to_idl2_visitor::visit_root - ")
                         ACE_TEXT ("failed to initialize output file\n")),
                        -1);
    }

  ACE_CString raw_filename;
  ACE_CString filename;
  bool excluded_file_found;
  ACE_CString::size_type p;

  for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      excluded_file_found = false;
      p = 0;
    
      if (i == 0)
        {
          *os << be_nl;
        }

      raw_filename = idl_global->included_idl_files ()[i];
      
      // If this included IDL file matches one of the 'excluded' files,
      // generate the include without tacking on the suffix.
      while (p != ACE_CString::npos)
        {
          ACE_CString::size_type cursor = p;
          p = be_global->excluded_filenames ().find (' ', cursor);
          
          ACE_CString one_filename =
            be_global->excluded_filenames ().substr (cursor, p - cursor);
            
          if (one_filename == raw_filename)
            {
              excluded_file_found = true;
              break;
            }
            
          // Skip the whitespace.  
          if (p != ACE_CString::npos)
            {
              while (be_global->excluded_filenames ()[p] == ' ')
                {
                  p++;
                }
            }
        }

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

void
idl3_to_idl2_visitor::gen_provides (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->provides ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);
      
      Identifier *orig_id = IdentifierHelper::original_local_name (pd->id);

      *os << be_nl << be_nl
          << pd->impl->name () << " provide_" << orig_id << " ();";
          
      orig_id->destroy ();
      delete orig_id;
      orig_id = 0;
    }
}

void
idl3_to_idl2_visitor::gen_uses (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->uses ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      *os << be_nl << be_nl;
      
      Identifier *orig_id = IdentifierHelper::original_local_name (pd->id);

      if (pd->is_multiple)
        {
          *os << "struct " << pd->id << "Connection" << be_nl
              << "{" << be_idt_nl
              << pd->impl->name () << " objref;" << be_nl
              << "Components::Cookie ck;" << be_uidt_nl
              << "};" << be_nl << be_nl
              << "typedef sequence<" << orig_id << "Connection> "
              << pd->id << "Connections;"
              << be_nl << be_nl
              << "Components::Cookie connect_" << orig_id << " (in "
              << pd->impl->name () << " connection)" << be_idt_nl
              << "raises (Components::ExceededConnectionLimit, "
              << "Components::InvalidConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " disconnect_" << orig_id
              << " (in Components::Cookie ck)" << be_idt_nl
              << "raises (Components::InvalidConnection);"
              << be_uidt_nl << be_nl
              << orig_id << "Connections get_connections_" << orig_id
              << " ();";
        }
      else
        {
          *os << "void connect_" << orig_id << " (in "
              << pd->impl->name () << " conxn)" << be_idt_nl
              << "raises (Components::AlreadyConnected, "
              << "Components::InvalidConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " disconnect_" << orig_id
              << " ()" << be_idt_nl
              << "raises (Components::NoConnection);" << be_uidt_nl << be_nl
              << pd->impl->name () << " get_connection_" << orig_id
              << " ();";
        }
        
      orig_id->destroy ();
      delete orig_id;
      orig_id = 0;
    }
}

void
idl3_to_idl2_visitor::gen_publishes (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->publishes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);
      
      Identifier *orig_id = IdentifierHelper::original_local_name (pd->id);

      *os << be_nl << be_nl
          << "Components::Cookie subscribe_" << orig_id
          << " (in " << pd->impl->name () <<"Consumer consumer)"
          << be_idt_nl
          << "raises (Components::ExceededConnectionLimit);"
          << be_uidt_nl << be_nl
          << pd->impl->name () << "Consumer unsubscribe_" << orig_id
          << " (in Components::Cookie ck)" << be_idt_nl
          << "raises (Components::InvalidConnection);" << be_uidt;
         
      orig_id->destroy ();
      delete orig_id;
      orig_id = 0;
   }
}

void
idl3_to_idl2_visitor::gen_emits (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->emits ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      Identifier *orig_id = IdentifierHelper::original_local_name (pd->id);

      *os << be_nl << be_nl
          << "void connect_" << orig_id
          << " (in " << pd->impl->name ()
          << "Consumer consumer)" << be_idt_nl
          << "raises (Components::AlreadyConnected);"
          << be_uidt_nl << be_nl
          << pd->impl->name () << "Consumer disconnect_" << orig_id
          << " ()" << be_idt_nl
          << "raises (Components::NoConnection);" << be_uidt;
         
      orig_id->destroy ();
      delete orig_id;
      orig_id = 0;
    }
}

void
idl3_to_idl2_visitor::gen_consumes (AST_Component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s =
    node->consumes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);
      
      Identifier *orig_id = IdentifierHelper::original_local_name (pd->id);

      *os << be_nl << be_nl
          << pd->impl->name () << "Consumer get_consumer_" << orig_id
          << " ();";
         
      orig_id->destroy ();
      delete orig_id;
      orig_id = 0;
    }
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

void
idl3_to_idl2_visitor::gen_factories (AST_Home *node,
                                     AST_Interface &)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->factories ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);

      *os << be_nl << be_nl
          << node->managed_component ()->name () << " "
          << IdentifierHelper::try_escape ((*item)->original_local_name ()).c_str ()
          << " (";

      this->gen_params (*item, (*item)->argument_count ());

      *os << ")";

      UTL_ExceptList *exceps = (*item)->exceptions ();

      if (exceps != 0 && exceps->length () > 0)
        {
          this->gen_exception_list (exceps, "", false);
        }
      else
        {
          *os << be_idt_nl
              << "raises (";
        }

      *os << "Components::CreateFailure);" << be_uidt;
    }
}

void
idl3_to_idl2_visitor::gen_finders (AST_Home *node,
                                   AST_Interface &)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->finders ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);

      *os << be_nl << be_nl
          << node->managed_component ()->name () << " "
          << IdentifierHelper::try_escape( (*item)->original_local_name ()).c_str ()
          << " (";

      this->gen_params (*item, (*item)->argument_count ());

      *os << ")";

      UTL_ExceptList *exceps = (*item)->exceptions ();

      if (exceps != 0 && exceps->length () > 0)
        {
          this->gen_exception_list (exceps, "", false);
        }
      else
        {
          *os << be_idt_nl
              << "raises (";
        }

      *os << "Components::FinderFailure);" << be_uidt;
    }
}

