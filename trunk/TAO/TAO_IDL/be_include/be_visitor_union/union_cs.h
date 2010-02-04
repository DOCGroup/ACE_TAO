/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union_cs.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code in the client stubs.
 *
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
  be_visitor_union_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_cs (void);

  /// visit union. We provide code for this method in the derived class
  virtual int visit_union (be_union *node);

};

#endif /* _BE_VISITOR_UNION_UNION_CS_H_ */
