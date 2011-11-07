/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Typedef class in the client
 *  inline.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_

/**
 * @class be_visitor_typedef_ci
 *
 * @brief be_visitor_typedef_sh
 *
 * This is a concrete visitor to generate the server header for typedef
 */
class be_visitor_typedef_ci : public be_visitor_typedef
{
public:
  /// constructor
  be_visitor_typedef_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_ci (void);

  /// visit typedef. We provide code for this method in the derived class
  virtual int visit_typedef (be_typedef *node);

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  virtual int visit_array (be_array *node);

  /// visit a sequence
  virtual int visit_sequence (be_sequence *node);

  /// visit a structure
  virtual int visit_structure (be_structure *node);

  /// visit a union
  virtual int visit_union (be_union *node);
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_*/
