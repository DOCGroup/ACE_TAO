
//=============================================================================
/**
 *  @file    argument.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arguments
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARGUMENT_ARGUMENT_H_
#define _BE_VISITOR_ARGUMENT_ARGUMENT_H_

#include "ast_argument.h"

/**
 * @class be_visitor_args
 *
 * @brief Generates the code for arguments output.
 *
 * At several stages in the code generation a node of type
 * "be_operation" is visited, and the code for its arguments must
 * be generated.
 * Sometimes the argument declarations must be generated (such as
 * in the class declaration), other the arguments names (such as
 * in method invocations).
 * This class implements some common functionality required across all
 * argument visitors
 */
class be_visitor_args : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_args (be_visitor_context *ctx);

  /// destructor
  virtual ~be_visitor_args (void);

  //= helper
  /// helper that produces either a nested name for
  /// header files or full name for others.
  virtual const char *type_name (be_type *type,
                                 const char *suffix = 0);

  /// return the direction of the argument node
  virtual AST_Argument::Direction direction (void);

  /// must be overridden by derived classes
  virtual int visit_argument (be_argument *node);

  void set_fixed_direction (AST_Argument::Direction direction);

protected:
  /// Consolidation of common code from invoke_cs and marshal_ss
  /// for predefined type args.
  int gen_pd_arg (be_predefined_type *node,
                  bool for_stub);

private:
  int fixed_direction_;
};

#endif /* _BE_VISITOR_ARGUMENT_ARGUMENT_H_ */
