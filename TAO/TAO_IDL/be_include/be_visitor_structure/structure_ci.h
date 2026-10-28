/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    structure_ci.h
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the structure in the client inline.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_CI_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_CI_H_

/**
 * @class be_visitor_structure_ci
 *
 * @brief be_visitor_structure_ci
 *
 * This is a concrete visitor to generate the client inline for structure
 */
class be_visitor_structure_ci : public be_visitor_structure
{
public:
  /// constructureor
  explicit be_visitor_structure_ci (be_visitor_context *ctx);

  /// destructureor
  ~be_visitor_structure_ci () override  = default;

  be_visitor_structure_ci (const be_visitor_structure_ci &) = delete;
  be_visitor_structure_ci (be_visitor_structure_ci &&) = delete;
  be_visitor_structure_ci &operator= (const be_visitor_structure_ci &) = delete;
  be_visitor_structure_ci &operator= (be_visitor_structure_ci &&) = delete;

  /// visit structure. We provide code for this method in the derived class
  int visit_structure (be_structure *node) override;
};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_CI_H_ */
