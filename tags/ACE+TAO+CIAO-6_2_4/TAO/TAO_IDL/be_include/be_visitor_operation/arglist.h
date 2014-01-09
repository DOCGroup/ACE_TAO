
//=============================================================================
/**
 *  @file    arglist.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations. This generates the
 *  operation signature.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_ARGLIST_H_
#define _BE_VISITOR_OPERATION_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

/**
 * @class be_visitor_operation_arglist
 *
 * @brief be_visitor_operaion_arglist
 *
 * This is a visitor to generate operation argument list
 */
class be_visitor_operation_arglist : public be_visitor_operation
{
public:
  be_visitor_operation_arglist (be_visitor_context *ctx);

  ~be_visitor_operation_arglist (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_argument (be_argument *node);

  /// Stuff to output after each element of the scope is handled.
  virtual int post_process (be_decl *);

  /// Set the member.
  void unused (bool val);

private:
  bool unused_;
};

#endif /* _BE_VISITOR_OPERATION_ARGLIST_H_ */
