/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ROOT_ROOT_CS_H_
#define _BE_VISITOR_ROOT_ROOT_CS_H_

/**
 * @class be_visitor_root_cs
 *
 * @brief be_visitor_root_cs
 *
 * This is a concrete visitor to generate the client stubs for root
 */
class be_visitor_root_cs : public be_visitor_root
{
public:
  /// constructor
  be_visitor_root_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_root_cs (void);

  /// set the right context and make a visitor
  virtual int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CS_H_ */
