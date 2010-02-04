/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union_ci.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code in the client inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_UNION_CI_H_
#define _BE_VISITOR_UNION_UNION_CI_H_

/**
 * @class be_visitor_union_ci
 *
 * @brief be_visitor_union_ci
 *
 * This is a concrete visitor to generate the client inline for union
 */
class be_visitor_union_ci : public be_visitor_union
{
public:
  /// constructor
  be_visitor_union_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_ci (void);

  /// visit union. We provide code for this method in the derived class
  virtual int visit_union (be_union *node);

};

#endif /* _BE_VISITOR_UNION_UNION_CI_H_ */
