/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component Home node.
 *  This one provides code generation for component home node.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_HOME_CH_H_
#define _BE_VISITOR_HOME_CH_H_

/**
 * @class be_visitor_home_ch
 *
 * @brief be_visitor_home_ch
 *
 * This is the visitor for component home for the header file
 */
class be_visitor_home_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_home_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_home_ch (void);

  /// visit home.
  virtual int visit_home (be_home *node);
};

#endif /* _BE_VISITOR_HOME_CH_H_ */
