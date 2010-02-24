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
//    field_cs.h
//
// = DESCRIPTION
//     Visitor for the Valuetype class.
//     This one generates code for accessor and modifier functions of
//     valuetype state members (in the stub file).
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_union_branch/public_ci.h
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FIELD_CS_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CS_H_

class be_visitor_valuetype_field_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_valuetype_field_cs
  //
  // = DESCRIPTION
  //    This visitor is used to generate the accessors and modifiers
  //    for the members of valuetype
  //
public:
  be_visitor_valuetype_field_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_field_cs (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a union_branch can be

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
  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

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

  void setenclosings (const char *pre);
  const char *pre_op ();
  // print additional string before op (e.g. ACE_INLINE)

  void op_name (be_valuetype *node, TAO_OutStream *os);
  // if (in_obv_space_) print OBV_name else print name

  bool in_obv_space_;
private:
  const char *pre_op_;

  int valuetype_common (be_type *node);
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CS_H_ */

