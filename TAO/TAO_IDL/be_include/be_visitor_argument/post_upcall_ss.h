//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_upcall_ss.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments. This generates the
//    code for post upcall processing of the arguments
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================


#ifndef _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_
#define _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_

// ************************************************************
// class be_visitor_args_post_upcall_ss
// ************************************************************

class be_visitor_args_post_upcall_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_compiled_args_post_upcall
  //
  // = DESCRIPTION
  //   Visitor for post processing after upcall
  //
public:
  be_visitor_args_post_upcall_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_upcall_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  virtual int visit_operation (be_operation *node);
  // visit operation node

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type
};

#endif  /* _BE_VISITOR_ARGUMENT_POST_UPCALL_SS_H_ */
