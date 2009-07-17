//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_visitor.h
//
// = DESCRIPTION
//    The abstract visitor class. The concrete classes are responsible for code
//    generation.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Carlos O'Ryan, Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_VISITOR_H
#define TAO_BE_VISITOR_H

class be_decl;
class be_scope;
class be_type;
class be_predefined_type;
class be_module;
class be_interface;
class be_interface_fwd;
class be_template_interface;
class be_valuebox;
class be_valuetype;
class be_valuetype_fwd;
class be_eventtype;
class be_eventtype_fwd;
class be_component;
class be_component_fwd;
class be_porttype;
class be_provides;
class be_uses;
class be_publishes;
class be_emits;
class be_consumes;
class be_home;
class be_factory;
class be_structure;
class be_structure_fwd;
class be_exception;
class be_expression;
class be_enum;
class be_operation;
class be_field;
class be_argument;
class be_attribute;
class be_union;
class be_union_fwd;
class be_union_branch;
class be_union_label;
class be_constant;
class be_enum_val;
class be_array;
class be_sequence;
class be_string;
class be_typedef;
class be_root;
class be_native;

class be_visitor_context;
class TAO_OutStream;

class be_visitor
{
  //
  // = TITLE
  //   Base class for visitors.
  //
  // = DESCRIPTION
  //   To implement code generation we use the "Visitor Pattern", this
  //   is the abstract "Visitor".
  //   The "visit" methods are not pure virtual to facilitate the
  //   implementation of simple visitors that only override a few.
  //
public:
  virtual ~be_visitor (void);

  virtual int visit_decl (be_decl *d);
  virtual int visit_scope (be_scope *node);
  virtual int visit_type (be_type *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_module (be_module *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_template_interface (be_template_interface *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  virtual int visit_component (be_component *node);
  virtual int visit_component_fwd (be_component_fwd *node);
  virtual int visit_porttype (be_porttype *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);
  virtual int visit_home (be_home *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_expression (be_expression *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_field (be_field *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_union_branch (be_union_branch *node);
  virtual int visit_union_label (be_union_label *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_enum_val (be_enum_val *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_root (be_root *node);
  virtual int visit_native (be_native *node);

protected:
  // Not abstract but we want to force subclassing anyway.
  be_visitor (void);
};

#endif // TAO_BE_VISITOR_H
