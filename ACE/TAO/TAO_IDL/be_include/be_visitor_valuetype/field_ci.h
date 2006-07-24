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
//    field_ci.h
//
// = DESCRIPTION
//     Visitor for the Valuetype class.
//     This one generates code for accessor and modifier functions of
//     valuetype state members (in the inline file).
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FIELD_CI_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CI_H_

class be_visitor_valuetype_field_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_valuetype_field_ci
  //
  // = DESCRIPTION
  //    This visitor is used to generate the accessors and modifiers
  //    for the members of valuetype
  //
public:
  be_visitor_valuetype_field_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_field_ci (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_structure (be_structure *node);
  // visit structure type

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
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CI_H_ */

