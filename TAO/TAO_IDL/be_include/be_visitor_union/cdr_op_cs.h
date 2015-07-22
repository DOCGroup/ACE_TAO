/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  Concrete visitor for the Union class
 *  This one provides code generation for the CDR operators for the union
 *  in the client stub.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_CDR_OP_CS_H_
#define _BE_VISITOR_UNION_CDR_OP_CS_H_

/**
 * @class be_visitor_union_cdr_op_cs
 *
 * @brief be_visitor_union_cdr_op_cs
 *
 * This is a concrete visitor for union that generates the CDR operator
 * implementations
 */
class be_visitor_union_cdr_op_cs : public be_visitor_union
{

public:
  /// constructor
  be_visitor_union_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_cdr_op_cs (void);

  /// visit union
  virtual int visit_union (be_union *node);

  virtual int pre_process (be_decl *);
  virtual int post_process (be_decl *);

private:
  BoolUnionBranch latest_branch_;
};

#endif /* _BE_VISITOR_UNION_CDR_OP_CS_H_ */
