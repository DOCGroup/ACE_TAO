//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_marshal_ss.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments. This generates the
//    code that does post-marshal processing (cleaning up) for arguments
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H_
#define _BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H_

// ************************************************************
// class be_visitor_args_post_marshal_ss
// ************************************************************
class be_visitor_args_post_marshal_ss : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_marshal_ss
  //
  // = DESCRIPTION
  //   Visitor for post processing after marshal
  //
public:
  be_visitor_args_post_marshal_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_marshal_ss (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type
};

#endif /* _BE_VISITOR_ARGUMENT_POST_MARSHAL_SS_H */
