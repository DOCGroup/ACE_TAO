/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the Any operators for the structure
 *  in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_
#define _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_

/**
 * @class be_visitor_structure_any_op_ch
 *
 * @brief be_visitor_structure_any_op_ch
 *
 * This is a concrete visitor for structure that generates the Any operator
 * declarations
 */
class be_visitor_structure_any_op_ch : public be_visitor_structure
{
public:
  /// constructor
  be_visitor_structure_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_structure_any_op_ch () = default;

  /// visit structure
  int visit_structure (be_structure *node) override;

  /// visit field
  int visit_field (be_field *node) override;

  /// visit union
  int visit_union (be_union *node) override;

  /// visit field
  int visit_enum (be_enum *node) override;
};

#endif /* _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_ */
