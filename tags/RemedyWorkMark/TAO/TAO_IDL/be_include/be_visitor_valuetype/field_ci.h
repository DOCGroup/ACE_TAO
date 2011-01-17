/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_ci.h
 *
 *  $Id$
 *
 *   Visitor for the Valuetype class.
 *   This one generates code for accessor and modifier functions of
 *   valuetype state members (in the inline file).
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_FIELD_CI_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CI_H_

/**
 * @class be_visitor_valuetype_field_ci
 *
 * @brief be_visitor_valuetype_field_ci
 *
 * This visitor is used to generate the accessors and modifiers
 * for the members of valuetype
 */
class be_visitor_valuetype_field_ci : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuetype_field_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_field_ci (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit union type
  virtual int visit_union (be_union *node);

  /// print additional string before op (e.g. ACE_INLINE)
  void setenclosings (const char *pre);
  const char *pre_op ();

  /// if (in_obv_space_) print OBV_name else print name
  void op_name (be_valuetype *node, TAO_OutStream *os);

  bool in_obv_space_;

private:
  const char *pre_op_;
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CI_H_ */

