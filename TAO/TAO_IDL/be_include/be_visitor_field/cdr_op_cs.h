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
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//    This generates code for structure members in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_FIELD_CDR_OP_CS_H_
#define _BE_VISITOR_FIELD_CDR_OP_CS_H_

#include "be_visitor_scope.h"

class be_visitor_field_cdr_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_field_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_cdr_op_cs (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

class be_visitor_cdr_op_field_decl : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_cdr_op_cs_field_decl
  //
  // = DESCRIPTION
  //   When generating CDR operators for fields of structures and/or
  //   unions it may be necessary to define some local variables (for
  //   instance to hold temporary T_forany objects); this visitor
  //   generates that code.
  //
public:
  be_visitor_cdr_op_field_decl (be_visitor_context *ctx);
  // constructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // = visit operations for data types that require a local variable.

  virtual int visit_array (be_array *node);
  // visit array type
};

#endif /*  _BE_VISITOR_FIELD_CDR_OP_CS_H_ */
