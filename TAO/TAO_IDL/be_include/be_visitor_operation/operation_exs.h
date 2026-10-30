
//=============================================================================
/**
 *  @file    operation_exs.h
 *
 *  Visitor for generating code for IDL operations in
 *  CIAO executor implementations
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_EXS_H_
#define _BE_VISITOR_OPERATION_OPERATION_EXS_H_

#include <string>

// ************************************************************
// Operation visitor for CIAO executor implementations
// ************************************************************

/**
 * @class be_visitor_operation_exs
 *
 * @brief be_visitor_operation_exs
 *
 * This is a concrete visitor to generate the CIAO executor
 * implementations for operation
 */
class be_visitor_operation_exs : public be_visitor_scope
{
public:
  explicit be_visitor_operation_exs (be_visitor_context *ctx);

  ~be_visitor_operation_exs () override;
  be_visitor_operation_exs (be_visitor_operation_exs const &) = delete;
  be_visitor_operation_exs (be_visitor_operation_exs &&) = delete;
  be_visitor_operation_exs &operator= (be_visitor_operation_exs const &) = delete;
  be_visitor_operation_exs &operator= (be_visitor_operation_exs &&) = delete;

  int visit_operation (be_operation *node) override;

  void scope (be_decl *node);
  void class_extension (std::string const& extension);

private:
  int gen_op_body (be_type *return_type);

private:
  TAO_OutStream &os_;
  be_decl *scope_ {};
  const char *your_code_here_;
  std::string class_extension_;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_EXS_H_ */
