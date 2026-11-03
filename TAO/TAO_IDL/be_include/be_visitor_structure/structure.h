/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    structure.h
 *
 *  Concrete visitor for the STRUCTURE class
 *  This one provides the generic visitor for the Structure node.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_H_

/**
 * @class be_visitor_structure
 *
 * @brief be_visitor_structure
 *
 * This is the base visitor for structure
 */
class be_visitor_structure : public be_visitor_scope
{
public:
  /// constructur
  explicit be_visitor_structure (be_visitor_context *ctx);

  /// destructur
  ~be_visitor_structure () override = default;

  be_visitor_structure (const be_visitor_structure &) = delete;
  be_visitor_structure (be_visitor_structure &&) = delete;
  be_visitor_structure &operator= (const be_visitor_structure &) = delete;
  be_visitor_structure &operator= (be_visitor_structure &&) = delete;

  /// visit structure. We provide code for this method in the derived class
  int visit_structure (be_structure *node) override;

  // =visit operations on syntactically valid elements in our scope

  /// visit field
  int visit_field (be_field *node) override;
};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_H_ */
