/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_cs.h
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


#ifndef _BE_VISITOR_HOME_CS_H_
#define _BE_VISITOR_HOME_CS_H_

/**
 * @class be_visitor_home_cs
 *
 * @brief be_visitor_home_cs
 *
 * This is the visitor for component home for the source file
 */
class be_visitor_home_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_home_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_home_cs (void);

  /// visit home.
  virtual int visit_home (be_home *node);
};

#endif /* _BE_VISITOR_HOME_CS_H_ */
