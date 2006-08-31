/* -*- c++ -*- */
// $Id$

#include "idl_to_wsdl_visitor.h"
#include "XercesString.h"

#include "be_extern.h"

#include "ast_module.h"
#include "ast_predefined_type.h"
#include "ast_root.h"
#include "utl_identifier.h"
#include "nr_extern.h"

#include "ace/OS_NS_stdio.h"

idl_to_wsdl_visitor::idl_to_wsdl_visitor (DOMElement *sub_tree)
  : sub_tree_ (sub_tree),
    doc_ (sub_tree->getOwnerDocument ())
{
}

idl_to_wsdl_visitor::~idl_to_wsdl_visitor (void)
{
}

int
idl_to_wsdl_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d->node_type () == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      // Want to skip the uses_xxxConnection structs added by uses
      // multiple ports.
      // @@@ (JP) This will go away when the visitor is finished, since
      // those uses_xxxConnection structs will not be added to the AST.
      if (ScopeAsDecl (node)->node_type () == AST_Decl::NT_component
          && d->node_type () != AST_Decl::NT_attr)
        {
          continue;
        }

      if (d->ast_accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "idl_to_wsdl_visitor::visit_scope - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
idl_to_wsdl_visitor::visit_module (AST_Module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl_to_wsdl_visitor::visit_module - "
                          "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
idl_to_wsdl_visitor::visit_root (AST_Root *node)
{
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl_to_wsdl_visitor::visit_root - "
                          "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

void
idl_to_wsdl_visitor::type_name (ACE_CString &name,
                                AST_Decl *d,
                                bool as_ref)
{
  AST_PredefinedType *pdt = 0;

  switch (d->node_type ())
    {
      case AST_Decl::NT_interface:
        name = "corba:ObjectReference";
        break;
      case AST_Decl::NT_wstring:
      case AST_Decl::NT_string:
        name = "xsd:string";
        break;
      case AST_Decl::NT_pre_defined:
        pdt = AST_PredefinedType::narrow_from_decl (d);

        switch (pdt->pt ())
          {
            case AST_PredefinedType::PT_pseudo:
              name = "CORBA.TypeCode";
              break;
            case AST_PredefinedType::PT_object:
              name = "CORBA.Object";
              break;
            case AST_PredefinedType::PT_any:
              name = "CORBA.Any";
              break;
            case AST_PredefinedType::PT_long:
              name = "xsd:int";
              break;
            case AST_PredefinedType::PT_ulong:
              name = "xsd:unsignedInt";
              break;
            case AST_PredefinedType::PT_longlong:
              name = "xsd:long";
              break;
            case AST_PredefinedType::PT_ulonglong:
              name = "xsd:unsignedLong";
              break;
            case AST_PredefinedType::PT_short:
              name = "xsd:short";
              break;
            case AST_PredefinedType::PT_ushort:
              name = "xsd:unsignedShort";
              break;
            case AST_PredefinedType::PT_float:
              name = "xsd:float";
              break;
            case AST_PredefinedType::PT_double:
              name = "xsd:double";
              break;
            case AST_PredefinedType::PT_longdouble:
              // Mapping for this not found in CORBAi spec.
              name = "xsd:double";
              break;
            case AST_PredefinedType::PT_char:
              name = "xsd:string";
              break;
            case AST_PredefinedType::PT_wchar:
              name = "xsd:string";
              break;
            case AST_PredefinedType::PT_boolean:
              name = "xsd:boolean";
              break;
            case AST_PredefinedType::PT_octet:
              name = "xsd:unsignedByte";
              break;
            case AST_PredefinedType::PT_void:
              name = "CORBA.Any";
              break;
            default:
              break;
          }
          
        break;
      case AST_Decl::NT_typedef:
        {
          AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
          AST_Decl::NodeType nt = td->base_type ()->node_type ();
          
          // If we are not a typedef of an array or a sequence, just
          // fall through to the default case.
          if (AST_Decl::NT_array == nt || AST_Decl::NT_sequence == nt)
            {
              AST_Decl *scope = ScopeAsDecl (d->defined_in ());
              
              if (AST_Decl::NT_root != scope->node_type ())
                {
                  name += scope->full_name ();
                  be_global->to_wsdl_name (name);
                  name += ".";
                }
             
              if (as_ref)
                {
                  name = ACE_CString ("tns:") + name;
                }
                
              name += "_SE_";
              name += d->local_name ()->get_string ();
              break;
           }
        }
      default:
        name += d->full_name ();
        be_global->to_wsdl_name (name);
        
        if (as_ref)
          {
            name = ACE_CString ("tns:") + name;
          }
          
        break;
    }
}

DOMElement *
idl_to_wsdl_visitor::process_node (AST_Decl *node, const char *tag_name)
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
          this->node_status_ = NOT_SEEN;
          elem = this->doc_->createElement (X (tag_name));
        }

      // We add the elem to the table the first time the node is
      // seen, but we don't add the stuff below unless the elem
      // has not been imported, and we're in
      // the IDL file where the node is defined.
      if (!node->imported ())
        {
          this->sub_tree_->appendChild (elem);
        }
    }
  else
    {
      if (0 == result)
        {
          this->node_status_ = DONE;
        }
      else
        {
          this->node_status_ = IN_DOM_TREE;
        }
    }

  if (0 != result)
    {
      // Store the DOMElement.
      be_global->decl_elem_table ().bind (ACE::strnew (node->repoID ()),
                                          elem);
    }
  
  return elem;
}

int
idl_to_wsdl_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_interface (AST_Interface *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_valuebox (AST_ValueBox *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_valuetype (AST_ValueType *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_eventtype (AST_EventType *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_component (AST_Component *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_home (AST_Home *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_factory (AST_Factory *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_structure (AST_Structure *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_exception (AST_Exception *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_enum (AST_Enum *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_operation (AST_Operation *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_field (AST_Field *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_argument (AST_Argument *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_attribute (AST_Attribute *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_union (AST_Union *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_constant (AST_Constant *)
{
  return 0;
}

int 
idl_to_wsdl_visitor::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_array (AST_Array *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_sequence (AST_Sequence *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_string (AST_String *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_typedef (AST_Typedef *)
{
  return 0;
}

int
idl_to_wsdl_visitor::visit_native (AST_Native *)
{
  return 0;
}

