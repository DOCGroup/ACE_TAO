//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    pre_invoke_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arguments for pre invoke processing
//    in client side stub that uses compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARGUMENT_PRE_INVOKE_CS_H_
#define _BE_VISITOR_ARGUMENT_PRE_INVOKE_CS_H_

// ************************************************************
// class be_visitor_args_pre_invoke_cs
// ************************************************************
class be_visitor_args_pre_invoke_cs : public be_visitor_args_pre_docall_cs
{
  //
  // = TITLE
  //   be_visitor_args_docall_cs
  //
  // = DESCRIPTION
  //   Code to be generated when making the do_static_call
  //
public:
  be_visitor_args_pre_invoke_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_args_pre_invoke_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward

};

#endif  /* _BE_VISITOR_ARGUMENT_PRE_INVOKE_CS_H_ */
