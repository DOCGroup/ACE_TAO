//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_docall_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments for docall post processing
//    in the client side stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (_BE_VISITOR_ARGUMENT_POST_DOCALL_CS_H_)
#define _BE_VISITOR_ARGUMENT_POST_DOCALL_CS_H_

// ************************************************************
// class be_visitor_args_post_docall_cs
// ************************************************************
class be_visitor_args_post_docall_cs : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_docall_cs
  //
  // = DESCRIPTION
  //   Code to be generated after the do_static_call is done
  //
public:
  be_visitor_args_post_docall_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_docall_cs (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type
};

#endif /*  _BE_VISITOR_ARGUMENT_POST_DOCALL_CS_H_ */
