
//=============================================================================
/**
 *  @file    post_upcall_ss.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arguments. This generates the
 *  code for post upcall processing of the arguments
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================



#ifndef _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_
#define _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_

// ************************************************************
// class be_visitor_args_post_upcall_ss
// ************************************************************

/**
 * @class be_visitor_args_post_upcall_ss
 *
 * @brief be_visitor_compiled_args_post_upcall
 *
 * Visitor for post processing after upcall
 */
class be_visitor_args_post_upcall_ss : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_args_post_upcall_ss (be_visitor_context *ctx);

  /// destructor
  virtual ~be_visitor_args_post_upcall_ss (void);

  /// visit the argument node
  virtual int visit_argument (be_argument *node);

  /// visit operation node
  virtual int visit_operation (be_operation *node);

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit the typedef type
  virtual int visit_typedef (be_typedef *node);
};

#endif  /* _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_ */
