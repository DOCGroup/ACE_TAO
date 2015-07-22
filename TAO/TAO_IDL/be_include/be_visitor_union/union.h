/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union.h
 *
 *   Visitor for the Union class.
 *   This one is a generic visitor.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_UNION_H_
#define _BE_VISITOR_UNION_UNION_H_

/**
 * @class be_visitor_union
 *
 * @brief be_visitor_union
 *
 * This is the base visitor for union
 */
class be_visitor_union : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_union (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union (void);

  /// visit union. We provide code for this method in the derived class
  virtual int visit_union (be_union *node);

  // =visit operations on syntactically valid elements in our scope

  /// visit union_branch
  virtual int visit_union_branch (be_union_branch *node);

  enum BoolUnionBranch { BUB_NONE, BUB_UNCONDITIONAL, BUB_TRUE, BUB_FALSE };

  static BoolUnionBranch boolean_branch (be_union_branch *b);
};

#endif /* _BE_VISITOR_UNION_UNION_H_ */
