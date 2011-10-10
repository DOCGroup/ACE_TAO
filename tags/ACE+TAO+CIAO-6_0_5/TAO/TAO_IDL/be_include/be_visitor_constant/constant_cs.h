/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    constant_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the constant class generating code in the client stub.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_CONSTANT_CONSTANT_CS_H_
#define _BE_VISITOR_CONSTANT_CONSTANT_CS_H_

/**
 * @class be_visitor_constant_cs
 *
 * @brief be_visitor_constant_cs
 *
 * This is the visitor for constant for the impl file
 */
class be_visitor_constant_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_constant_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_constant_cs (void);

  /// visit constant
  virtual int visit_constant (be_constant *node);
};

#endif /* _BE_VISITOR_CONSTANT_CONSTANT_CS_H_ */
