//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_invoke_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments for docall post processing
//    in the client side stub when compiled marshaling (default) is enabled.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_POST_INVOKE_CS_H_
#define _BE_VISITOR_ARGUMENT_POST_INVOKE_CS_H_

// ************************************************************
// class be_visitor_args_post_invoke_cs
// ************************************************************

class be_visitor_args_post_invoke_cs : public be_visitor_args
{
  //
  // = TITLE
  //   be_visitor_args_post_invoke_cs
  //
  // = DESCRIPTION
  //   Code to be generated after the invocation is done
  //
public:
  be_visitor_args_post_invoke_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_post_invoke_cs (void);
  // destructor

  virtual int visit_argument (be_argument *node);
  // visit the argument node

  // =visit all the nodes that can be the types for the argument

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

#ifdef IDL_HAS_VALUETYPE
  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype forward
#endif /* IDL_HAS_VALUETYPE */

  virtual int visit_typedef (be_typedef *node);
  // visit the typedef type

  virtual int visit_string (be_string *node);
  // visit string
};

#endif /*  _BE_VISITOR_ARGUMENT_POST_INVOKE_CS_H_ */
