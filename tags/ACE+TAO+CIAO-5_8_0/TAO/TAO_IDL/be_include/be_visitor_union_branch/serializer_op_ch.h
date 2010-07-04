/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    serializer_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Union_Branch" node
 *  This generates TAO::DCPS::Serializer operators code for structure members
 *  in the client header.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCH_serializer_op_CH_H_
#define _BE_VISITOR_UNION_BRANCH_serializer_op_CH_H_

/**
 * @class be_visitor_union_branch_serializer_op_ch
 *
 * @brief be_visitor_serializer_op_ch
 *
 * This is a concrete visitor for the be_union_branch node for the client header.
 */
class be_visitor_union_branch_serializer_op_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_branch_serializer_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_branch_serializer_op_ch (void);

  /// visit the union_branch node
  virtual int visit_union_branch (be_union_branch *node);

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);

};

#endif /*  _BE_VISITOR_UNION_BRANCH_serializer_op_CH_H_ */
