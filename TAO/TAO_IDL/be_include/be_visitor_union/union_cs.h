/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union_cs.h
 *
 *   Visitor for the Union class.
 *   This one generates code in the client stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_UNION_CS_H_
#define _BE_VISITOR_UNION_UNION_CS_H_

/**
 * @class be_visitor_union_cs
 *
 * @brief be_visitor_union_cs
 *
 * This is a concrete visitor to generate the client stubs for union
 */
class be_visitor_union_cs : public be_visitor_union
{
public:
  /// constructor
  explicit be_visitor_union_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_cs () override;

  be_visitor_union_cs (be_visitor_union_cs const &) = delete;
  be_visitor_union_cs (be_visitor_union_cs &&) = delete;
  be_visitor_union_cs &operator= (be_visitor_union_cs const &) = delete;
  be_visitor_union_cs &operator= (be_visitor_union_cs &&) = delete;

  /// visit union. We provide code for this method in the derived class
  int visit_union (be_union *node) override;
};

#endif /* _BE_VISITOR_UNION_UNION_CS_H_ */
