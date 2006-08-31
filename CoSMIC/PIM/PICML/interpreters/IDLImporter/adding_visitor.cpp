/* -*- c++ -*- */
// $Id$

#include "adding_visitor.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_component_fwd.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_exception.h"
#include "ast_factory.h"
#include "ast_field.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_structure_fwd.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_fwd.h"
#include "ast_union_label.h"
#include "ast_valuebox.h"
#include "ast_valuetype_fwd.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "XercesString.h"

#include "ace/OS_NS_stdio.h"

// The XMAX_ and YMAX_ values below work well for a screen size
// of 1600 x 1200, giving a likely size for the IDML Model Editor
// window.
adding_visitor::adding_visitor (DOMElement *sub_tree,
                                unsigned long rel_id)
  : sub_tree_ (sub_tree),
    previous_ (0),
    doc_ (sub_tree->getOwnerDocument ()),
    rel_id_ (rel_id),
    XMAX_ (1100UL),
    YMAX_ (700UL),
    ICON_SIZE_ (64UL),
    private_relid_offset_ (0UL),
    manages_relid_offset_ (0UL),
    import_relid_offset_ (0UL),
    n_basic_seqs_ (0UL)
{
  if (be_global->first_file ())
    {
      DOMNodeList *children = sub_tree->getChildNodes ();

      for (XMLSize_t index = 0; index < children->getLength (); ++index)
        {
          DOMElement *child = (DOMElement *) children->item (index);

          if (child == 0)
            {
              continue;
            }

          const XMLCh *tag = child->getTagName ();

          if (X ("model") == tag
              || X ("folder") == tag
              || X ("reference") == tag
              || X ("atom") == tag
              || X ("connection") == tag)
            {
              ++this->import_relid_offset_;
            }
        }
    }

  this->rel_id_ += this->import_relid_offset_;
}

adding_visitor::~adding_visitor (void)
{
}

int
adding_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
adding_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      // Want to skip the uses_xxxConnection structs added by uses
      // multiple ports.
      if (ScopeAsDecl (node)->node_type () == AST_Decl::NT_component
          && nt != AST_Decl::NT_attr)
        {
          continue;
        }

      if (d->ast_accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "adding_visitor::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
adding_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
adding_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
adding_visitor::visit_module (AST_Module *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_module_dom_elem (this->sub_tree_, node);
  unsigned long holder = 0;

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("model"));

      // Simple check to see if the stuff in the block below
      // has been set already. If the elem was not imported,
      // the table entry, if any, could be from an interface_fwd.
      const XMLCh *relid = elem->getAttribute (X ("relid"));

      // We don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          // Homes can be declared at global scope or in (possibly nested)
          // modules, so we need to preserve the value of the offset for
          // the outer scope, and reset it for the inner scope.
          holder = this->manages_relid_offset_;
          this->manages_relid_offset_ = 0UL;

          this->set_id_attr (elem, BE_GlobalData::MODEL);
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          elem->setAttribute (X ("kind"), X ("Package"));
          elem->setAttribute (X ("role"), X ("Package"));
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_prefix_element (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  adding_visitor scope_visitor (elem);
  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_module - "
                         "codegen for scope failed\n"),
                        -1);
    }

  if (!node->imported ())
    {
      // Restore the offset value for the outer scope.
      this->manages_relid_offset_ = holder;
    }

  return 0;
}

int
adding_visitor::visit_interface (AST_Interface *node)
{
  // First see if it's been imported from an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // Simple check to see if the stuff in the block below
      // has been set already. If the elem was not imported,
      // the table entry, if any, could be from an interface_fwd.
      const XMLCh *relid = elem->getAttribute (X ("relid"));

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          elem->setAttribute (X ("kind"), X ("Object"));
          elem->setAttribute (X ("role"), X ("Object"));
          this->add_name_element (elem,
                                  node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (),
                              elem,
                              this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      // An XME-imported interface may still have additional base classes
      // in modified IDL.
      this->add_inherited_elements (elem, node);

      // These will modify existing values if necessary.
      this->add_prefix_element (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);
      this->add_abstract_element (elem, node);
      this->add_local_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  adding_visitor scope_visitor (elem, node->n_inherits () + 1);

  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_interface_fwd (AST_InterfaceFwd *node)
{
  DOMElement *elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), elem);

  // If there is an entry in the table, there has been a previous
  // declaration/definition or an import, and we can just return.
  if (0 == result)
    {
      return 0;
    }

  // See if it's been imported with an XME file.
  elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // If we have not imported this element, create it now.
  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("model"));
      this->set_id_attr (elem, BE_GlobalData::MODEL);
    }

  // Store it in the tables whether imported or not.
  (void) be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                             elem);
  (void) be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                           elem->getAttribute (X ("id")));
  return 0;
}

int
adding_visitor::visit_valuetype (AST_ValueType *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          AST_Decl::NodeType nt = node->node_type ();
          const char *kind =
            nt == AST_Decl::NT_valuetype ? "ValueObject" : "Event";
          elem->setAttribute (X ("kind"), X (kind));
          elem->setAttribute (X ("role"), X (kind));
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_prefix_element (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);
      this->add_abstract_element (elem, node);
      this->add_inherited_elements (elem, node);
      this->add_supported_elements (elem,
                                    node,
                                    node->supports (),
                                    node->n_supports ());

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  unsigned long start_id =
    static_cast<unsigned long> (node->n_inherits ()
                                + node->n_supports ()
                                + 1);

  adding_visitor scope_visitor (elem, start_id);
  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_valuetype - "
                         "code generation for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *node)
{
  return this->visit_interface_fwd (node);
}

int
adding_visitor::visit_component (AST_Component *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  const char *node_id = node->repoID ();
  int result =
    be_global->decl_elem_table ().find (node_id, table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          (void) this->set_id_attr (elem, BE_GlobalData::MODEL);

          // Store the DOMElement and GME id in their respective tables.
          be_global->decl_elem_table ().bind (ACE::strnew (node_id),
                                              elem);
          be_global->decl_id_table ().bind (ACE::strnew (node_id),
                                            elem->getAttribute (X ("id")));
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          elem->setAttribute (X ("kind"), X ("Component"));
          elem->setAttribute (X ("role"), X ("Component"));
          this->add_name_element (elem,
                                  node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (),
                              elem,
                              this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);

          // Add the ComponentContainer model element, and its
          // contents.
          this->add_default_container (node);

          // Add default implementation artifact model elements.
          DOMElement *artifact_container =
            this->add_implementation_artifacts (node);

          // Add default implementation for single component.
          char *id = XMLString::transcode (elem->getAttribute (X ("id")));
          this->add_implementation (id,
                                    node,
                                    artifact_container);
          XMLString::release (&id);
        }
    }

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node_id),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node_id),
                                        elem->getAttribute (X ("id")));
    }

  if (!node->imported ())
    {
      // A component imported from XME could still have modifications
      // in IDL.
      this->add_base_component (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);
      this->add_ports (elem, node);
      this->add_supported_elements (elem,
                                    node,
                                    node->supports (),
                                    node->n_supports ());

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  unsigned long start_id =
    (0 == node->base_component () ? 0UL : 1UL)
    + static_cast<unsigned long> (node->n_supports ())
    + node->provides ().size ()
    + node->uses ().size ()
    + node->publishes ().size ()
    + node->emits ().size ()
    + node->consumes ().size ()
    + 1;

  adding_visitor scope_visitor (elem, start_id);
  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_component - "
                         "code generation for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  return this->visit_interface_fwd (node);
}

int
adding_visitor::visit_eventtype (AST_EventType *node)
{
  return this->visit_valuetype (node);
}

int
adding_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  return this->visit_interface_fwd (node);
}

int
adding_visitor::visit_home (AST_Home *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          (void) this->set_id_attr (elem, BE_GlobalData::MODEL);

          // Store the DOMElement and GME id in their respective tables.
          be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                              elem);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          elem->setAttribute (X ("kind"), X ("ComponentFactory"));
          elem->setAttribute (X ("role"), X ("ComponentFactory"));
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                    elem->getAttribute (X ("id")));

  if (!node->imported ())
    {
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);
      this->add_base_home (elem, node);
      this->add_supported_elements (elem,
                                    node,
                                    node->supports (),
                                    node->n_supports ());
      this->add_manages (node);
      this->add_lookup_key (elem, node);
      this->add_home_factories (elem, node);
      this->add_finders (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // element can be inserted in the correct position.
  this->previous_ = elem;

  unsigned long start_id = (0 == node->base_home () ? 0UL : 1UL)
                           + static_cast<unsigned long> (node->n_supports ())
                           + (0 == node->primary_key () ? 0UL : 1UL)
                           + node->factories ().size ()
                           + node->finders ().size ()
                           + 1;
  adding_visitor scope_visitor (elem, start_id);
  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_home - "
                         "code generation for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_factory (AST_Factory *node)
{
  if (node->imported ())
    {
      return 0;
    }

  DOMElement *elem = 0;

  // See if it's been imported with an XME file.
  elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // If not, create it.
  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("model"));
      this->set_id_attr (elem, BE_GlobalData::MODEL);
      elem->setAttribute (X ("kind"), X ("FactoryOperation"));
      elem->setAttribute (X ("role"), X ("FactoryOperation"));
      this->set_relid_attr (elem);
      this->set_childrelidcntr_attr (elem, node);
      this->add_name_element (elem, node->local_name ()->get_string ());
      this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);
    }

  this->add_replace_id_element (elem, node);
  this->add_version_element (elem, node);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
    }

  adding_visitor scope_visitor (elem);
  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_factory - "
                         "code generation for scope failed\n"),
                        -1);
    }

  this->add_exception_elements (elem,
                                node,
                                0,
                                node->exceptions (),
                                "ExceptionRef",
                                scope_visitor.rel_id_);

  this->insert_element (elem);
  be_global->emit_diagnostic (elem);

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  return 0;
}

int
adding_visitor::visit_structure (AST_Structure *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("Aggregate"));
          elem->setAttribute (X ("role"), X ("Aggregate"));
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // If in_recursion() returns TRUE and this node is already in the
      // decl id table, we are in the first level of recursion and do
      // not need to visit the scope.
      ACE_Unbounded_Queue<AST_Type *> list;
      if (node->in_recursion (list))
        {
          return 0;
        }

      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  adding_visitor scope_visitor (elem);
  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_structure_fwd (AST_StructureFwd *node)
{
  DOMElement *elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), elem);

  // If there is an entry in the table, there has been a previous
  // declaration/definition or an import, and we can just return.
  if (0 == result)
    {
      return 0;
    }

  // See if it's been imported with an XME file.
  elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // If we have imported this element, just put it in the tables
  // and return.
  if (0 != elem)
    {
      (void) be_global->decl_elem_table ().bind (
          ACE::strnew (node->repoID ()),
          elem
        );

      (void) be_global->decl_id_table ().bind (
          ACE::strnew (node->repoID ()),
          elem->getAttribute (X ("id"))
        );

      return 0;
    }

  // Create a DOMElement and a GME id and store them in their
  // respective tables.
  elem = this->doc_->createElement (X ("model"));
  (void) be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                             elem);
  this->set_id_attr (elem, BE_GlobalData::MODEL);
  (void) be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                           elem->getAttribute (X ("id")));
  return 0;
}

int
adding_visitor::visit_exception (AST_Exception *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("Exception"));
          elem->setAttribute (X ("role"), X ("Exception"));
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  adding_visitor scope_visitor (elem);
  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
adding_visitor::visit_enum (AST_Enum *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("Enum"));
          elem->setAttribute (X ("role"), X ("Enum"));
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (!node->imported () && be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  adding_visitor scope_visitor (elem);
  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_enum - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_operation (AST_Operation *node)
{
  if (node->imported ())
    {
      return 0;
    }

  // See if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  DOMElement *return_type = 0;
  bool vrt = (node->void_return_type () != 0);

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("model"));
      this->set_id_attr (elem, BE_GlobalData::MODEL);
      ACE_CString kind =
        node->flags () == AST_Operation::OP_oneway ? "Oneway" : "Twoway";
      kind += "Operation";
      elem->setAttribute (X ("kind"), X (kind.c_str ()));
      elem->setAttribute (X ("role"), X (kind.c_str ()));
      this->set_relid_attr (elem);
      this->set_childrelidcntr_attr (elem, node);
      this->add_name_element (elem, node->local_name ()->get_string ());
      this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

      this->insert_element (elem);
      be_global->emit_diagnostic (elem);
    }

  this->add_replace_id_element (elem, node);
  this->add_version_element (elem, node);

  if (!vrt)
    {
      AST_Type *rt = node->return_type ();
      DOMNodeList *children = elem->getElementsByTagName (X ("reference"));

      for (XMLSize_t i = 0; i < children->getLength (); ++i)
        {
          DOMElement *child = (DOMElement *) children->item (i);

          if (0 == child)
            {
              continue;
            }

          const XMLCh *kind = child->getAttribute (X ("kind"));

          if (X ("ReturnType") == kind)
            {
              return_type = child;
              break;
            }
        }

      const XMLCh *gme_id  = be_global->lookup_id (rt);

      if (0 == return_type)
        {
          return_type = this->doc_->createElement (X ("reference"));
          this->set_id_attr (return_type, BE_GlobalData::REF);
          return_type->setAttribute (X ("kind"), X ("ReturnType"));
          return_type->setAttribute (X ("role"), X ("ReturnType"));
          return_type->setAttribute (X ("relid"),
                                     X (be_global->hex_string (1UL)));
          this->add_name_element (return_type, "ReturnType");
          this->add_regnodes (node, return_type, 1UL);

          // This should be the first element in the operation's scope.
          elem->appendChild (return_type);

          // Must set this before calling emit_diagnostic(), since the
          // attribute may be looked up in the call body.
          return_type->setAttribute (X ("referred"), gme_id);
          be_global->emit_diagnostic (return_type);
        }
      else
        {
          be_global->type_change_diagnostic (return_type, gme_id);
          return_type->setAttribute (X ("referred"), gme_id);
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              return_type->getAttribute (X ("id"))
            );
        }
    }

  adding_visitor scope_visitor (elem, vrt ? 1 : 2);
  scope_visitor.previous_ = return_type;

  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_operation - "
                         "code generation for scope failed\n"),
                        -1);
    }

  this->add_exception_elements (elem,
                                node,
                                0,
                                node->exceptions (),
                                "ExceptionRef",
                                scope_visitor.rel_id_);

  // Add to list used in check for removed IDL decls.
  if (!node->imported () && be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  return 0;
}

int
adding_visitor::visit_field (AST_Field *node)
{
  if (node->imported ())
    {
      return 0;
    }

  unsigned long slot = 0UL;

  // See if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("reference"));
      (void) this->set_id_attr (elem, BE_GlobalData::REF);
      elem->setAttribute (X ("kind"), X ("Member"));
      elem->setAttribute (X ("role"), X ("Member"));
      this->set_relid_attr (elem);
      this->add_name_element (elem, node->local_name ()->get_string ());
      slot = this->rel_id_ - 1;
      this->add_regnodes (node->defined_in (), elem, slot);

      this->insert_element (elem);
      be_global->emit_diagnostic (elem);
    }
  else
    {
      // We need the 'relid' attribute to compute the position of
      // of private flag, is it needs to be added. If the DOMElement
      // is not imported, this value is calculated and set, above.
      const XMLCh *relid = elem->getAttribute (X ("relid"));
      char *relid_string = XMLString::transcode (relid);
      slot = ACE_OS::strtoul (relid_string, 0, 16);
      XMLString::release (&relid_string);
   }

  const XMLCh *elem_id = elem->getAttribute (X ("id"));

  if (node->visibility () == AST_Field::vis_PRIVATE)
    {
      // If the member was imported from XML as private, this call just
      // stores the relevant ids for the removing visitors to check.
      this->add_private (node, elem_id, slot);
    }

  AST_Type *ft = node->field_type ();
  AST_Decl::NodeType nt = ft->node_type ();

  if (nt == AST_Decl::NT_array || nt == AST_Decl::NT_sequence)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_field - "
                         "anonymous types not supported: %s\n",
                         ft->full_name ()),
                        -1);
    }

  // These are outside the IF block to pick up possible changes
  // in the IDL.
  const XMLCh *gme_id = be_global->lookup_id (ft);
  be_global->type_change_diagnostic (elem, gme_id);
  elem->setAttribute (X ("referred"), gme_id);

  // Add to list used in check for removed IDL decls.
  if (0 != be_global->input_xme ())
    {
      be_global->gme_id_set ().insert (elem_id);
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  return 0;
}

int
adding_visitor::visit_argument (AST_Argument *node)
{
  // See if it's been imported with an XME file.
  DOMElement *arg =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  if (0 == arg)
    {
      arg = this->doc_->createElement (X ("reference"));
      this->set_id_attr (arg, BE_GlobalData::REF);
      ACE_CString kind;

      switch (node->direction ())
        {
          case AST_Argument::dir_IN:
            kind = "In";
            break;
          case AST_Argument::dir_INOUT:
            kind = "Inout";
            break;
          case AST_Argument::dir_OUT:
            kind = "Out";
            break;
          default:
            break;
        }

      kind += "Parameter";
      arg->setAttribute (X ("kind"), X (kind.c_str ()));
      arg->setAttribute (X ("role"), X (kind.c_str ()));
      this->set_relid_attr (arg);
      this->add_name_element (arg, node->local_name ()->get_string ());
      this->add_regnodes (node->defined_in (), arg, this->rel_id_ - 1);

      this->insert_element (arg);
      be_global->emit_diagnostic (arg);
    }

  // Outside the IF block so change in the type of an argument can
  // be reflected in the PICML model.
  AST_Type *ft = node->field_type ();
  const XMLCh *gme_id = be_global->lookup_id (ft);
  be_global->type_change_diagnostic (arg, gme_id);
  arg->setAttribute (X ("referred"), gme_id);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (arg->getAttribute (X ("id")));
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = arg;

  return 0;
}

int
adding_visitor::visit_attribute (AST_Attribute *node)
{
  if (node->imported ())
    {
      return 0;
    }

  DOMElement *member = 0;

  // See if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("model"));
      this->set_id_attr (elem, BE_GlobalData::MODEL);
      bool read_only = node->readonly ();
      const char *kind = read_only ? "ReadonlyAttribute" : "Attribute";
      elem->setAttribute (X ("kind"), X (kind));
      elem->setAttribute (X ("role"), X (kind));
      this->set_relid_attr (elem);
      this->set_childrelidcntr_attr (elem, 0, node);
      this->add_name_element (elem, node->local_name ()->get_string ());
      this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

      member = this->doc_->createElement (X ("reference"));
      this->set_id_attr (member, BE_GlobalData::REF);
      member->setAttribute (X ("kind"), X ("AttributeMember"));
      member->setAttribute (X ("role"), X ("AttributeMember"));
      member->setAttribute (X ("relid"), X (be_global->hex_string (1UL)));
      this->add_name_element (member, "AttributeMember");
      this->add_regnodes (0, member, 1UL, node);
      elem->appendChild (member);

      UTL_ExceptList *get_ex = node->get_get_exceptions ();
      this->add_exception_elements (elem,
                                    0,
                                    node,
                                    node->get_get_exceptions (),
                                    "GetException",
                                    2UL);

      if (!read_only)
        {
          unsigned long slot =
            (get_ex != 0 ? get_ex->length () : 0) + 2UL;
          this->add_exception_elements (
                    elem,
                    0,
                    node,
                    node->get_set_exceptions (),
                    "SetException",
                    slot
                  );
        }

      this->insert_element (elem);
      be_global->emit_diagnostic (elem);
      be_global->emit_diagnostic (member);
    }

  this->add_replace_id_element (elem, node);
  this->add_version_element (elem, node);

  // Get the AttributeMember and set its referenced type outside the
  // IF block so we can pick up a change from IDL.
  DOMNodeList *ref = elem->getElementsByTagName (X ("reference"));
  member = (DOMElement *) ref->item (0);
  AST_Type *ft = node->field_type ();
  const XMLCh *gme_id = be_global->lookup_id (ft);
  be_global->type_change_diagnostic (member, gme_id);
  member->setAttribute (X ("referred"), gme_id);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
      be_global->gme_id_set ().insert (member->getAttribute (X ("id")));
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  return 0;
}

int
adding_visitor::visit_union (AST_Union *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string ()
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("model"));
          this->set_id_attr (elem, BE_GlobalData::MODEL);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("SwitchedAggregate"));
          elem->setAttribute (X ("role"), X ("SwitchedAggregate"));
          this->set_relid_attr (elem);
          this->set_childrelidcntr_attr (elem, node);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      this->add_discriminator (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  // If in_recursion() returns TRUE and this node is already in the
  // decl id table, we are in the first level of recursion and do
  // not need to visit the scope.
  ACE_Unbounded_Queue<AST_Type *> list;
  if (result != 0 && node->in_recursion (list))
    {
      return 0;
    }

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  // Bump the rel_id by 1 since we've already added the discriminator.
  adding_visitor scope_visitor (elem, 2UL);
  if (scope_visitor.visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
adding_visitor::visit_union_fwd (AST_UnionFwd *node)
{
  return this->visit_structure_fwd (node);
}

int
adding_visitor::visit_union_branch (AST_UnionBranch *node)
{
  if (node->imported ())
    {
      return 0;
    }

  unsigned long slot = 0UL;
  AST_Type *ft = node->field_type ();

  // See if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  if (0 == elem)
    {
      AST_Decl::NodeType nt = ft->node_type ();

      if (nt == AST_Decl::NT_array || nt == AST_Decl::NT_sequence)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "adding_visitor::visit_union_branch - "
                             "anonymous types not supported: %s\n",
                             ft->full_name ()),
                            -1);
        }

      elem = this->doc_->createElement (X ("reference"));
      (void) this->set_id_attr (elem, BE_GlobalData::REF);
      elem->setAttribute (X ("kind"), X ("Member"));
      elem->setAttribute (X ("role"), X ("Member"));
      this->set_relid_attr (elem);
      this->add_name_element (elem, node->local_name ()->get_string ());
      slot = this->rel_id_ - 1;
      this->add_regnodes (node->defined_in (), elem, slot);

      this->insert_element (elem);
      be_global->emit_diagnostic (elem);
    }
  else
    {
      // We need the 'relid' attribute to compute the position of
      // of private flag, is it needs to be added. If the DOMElement
      // is not imported, this value is calculated and set, above.
      const XMLCh *relid = elem->getAttribute (X ("relid"));
      char *relid_string = XMLString::transcode (relid);
      slot = ACE_OS::strtoul (relid_string, 0, 16);
      XMLString::release (&relid_string);
   }

  // This will modify the attribute if the type has changed in IDL.
  const XMLCh *field_type_id = be_global->lookup_id (ft);
  be_global->type_change_diagnostic (elem, field_type_id);
  elem->setAttribute (X ("referred"), field_type_id);

  const XMLCh *elem_id = elem->getAttribute (X ("id"));
  this->add_labels (node, elem_id, slot);

  // Add to list used in check for removed IDL decls.
  if (!node->imported () && be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem_id);
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  return 0;
}

int
adding_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
adding_visitor::visit_constant (AST_Constant *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("reference"));
          this->set_id_attr (elem, BE_GlobalData::REF);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("Constant"));
          elem->setAttribute (X ("role"), X ("Constant"));
          this->set_relid_attr (elem);
          this->add_name_element (elem,
                                  node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (),
                              elem,
                              this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      const XMLCh *const_type_id = this->lookup_constant_type (node);

      if (0 == const_type_id)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "adding_visitor::visit_constant - "
                            "constant type id lookup failed\n"),
                            -1);
        }

      // Set the 'referred' attribute out here in case the
      // constant's type has changed in IDL.
      be_global->type_change_diagnostic (elem, const_type_id);
      elem->setAttribute (X ("referred"), const_type_id);

      this->add_constant_value (elem, node);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          const XMLCh *id = elem->getAttribute (X ("id"));
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  return 0;
}

int
adding_visitor::visit_enum_val (AST_EnumVal *node)
{
  // Enum values are also added to the enum's enclosing scope.
  // We don't want to generate anything for these nodes.
  const XMLCh *kind = this->sub_tree_->getAttribute (X ("kind"));
  if (X (kind) != X ("Enum"))
    {
      return 0;
    }

  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::ATOM
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("atom"));
          this->set_id_attr (elem, BE_GlobalData::ATOM);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("EnumValue"));
          elem->setAttribute (X ("role"), X ("EnumValue"));
          this->set_relid_attr (elem);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  // Add to list used in check for removed IDL decls.
  if (!node->imported () && be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  return 0;
}

// These next three are all handled elsewhere, if legal
// anonymous sequences and arrays are not supported in
// this visitor.

int
adding_visitor::visit_array (AST_Array *)
{
  return 0;
}

int
adding_visitor::visit_sequence (AST_Sequence *)
{
  return 0;
}

int
adding_visitor::visit_string (AST_String *)
{
  return 0;
}

int
adding_visitor::visit_typedef (AST_Typedef *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  AST_Type *bt = node->base_type ();
  const char *role = "Alias";

  // Collection types in IDML are generated as IDL typedefs, so we
  // want the reverse of that as the behavior here.
  switch (bt->node_type ())
    {
      case AST_Decl::NT_array:
        role = "Collection";
        bt = AST_Array::narrow_from_decl (bt)->base_type ();
        break;
      case AST_Decl::NT_sequence:
        role = "Collection";
        bt = AST_Sequence::narrow_from_decl (bt)->base_type ();
        break;
      default:
        break;
    }

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("reference"));
          this->set_id_attr (elem, BE_GlobalData::REF);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X (role));
          elem->setAttribute (X ("role"), X (role));
          this->set_relid_attr (elem);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      const XMLCh *new_id = be_global->lookup_id (bt);
      be_global->type_change_diagnostic (elem, new_id);
      elem->setAttribute (X ("referred"), new_id);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  return 0;
}

int
adding_visitor::visit_root (AST_Root *node)
{
  if (be_global->first_file ())
    {
      // If we are importing, root folder and interface definitions
      // folder are set in BE_GlobalData::cache_files().
      if (0 == be_global->input_xme ())
        {
          sub_tree_->setAttribute (
              X ("guid"),
              X ("{00000000-0000-0000-0000-000000000000}")
            );

          ACE_TCHAR day_and_time[27];
          ACE_TCHAR *s =
            this->timestamp (day_and_time, sizeof day_and_time);

          sub_tree_->setAttribute (X ("cdate"), X (s));
          sub_tree_->setAttribute (X ("mdate"), X (s));
          sub_tree_->setAttribute (
              X ("metaguid"),
              X ("{1D244777-DEAD-BEEF-FEED-DAD000000001}")
            );
          sub_tree_->setAttribute (X ("metaname"), X ("PICML"));

          ACE_CString project_name = be_global->output_file ();
          this->add_name_element (sub_tree_, project_name.c_str ());

          DOMElement *comment = doc_->createElement (X ("comment"));
          DOMText *comment_val = doc_->createTextNode (X (""));
          comment->appendChild (comment_val);
          sub_tree_->appendChild (comment);

          DOMElement *author = doc_->createElement (X ("author"));
          DOMText *author_val = doc_->createTextNode (X (""));
          author->appendChild (author_val);
          sub_tree_->appendChild (author);

          DOMElement *rf = doc_->createElement (X ("folder"));
          be_global->root_folder (rf);
          this->set_id_attr (rf, BE_GlobalData::FOLDER);
          this->set_relid_attr (rf);
          rf->setAttribute (X ("childrelidcntr"), X ("0x2"));
          rf->setAttribute (X ("kind"), X ("RootFolder"));
          sub_tree_->appendChild (rf);
          this->add_name_element (rf, project_name.c_str ());

          DOMElement *interface_definitions =
            doc_->createElement (X ("folder"));
          this->set_id_attr (interface_definitions,
                             BE_GlobalData::FOLDER);
          this->set_relid_attr (interface_definitions);
          interface_definitions->setAttribute (
              X ("childrelidcntr"),
              X (be_global->hex_string (be_global->nfiles ()))
            );
          interface_definitions->setAttribute (
              X ("kind"),
              X ("InterfaceDefinitions")
            );
          this->add_name_element (interface_definitions,
                                  "InterfaceDefinitions");
          rf->appendChild (interface_definitions);

          be_global->interface_definitions_folder (interface_definitions);
        }

      this->add_predefined_types ();
      this->add_picml_boilerplate ();
    }

  // The call to add a file element below is outside the normal
  // scope visitor mechanism, so we have to maintain a separate
  // rel_id counter for files.
  static unsigned long pass = 1UL;

  DOMElement *file =
    this->add_file_element (be_global->interface_definitions_folder (),
                            node,
                            pass++);

  unsigned long start_relid =
    this->user_includes () + this->n_basic_seqs_ + 1UL;
  adding_visitor scope_visitor (file, start_relid);

  // Affects the offset when drawing the scope elements.
  scope_visitor.n_basic_seqs_ = this->n_basic_seqs_;

  if (scope_visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::visit_root - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Reset this, in case it was modified.
  this->n_basic_seqs_ = 0UL;

  be_global->first_file (false);

  return 0;
}

int
adding_visitor::visit_native (AST_Native *)
{
  return 0;
}

int
adding_visitor::visit_valuebox (AST_ValueBox *node)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        this->sub_tree_,
        node->local_name ()->get_string (),
        BE_GlobalData::REF
      );

  // Also see if it's been put in the decl table.
  DOMElement *table_elem = 0;
  int result =
    be_global->decl_elem_table ().find (node->repoID (), table_elem);

  AST_Type *bt = node->boxed_type ();

  if (0 == elem)
    {
      if (0 == result)
        {
          elem = table_elem;
        }
      else
        {
          elem = this->doc_->createElement (X ("reference"));
          this->set_id_attr (elem, BE_GlobalData::REF);
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          elem->setAttribute (X ("kind"), X ("Boxed"));
          elem->setAttribute (X ("role"), X ("Boxed"));
          this->set_relid_attr (elem);
          this->add_name_element (elem, node->local_name ()->get_string ());
          this->add_regnodes (node->defined_in (), elem, this->rel_id_ - 1);

          this->insert_element (elem);
          be_global->emit_diagnostic (elem);
        }
    }

  if (!node->imported ())
    {
      const XMLCh *new_id = be_global->lookup_id (bt);
      be_global->type_change_diagnostic (elem, new_id);
      elem->setAttribute (X ("referred"), new_id);
      this->add_replace_id_element (elem, node);
      this->add_version_element (elem, node);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }
    }

  // Keep track of where we are in the DOM tree so the next
  // new element can be inserted in the correct position.
  this->previous_ = elem;

  if (0 != result)
    {
      // Store the DOMElement and GME id in their respective tables.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
      be_global->decl_id_table ().bind (ACE::strnew (node->repoID ()),
                                        elem->getAttribute (X ("id")));
    }

  return 0;
}

ACE_CString
adding_visitor::set_id_attr (DOMElement *elem, BE_GlobalData::kind_id kind)
{
  ACE_CString val = be_global->make_gme_id (kind);
  elem->setAttribute (X ("id"), X (val.c_str ()));
  elem->setIdAttribute (X ("id"));
  return val;
}

void
adding_visitor::set_relid_attr (DOMElement *elem)
{
  char *hex_relid = be_global->hex_string (this->rel_id_++);
  elem->setAttribute (X ("relid"), X (hex_relid));
}

void
adding_visitor::set_childrelidcntr_attr (DOMElement *elem,
                                         UTL_Scope *s,
                                         AST_Attribute *a)
{
  unsigned long nkids = this->nmembers_gme (s, a);

  if (s != 0)
    {
      AST_Decl::NodeType nt = ScopeAsDecl (s)->node_type ();
      AST_Decl *d = 0;

      for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_decls);
          !i.is_done ();
          i.next ())
        {
          d = i.item ();

          if (nt == AST_Decl::NT_union)
            {
              // Add 2 (atom & connection) for each label of each union member.
              // Increment for the discriminator is made in nmembers_gme().
              AST_UnionBranch *ub =
                AST_UnionBranch::narrow_from_decl (d);

              nkids += 2 * ub->label_list_length ();
            }
          else if (nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype)
            {
              AST_Field *f = AST_Field::narrow_from_decl (d);

              // One for the private flag and one for the connection.
              if (f != 0 && f->visibility () == AST_Field::vis_PRIVATE)
                {
                  nkids += 2;
                }
            }
          else if (nt == AST_Decl::NT_root || nt == AST_Decl::NT_module)
            {
              if (d->node_type () == AST_Decl::NT_home)
                {
                  // One for the connection to the managed component.
                  ++nkids;
                  AST_Home *h = AST_Home::narrow_from_decl (d);
                  AST_Component *c = h->managed_component ();

                  if (h->defined_in () != c->defined_in ())
                    {
                      // One for the ComponentRef we create if the managed
                      // component is not in the same scope as the home.
                      ++nkids;
                    }
                }
            }
        }
    }

  char *hex_nkids = be_global->hex_string (nkids);
  elem->setAttribute (X ("childrelidcntr"), X (hex_nkids));
}

void
adding_visitor::add_name_element (DOMElement *elem, const char *name)
{
  DOMElement *elem_name = doc_->createElement (X ("name"));
  DOMText *elem_name_val = doc_->createTextNode (X (name));
  elem_name->appendChild (elem_name_val);
  elem->appendChild (elem_name);
}

void
adding_visitor::add_predefined_types (void)
{
  unsigned long npdt = be_global->npredefined ();
  DOMElement *pdt_folder = 0;

  if (0 == be_global->input_xme ())
    {
      pdt_folder = doc_->createElement (X ("folder"));
      this->set_id_attr (pdt_folder, BE_GlobalData::FOLDER);
      pdt_folder->setAttribute (X ("relid"), X ("0x1"));

      pdt_folder->setAttribute (X ("childrelidcntr"),
                                X (be_global->hex_string (npdt)));

      pdt_folder->setAttribute (X ("kind"), X ("PredefinedTypes"));
      this->add_name_element (pdt_folder, "PredefinedTypes");
      be_global->root_folder ()->appendChild (pdt_folder);
    }
  else
    {
      pdt_folder =
        be_global->lookup_by_tag_and_kind (be_global->root_folder (),
                                           "folder",
                                           "PredefinedTypes");
    }

  for (unsigned long i = 0; i < npdt; ++i)
    {
      DOMElement *pdt = 0;
      const char *pdt_name = be_global->pdt_names ()[i];

      if (0 == be_global->input_xme ())
        {
          pdt = doc_->createElement (X ("atom"));
          this->set_id_attr (pdt, BE_GlobalData::ATOM);
          pdt->setAttribute (X ("kind"), X (pdt_name));

          // The relid attributes are numbered in reverse order in all sample
          // files - don't know if it's true for all attached libraries or not.
          char *relid_str = be_global->hex_string (npdt - i);
          pdt->setAttribute (X ("relid"), X (relid_str));
          this->add_name_element (pdt, pdt_name);
          pdt_folder->appendChild (pdt);
        }
      else
        {
          pdt =
            be_global->lookup_by_tag_and_kind (pdt_folder,
                                               "atom",
                                               pdt_name);
        }

      // Store the GME id for possible rererence by other XML elements.
      be_global->decl_id_table ().bind (ACE::strnew (pdt_name),
                                        pdt->getAttribute (X ("id")));
    }
}

void
adding_visitor::add_predefined_sequences (DOMElement *parent,
                                          AST_Root *node)
{
  this->set_n_basic_seqs ();
  unsigned long slot = 1UL;

  if (idl_global->string_seq_seen_
      || idl_global->wstring_seq_seen_)
    {
      this->add_one_predefined_sequence (parent, node,
                                         "String",
                                         slot,
                                         2UL);
    }

  if (idl_global->long_seq_seen_
      || idl_global->ulong_seq_seen_
      || idl_global->longlong_seq_seen_
      || idl_global->ulonglong_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "LongInteger",
                                         slot,
                                         6UL);
    }

  if (idl_global->float_seq_seen_
      || idl_global->double_seq_seen_
      || idl_global->longdouble_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "RealNumber",
                                         slot,
                                         5UL);
    }

  if (idl_global->short_seq_seen_
      || idl_global->ushort_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "ShortInteger",
                                         slot,
                                         4UL);
    }

  if (idl_global->char_seq_seen_
      || idl_global->wchar_seq_seen_
      || idl_global->octet_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "Byte",
                                         slot,
                                         10UL);
    }

  if (idl_global->boolean_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "Boolean",
                                         slot,
                                         3UL);
    }

  if (idl_global->any_seq_seen_)
    {
      this->add_one_predefined_sequence (parent,
                                         node,
                                         "GenericValue",
                                         slot,
                                         8UL);
    }
}

void
adding_visitor::add_one_predefined_sequence (DOMElement *parent,
                                             AST_Root *node,
                                             const char *type,
                                             unsigned long &model_slot,
                                             unsigned long pdt_slot)
{
  // Can't create a basic type sequence more than once in a project.
  // A processed basic type sequence is stored by its GME id.
  const XMLCh *gme_id = 0;
  ACE_CString name (type);
  name += be_global->basic_seq_suffix ();
  int result = be_global->decl_id_table ().find (name.c_str (), gme_id);

  if (0 == result)
    {
      return;
    }

  DOMElement *seq = 0;
  result = be_global->decl_elem_table ().find (name.c_str (), seq);

  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "adding_visitor::add_one_predefined_sequence - "
                  "lookup of sequence DOCElement %s failed\n",
                  name.c_str ()));
      return;
    }

  this->set_id_attr (seq, BE_GlobalData::REF);
  seq->setAttribute (X ("relid"), X (be_global->hex_string (model_slot)));
  seq->setAttribute (X ("kind"), X ("Collection"));
  seq->setAttribute (X ("role"), X ("Collection"));

  const char **pdts = be_global->pdt_names ();
  const XMLCh *pdt_id = 0;
  result = be_global->decl_id_table ().find (pdts[pdt_slot], pdt_id);

  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "adding_visitor::add_one_predefined_sequence - "
                  "lookup of base type id %s failed\n",
                  type));
      return;
    }

  seq->setAttribute (X ("referred"), pdt_id);

  this->add_name_element (seq, name.c_str ());
  this->add_regnodes (node, seq, model_slot);
  ++model_slot;

  be_global->decl_id_table ().bind (ACE::strnew (name.c_str ()),
                                                 seq->getAttribute (X ("id")));

  parent->appendChild (seq);
}

DOMElement *
adding_visitor::add_file_element (DOMElement *parent,
                                  AST_Root *node,
                                  unsigned long rel_id)
{
  ACE_CString tmp (idl_global->stripped_filename ()->get_string ());
  tmp = tmp.substr (0, tmp.rfind ('.'));
  const char *tmp_cstr = tmp.c_str ();
  int result = 0;

  // See if we have already imported this file. If so, just return it.
  DOMElement *file = be_global->imported_dom_element (parent,
                                                      tmp_cstr);

  if (0 == file)
    {
      result = be_global->decl_elem_table ().find (tmp_cstr, file);

      // All files should have been stored as part of BE_init(). If
      // this lookup fails, we will crash if we continue.
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "file %s not found in decl table\n",
                      tmp_cstr));

          exit (99);
        }

      char *hex_relid = be_global->hex_string (rel_id);
      file->setAttribute (X ("relid"), X (hex_relid));
      file->setAttribute (X ("kind"), X ("File"));

      const XMLCh *file_id = 0;
      result = be_global->decl_id_table ().find (tmp_cstr, file_id);

      // All files should have been stored as part of BE_init(). If
      // this lookup fails, we will crash if we continue.
      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: file %s id not found in id table\n",
                      tmp_cstr));

          exit (99);
        }

      file->setAttribute (X ("id"), file_id);
      this->add_name_element (file, tmp_cstr);
      this->add_prefix_element (file, node);

      // This has to come before add_include_elements, because it adjusts
      // the number of members, but only for the first file processed.
      this->add_predefined_sequences (file, node);

      // This has to come after add_predefined_sequences, to get the possibly
      // updated number of members.
      this->set_childrelidcntr_attr (file, node);

      parent->appendChild (file);
      be_global->emit_diagnostic (file);
    }

  // This first checks if an include element is imported from XML.
  this->add_include_elements (node, file);

  // Update the global state.
  be_global->current_idl_file (file);

  // Add to list used in check for removed IDL decls.
  result =
    be_global->gme_id_set ().insert (file->getAttribute (X ("id")));

  return file;
}

void
adding_visitor::add_prefix_element (DOMElement *parent, AST_Decl *node)
{
  const char *prefix = node->prefix ();

  if (node->typeid_set ())
    {
      // SpecifyIdTag will handle it.
      prefix = "";
    }
  else
    {
      UTL_Scope *s = node->defined_in ();

      // If our prefix was set in an outer scope (or not set at all), we
      // should set it to the empty string here.
      if (s != 0)
        {
          if (0 == ACE_OS::strcmp (prefix, ScopeAsDecl (s)->prefix ()))
            {
              prefix = "";
            }
        }
    }

  this->add_tag_common (prefix, "PrefixTag", parent);
}

void
adding_visitor::add_replace_id_element (DOMElement *parent, AST_Decl *node)
{
  const char *id = 0;

  if (0 == node || !node->typeid_set ())
    {
      id = "";
    }
  else
    {
      id = node->prefix ();
    }

  this->add_tag_common (id, "SpecifyIdTag", parent);
}

void
adding_visitor::add_version_element (DOMElement *parent, AST_Decl *node)
{
  const char *version = 0;

  if (0 == node)
    {
      version = "";
    }
  else
    {
      version = node->version ();

      // If it's the default version, set the attribute to the empty string.
      if (0 == ACE_OS::strcmp (version, "1.0"))
        {
          version = "";
        }
   }

  this->add_tag_common (version, "VersionTag", parent);
}

void
adding_visitor::add_tag_common (const char *value,
                                const char *name,
                                DOMElement *parent,
                                bool is_meta)
{
  DOMElement *tag_elem =
    be_global->lookup_by_tag_and_kind (parent, "attribute", name);
  DOMElement *value_elem = 0;
  DOMText *old_tag_value = 0;
  DOMText *new_tag_value = 0;

  // If we are importing XME, tag_elem will always exist,
  // otherwise not.
  if (0 == tag_elem)
    {
      tag_elem = this->doc_->createElement (X ("attribute"));
      tag_elem->setAttribute (X ("kind"), X (name));

      if (is_meta)
        {
          tag_elem->setAttribute (X ("status"), X ("meta"));
        }

      value_elem = this->doc_->createElement (X ("value"));
      new_tag_value = this->doc_->createTextNode (X (value));

      value_elem->appendChild (new_tag_value);
      tag_elem->appendChild (value_elem);
      parent->appendChild (tag_elem);
    }
  else
    {
      // Just replace whether the value has changed or not.
      value_elem = (DOMElement *) tag_elem->getFirstChild ();
      old_tag_value = (DOMText *) value_elem->getFirstChild ();
      new_tag_value = this->doc_->createTextNode (X (value));

      // For some reason, we must set the parser feature that
      // tells it to leave out text nodes with just whitespace.
      // Otherwise there is a crash at init time. So we check
      // for a non-existent text node and replace or append.
      if (0 == old_tag_value)
        {
          value_elem->appendChild (new_tag_value);
        }
      else
        {
          value_elem->replaceChild (new_tag_value, old_tag_value);
        }

      be_global->emit_attribute_diagnostic (parent,
                                            name,
                                            value,
                                            old_tag_value);
    }
}

void
adding_visitor::add_local_element (DOMElement *parent, AST_Decl *node)
{
  const char *value = (node->is_local () ? "true" : "false");
  this->add_tag_common (value, "local", parent);
}

void
adding_visitor::add_abstract_element (DOMElement *parent, AST_Decl *node)
{
  const char *value = (node->is_abstract () ? "true" : "false");
  this->add_tag_common (value, "abstract", parent);
}

void
adding_visitor::add_include_elements (UTL_Scope *container, DOMElement *parent)
{
  idl_global->validate_included_idl_files ();
  unsigned long slot = this->n_basic_seqs_ + 1UL;

  for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      ACE_CString fname (idl_global->included_idl_files ()[i]);
      int pos = fname.rfind ('/');
      ACE_CString lname =
        (pos == ACE_CString::npos ? fname : fname.substr (pos + 1));

      // Types in these files should never be referenced in application
      // IDL files, so we don't included references to the files in the
      // PICML model.
      if (lname == "Components.idl" || lname == "orb.idl")
        {
          continue;
        }

      // The only types from these files that are referenced directly are
      // the sequences of basic types, which will probably be added to
      // the basic types library in PICML.
      if (lname.find (".pidl") != ACE_CString::npos)
        {
          continue;
        }

      ACE_CString lname_noext = lname.substr (0, lname.rfind ('.'));
      const char *tmp = lname_noext.c_str ();
      const XMLCh *id = 0;

      int result =
        be_global->decl_id_table ().find (tmp, id);

      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error: Filename %s, included in %s, not found "
                      "in id table. %s was omitted from command line "
                      "or spelling differs in case.\n",
                      lname.c_str (),
                      idl_global->filename ()->get_string (),
                      lname.c_str ()));

          exit (99);
        }

      DOMElement *fileref =
        be_global->imported_dom_element (parent,
                                         tmp,
                                         BE_GlobalData::REF,
                                         true);

      if (0 == fileref)
        {
          fileref = this->doc_->createElement (X ("reference"));
          this->set_id_attr (fileref, BE_GlobalData::REF);
          const char *hex_rel_id = be_global->hex_string (slot);
          fileref->setAttribute (X ("relid"), X (hex_rel_id));
          ++this->rel_id_;
          fileref->setAttribute (X ("kind"), X ("FileRef"));
          fileref->setAttribute (X ("role"), X ("FileRef"));
          fileref->setAttribute (X ("referred"), id);
          this->add_name_element (fileref, "FileRef");
          this->add_regnodes (container, fileref, slot++);

          parent->appendChild (fileref);
          be_global->included_file_diagnostic (fileref, parent, tmp);
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              fileref->getAttribute (X ("id"))
            );
        }
    }
}

void
adding_visitor::add_regnodes (UTL_Scope *container,
                              DOMElement *parent,
                              size_t slot,
                              AST_Attribute *a,
                              bool is_connected,
                              const char *aspect,
                              unsigned long num_slices)
{
  bool ifacedef_aspect =
    (0 == ACE_OS::strcmp (aspect, "InterfaceDefinition"));

  DOMElement *outer = this->doc_->createElement (X ("regnode"));
  outer->setAttribute (X ("name"), X ("PartRegs"));

  if (ifacedef_aspect)
    {
      outer->setAttribute (X ("isopaque"), X ("yes"));
    }

  DOMElement *ovalue = this->doc_->createElement (X ("value"));
  DOMText *oval = this->doc_->createTextNode (X (""));
  ovalue->appendChild (oval);
  outer->appendChild (ovalue);

  DOMElement *middle = this->doc_->createElement (X ("regnode"));
  middle->setAttribute (X ("name"), X (aspect));

  if (ifacedef_aspect)
    {
      middle->setAttribute (X ("isopaque"), X ("yes"));
    }

  DOMElement *mvalue = this->doc_->createElement (X ("value"));
  DOMText *mval = this->doc_->createTextNode (X (""));
  mvalue->appendChild (mval);
  middle->appendChild (mvalue);

  DOMElement *inner = this->doc_->createElement (X ("regnode"));
  inner->setAttribute (X ("name"), X ("Position"));
  inner->setAttribute (X ("isopaque"), X ("yes"));

  DOMElement *ivalue = this->doc_->createElement (X ("value"));

  this->add_pos_element (container, ivalue, slot, a, is_connected, num_slices);

  inner->appendChild (ivalue);
  middle->appendChild (inner);

  outer->appendChild (middle);
  parent->appendChild (outer);
}

void
adding_visitor::add_pos_element (UTL_Scope *container,
                                 DOMElement *parent,
                                 size_t slot,
                                 AST_Attribute *a,
                                 bool is_connected,
                                 unsigned long num_slices)
{
  static char holder[9];
  ACE_OS::memset (holder,
                  '\0',
                  9);

  unsigned long nslices =
    (num_slices != 0
      ? num_slices
      : (container != 0
          ? this->nmembers_gme (container, a) + 1UL
          : 2UL));

  unsigned long slice_width = XMAX_ / nslices;
  unsigned long slice_height = YMAX_ / nslices;

  // Icons are positioned according to their upper left corners,
  // so we subtract half the size in each dimention.
  unsigned long xpos = slot * slice_width - ICON_SIZE_ / 2;
  unsigned long ypos = slot * slice_height - ICON_SIZE_ / 2;

  // If we are connected to the first element in the scope, we draw ourselves
  // 200 pixels to the right of the element, otherwise we draw ourselves one
  // slice width to the left.
  if (is_connected)
    {
      if (this->rel_id_ == 2UL)
        {
          xpos += 200;
        }
      else
        {
          xpos -= slice_width;
        }
    }

  ACE_OS::sprintf (holder, "%d,%d", xpos, ypos);
  DOMText *coordinates = this->doc_->createTextNode (X (holder));
  parent->appendChild (coordinates);
}

void
adding_visitor::add_inherited_elements (DOMElement *parent, AST_Interface *node)
{
  for (long i = 0; i < node->n_inherits (); ++i)
    {
      AST_Interface *p = node->inherits ()[i];
      unsigned long slot = static_cast<unsigned long> (i + 1);
      this->add_one_inherited (parent, node, p, slot);
    }
}

void
adding_visitor::add_one_inherited (DOMElement *parent,
                                   AST_Interface *node,
                                   AST_Interface *base,
                                   unsigned long slot)
{
  // First see if it's been imported with an XME file.
  DOMElement *elem =
    be_global->imported_dom_element (
        parent,
        node->local_name ()->get_string (),
        BE_GlobalData::REF,
        true
      );

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("reference"));
      this->set_id_attr (elem, BE_GlobalData::REF);
      elem->setAttribute (X ("kind"), X ("Inherits"));
      elem->setAttribute (X ("role"), X ("Inherits"));
      const char *hex_rel_id = be_global->hex_string (slot);
      elem->setAttribute (X ("relid"), X (hex_rel_id));

      const XMLCh *id = 0;
      int result =
        be_global->decl_id_table ().find (base->repoID (), id);

      if (result != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "adding_visitor::add_one_inherited - "
                      "lookup of parent %s failed\n",
                      base->full_name ()));
        }

      elem->setAttribute (X ("referred"), id);
      this->add_name_element (elem, "Inherits");

      // We'll let the parent interfaces be in the upper left of the
      // Model Editor so we can just pass the index as the slot.
      this->add_regnodes (node, elem, slot);

      parent->appendChild (elem);
      be_global->emit_diagnostic (elem);
    }

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
    }
}

void
adding_visitor::add_supported_elements (DOMElement *parent,
                                        AST_Interface *node,
                                        AST_Interface **supports,
                                        long n_supports)
{
  unsigned long nparents = 0;

  switch (node->node_type ())
    {
      case AST_Decl::NT_interface:
      case AST_Decl::NT_valuetype:
      case AST_Decl::NT_eventtype:
        nparents = static_cast<unsigned long> (node->n_inherits ());
        break;
      case AST_Decl::NT_component:
        {
          AST_Component *c = AST_Component::narrow_from_decl (node);
          nparents = (0 == c->base_component () ? 0 : 1);
          break;
        }
      case AST_Decl::NT_home:
        {
          AST_Home *h = AST_Home::narrow_from_decl (node);
          nparents = (0 == h->base_home () ? 0 : 1);
          break;
        }
      default:
        break;
    }

  for (unsigned long i = 0; i < static_cast<unsigned long> (n_supports); ++i)
    {
      // First see if it's been imported with an XME file.
      DOMElement *supported =
        be_global->imported_dom_element (
            parent,
            supports[i]->local_name ()->get_string (),
            BE_GlobalData::REF,
            true
          );

      if (0 == supported)
        {
          supported = this->doc_->createElement (X ("reference"));
          this->set_id_attr (supported, BE_GlobalData::REF);
          supported->setAttribute (X ("kind"), X ("Supports"));
          supported->setAttribute (X ("role"), X ("Supports"));
          supported->setAttribute (
              X ("relid"),
              X (be_global->hex_string (nparents + i + 1))
            );
          supported->setAttribute (X ("referred"),
                                  be_global->lookup_id (supports[i]));

          this->add_name_element (supported, "Supports");
          this->add_regnodes (node, supported, nparents + i + 1);
          parent->appendChild (supported);
          be_global->emit_diagnostic (supported);
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              supported->getAttribute (X ("id"))
            );
        }
    }
}

void
adding_visitor::add_exception_elements (DOMElement *parent,
                                        UTL_Scope *s,
                                        AST_Attribute *a,
                                        UTL_ExceptList *el,
                                        const char *name,
                                        unsigned long start_slot)
{
  for (UTL_ExceptlistActiveIterator ei (el);
       !ei.is_done ();
       ei.next ())
    {
      AST_Exception *ex = ei.item ();
      const XMLCh *gme_id = be_global->lookup_id (ex);

      // Since the members of an exception list must be unique, we
      // just check for a DOM element in this scope that refers to
      // the exception. If there already is one, we can assume it
      // came from importing a previous version of the IDL file.
      DOMElement *elem =
        be_global->imported_dom_element (parent,
                                         ex->local_name ()->get_string (),
                                         BE_GlobalData::REF,
                                         true);

      if (elem == 0)
        {
          elem = this->doc_->createElement (X ("reference"));
          this->set_id_attr (elem, BE_GlobalData::REF);
          elem->setAttribute (X ("kind"), X (name));
          elem->setAttribute (X ("role"), X (name));
          elem->setAttribute (X ("relid"),
                              X (be_global->hex_string (start_slot)));
          elem->setAttribute (X ("referred"), gme_id);
          this->add_name_element (elem, name);
          this->add_regnodes (s, elem, start_slot++, a);
          parent->appendChild (elem);
          be_global->emit_diagnostic (elem);
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
        }

      char *id = XMLString::transcode (gme_id);
      int result =
        be_global->ref_decl_table ().bind (id, ex);

      // Has a reference to this exception already been added?
      if (0 != result)
        {
          // If so, the table takes doesn't over the memory allocated by
          // transcode(), so we must cleaned it up.
          XMLString::release (&id);
        }
    }
}

void
adding_visitor::add_constant_value (DOMElement *parent, AST_Constant *c)
{
  AST_Expression *cv = c->constant_value ();
  ACE_CString value = (c->et () == AST_Expression::EV_enum
                       ? this->print_scoped_name (cv->n ())
                       : this->expr_val_to_string (cv->ev ()));
  this->add_tag_common (value.c_str (), "value", parent, false);
}

void
adding_visitor::add_discriminator (DOMElement *parent, AST_Union *u)
{
  DOMElement *elem =
    be_global->imported_dom_element (parent,
                                     "Discriminator",
                                     BE_GlobalData::REF);

  if (0 == elem)
    {
      elem = this->doc_->createElement (X ("reference"));
      this->set_id_attr (elem, BE_GlobalData::REF);
      elem->setAttribute (X ("kind"), X ("Discriminator"));
      elem->setAttribute (X ("role"), X ("Discriminator"));
      elem->setAttribute (X ("relid"),
                          X (be_global->hex_string (1UL)));
      this->add_name_element (elem, "Discriminator");
      this->add_regnodes (u, elem, 1UL);

      parent->appendChild (elem);
    }

  const XMLCh *dtype = be_global->lookup_id (u->disc_type ());
  be_global->type_change_diagnostic (elem, dtype);
  elem->setAttribute (X ("referred"), dtype);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (elem->getAttribute (X ("id")));
    }
}

void
adding_visitor::add_labels (AST_UnionBranch *ub,
                            const XMLCh *ub_id,
                            unsigned long member_slot)
{
  UTL_Scope *u = ub->defined_in ();
  unsigned long label_base = this->nmembers_gme (u) + member_slot - 1;
  DOMElement *connection = 0;

  for (unsigned long i = 0; i < ub->label_list_length (); ++i)
    {
      ACE_CString label_name = this->get_label_name (ub->label (i), u);

      DOMElement *label =
        be_global->imported_dom_element (this->sub_tree_,
                                         label_name.c_str (),
                                         BE_GlobalData::ATOM);

      if (0 == label)
        {
          label = this->doc_->createElement (X ("atom"));
          ACE_CString label_id =
            this->set_id_attr (label, BE_GlobalData::ATOM);
          label->setAttribute (X ("kind"), X ("Label"));
          label->setAttribute (X ("role"), X ("Label"));
          label->setAttribute (
              X ("relid"),
              X (be_global->hex_string (label_base + 2 * i))
            );
          this->add_name_element (label, label_name.c_str ());
          this->add_regnodes (u, label, member_slot, 0, true);
          this->sub_tree_->appendChild (label);
          be_global->emit_diagnostic (label);

          connection =
            this->doc_->createElement (X ("connection"));
          this->set_id_attr (connection, BE_GlobalData::CONN);
          connection->setAttribute (X ("kind"), X ("LabelConnection"));
          connection->setAttribute (X ("role"), X ("LabelConnection"));
          char *hex_id =
            be_global->hex_string (label_base + 2 * i + 1);
          connection->setAttribute(X ("relid"), X (hex_id));
          this->add_name_element (connection, "LabelConnection");

          DOMElement *conn_reg =
            this->doc_->createElement (X ("regnode"));
          conn_reg->setAttribute (X ("name"), X ("autorouterPref"));
          conn_reg->setAttribute (X ("isopaque"), X ("yes"));
          DOMElement *conn_value =
            this->doc_->createElement (X ("value"));
          DOMText *val = this->doc_->createTextNode (X ("We"));
          conn_value->appendChild (val);
          conn_reg->appendChild (conn_value);
          connection->appendChild (conn_reg);

          DOMElement *dst = this->doc_->createElement (X ("connpoint"));
          dst->setAttribute (X ("role"), X ("dst"));
          dst->setAttribute (X ("target"), X (label_id.c_str ()));
          connection->appendChild (dst);

          DOMElement *src = this->doc_->createElement (X ("connpoint"));
          src->setAttribute (X ("role"), X ("src"));
          src->setAttribute (X ("target"), X (ub_id));
          connection->appendChild (src);

          this->sub_tree_->appendChild (connection);
          be_global->emit_diagnostic (connection);
        }
      else
        {
          connection =
            this->find_connection (label->getAttribute (X ("id")), "dst");
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              label->getAttribute (X ("id"))
            );

          be_global->gme_id_set ().insert (
              connection->getAttribute (X ("id"))
            );
        }
    }
}

ACE_CString
adding_visitor::get_label_name (AST_UnionLabel *ul,
                                UTL_Scope *s)
{
  ACE_CString name;
  AST_Expression *e = ul->label_val ();
  AST_Union *u = AST_Union::narrow_from_scope (s);

  if (ul->label_kind () == AST_UnionLabel::UL_default)
    {
      name = "default";
    }
  else
    {
      if (u->disc_type ()->node_type () == AST_Decl::NT_enum)
        {
          UTL_Scope *parent = u->disc_type ()->defined_in ();

          if (0 == parent)
            {
              name = this->print_scoped_name (e->n ());
            }
          else
            {
              if (ScopeAsDecl (parent)->node_type () != AST_Decl::NT_root)
                {
                  name =
                    this->print_scoped_name (ScopeAsDecl (parent)->name ());
                  name += "::";
                }

              name += e->n ()->last_component ()->get_string ();
            }
        }
      else
        {
          name = this->expr_val_to_string (ul->label_val ()->ev ());
        }
    }

  return name;
}

void
adding_visitor::add_private (AST_Field *f,
                             const XMLCh *member_id,
                             unsigned long)
{
  // We can't use imported_dom_element() here because PrivateFlag
  // atoms have no unique name.
  DOMElement *connection = this->find_connection (member_id, "src");

  // If the private flag is already there, just return.
  if (0 != connection)
    {
      // No need to check be_global->input_xme(), if we have got
      // to this point.
      be_global->gme_id_set ().insert (
          connection->getAttribute (X ("id"))
        );

      DOMNodeList *endpoints =
        connection->getElementsByTagName (X ("connpoint"));

      for (XMLSize_t index = 0; index < endpoints->getLength (); ++index)
        {
          DOMElement *endpoint = (DOMElement *) endpoints->item (index);
          const XMLCh *role = endpoint->getAttribute (X ("role"));
          const XMLCh *target = endpoint->getAttribute (X ("target"));

          if (X ("dst") == role)
            {
              // Have to go this circuitous route because the set stores
              // addresses, not values.
              DOMElement *private_flag =
                this->doc_->getElementById (target);
              const XMLCh *id = private_flag->getAttribute (X ("id"));
              be_global->gme_id_set ().insert (id);
              break;
            }
        }

      return;
    }

  unsigned long base = this->nmembers_gme (f->defined_in ())
                       + this->private_relid_offset_
                       + 1;

  DOMElement *pflag = this->doc_->createElement (X ("atom"));
  ACE_CString flag_id = this->set_id_attr (pflag, BE_GlobalData::ATOM);
  pflag->setAttribute (X ("kind"), X ("PrivateFlag"));
  pflag->setAttribute (X ("role"), X ("PrivateFlag"));
  pflag->setAttribute (X ("relid"),
                       X (be_global->hex_string (base++)));
  ++this->private_relid_offset_;
  this->add_name_element (pflag, "PrivateFlag");
  this->add_regnodes (f->defined_in (), pflag, this->rel_id_ - 1, 0, true);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (
          pflag->getAttribute (X ("id"))
        );
    }

  connection = this->doc_->createElement (X ("connection"));
  this->set_id_attr (connection, BE_GlobalData::CONN);
  connection->setAttribute (X ("kind"), X ("MakeMemberPrivate"));
  connection->setAttribute (X ("role"), X ("MakeMemberPrivate"));
  connection->setAttribute (X ("relid"),
                            X (be_global->hex_string (base)));
  ++this->private_relid_offset_;
  this->add_name_element (connection, "MakeMemberPrivate");

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (
          connection->getAttribute (X ("id"))
        );
    }

  DOMElement *conn_reg = this->doc_->createElement (X ("regnode"));
  conn_reg->setAttribute (X ("name"), X ("autorouterPref"));
  conn_reg->setAttribute (X ("isopaque"), X ("yes"));
  DOMElement *conn_value = this->doc_->createElement (X ("value"));
  DOMText *val = this->doc_->createTextNode (X ("Ew"));
  conn_value->appendChild (val);
  conn_reg->appendChild (conn_value);
  connection->appendChild (conn_reg);

  DOMElement *dst = this->doc_->createElement (X ("connpoint"));
  dst->setAttribute (X ("role"), X ("dst"));
  dst->setAttribute (X ("target"), X (flag_id.c_str ()));
  connection->appendChild (dst);

  DOMElement *src = this->doc_->createElement (X ("connpoint"));
  src->setAttribute (X ("role"), X ("src"));
  src->setAttribute (X ("target"), X (member_id));
  connection->appendChild (src);

  this->sub_tree_->appendChild (pflag);
  this->sub_tree_->appendChild (connection);
}

DOMElement *
adding_visitor::find_connection (const XMLCh *endpoint_id,
                                 const char *role)
{
  // We will be here only via a call from inside an AST_ValueType,
  // so the only connections in this scope will be MakeMemberPrivate.
  DOMElement *connection = 0;
  DOMNodeList *connections =
    this->sub_tree_->getElementsByTagName (X ("connection"));

  for (XMLSize_t index = 0; index < connections->getLength (); ++ index)
    {
      connection = (DOMElement *) connections->item (index);

      DOMNodeList *endpoints =
        connection->getElementsByTagName (X ("connpoint"));

      for (XMLSize_t i = 0; i < endpoints->getLength (); ++i)
        {
          DOMElement *endpoint = (DOMElement *) endpoints->item (i);
          const XMLCh *target_attr = endpoint->getAttribute (X ("target"));
          const XMLCh *role_attr = endpoint->getAttribute (X ("role"));

          if (X (role) == role_attr && X (endpoint_id) == target_attr)
            {
              return connection;
            }
        }
    }

  return 0;
}

void
adding_visitor::add_base_component (DOMElement *elem, AST_Component *node)
{
  // Diagnostic messages, if any, are handled right here rather than by
  // one of the existing methods because component inheritance is
  // represented in PICML differently than inheritance for other types.

  AST_Component *base = node->base_component ();
  bool was_derived = elem->hasAttribute (X ("derivedfrom"));

  if (0 == base)
    {
      if (was_derived)
        {
          be_global->base_component_diagnostic (elem,
                                                node,
                                                base,
                                                was_derived);
          elem->removeAttribute (X ("derivedfrom"));
          elem->removeAttribute (X ("isinstance"));
          elem->removeAttribute (X ("isprimary"));
        }

      return;
    }

  const XMLCh *base_id_from_idl = 0;
  int result =
    be_global->decl_id_table ().find (base->repoID (), base_id_from_idl);

  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "adding_visitor::add_base_component - "
                  "lookup of parent %s failed\n",
                  base->full_name ()));
    }

  be_global->base_component_diagnostic (elem,
                                        node,
                                        base,
                                        was_derived,
                                        base_id_from_idl);

  // XML generated from GME models has these attributes also for
  // derived ports, but it seems we don't need all that for
  // correct importing, so we just generate the attributes for
  // the derived component.
  elem->setAttribute (X ("derivedfrom"), base_id_from_idl);
  elem->setAttribute (X ("isinstance"), X ("no"));
  elem->setAttribute (X ("isprimary"), X ("yes"));
}

void
adding_visitor::add_base_home (DOMElement *parent, AST_Home *node)
{
  AST_Home *base = node->base_home ();
  if (0 == base)
    {
      return;
    }

  this->add_one_inherited (parent, node, base, 1UL);
}

void
adding_visitor::add_ports (DOMElement *parent, AST_Component *node)
{
  AST_Component::port_description *pd = 0;
  unsigned long slot = (0 == node->base_component () ? 0 : 1)
                       + node->n_supports ()
                       + 1;

  // Provides ports.
  for (PORT_ITER provides_iter (node->provides ());
       ! provides_iter.done ();
       provides_iter.advance ())
    {
      provides_iter.next (pd);
      DOMElement *provides_port =
        be_global->imported_dom_element (parent,
                                         pd->id->get_string (),
                                         BE_GlobalData::REF);

      if (0 == provides_port)
        {
          provides_port =
            this->doc_->createElement (X ("reference"));
          this->set_id_attr (provides_port, BE_GlobalData::REF);
          provides_port->setAttribute (X ("kind"),
                                       X ("ProvidedRequestPort"));
          provides_port->setAttribute (X ("role"),
                                       X ("ProvidedRequestPort"));
          provides_port->setAttribute (X ("relid"),
                                       X (be_global->hex_string (slot)));
          this->add_name_element (provides_port, pd->id->get_string ());
          this->add_regnodes (node, provides_port, slot++);

          // Since we aren't adding port elements by visiting the scope of
          // the AST_Component, using insert_element won't work here. The
          // order of port declarations in IDL shouldn't matter anyway.
          parent->appendChild (provides_port);
          be_global->emit_diagnostic (provides_port);
        }

      // These emit diagnostics if changed, idempotent otherwise.
      const XMLCh *referred = be_global->lookup_id (pd->impl);
      be_global->type_change_diagnostic (provides_port, referred);
      provides_port->setAttribute (X ("referred"), referred);
      this->add_replace_id_element (provides_port, 0);
      this->add_version_element (provides_port, 0);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              provides_port->getAttribute (X ("id"))
            );
        }
    }

  // Uses ports.
  for (PORT_ITER uses_iter (node->uses ());
       ! uses_iter.done ();
       uses_iter.advance ())
    {
      DOMElement *mult_attr = 0;
      uses_iter.next (pd);
      DOMElement *uses_port =
        be_global->imported_dom_element (parent,
                                         pd->id->get_string (),
                                         BE_GlobalData::REF);

      if (0 == uses_port)
        {
          uses_port =
            this->doc_->createElement (X ("reference"));
          this->set_id_attr (uses_port, BE_GlobalData::REF);
          uses_port->setAttribute (X ("kind"), X ("RequiredRequestPort"));
          uses_port->setAttribute (X ("role"), X ("RequiredRequestPort"));
          uses_port->setAttribute (X ("relid"),
                                  X (be_global->hex_string (slot)));
          this->add_name_element (uses_port, pd->id->get_string ());
          this->add_regnodes (node, uses_port, slot++);

          // Since we aren't adding port elements by visiting the scope of
          // the AST_Component, using insert_element won't work here. The
          // order of port declarations in IDL shouldn't matter anyway.
          parent->appendChild (uses_port);
        }

      // These emit diagnostics if changed, idempotent otherwise.
      const XMLCh *referred = be_global->lookup_id (pd->impl);
      be_global->type_change_diagnostic (uses_port, referred);
      uses_port->setAttribute (X ("referred"), referred);
      this->add_replace_id_element (uses_port, 0);
      this->add_version_element (uses_port, 0);
      this->add_tag_common (pd->is_multiple ? "true" : "false",
                            "multiple_connections",
                            uses_port,
                            false);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              uses_port->getAttribute (X ("id"))
            );
        }
    }

  // Emits ports.
  for (PORT_ITER emits_iter (node->emits ());
       ! emits_iter.done ();
       emits_iter.advance ())
    {
      emits_iter.next (pd);
      DOMElement *emits_port =
        be_global->imported_dom_element (parent,
                                         pd->id->get_string (),
                                         BE_GlobalData::REF);

      if (0 == emits_port)
        {
          emits_port =
            this->doc_->createElement (X ("reference"));
          this->set_id_attr (emits_port, BE_GlobalData::REF);
          emits_port->setAttribute (X ("kind"), X ("OutEventPort"));
          emits_port->setAttribute (X ("role"), X ("OutEventPort"));
          emits_port->setAttribute (X ("relid"),
                                    X (be_global->hex_string (slot)));
          this->add_name_element (emits_port, pd->id->get_string ());
          this->add_regnodes (node, emits_port, slot++);

          // Since we aren't adding port elements by visiting the scope of
          // the AST_Component, using insert_element won't work here. The
          // order of port declarations in IDL shouldn't matter anyway.
          parent->appendChild (emits_port);
        }

      // These emit diagnostics if changed, idempotent otherwise.
      const XMLCh *referred = be_global->lookup_id (pd->impl);
      be_global->type_change_diagnostic (emits_port, referred);
      emits_port->setAttribute (X ("referred"), referred);
      this->add_replace_id_element (emits_port, 0);
      this->add_version_element (emits_port, 0);
      this->add_tag_common ("true",
                            "single_destination",
                            emits_port,
                            false);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              emits_port->getAttribute (X ("id"))
            );
        }
    }

  // Publishes ports.
  for (PORT_ITER publishes_iter (node->publishes ());
       ! publishes_iter.done ();
       publishes_iter.advance ())
    {
      publishes_iter.next (pd);
      DOMElement *publishes_port =
        be_global->imported_dom_element (parent,
                                         pd->id->get_string (),
                                         BE_GlobalData::REF);

      if (0 == publishes_port)
        {
          publishes_port =
            this->doc_->createElement (X ("reference"));
          this->set_id_attr (publishes_port, BE_GlobalData::REF);
          publishes_port->setAttribute (X ("kind"), X ("OutEventPort"));
          publishes_port->setAttribute (X ("role"), X ("OutEventPort"));
          publishes_port->setAttribute (X ("relid"),
                                        X (be_global->hex_string (slot)));
          this->add_name_element (publishes_port, pd->id->get_string ());
          this->add_regnodes (node, publishes_port, slot++);

          // Since we aren't adding port elements by visiting the scope of
          // the AST_Component, using insert_element won't work here. The
          // order of port declarations in IDL shouldn't matter anyway.
          parent->appendChild (publishes_port);
        }

      // These emit diagnostics if changed, idempotent otherwise.
      const XMLCh *referred = be_global->lookup_id (pd->impl);
      be_global->type_change_diagnostic (publishes_port, referred);
      publishes_port->setAttribute (X ("referred"), referred);
      this->add_replace_id_element (publishes_port, 0);
      this->add_version_element (publishes_port, 0);
      this->add_tag_common ("false",
                            "single_destination",
                            publishes_port,
                            false);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              publishes_port->getAttribute (X ("id"))
            );
        }
    }

  // Consumes ports.
  for (PORT_ITER consumes_iter (node->consumes ());
       ! consumes_iter.done ();
       consumes_iter.advance ())
    {
      consumes_iter.next (pd);
      DOMElement *consumes_port =
        be_global->imported_dom_element (parent,
                                         pd->id->get_string (),
                                         BE_GlobalData::REF);

      if (0 == consumes_port)
        {
          consumes_port =
            this->doc_->createElement (X ("reference"));
          this->set_id_attr (consumes_port, BE_GlobalData::REF);
          consumes_port->setAttribute (X ("kind"), X ("InEventPort"));
          consumes_port->setAttribute (X ("role"), X ("InEventPort"));
          consumes_port->setAttribute (X ("relid"),
                                      X (be_global->hex_string (slot)));
          this->add_name_element (consumes_port, pd->id->get_string ());
          this->add_regnodes (node, consumes_port, slot++);

          // Since we aren't adding port elements by visiting the scope of
          // the AST_Component, using insert_element won't work here. The
          // order of port declarations in IDL shouldn't matter anyway.
          parent->appendChild (consumes_port);
        }

      // These emit diagnostics if changed, idempotent otherwise.
      const XMLCh *referred = be_global->lookup_id (pd->impl);
      be_global->type_change_diagnostic (consumes_port, referred);
      consumes_port->setAttribute (X ("referred"), referred);
      this->add_replace_id_element (consumes_port, 0);
      this->add_version_element (consumes_port, 0);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              consumes_port->getAttribute (X ("id"))
            );
        }
    }
}

void
adding_visitor::add_manages (AST_Home *node)
{
  DOMElement *connection = 0;
  DOMNodeList *connections =
    this->sub_tree_->getElementsByTagName (X ("connection"));

  for (XMLSize_t index = 0; index < connections->getLength (); ++index)
    {
      DOMElement *holder = (DOMElement *) connections->item (index);

      if (0 == holder)
        {
          continue;
        }

      // This depends on the "src" connpoint being added last.
      DOMElement *src = (DOMElement *) holder->getLastChild ();
      const XMLCh *target = src->getAttribute (X ("target"));

      // A ComponentFactory has exactly one connection.
      if (X (target) == be_global->lookup_id (node))
        {
          connection = holder;
          break;
        }
    }

  if (0 == connection)
    {
      UTL_Scope *s = node->defined_in ();
      unsigned long base = this->nmembers_gme (s)
                          + this->manages_relid_offset_
                          + 1;

      connection = this->doc_->createElement (X ("connection"));
      this->set_id_attr (connection, BE_GlobalData::CONN);
      connection->setAttribute (X ("kind"), X ("ManagesComponent"));
      connection->setAttribute (X ("role"), X ("ManagesComponent"));
      connection->setAttribute (X ("relid"),
                                X (be_global->hex_string (base++)));
      ++this->manages_relid_offset_;
      this->add_name_element (connection, "ManagesComponent");

      DOMElement *conn_reg = this->doc_->createElement (X ("regnode"));
      conn_reg->setAttribute (X ("name"), X ("autorouterPref"));
      conn_reg->setAttribute (X ("isopaque"), X ("yes"));
      DOMElement *conn_value = this->doc_->createElement (X ("value"));
      DOMText *val = this->doc_->createTextNode (X ("Ws"));
      conn_value->appendChild (val);
      conn_reg->appendChild (conn_value);
      connection->appendChild (conn_reg);

      AST_Component *c = node->managed_component ();
      const XMLCh *comp_id = be_global->lookup_id (c);
      bool same_scope = (c->defined_in () == s);
      ACE_CString comp_ref_id;

      if (!same_scope)
        {
          // Create a ComponentRef node and make that the dst end of the
          // ManagesComponent connection.
          DOMElement *comp_ref = this->doc_->createElement (X ("reference"));
          comp_ref_id = this->set_id_attr (comp_ref, BE_GlobalData::REF);

          // Add to list used in check for removed IDL decls.
          if (be_global->input_xme () != 0)
            {
              be_global->gme_id_set ().insert (
                  comp_ref->getAttribute (X ("id"))
                );
            }

          comp_ref->setAttribute (X ("kind"), X ("ComponentRef"));
          comp_ref->setAttribute (X ("role"), X ("ComponentRef"));
          comp_ref->setAttribute (X ("relid"),
                                  X (be_global->hex_string (base)));
          ++this->manages_relid_offset_;
          comp_ref->setAttribute (X ("referred"), comp_id);
          this->add_name_element (comp_ref, "ComponentRef");
          this->add_regnodes (s, comp_ref, this->rel_id_ - 1, 0, true);
          this->sub_tree_->appendChild (comp_ref);
        }

      DOMElement *dst = this->doc_->createElement (X ("connpoint"));
      dst->setAttribute (X ("role"), X ("dst"));
      dst->setAttribute (X ("target"),
                        same_scope ? X( comp_id) : X (comp_ref_id.c_str ()));
      connection->appendChild (dst);

      // Code above depends on this node being added last.
      DOMElement *src = this->doc_->createElement (X ("connpoint"));
      src->setAttribute (X ("role"), X ("src"));
      src->setAttribute (X ("target"), be_global->lookup_id (node));
      connection->appendChild (src);

      this->sub_tree_->appendChild (connection);
    }

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (
          connection->getAttribute (X ("id"))
        );
    }
}

void
adding_visitor::add_lookup_key (DOMElement *parent, AST_Home *node)
{
  AST_ValueType *pk = node->primary_key ();

  if (0 == pk)
    {
      return;
    }

  unsigned long slot = (0 == node->base_home () ? 0UL : 1UL)
                       + static_cast<unsigned long> (node->n_supports ())
                       + 1;

  DOMElement *lookup_key = 0;
  DOMNodeList *children = parent->getChildNodes ();

  for (XMLSize_t i = 0; i < children->getLength (); ++i)
    {
      DOMElement *child = (DOMElement *) children->item (i);

      if (0 == child)
        {
          continue;
        }

      // There should be only one "name" node.
      DOMNode *name_elem = child->getFirstChild ();
      DOMNode *name_node = name_elem->getFirstChild ();
      DOMText *name_text = (DOMText *) name_node;
      const XMLCh *name = name_text->getData ();

      if (X ("LookupKey") = name)
        {
          lookup_key = child;
          break;
        }
    }

  if (0 == lookup_key)
    {
      lookup_key = this->doc_->createElement (X ("reference"));
      this->set_id_attr (lookup_key, BE_GlobalData::REF);
      lookup_key->setAttribute (X ("kind"), X ("LookupKey"));
      lookup_key->setAttribute (X ("role"), X ("LookupKey"));
      lookup_key->setAttribute (X ("relid"),
                                X (be_global->hex_string (slot)));
      this->add_name_element (lookup_key, "LookupKey");
      this->add_regnodes (node, lookup_key, slot);
      parent->appendChild (lookup_key);
      be_global->emit_diagnostic (lookup_key);
    }

  // Just in case the primary key is now a different valuetype.
  const XMLCh *pk_id = be_global->lookup_id (pk);
  be_global->type_change_diagnostic (lookup_key, pk_id);
  lookup_key->setAttribute (X ("referred"), pk_id);

  // Add to list used in check for removed IDL decls.
  if (be_global->input_xme () != 0)
    {
      be_global->gme_id_set ().insert (
          lookup_key->getAttribute (X ("id"))
        );
    }
}

void
adding_visitor::add_home_factories (DOMElement *parent, AST_Home *node)
{
  AST_Operation **op = 0;
  unsigned long slot = (0 == node->base_home () ? 0UL : 1UL)
                       + static_cast<unsigned long> (node->n_supports ())
                       + (0 == node->primary_key () ? 0UL : 1UL)
                       + 1;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->factories ());
       !i.done ();
       i.advance ())
    {
      i.next (op);
      DOMElement *factory =
        be_global->imported_dom_element (parent,
                                         (*op)->local_name ()->get_string ());

      if (0 == factory)
        {
          factory = this->doc_->createElement (X ("model"));
          this->set_id_attr (factory, BE_GlobalData::MODEL);
          this->set_childrelidcntr_attr (factory, *op);
          factory->setAttribute (X ("relid"),
                                 X (be_global->hex_string (slot)));
          factory->setAttribute (X ("kind"), X ("FactoryOperation"));
          factory->setAttribute (X ("role"), X ("FactoryOperation"));
          this->add_name_element (factory,
                                  (*op)->local_name ()->get_string ());
          this->add_regnodes (node, factory, slot++);
          parent->appendChild (factory);
          be_global->emit_diagnostic (factory);
        }

      // Emits diagnostic if changed, idempotent otherwise.
      this->add_replace_id_element (factory, *op);
      this->add_version_element (factory, *op);

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              factory->getAttribute (X ("id"))
            );
        }

      adding_visitor scope_visitor (factory);
      if (scope_visitor.visit_scope (*op) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "adding_visitor::add_home_factories - "
                      "code generation for scope failed\n"));
        }

      this->add_exception_elements (factory,
                                    *op,
                                    0,
                                    (*op)->exceptions (),
                                    "ExceptionRef",
                                    scope_visitor.rel_id_);
    }
}

void
adding_visitor::add_finders (DOMElement *parent, AST_Home *node)
{
  AST_Operation **op = 0;
  unsigned long slot = (0 == node->base_home () ? 0UL : 1UL)
                       + static_cast<unsigned long> (node->n_supports ())
                       + (0 == node->primary_key () ? 0UL : 1UL)
                       + node->factories ().size ()
                       + 1;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->finders ());
       !i.done ();
       i.advance ())
    {
      i.next (op);
      DOMElement *finder =
        be_global->imported_dom_element (parent,
                                         (*op)->local_name ()->get_string ());

      if (0 == finder)
        {
          finder = this->doc_->createElement (X ("model"));
          this->set_id_attr (finder, BE_GlobalData::MODEL);
          this->set_childrelidcntr_attr (finder, *op);
          finder->setAttribute (X ("relid"),
                                X (be_global->hex_string (slot)));
          finder->setAttribute (X ("kind"), X ("LookupOperation"));
          finder->setAttribute (X ("role"), X ("LookupOperation"));
          this->add_name_element (finder,
                                  (*op)->local_name ()->get_string ());
          this->add_regnodes (node, finder, slot++);
          parent->appendChild (finder);
          be_global->emit_diagnostic (finder);
        }

      // Add to list used in check for removed IDL decls.
      if (be_global->input_xme () != 0)
        {
          be_global->gme_id_set ().insert (
              finder->getAttribute (X ("id"))
            );
        }

      // Emits diagnostic if changed, idempotent otherwise.
      this->add_replace_id_element (finder, *op);
      this->add_version_element (finder, *op);

      adding_visitor scope_visitor (finder);
      if (scope_visitor.visit_scope (*op) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "adding_visitor::add_finders - "
                      "code generation for scope failed\n"));
        }

      this->add_exception_elements (finder,
                                    *op,
                                    0,
                                    (*op)->exceptions (),
                                    "ExceptionRef",
                                    scope_visitor.rel_id_);
    }
}

ACE_TCHAR *
adding_visitor::timestamp (ACE_TCHAR date_and_time[],
                          int length)
{
  if (length < 27)
    {
      errno = EINVAL;
      return 0;
    }

#if defined (WIN32)
   // Emulate Unix.  Win32 does NOT support all the UNIX versions
   // below, so DO we need this ifdef.
  static const ACE_TCHAR *day_of_week_name[] =
  {
    ACE_LIB_TEXT ("Sun"),
    ACE_LIB_TEXT ("Mon"),
    ACE_LIB_TEXT ("Tue"),
    ACE_LIB_TEXT ("Wed"),
    ACE_LIB_TEXT ("Thu"),
    ACE_LIB_TEXT ("Fri"),
    ACE_LIB_TEXT ("Sat")
  };

  static const ACE_TCHAR *month_name[] =
  {
    ACE_LIB_TEXT ("Jan"),
    ACE_LIB_TEXT ("Feb"),
    ACE_LIB_TEXT ("Mar"),
    ACE_LIB_TEXT ("Apr"),
    ACE_LIB_TEXT ("May"),
    ACE_LIB_TEXT ("Jun"),
    ACE_LIB_TEXT ("Jul"),
    ACE_LIB_TEXT ("Aug"),
    ACE_LIB_TEXT ("Sep"),
    ACE_LIB_TEXT ("Oct"),
    ACE_LIB_TEXT ("Nov"),
    ACE_LIB_TEXT ("Dec")
  };

  SYSTEMTIME local;
  ::GetLocalTime (&local);

  ACE_OS::sprintf (date_and_time,
                   ACE_LIB_TEXT ("%3s %3s %02d% 02d:%02d:%02d %04d"),
                   day_of_week_name[local.wDayOfWeek],
                   month_name[local.wMonth - 1],
                   (int) local.wDay,
                   (int) local.wHour,
                   (int) local.wMinute,
                   (int) local.wSecond,
                   (int) local.wYear);
#else  /* UNIX */
  ACE_TCHAR timebuf[26]; // This magic number is based on the ctime(3c) man page.
  ACE_Time_Value cur_time = ACE_OS::gettimeofday ();
  time_t secs = cur_time.sec ();

  ACE_OS::ctime_r (&secs,
                   timebuf,
                   sizeof timebuf);
  // date_and_timelen > sizeof timebuf!
  ACE_OS::strsncpy (date_and_time,
                    timebuf,
                    length);
  date_and_time[26] = '\0';
#endif /* WIN32 */

  return &date_and_time[0];
}

unsigned long
adding_visitor::nmembers_gme (UTL_Scope *s, AST_Attribute *a)
{
  unsigned long retval = 0;

  if (a != 0)
    {
      UTL_ExceptList *get_ex = a->get_get_exceptions ();
      UTL_ExceptList *set_ex = a->get_set_exceptions ();

      // The exceptions plus 1 for the attribute field type.
      return (get_ex != 0 ? get_ex->length () : 0)
             + (set_ex != 0 ? set_ex->length () : 0)
             + 1;
    }

  AST_Decl::NodeType snt = ScopeAsDecl (s)->node_type ();

  for (UTL_ScopeActiveIterator si (s, UTL_Scope::IK_decls);
        !si.is_done ();
        si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->imported ())
        {
          continue;
        }

      AST_Decl::NodeType nt = d->node_type ();

      // Don't count predefined types (applies only if s == root),
      // or any kind of forward declaration or enum value (the
      // latter are included in their enclosing scope by the IDL
      // compiler since enums are not a scope in C++.
      if (nt == AST_Decl::NT_pre_defined
          || nt == AST_Decl::NT_interface_fwd
          || nt == AST_Decl::NT_component_fwd
          || nt == AST_Decl::NT_valuetype_fwd
          || nt == AST_Decl::NT_eventtype_fwd
          || nt == AST_Decl::NT_struct_fwd
          || nt == AST_Decl::NT_union_fwd
          || nt == AST_Decl::NT_enum_val && snt != AST_Decl::NT_enum)
        {
          continue;
        }

      // Skip the Cookie-related stuff added in the front end for uses
      // multiple ports. Ports are stored in queues so all that's left
      // are attributes.
      if (snt == AST_Decl::NT_component && nt != AST_Decl::NT_attr)
        {
          continue;
        }

      ++retval;
    }

  AST_Operation *op = AST_Operation::narrow_from_scope (s);
  if (op != 0)
    {
      // The return type of a home factory and finder operations
      // are implicit in IDML.
      if (ScopeAsDecl (op->defined_in ())->node_type () != AST_Decl::NT_home)
        {
          retval += (0 == op->void_return_type () ? 1 : 0);
        }

      UTL_ExceptList *ex = op->exceptions ();
      retval += (0 == ex ? 0UL : ex->length ());
      return retval;
    }

  AST_Factory *f = AST_Factory::narrow_from_scope (s);
  if (f != 0)
    {
      UTL_ExceptList *ex = f->exceptions ();
      retval += (0 == ex ? 0UL : ex->length ());
      return retval;
    }

  AST_Union *u = AST_Union::narrow_from_scope (s);
  if (u != 0)
    {
      // Add 1 for the discriminator.
      ++retval;
      return retval;
    }

  AST_Root *r = AST_Root::narrow_from_scope (s);
  if (r != 0)
    {
      retval += this->user_includes () + this->n_basic_seqs_;
      return retval;
    }

  AST_Interface *i = AST_Interface::narrow_from_scope (s);
  if (i != 0)
    {
      // This covers supported interfaces for components and homes.
      retval += static_cast<unsigned long> (i->n_inherits ());
      // No 'return' here - this is a base class for all the others
      // below so we want to fall through.
    }

  AST_ValueType *v = AST_ValueType::narrow_from_scope (s);
  if (v != 0)
    {
      // Inherited valuetypes are in AST_Interface inheritance list.
      // This covers AST_EventType as well.
      retval += static_cast<unsigned long> (v->n_supports ());

      return retval;
    }

  AST_Component *c = AST_Component::narrow_from_scope (s);
  if (c != 0)
    {
      // Supported interfaces are in AST_Interface inheritance list.
      retval += (c->base_component () != 0 ? 1 : 0);

      // Add in the ports.
      retval += c->provides ().size ()
                 + c->uses ().size ()
                 + c->emits ().size ()
                 + c->publishes ().size ()
                 + c->consumes ().size ();
      return retval;
    }

  AST_Home *h = AST_Home::narrow_from_scope (s);
  if (h != 0)
    {
      // Supported interfaces are in AST_Interface inheritance list.
      retval += (h->base_home () != 0 ? 1 : 0)
                + (h->primary_key () != 0 ? 1: 0)
                + h->factories ().size ()
                + h->finders ().size ();
    }

  return retval;
}

const XMLCh *
adding_visitor::lookup_constant_type (AST_Constant *c)
{
  const char *ext_id = 0;
  const char **namelist = be_global->pdt_names ();
  AST_Decl *enum_type = 0;

  switch (c->et ())
    {
      case AST_Expression::EV_enum:
        enum_type =
          c->defined_in ()->lookup_by_name (c->enum_full_name (), true);
        ext_id = enum_type->repoID ();
        break;
      case AST_Expression::EV_short:
      case AST_Expression::EV_ushort:
        ext_id = namelist[4UL];
        break;
      case AST_Expression::EV_long:
      case AST_Expression::EV_ulong:
      case AST_Expression::EV_longlong:
      case AST_Expression::EV_ulonglong:
        ext_id = namelist[6UL];
        break;
      case AST_Expression::EV_bool:
        ext_id = namelist[3UL];
        break;
      case AST_Expression::EV_float:
      case AST_Expression::EV_longdouble:
      case AST_Expression::EV_double:
        ext_id = namelist[5UL];
        break;
      case AST_Expression::EV_char:
      case AST_Expression::EV_wchar:
      case AST_Expression::EV_octet:
        ext_id = namelist[10UL];
        break;
      case AST_Expression::EV_string:
      case AST_Expression::EV_wstring:
        ext_id = namelist[2UL];
        break;
      default:
        break;
    }

  const XMLCh *retval = 0;
  int result = be_global->decl_id_table ().find (ext_id, retval);

  if (result != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "adding_visitor::lookup_constant_type - "
                         "lookup of id %s failed\n",
                         ext_id),
                        0);
    }

  return retval;
}

ACE_CString
adding_visitor::print_scoped_name (UTL_IdList *sn)
{
  long first = true;
  long second = false;
  Identifier *id = 0;
  ACE_CString retval;

  for (UTL_IdListActiveIterator i (sn); !i.is_done (); i.next ())
    {
      if (!first)
        {
          retval += "::";
        }
      else if (second)
        {
          first = second = false;
        }

      // Print the identifier.
      id = i.item ();
      retval += id->get_string ();

      if (first)
        {
          if (ACE_OS::strcmp (id->get_string (), "") != 0)
            {
              // Does not start with a "".
              first = false;
            }
          else
            {
              second = true;
            }
        }
    }

  return retval;
}

ACE_CString
adding_visitor::expr_val_to_string (AST_Expression::AST_ExprValue *ev)
{
  ACE_CString value;
  char buffer[33] = {'\0'};

  switch (ev->et)
    {
      case AST_Expression::EV_short:
        ACE_OS::sprintf (buffer, "%hd", ev->u.sval);
        value = buffer;
        break;
      case AST_Expression::EV_ushort:
        ACE_OS::sprintf (buffer, "%hu", ev->u.usval);
        value = buffer;
        break;
      case AST_Expression::EV_long:
        ACE_OS::sprintf (buffer, "%ld", ev->u.lval);
        value = buffer;
        break;
      case AST_Expression::EV_ulong:
        ACE_OS::sprintf (buffer, "%u", ev->u.ulval);
        value = buffer;
        break;
      case AST_Expression::EV_longlong:
#if ! defined (ACE_LACKS_LONGLONG_T)
        ACE_OS::sprintf (buffer, "%ld", ev->u.llval);
        value = buffer;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        break;
      case AST_Expression::EV_ulonglong:
#if ! defined (ACE_LACKS_LONGLONG_T)
        ACE_OS::sprintf (buffer, ACE_UINT64_FORMAT_SPECIFIER, ev->u.ullval);
        value = buffer;
#endif /* ! defined (ACE_LACKS_LONGLONG_T) */
        break;
      case AST_Expression::EV_bool:
        value = (ev->u.bval ? "TRUE" : "FALSE");
        break;
      case AST_Expression::EV_float:
        ACE_OS::sprintf (buffer, "%f", ev->u.fval);
        value = buffer;
        break;
      case AST_Expression::EV_double:
        ACE_OS::sprintf (buffer, "%24.16G", ev->u.dval);
        value = buffer;
        break;
      case AST_Expression::EV_char:
        ACE_OS::sprintf (buffer, "%c", ev->u.cval);
        value = buffer;
        break;
      case AST_Expression::EV_wchar:
        ACE_OS::sprintf (buffer, "%lc", ev->u.wcval);
        value = buffer;
        break;
      case AST_Expression::EV_octet:
        ACE_OS::sprintf (buffer, "%d", ev->u.oval);
        value = buffer;
        break;
      case AST_Expression::EV_wstring:
        value = ev->u.wstrval;
        break;
      case AST_Expression::EV_string:
        value = ev->u.strval->get_string ();
        break;
      case AST_Expression::EV_longdouble:
      default:
        break;
    }

  return value;
}

unsigned long
adding_visitor::user_includes (void)
{
  unsigned long retval = 0;

  // Included IDL files are modeled as GME references in the File model,
  // except canonical includes orb.idl and Components.idl.
  for (unsigned long i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      ACE_CString fname (idl_global->included_idl_files ()[i]);
      int pos = fname.rfind ('/');
      ACE_CString lname =
        (pos == ACE_CString::npos ? fname : fname.substr (pos + 1));

      // Types in these files should never be referenced in application
      // IDL files, so we don't included references to the files in the
      // PICML model.
      if (lname == "Components.idl" || lname == "orb.idl")
        {
          continue;
        }

      // The only types from these files that are referenced directly are
      // the sequences of basic types, which will probably be added to
      // the basic types library in PICML.
      if (lname.find (".pidl") != ACE_CString::npos)
        {
          continue;
        }

      ++retval;
    }

  return retval;
}

void
adding_visitor::set_n_basic_seqs (void)
{
  const char **pdts = be_global->pdt_names ();

  if (idl_global->string_seq_seen_
      || idl_global->wstring_seq_seen_)
    {
      this->set_one_basic_seq (pdts[2UL]);
    }

  if (idl_global->short_seq_seen_
      || idl_global->ushort_seq_seen_)
    {
      this->set_one_basic_seq (pdts[4UL]);
    }

  if (idl_global->long_seq_seen_
      || idl_global->ulong_seq_seen_
      || idl_global->longlong_seq_seen_
      || idl_global->ulonglong_seq_seen_)
    {
      this->set_one_basic_seq (pdts[6UL]);
    }

  if (idl_global->float_seq_seen_
      || idl_global->double_seq_seen_
      || idl_global->longdouble_seq_seen_)
    {
      this->set_one_basic_seq (pdts[5UL]);
    }

  if (idl_global->char_seq_seen_
      || idl_global->wchar_seq_seen_
      || idl_global->octet_seq_seen_)
    {
      this->set_one_basic_seq (pdts[10UL]);
    }

  if (idl_global->boolean_seq_seen_)
    {
      this->set_one_basic_seq (pdts[3UL]);
    }

  if (idl_global->any_seq_seen_)
    {
      this->set_one_basic_seq (pdts[8UL]);
    }
}

void
adding_visitor::set_one_basic_seq (const char *base_type)
{
  DOMElement *elem = 0;
  ACE_CString name (base_type);
  name += be_global->basic_seq_suffix ();
  int result = be_global->decl_elem_table ().find (name.c_str (), elem);

  if (result != 0)
    {
      elem = this->doc_->createElement (X ("reference"));
      be_global->decl_elem_table ().bind (ACE::strnew (name.c_str ()), elem);
      ++this->n_basic_seqs_;
    }
}

// If an IDL module contains only other modules and forward decls,
// we can skip importing it, and at the same time avoid the
// 'empty package' constraint violation.
bool
adding_visitor::can_skip_import (UTL_Scope *node, DOMElement *parent)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_Decl::NodeType nt = d->node_type ();

      // These are not in the DOM tree or have no effect.
      if (nt == AST_Decl::NT_interface_fwd
          || nt == AST_Decl::NT_valuetype_fwd
          || nt == AST_Decl::NT_union_fwd
          || nt == AST_Decl::NT_struct_fwd
          || nt == AST_Decl::NT_component_fwd
          || nt == AST_Decl::NT_eventtype_fwd
          || nt == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      DOMElement *elem =
        be_global->imported_dom_element (
            parent,
            d->local_name ()->get_string ()
          );

      // If it's not one of the above types and not in the
      // DOM tree, we can't skip importing the IDL.
      if (0 == elem)
        {
          return false;
        }

      UTL_Scope *nested_node = DeclAsScope (d);

      // If this item is not a scope, don't recurse on it.
      if (0 == nested_node)
        {
          continue;
        }

      // Recurse on the scope member.
      if (!this->can_skip_import (nested_node, elem))
        {
          return false;
        }
    }

  return true;
}

void
adding_visitor::add_picml_boilerplate (void)
{
  this->add_folder ("ComponentImplementations",
                    &BE_GlobalData::implementations_folder);
  this->add_folder ("ImplementationArtifacts",
                    &BE_GlobalData::implementation_artifacts_folder);
  this->add_folder ("ComponentTypes",
                    &BE_GlobalData::component_types_folder);
  this->add_folder ("PackageConfigurations");
  this->add_folder ("ComponentPackages");
  this->add_folder ("DeploymentPlans");
  this->add_folder ("Targets");
  this->add_folder ("TopLevelPackages");
  this->add_folder ("ComponentBuild");
}

void
adding_visitor::add_folder (const char *kind, folder_setter pmf)
{
  // Just try to look it up first. If we are importing XME,
  // chances are this will succeed. However, the modeler may
  // have removed the folder from the model before exporting
  // to XME, so if the lookup fails for any reason, we create
  // the folder below.
  DOMElement *new_folder =
    be_global->lookup_by_tag_and_kind (be_global->root_folder (),
                                       "folder",
                                       kind);

  if (0 == new_folder)
    {
      new_folder = doc_->createElement (X ("folder"));
      this->set_id_attr (new_folder, BE_GlobalData::FOLDER);
      this->set_relid_attr (new_folder);
      new_folder->setAttribute (X ("childrelidcntr"), X ("0x0")); // TODO
      new_folder->setAttribute (X ("kind"), X (kind));
      this->add_name_element (new_folder, kind);
      be_global->root_folder ()->appendChild (new_folder);
    }

  // This may already be properly set, but calling the setter is idempotent.
  if (0 != pmf)
    {
      (be_global->*pmf) (new_folder);
    }
}

void
adding_visitor::add_default_container (AST_Component *node)
{
  DOMElement *container = doc_->createElement (X ("model"));
  this->set_id_attr (container, BE_GlobalData::MODEL);
  char *hex_relid =
    be_global->hex_string (be_global->component_types_rel_id ());
  container->setAttribute (X ("relid"), X (hex_relid));
  be_global->incr_component_types_rel_id ();
  container->setAttribute (X ("childrelidcntr"), X ("0x1"));
  container->setAttribute (X ("kind"), X ("ComponentContainer"));
  container->setAttribute (X ("role"), X ("ComponentContainer"));
  ACE_CString name (node->local_name ()->get_string ());
  name += "Container";
  this->add_name_element (container, name.c_str ());

  DOMElement *reference = doc_->createElement (X ("reference"));
  this->set_id_attr (reference, BE_GlobalData::REF);
  reference->setAttribute (X ("kind"), X ("ComponentRef"));
  reference->setAttribute (X ("role"), X ("ComponentRef"));
  reference->setAttribute (X ("relid"), X ("0x1"));
  reference->setAttribute (X ("referred"), be_global->lookup_id (node));
  ACE_CString refname (node->local_name ()->get_string ());
  refname += "Ref";
  this->add_name_element (reference, refname.c_str ());
  this->add_regnodes (0, reference, 1UL, 0, false, "Packaging");

  container->appendChild (reference);

  DOMElement *ctf = be_global->component_types_folder ();

  if (0 == ctf)
    {
      // If we import an XME file, the modeler may have removed
      // this folder for some reason. One day, we'll make it
      // readonly.
      this->add_folder ("ComponentTypes",
                        &BE_GlobalData::component_types_folder);
      ctf = be_global->component_types_folder ();
    }

  ctf->appendChild (container);
}

DOMElement *
adding_visitor::add_implementation_artifacts (AST_Component *node)
{
  DOMElement *container = doc_->createElement (X ("model"));
  this->set_id_attr (container, BE_GlobalData::MODEL);
  char *hex_relid =
    be_global->hex_string (be_global->implementation_artifacts_rel_id ());
  container->setAttribute (X ("relid"), X (hex_relid));
  be_global->incr_implementation_artifacts_rel_id ();
  container->setAttribute (X ("childrelidcntr"), X ("0xa"));
  container->setAttribute (X ("kind"), X ("ArtifactContainer"));
  ACE_CString name (node->local_name ()->get_string ());
  name += "Artifacts";
  this->add_name_element (container, name.c_str ());

  DOMElement *artifacts[3] = {0};
  unsigned long i = 0;

  for (i = 0; i < 3; ++i)
    {
      // Add stub, svnt or exec artifact. They are always added in the
      // same order, so the called function will know which one to add.
      artifacts[i] = this->add_one_impl_artifact (container, node, i);

      if (i > 0)
        {
          // svnt depends on stub, exec depends on svnt.
          this->add_artifact_depends (container,
                                      artifacts[i],
                                      artifacts [i - 1],
                                      i - 1);
        }
    }

  // exec also depends on stub.
  this->add_artifact_depends (container,
                              artifacts[2],
                              artifacts [0],
                              i - 1);

  DOMElement *iaf = be_global->implementation_artifacts_folder ();

  if (0 == iaf)
    {
      // If we import an XME file, the modeler may have removed
      // this folder for some reason. One day, we'll make it
      // readonly.
      this->add_folder ("ImplementationArtifacts",
                        &BE_GlobalData::implementation_artifacts_folder);
      iaf = be_global->implementation_artifacts_folder ();
    }

  iaf->appendChild (container);

  return container;
}

DOMElement *
adding_visitor::add_one_impl_artifact (DOMElement *container,
                                       AST_Component *node,
                                       unsigned long index)
{
  static const char *artifact_suffixes[] =
  {
    "_stub",
    "_svnt",
    "_exec"
  };

  DOMElement *artifact = doc_->createElement (X ("atom"));
  this->set_id_attr (artifact, BE_GlobalData::ATOM);
  char *hex_relid = be_global->hex_string (2 * index + 1);
  artifact->setAttribute (X ("relid"), X (hex_relid));
  artifact->setAttribute (X ("kind"), X ("ImplementationArtifact"));
  artifact->setAttribute (X ("role"), X ("ImplementationArtifact"));

  ACE_CString name (node->local_name ()->get_string ());
  name += artifact_suffixes[index];
  this->add_name_element (artifact, name.c_str ());

  this->add_regnodes (0,
                      artifact,
                      2 * index + 1,
                      0,
                      false,
                      "Packaging",
                      6UL);

  this->add_attribute (artifact, "UUID", true, "");
  this->add_attribute (artifact, "configuration", true, "");
  this->add_attribute (artifact, "label", false, "");
  this->add_attribute (artifact, "location", false, name.c_str ());

  container->appendChild (artifact);

  // If the artifact is a 'stub', this is a no-op.
  this->add_entrypoint (container, artifact, node, index);

  return artifact;
}

void
adding_visitor::add_entrypoint (DOMElement *container,
                                DOMElement *artifact,
                                AST_Component *node,
                                unsigned long index)
{
  ACE_CString suffix;

  switch (index)
    {
      case 1:
        suffix = "_Home_Servant";
        break;
      case 2:
        suffix = "_Home_Impl";
        break;
      default:
        return;
    }

  ACE_CString value_string ("create_");
  value_string += node->flat_name ();
  value_string += suffix;

  DOMElement *entry_point =
    this->add_property ("entryPoint",
                        2 * index,
                        2 * index,
                        6UL,
                        value_string.c_str ());

  container->appendChild (entry_point);

  DOMElement *connection =
    this->add_connection (artifact,
                          entry_point,
                          "ArtifactExecParameter",
                          index + 6);

  container->appendChild (connection);
}

void
adding_visitor::add_artifact_depends (DOMElement *container,
                                      DOMElement *src,
                                      DOMElement *dst,
                                      unsigned long index)
{
  DOMElement *connection =
    this->add_connection (src, dst, "ArtifactDependency", index + 8);

  container->appendChild (connection);
}

void
adding_visitor::add_implementation (const char *id,
                                    AST_Component *node,
                                    DOMElement *artifact_container)
{
  DOMElement *container = doc_->createElement (X ("model"));
  this->set_id_attr (container, BE_GlobalData::MODEL);
  char *hex_relid =
    be_global->hex_string (be_global->implementations_rel_id ());
  container->setAttribute (X ("relid"), X (hex_relid));
  be_global->incr_implementations_rel_id ();
  container->setAttribute (X ("childrelidcntr"), X ("0xb"));
  container->setAttribute (X ("kind"), X ("ComponentImplementationContainer"));
  ACE_CString name (node->local_name ()->get_string ());
  name += "Implementation";
  this->add_name_element (container, name.c_str ());

  DOMElement *impl = doc_->createElement (X ("atom"));
  (void) this->set_id_attr (impl, BE_GlobalData::ATOM);
  impl->setAttribute (X ("kind"), X ("MonolithicImplementation"));
  impl->setAttribute (X ("role"), X ("MonolithicImplementation"));
  impl->setAttribute (X ("relid"), X ("0x1"));

  ACE_CString component_name (node->local_name ()->get_string ());
  ACE_CString impl_name (component_name);
  impl_name += "MonolithicImpl";
  this->add_name_element (impl, impl_name.c_str ());

  this->add_regnodes (0, impl, 1UL, 0, false, "Packaging", 7UL);
  container->appendChild (impl);

  ACE_CString ior_name (component_name);
  ior_name += ".ior";
  DOMElement *component_ior =
    this->add_property ("ComponentIOR", 2UL, 2UL, 7UL, ior_name.c_str ());
  container->appendChild (component_ior);
  DOMElement *connection =
    this->add_connection (impl, component_ior, "ConfigProperty", 10UL);
  container->appendChild (connection);

  this->add_artifact_refs (container, impl, artifact_container);

  this->add_component_ref (container, impl, id, node);

  DOMElement *cif = be_global->implementations_folder ();

  if (0 == cif)
    {
      // If we import an XME file, the modeler may have removed
      // this folder for some reason. One day, we'll make it
      // readonly.
      this->add_folder ("ComponentImplementations",
                        &BE_GlobalData::implementations_folder);
      cif = be_global->implementations_folder ();
    }

  cif->appendChild (container);
}

DOMElement *
adding_visitor::add_property (const char *name,
                              unsigned long rel_id,
                              unsigned long,
                              unsigned long nslices,
                              const char *value)
{
  DOMElement *property = doc_->createElement (X ("model"));
  (void) this->set_id_attr (property, BE_GlobalData::MODEL);
  char *hex_relid = be_global->hex_string (rel_id);
  property->setAttribute (X ("relid"), X (hex_relid));
  property->setAttribute (X ("kind"), X ("Property"));
  property->setAttribute (X ("role"), X ("Property"));
  property->setAttribute (X ("childrelidcntr"), X ("0x2"));
  this->add_name_element (property, name);

  this->add_regnodes (0,
                      property,
                      rel_id,
                      0,
                      false,
                      "Packaging",
                      nslices);

  this->add_attribute (property, "DataValue", false, value);

  DOMElement *data_type = doc_->createElement (X ("reference"));
  this->set_id_attr (data_type, BE_GlobalData::REF);
  data_type->setAttribute (X ("relid"), X ("0x2"));
  data_type->setAttribute (X ("kind"), X ("DataType"));
  data_type->setAttribute (X ("role"), X ("DataType"));
  this->add_name_element (data_type, "String");

  const char **pdts = be_global->pdt_names ();
  const XMLCh *pdt_id = 0;

  // The slot for the predefined type 'string' is 2.
  int result = be_global->decl_id_table ().find (pdts[2UL], pdt_id);

  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "adding_visitor::add_property - "
                  "lookup of data type id for %s failed\n",
                  name));
      return 0;
    }

  data_type->setAttribute (X ("referred"), pdt_id);

  this->add_regnodes (0, data_type, 1UL, 0, false, "Packaging", 2UL);

  property->appendChild (data_type);

  return property;
}

DOMElement *
adding_visitor::add_connection (DOMElement *source,
                                DOMElement *destination,
                                const char *name,
                                unsigned long rel_id)
{
  DOMElement *connection = this->doc_->createElement (X ("connection"));
  (void) this->set_id_attr (connection, BE_GlobalData::CONN);
  connection->setAttribute (X ("kind"), X (name));
  connection->setAttribute (X ("role"), X (name));
  char *hex_id = be_global->hex_string (rel_id);
  connection->setAttribute(X ("relid"), X (hex_id));
  this->add_name_element (connection, name);

  DOMElement *dst = doc_->createElement (X ("connpoint"));
  dst->setAttribute (X ("role"), X ("dst"));
  dst->setAttribute (X ("target"), destination->getAttribute (X ("id")));
  connection->appendChild (dst);

  DOMElement *src = doc_->createElement (X ("connpoint"));
  src->setAttribute (X ("role"), X ("src"));
  src->setAttribute (X ("target"), source->getAttribute (X ("id")));
  connection->appendChild (src);

  return connection;
}

void
adding_visitor::add_attribute (DOMElement *container,
                               const char *kind,
                               bool meta,
                               const char *value)
{
  DOMElement *attr = doc_->createElement (X ("attribute"));
  attr->setAttribute (X ("kind"), X (kind));

  if (meta)
    {
      attr->setAttribute (X ("status"), X ("meta"));
    }

  container->appendChild (attr);

  DOMElement *value_elem = doc_->createElement (X ("value"));
  attr->appendChild (value_elem);
  DOMText *value_text = doc_->createTextNode (X (value));
  value_elem->appendChild (value_text);
}

void
adding_visitor::add_artifact_refs (DOMElement *impl_container,
                                   DOMElement *impl,
                                   DOMElement *artifact_container)
{
  DOMNodeList *artifacts =
    artifact_container->getElementsByTagName (X ("atom"));

  for (XMLSize_t index = 0; index < artifacts->getLength (); ++index)
    {
      DOMElement *artifact =
        dynamic_cast<DOMElement *> (artifacts->item (index));
      this->add_one_artifact_ref (impl_container, impl, artifact, index);
    }
}

void
adding_visitor::add_one_artifact_ref (DOMElement *impl_container,
                                      DOMElement *impl,
                                      DOMElement *artifact,
                                      XMLSize_t index)
{
  DOMElement *artifact_ref = doc_->createElement (X ("reference"));
  this->set_id_attr (artifact_ref, BE_GlobalData::REF);
  artifact_ref->setAttribute (X ("kind"),
                              X ("ImplementationArtifactReference"));
  artifact_ref->setAttribute (X ("role"),
                              X ("ImplementationArtifactReference"));
  char *hex_id = be_global->hex_string (index + 3);
  artifact_ref->setAttribute (X ("relid"), X (hex_id));
  artifact_ref->setAttribute (X ("referred"),
                              artifact->getAttribute (X ("id")));

  DOMElement *artifact_name_node =
    dynamic_cast<DOMElement *> (artifact->getFirstChild ());
  DOMText *artifact_name =
    dynamic_cast<DOMText *> (artifact_name_node->getFirstChild ());
  char *data = XMLString::transcode (artifact_name->getData ());
  ACE_CString ref_name (data);
  ref_name += "Ref";
  this->add_name_element (artifact_ref, ref_name.c_str ());
  XMLString::release (&data);

  impl_container->appendChild (artifact_ref);

  this->add_regnodes (0,
                      artifact_ref,
                      index + 3,
                      0,
                      false,
                      "Packaging",
                      7UL);

  DOMElement *connection = this->add_connection (impl,
                                                 artifact_ref,
                                                 "MonolithprimaryArtifact",
                                                 index + 6);
  impl_container->appendChild (connection);
}

void
adding_visitor::add_component_ref (DOMElement *container,
                                   DOMElement *impl,
                                   const char *gme_id,
                                   AST_Component *node)
{
  DOMElement *ref = doc_->createElement (X ("reference"));
  this->set_id_attr (ref, BE_GlobalData::REF);
  ref->setAttribute (X ("kind"), X ("ComponentRef"));
  ref->setAttribute (X ("role"), X ("ComponentRef"));
  ref->setAttribute (X ("relid"), X ("0x9"));
  ref->setAttribute (X ("referred"), X (gme_id));

  container->appendChild (ref);

  ACE_CString ref_name (node->local_name ()->get_string ());
  ref_name += "Ref";
  this->add_name_element (ref, ref_name.c_str ());

  this->add_regnodes (0, ref, 6UL, 0, false, "Packaging", 7UL);

  DOMElement *connection =
    this->add_connection (impl, ref, "Implements", 11);
  container->appendChild (connection);
}

void
adding_visitor::insert_element (DOMElement *elem)
{
  DOMElement *next = 0;

  // If we have no previous_, then get the first element in this DOM
  // tree scope corresonding to an IDL declaration. Otherwise, get
  // the next DOM element after previous_, if any.
  if (0 == this->previous_)
    {
      DOMNodeList *children = this->sub_tree_->getChildNodes ();

      for (XMLSize_t i = 0; i < children->getLength (); ++i)
        {

          DOMElement *child = (DOMElement *) children->item (i);

          if (0 == child)
            {
              continue;
            }

          const XMLCh *tag = child->getTagName ();

          // An element corresponding to something in the IDL file
          // will have to be one of these.
          if (X ("model") == tag
              || X ("reference") == tag
              || X ("atom") == tag)
            {
              next = child;
              break;
            }
        }
    }
  else
    {
      next = (DOMElement *) this->previous_->getNextSibling ();
    }

  // Behaves like appendChild() if next == 0.
  (void) this->sub_tree_->insertBefore (elem, next);
}
