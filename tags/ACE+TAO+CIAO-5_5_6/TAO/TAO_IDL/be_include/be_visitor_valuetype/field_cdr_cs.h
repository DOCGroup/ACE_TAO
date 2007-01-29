/* -*- c++ -*- */
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_cdr_cs.h
//
// = DESCRIPTION
//    Visitor generating code for Field in the client stubs file.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_field/cdr_op_cs.h
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FIELD_CDR_C_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CDR_CI_H_

#include "be_visitor_scope.h"

class be_visitor_valuetype_field_cdr_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_valuetype_field_cdr_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_valuetype_field_cdr_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_field_cdr_cs (void);
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

  virtual int visit_valuebox (be_valuebox *node);
  // visit valuebox type

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  virtual int visit_component (be_component *node);
  // visit component type

  virtual int visit_component_fwd (be_component_fwd *node);
  // visit interface forward type

  virtual int visit_eventtype (be_eventtype *node);
  // visit eventtype type

  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  // visit eventtype forward type

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

  const char *pre_, *post_;

private:
  int valuetype_common (be_type *);

};

class be_visitor_valuetype_field_cdr_decl : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_cdr_valuetype_field_cdr_decl
  //
  // = DESCRIPTION
  //   When generating CDR operators for fields of structures and/or
  //   unions it may be necessary to define some local variables (for
  //   instance to hold temporary T_forany objects); this visitor
  //   generates that code.
  //
public:
  be_visitor_valuetype_field_cdr_decl (be_visitor_context *ctx);
  // constructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // = visit operations for data types that require a local variable.

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_typedef (be_typedef *node);
  // visit typedef type
};

#endif /*  _BE_VISITOR_VALUETYPE_FIELD_CDR_CI_H_ */
