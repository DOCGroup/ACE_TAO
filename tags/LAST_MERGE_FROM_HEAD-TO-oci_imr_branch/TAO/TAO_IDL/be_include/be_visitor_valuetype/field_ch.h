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
//    field_ch.h
//
// = DESCRIPTION
//    Visitor generating code for valuetype fields in the valuetype class
//    (header).
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_union_branch/public_ch.h
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FIELD_CH_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CH_H_

class be_visitor_valuetype_field_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_ch
  //
  // = DESCRIPTION
  //    Visitor generating code for valuetype fields in the valuetype class
  //    (header).
  //
public:
  be_visitor_valuetype_field_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_field_ch (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a valuetype
  //  state member can be.

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype type

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype forward type

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

  void setenclosings (const char *pre, const char *post);
  const char *pre_op ();
  const char *post_op ();
  // print additional material around the field (e.g. virtual before)

private:
  const char *pre_op_;
  const char *post_op_;
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CH_H_ */
