/* -*- c++ -*- */
// $Id$

#include "type_visitor.h"
#include "XercesString.h"

#include "be_extern.h"

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
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_union.h"
#include "ast_union_branch.h"
#include "ast_union_fwd.h"
#include "ast_union_label.h"
#include "ast_valuebox.h"
#include "ast_valuetype_fwd.h"
#include "ast_native.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "ace/OS_NS_stdio.h"

type_visitor::type_visitor (DOMElement *sub_tree)
  : idl_to_wsdl_visitor (sub_tree),
    current_union_ (0),
    alias_ (0)
{
}

type_visitor::~type_visitor (void)
{
}

int
type_visitor::visit_interface (AST_Interface *node)
{
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "type_visitor::visit_interface - "
                          "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
type_visitor::visit_valuetype (AST_ValueType *node)
{
  // Fetch from DOM tree, from table, or create.
  DOMElement *elem = this->process_node (node, "xsd:complexType");
  
  // Fetch from DOM tree if imported.
  DOMElement *seq = 0;
  
  if (NOT_SEEN == this->node_status_)
    {  
      ACE_CString name (node->full_name ());
      be_global->to_wsdl_name (name);
      elem->setAttribute (X ("name"), X (name.c_str ()));
      
      seq = this->doc_->createElement (X ("xsd:sequence"));
      elem->appendChild (seq);
    
      DOMElement *attr = this->doc_->createElement (X ("xsd:attribute"));
      attr->setAttribute (X ("name"), X ("id"));
      attr->setAttribute (X ("type"), X ("xsd:ID"));
      attr->setAttribute (X ("use"), X ("optional"));
      elem->appendChild (attr);
    }
  
  if (DONE != this->node_status_)
    {
      seq = dynamic_cast<DOMElement *> (elem->getFirstChild ());
      type_visitor scope_visitor (seq);
      
      if (scope_visitor.visit_scope (node) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "type_visitor::visit_structure - "
                              "codegen for scope failed\n"),
                            -1);
        }
        
      this->gen_inherited_vt_members (node, scope_visitor);
    }

  return 0;
}

int
type_visitor::visit_eventtype (AST_EventType *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (this->visit_valuetype (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "type_visitor::visit_eventtype - "
                          "codegen for valuetype failed\n"),
                        -1);
    }

  return 0;
}

int
type_visitor::visit_structure (AST_Structure *node)
{
  // Fetch from DOM tree, from table, or create.
  DOMElement *elem = this->process_node (node, "xsd:complexType");
  
  // Fetch from DOM tree if imported.
  DOMElement *seq = 0;
  
  if (NOT_SEEN == this->node_status_)
    {  
      ACE_CString name (node->full_name ());
      be_global->to_wsdl_name (name);
      elem->setAttribute (X ("name"), X (name.c_str ()));
      
      seq = this->doc_->createElement (X ("xsd:sequence"));
      elem->appendChild (seq);
    }
  
  if (DONE != this->node_status_)
    {
      seq = dynamic_cast<DOMElement *> (elem->getFirstChild ());
      type_visitor scope_visitor (seq);
      
      if (scope_visitor.visit_scope (node) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "type_visitor::visit_structure - "
                              "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
type_visitor::visit_exception (AST_Exception *node)
{
  if (0 != this->visit_structure (node))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "type_visitor::visit_exception - "
                          "codegen for exception failed\n"),
                        -1);
    }
    
  return 0;
}

int
type_visitor::visit_enum (AST_Enum *node)
{
  // Fetch from DOM tree, from table, or create.
  DOMElement *elem = this->process_node (node, "xsd:simpleType");
  DOMElement *restriction = 0;
  
  if (NOT_SEEN == this->node_status_)
    {
      ACE_CString name (node->full_name ());
      be_global->to_wsdl_name (name);
      elem->setAttribute (X ("name"), X (name.c_str ()));
      
      // Enum values go inside this.
      restriction =
        this->doc_->createElement (X ("xsd:restriction"));
      restriction->setAttribute (X ("base"), X ("xsd:string"));
      elem->appendChild (restriction);
    }

  if (DONE != this->node_status_)
    {
      restriction = dynamic_cast<DOMElement *> (elem->getFirstChild ());
      type_visitor scope_visitor (restriction);
      
      if (scope_visitor.visit_scope (node) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "type_visitor::visit_enum - "
                             "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
type_visitor::visit_field (AST_Field *node)
{
  AST_Type *ft = node->field_type ();
  AST_Decl::NodeType nt = ft->node_type ();
  bool vt_member =
    AST_Decl::NT_valuetype == nt || AST_Decl::NT_eventtype == nt;
  DOMElement *sub_tree_holder = this->sub_tree_;
  
  if (vt_member)
    {
      DOMElement *choice = this->doc_->createElement (X ("xsd:choice"));
      this->sub_tree_->appendChild (choice);
      this->sub_tree_ = choice;
    }
  
  DOMElement *elem = this->doc_->createElement (X ("xsd:element"));
  this->sub_tree_->appendChild (elem);
  
  elem->setAttribute (X ("name"),
                      X (node->local_name ()->get_string ()));
  elem->setAttribute (X ("minOccurs"), X ("1"));
  elem->setAttribute (X ("maxOccurs"), X ("1"));
  
  ACE_CString ft_name;
  this->type_name (ft_name, ft, true);
  elem->setAttribute (X ("type"), X (ft_name.c_str ()));
      
  // Not sure if this is the only case where 'nillable' appears.          
  if (AST_Decl::NT_string == nt)
    {
      elem->setAttribute (X ("nillable"), X ("true"));
    }
    
  if (vt_member)
    {
      DOMElement *vt_ref =
        this->doc_->createElement (X ("xsd:element"));
      ACE_CString name ("_REF_");
      name += node->local_name ()->get_string ();
      vt_ref->setAttribute (X ("name"), X (name.c_str ()));
      vt_ref->setAttribute (X ("type"), X ("corba:_VALUEREF"));
      this->sub_tree_->appendChild (vt_ref);
    }
  
  this->sub_tree_ = sub_tree_holder;
  return 0;
}

int
type_visitor::visit_union (AST_Union *node)
{
  // Fetch from DOM tree, from table, or create.
  DOMElement *elem = this->process_node (node, "xsd:complexType");
  
  // Fetch from DOM tree if imported.
  DOMElement *seq = 0;
  DOMElement *choice = 0;
  
  if (NOT_SEEN == this->node_status_)
    {  
      ACE_CString name (node->full_name ());
      be_global->to_wsdl_name (name);
      elem->setAttribute (X ("name"), X (name.c_str ()));
      
      seq = this->doc_->createElement (X ("xsd:sequence"));
      elem->appendChild (seq);
      
      DOMElement *disc = this->doc_->createElement (X ("xsd:element"));
      disc->setAttribute (X ("name"), X ("discriminator"));
      AST_Type *dt = node->disc_type ();
      ACE_CString disc_name = dt->full_name ();
      this->type_name (disc_name, dt);
      disc->setAttribute (X ("type"), X (disc_name.c_str ()));
      seq->appendChild (disc);
      
      choice = this->doc_->createElement (X ("xsd:choice"));
      seq->appendChild (choice);
    }
  
  if (DONE != this->node_status_)
    {
      seq = dynamic_cast<DOMElement *> (elem->getFirstChild ());
      choice = dynamic_cast<DOMElement *> (seq->getLastChild ());
      
      type_visitor scope_visitor (choice);
      scope_visitor.current_union_ = node;
      
      if (scope_visitor.visit_scope (node) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "type_visitor::visit_structure - "
                              "codegen for scope failed\n"),
                            -1);
        }
    }

  return 0;
}

int
type_visitor::visit_union_branch (AST_UnionBranch *node)
{
  this->label_list_ = "";

  for (unsigned long i = 0; i < node->label_list_length (); ++i)
    {
      this->label_list_ += (0 == i ? "" : ", ");
      AST_UnionLabel *ul = node->label (i);
    
      if (ul->label_kind () == AST_UnionLabel::UL_default)
        {
          this->label_list_ += "default case";
          break;
        }
      else
        {
          this->label_list_ += (0 == i ? "case " : "");
        }

      if (this->visit_union_label (ul) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                              "type_visitor::visit_union_branch - "
                              "codegen for label failed\n"),
                            -1);
        }
    }
    
  DOMComment *comment =
    this->doc_->createComment (X (this->label_list_.c_str ()));
  this->sub_tree_->appendChild (comment);
  
  DOMElement *member = this->doc_->createElement (X ("xsd:element"));
  member->setAttribute (X ("name"), X (node->local_name ()->get_string ()));
  
  ACE_CString tname;
  AST_Type *ft = node->field_type ();
  this->type_name (tname, ft);
  member->setAttribute (X ("type"), X (tname.c_str ()));
  
  member->setAttribute (X ("minOccurs"), X ("0"));
  member->setAttribute (X ("maxOccurs"), X ("1"));
                
  if (AST_Type::VARIABLE == ft->size_type ())
    {
      member->setAttribute (X ("nillable"), X ("true"));
    }
  
  this->sub_tree_->appendChild (member);

  return 0;
}

int
type_visitor::visit_union_label (AST_UnionLabel *node)
{
  ACE_CString name_holder;
  AST_Expression *e = node->label_val ();
  AST_Decl::NodeType nt =
    this->current_union_->disc_type ()->node_type ();

  if (AST_Decl::NT_enum == nt)
    {
      UTL_Scope *parent =
        this->current_union_->disc_type ()->defined_in ();

      if (0 == parent)
        {
          name_holder = this->print_scoped_name (e->n ());
          be_global->to_wsdl_name (name_holder);
          this->label_list_ += name_holder;
        }
      else
        {
          if (ScopeAsDecl (parent)->node_type () != AST_Decl::NT_root)
            {
              name_holder = ScopeAsDecl (parent)->full_name ();
              be_global->to_wsdl_name (name_holder);
              this->label_list_ += name_holder;
              this->label_list_ += ".";
            }

          this->label_list_ += e->n ()->last_component ()->get_string ();
        }
    }
  else
    {
      this->label_list_ +=
        this->expr_val_to_string (node->label_val ()->ev ());
    }

  return 0;
}

int
type_visitor::visit_enum_val (AST_EnumVal *node)
{
  // Enum values are also added to the enum's enclosing scope.
  // We don't want to generate anything for these nodes.
  if (X ("xsd:restriction") == this->sub_tree_->getTagName ())
    {
      DOMElement *elem = this->process_node (node, "xsd:enumeration");
      
      if (NOT_SEEN == this->node_status_)
        {
          elem->setAttribute (X ("value"),
                              X (node->local_name ()->get_string ()));
        }
    }
    
  return 0;
}

int
type_visitor::visit_array (AST_Array *node)
{
  return 0;
}

int
type_visitor::visit_sequence (AST_Sequence *node)
{
  // Fetch from DOM tree, from table, or create.
  DOMElement *elem = this->process_node (this->alias_, "xsd:complexType");
  
  if (NOT_SEEN == this->node_status_)
    {
      this->gen_seq_array_common (elem,
                                  node->base_type (),
                                  node->max_size (),
                                  false,
                                  0);
    }
    
  return 0;
}

int
type_visitor::visit_typedef (AST_Typedef *node)
{
  // TODO - typedefs of complex types.
  AST_Type *bt = node->base_type ();
  AST_Decl::NodeType nt = bt->node_type ();
  DOMElement *elem = 0;
  DOMElement *td_elem = 0;
  DOMElement *restriction = 0;
      
  ACE_CString name (node->full_name ());
  be_global->to_wsdl_name (name);
  
  ACE_CString bt_name;
  this->type_name (bt_name, bt);
  bool complex_type = false;

  switch (nt)
    {
      case AST_Decl::NT_sequence:
        {
          AST_Sequence *seq = AST_Sequence::narrow_from_decl (bt);
          this->alias_ = node;
          return this->visit_sequence (seq);
        }
      case AST_Decl::NT_pre_defined:
      case AST_Decl::NT_string:
      case AST_Decl::NT_typedef:
        // Fetch from DOM tree, from table, or create.
        elem = this->process_node (node, "xsd:simpleType");
        td_elem = elem;
        break;
      default:
        break;
    }
  
  if (NOT_SEEN == this->node_status_)
    {
      elem->setAttribute (X ("name"), X (name.c_str ()));
      restriction = this->doc_->createElement (X ("xsd:restriction"));
      restriction->setAttribute (X ("base"), X (bt_name.c_str ()));
      td_elem->appendChild (restriction);
    }
    
  if (DONE != this->node_status_ && complex_type)
    {
      // TODO
    }

  return 0;
}

void
type_visitor::gen_seq_array_common (DOMElement *elem,
                                           AST_Type *base_type,
                                           AST_Expression *size,
                                           bool is_array,
                                           const char *insert)
{
  ACE_CString name;
  this->type_name (name, this->alias_, false);
  elem->setAttribute (X ("name"), X (name.c_str ()));
  
  DOMElement *content =
    this->doc_->createElement (X ("complexContent"));
  elem->appendChild (content);
  
  DOMElement *restriction =
    this->doc_->createElement (X ("xsd:restriction"));
  restriction->setAttribute (X ("base"), X ("soap-enc:Array"));
  content->appendChild (restriction);
  
  DOMElement *seq = this->doc_->createElement (X ("xsd:sequence"));
  restriction->appendChild (seq);
  
  DOMElement *item = this->doc_->createElement (X ("xsd:element"));
  item->setAttribute (X ("name"), X ("item"));
  
  // Can't be 0 for an array, parser would catch it.
  unsigned long max_size = size->ev ()->u.ulval;
  ACE_CString bound (
      0 == max_size
        ? "unbounded"
        : this->expr_val_to_string (size->ev ()));
         
  item->setAttribute (X ("minOccurs"), X (is_array ? bound.c_str () : "0"));
  item->setAttribute (X ("maxOccurs"), X (bound.c_str ()));
  
  ACE_CString tname;
  this->type_name (tname, base_type);
  item->setAttribute (X ("type"), X (tname.c_str ()));
  
  seq->appendChild (item);
  
  DOMElement *attr = this->doc_->createElement (X ("xsd:attribute"));
  attr->setAttribute (X ("ref"), X ("soap-enc:arrayType"));
  tname += "[]";
  attr->setAttribute (X ("wsdl:arrayType"), X (tname.c_str ()));
  
  restriction->appendChild (attr);
}

ACE_CString
type_visitor::print_scoped_name (UTL_IdList *sn)
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
type_visitor::expr_val_to_string (AST_Expression::AST_ExprValue *ev)
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

void
type_visitor::gen_inherited_vt_members (AST_ValueType *node,
                                        type_visitor &visitor)
{
  AST_ValueType *parent = node->inherits_concrete ();
  
  if (0 != parent)
    {
      if (0 != visitor.visit_scope (parent))
        {
          ACE_ERROR ((LM_ERROR,
                      "type_visitor::gen_inherited_vt_members - "
                      "codegen for base valuetype failed\n"));
        }
        
      this->gen_inherited_vt_members (parent, visitor);
    }
}

