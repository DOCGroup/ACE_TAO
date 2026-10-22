/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the CDR operators for the structure
 *  in the client stub.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_
#define _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_

/**
 * @class be_visitor_structure_cdr_op_cs
 *
 * @brief be_visitor_structure_cdr_op_cs
 *
 * This is a concrete visitor for structure that generates the CDR operator
 * implementations
 */
class be_visitor_structure_cdr_op_cs : public be_visitor_structure
{
public:
  /// constructor
  be_visitor_structure_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_structure_cdr_op_cs () override;

  /// visit structure
  int visit_structure (be_structure *node) override;

  /// any post processing that needs to be done after a scope element is handled
  int post_process (be_decl *) override;
};

#endif /* _BE_VISITOR_STRUCTURE_CDR_OP_CS_H_ */
