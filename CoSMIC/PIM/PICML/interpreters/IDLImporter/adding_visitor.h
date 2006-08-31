// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    adding_visitor.h
//
// = DESCRIPTION
//    The PICML IDL importer AST visitor class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDL_TO_PICML_ADDING_VISITOR_H
#define IDL_TO_PICML_ADDING_VISITOR_H

#include "ast_visitor.h"
#include "ast_expression.h"
#include "ast_component.h"
#include "be_extern.h"

#include "ace/Unbounded_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/dom/DOM.hpp>

using namespace xercesc;

class UTL_ExceptList;
class UTL_IdList;

class adding_visitor : public ast_visitor
{
  //
  // = TITLE
  //    adding_visitor
  //
  // = DESCRIPTION
  //    PICML IDL importer AST visitor. This visitor traverses
  //    the AST constructed by the IDL compiler front end,
  //    and creates or extends the corresponding DOM tree.
  //
public:
  adding_visitor (DOMElement *sub_tree,
                  unsigned long relid = 1UL);
  virtual ~adding_visitor (void);

  virtual int visit_decl (AST_Decl *d);
  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_type (AST_Type *node);
  virtual int visit_predefined_type (AST_PredefinedType *node);
  virtual int visit_module (AST_Module *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
  virtual int visit_factory (AST_Factory *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_structure_fwd (AST_StructureFwd *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_expression (AST_Expression *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_field (AST_Field *node);
  virtual int visit_argument (AST_Argument *node);
  virtual int visit_attribute (AST_Attribute *node);
  virtual int visit_union (AST_Union *node);
  virtual int visit_union_fwd (AST_UnionFwd *node);
  virtual int visit_union_branch (AST_UnionBranch *node);
  virtual int visit_union_label (AST_UnionLabel *node);
  virtual int visit_constant (AST_Constant *node);
  virtual int visit_enum_val (AST_EnumVal *node);
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_string (AST_String *node);
  virtual int visit_typedef (AST_Typedef *node);
  virtual int visit_root (AST_Root *node);
  virtual int visit_native (AST_Native *node);
  virtual int visit_valuebox (AST_ValueBox *node);

private:
  typedef ACE_Unbounded_Queue_Iterator<AST_Component::port_description>
    PORT_ITER;

  typedef void (BE_GlobalData::*folder_setter) (DOMElement *);

private:
  ACE_CString set_id_attr (DOMElement *elem, BE_GlobalData::kind_id kind);
  void set_relid_attr (DOMElement *elem);
  void set_childrelidcntr_attr (DOMElement *elem,
                                UTL_Scope *s,
                                AST_Attribute *a = 0);

  void add_name_element (DOMElement *elem, const char *name);
  void add_predefined_types (void);
  void add_predefined_sequences (DOMElement *parent, AST_Root *node);
  void add_one_predefined_sequence (DOMElement *parent,
                                    AST_Root *node,
                                    const char *type,
                                    unsigned long &model_slot,
                                    unsigned long pdt_slot);
  DOMElement *add_file_element (DOMElement *parent,
                                AST_Root *node,
                                unsigned long rel_id);
  void add_prefix_element (DOMElement *parent, AST_Decl *node);
  void add_replace_id_element (DOMElement *parent, AST_Decl *node);
  void add_version_element (DOMElement *parent, AST_Decl *node);
  void add_tag_common (const char *value,
                       const char *name,
                       DOMElement *parent,
                       bool is_meta = true);
  void add_local_element (DOMElement *parent, AST_Decl *node);
  void add_abstract_element (DOMElement *parent, AST_Decl *node);
  void add_include_elements (UTL_Scope *container, DOMElement *parent);
  void add_regnodes (UTL_Scope *container,
                     DOMElement *parent,
                     size_t slot,
                     AST_Attribute *a = 0,
                     bool is_connected = false,
                     const char *aspect = "InterfaceDefinition",
                     unsigned long num_slices = 0UL);
  void add_pos_element (UTL_Scope *container,
                        DOMElement *parent,
                        size_t slot,
                        AST_Attribute *a = 0,
                        bool is_connected = false,
                        unsigned long num_slices = 0UL);
  void add_inherited_elements (DOMElement *parent, AST_Interface *node);
  void add_one_inherited (DOMElement *parent,
                          AST_Interface *node,
                          AST_Interface *base,
                          unsigned long slot);
  void add_supported_elements (DOMElement *parent,
                               AST_Interface *node,
                               AST_Interface **supports,
                               long n_supports);
  void add_exception_elements (DOMElement *parent,
                               UTL_Scope *s,
                               AST_Attribute *a,
                               UTL_ExceptList *el,
                               const char *name,
                               unsigned long start_slot);
  void add_constant_value (DOMElement *parent, AST_Constant *node);
  void add_discriminator (DOMElement *parent, AST_Union *u);
  void add_labels (AST_UnionBranch *ub,
                   const XMLCh *ub_id,
                   unsigned long member_slot);
  ACE_CString get_label_name (AST_UnionLabel *ul, UTL_Scope *u);
  void add_private (AST_Field *f,
                    const XMLCh *member_id,
                    unsigned long member_slot);
  DOMElement *find_connection (const XMLCh *endpoint_id,
                               const char *role);
  void add_base_component (DOMElement *parent, AST_Component *node);
  void add_base_home (DOMElement *parent, AST_Home *node);
  void add_ports (DOMElement *parent, AST_Component *node);
  void add_manages (AST_Home *node);
  void add_lookup_key (DOMElement *parent, AST_Home *node);
  void add_home_factories (DOMElement *parent, AST_Home *node);
  void add_finders (DOMElement *parent, AST_Home *node);

  ACE_TCHAR *timestamp (ACE_TCHAR date_and_time[], int length);
  unsigned long nmembers_gme (UTL_Scope *s, AST_Attribute *a = 0);
  const XMLCh *lookup_constant_type (AST_Constant *c);
  ACE_CString print_scoped_name (UTL_IdList *sn);
  ACE_CString expr_val_to_string (AST_Expression::AST_ExprValue *ev);
  unsigned long user_includes (void);
  void set_n_basic_seqs (void);
  void set_one_basic_seq (const char *base_type);
  bool can_skip_import (UTL_Scope *node, DOMElement *parent);

  void add_picml_boilerplate (void);
  void add_folder (const char *kind, folder_setter pmf = 0);

  void add_default_container (AST_Component *node);
  DOMElement *add_implementation_artifacts (AST_Component *node);
  DOMElement *add_one_impl_artifact (DOMElement *container,
                                     AST_Component *node,
                                     unsigned long index);
  void add_entrypoint (DOMElement *container,
                       DOMElement *artifact,
                       AST_Component *node,
                       unsigned long index);
  void add_artifact_depends (DOMElement *container,
                             DOMElement *src,
                             DOMElement *dst,
                             unsigned long index);
   void add_implementation (const char *gme_id,
                            AST_Component *node,
                            DOMElement *artifact_contaienr);
   DOMElement *add_property (const char *name,
                             unsigned long rel_id,
                             unsigned long slot,
                             unsigned long nslices,
                             const char *value);
   DOMElement *add_connection (DOMElement *source,
                               DOMElement *destination,
                               const char *name,
                               unsigned long rel_id);
   void add_attribute (DOMElement *container,
                       const char *kind,
                       bool meta,
                       const char *value);
   void add_artifact_refs (DOMElement *impl_container,
                           DOMElement *impl,
                           DOMElement *artifact_container);
   void add_one_artifact_ref (DOMElement *impl_container,
                              DOMElement *impl,
                              DOMElement *artifact,
                              XMLSize_t index);
   void add_component_ref (DOMElement *container,
                           DOMElement *impl,
                           const char *gme_id,
                           AST_Component *node);

   void insert_element (DOMElement *elem);

private:
  DOMElement *sub_tree_;
  DOMElement *previous_;
  DOMDocument *doc_;
  unsigned long rel_id_;
  const unsigned long XMAX_;
  const unsigned long YMAX_;
  const unsigned long ICON_SIZE_;
  unsigned long private_relid_offset_;
  unsigned long manages_relid_offset_;
  unsigned long import_relid_offset_;
  unsigned long n_basic_seqs_;
};

#endif /* IDL_TO_PICML_ADDING_VISITOR_H */
