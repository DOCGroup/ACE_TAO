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
 * This is a concrete visitor to generate the client
 * stubs for root
 */
class be_visitor_root_cs : public be_visitor_root
{
public:
  be_visitor_root_cs (be_visitor_context *ctx);

  ~be_visitor_root_cs (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);

  /// Encapsulating various bits of code generation peculiar
  /// to the client source file, done either before or after
  /// he main scope traversal.
  int gen_obv_defns (be_root *node);
  int gen_any_ops (be_root *node);
  int gen_cdr_ops (be_root *node);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CS_H_ */
