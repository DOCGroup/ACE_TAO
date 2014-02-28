/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union_ch.h
 *
 *  $Id$
 *
 *   Visitor for the Union class.
 *   This one generates code in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_UNION_CH_H_
#define _BE_VISITOR_UNION_UNION_CH_H_

/**
 * @class be_visitor_union_ch
 *
 * @brief be_visitor_union_ch
 *
 * This is a concrete visitor to generate the client header for union
 */
class be_visitor_union_ch : public be_visitor_union
{
public:
  /// constructor
  be_visitor_union_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_ch (void);

  /// visit union. We provide code for this method in the derived class
  virtual int visit_union (be_union *node);

};

#endif /* _BE_VISITOR_UNION_UNION_CH_H_ */
