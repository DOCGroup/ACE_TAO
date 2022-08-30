/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef_ci.h
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Typedef class in the client
 *  inline.
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
  ~be_visitor_typedef_ci ();

  /// visit typedef. We provide code for this method in the derived class
  int visit_typedef (be_typedef *node) override;

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  int visit_array (be_array *node) override;

  /// visit a sequence
  int visit_sequence (be_sequence *node) override;

  /// visit a structure
  int visit_structure (be_structure *node) override;

  /// visit a union
  int visit_union (be_union *node) override;
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_*/
