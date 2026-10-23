/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  Concrete visitor for Enums generating code for the CDR operators
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ENUM_CDR_OP_CH_H_
#define _BE_VISITOR_ENUM_CDR_OP_CH_H_

/**
 * @class be_visitor_enum_cdr_op_ch
 *
 * @brief be_visitor_enum_cdr_op_ch
 *
 * This is a concrete visitor for enum that generates the CDR operator
 * declarations
 */
class be_visitor_enum_cdr_op_ch : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_enum_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_enum_cdr_op_ch () override;
  be_visitor_enum_cdr_op_ch (be_visitor_enum_cdr_op_ch const &) = delete;
  be_visitor_enum_cdr_op_ch (be_visitor_enum_cdr_op_ch &&) = delete;
  be_visitor_enum_cdr_op_ch &operator= (be_visitor_enum_cdr_op_ch const &) = delete;
  be_visitor_enum_cdr_op_ch &operator= (be_visitor_enum_cdr_op_ch &&) = delete;

  /// visit enum
  int visit_enum (be_enum *node) override;
};

#endif /* _BE_VISITOR_ENUM_CDR_OP_CH_H_ */
