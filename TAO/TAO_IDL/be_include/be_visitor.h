//
// $Id$
//

#ifndef _BE_VISITOR_H
#define _BE_VISITOR_H

class be_decl;
class be_scope;
class be_type;
class be_predefined_type;
class be_module;
class be_interface;
class be_interface_fwd;
class be_structure;
class be_exception;
class be_expression;
class be_enum;
class be_operation;
class be_field;
class be_argument;
class be_attribute;
class be_union;
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

  virtual int visit_decl (be_decl *node);
  virtual int visit_scope (be_scope *node);
  virtual int visit_type (be_type *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_module (be_module *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_expression (be_expression *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_field (be_field *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_union (be_union *node);
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
};

#endif // _BE_VISITOR_H
