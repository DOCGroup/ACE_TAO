//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
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
#ifdef IDL_HAS_VALUETYPE
  class be_valuetype;
  class be_valuetype_fwd;
#endif /* IDL_HAS_VALUETYPE */
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
  // visit a decl node

  virtual int visit_scope (be_scope *node);
  // visit the scope

  virtual int visit_type (be_type *node);
  // visit the type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit the predefined type

  virtual int visit_module (be_module *node);
  // visit module

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd

#ifdef IDL_HAS_VALUETYPE
  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype_fwd
#endif /* IDL_HAS_VALUETYPE */

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_exception (be_exception *node);
  // visit exception

  virtual int visit_expression (be_expression *node);
  // visit expression

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_operation (be_operation *node);
  // visit an operation

  virtual int visit_field (be_field *node);
  // visit a field

  virtual int visit_argument (be_argument *node);
  // visit argument

  virtual int visit_attribute (be_attribute *node);
  // visit an attribute

  virtual int visit_union (be_union *node);
  // visit union

  virtual int visit_union_branch (be_union_branch *node);
  // visit union branch

  virtual int visit_union_label (be_union_label *node);
  // visit union label

  virtual int visit_constant (be_constant *node);
  // visit a constant

  virtual int visit_enum_val (be_enum_val *node);
  // visit enum val

  virtual int visit_array (be_array *node);
  // visit an array

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_string (be_string *node);
  // visit a string

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef

  virtual int visit_root (be_root *node);
  // visit a root

  virtual int visit_native (be_native *node);
  // visit a native

protected:
  be_visitor (void);
  // constructor is protected

};

#endif // TAO_BE_VISITOR_H
