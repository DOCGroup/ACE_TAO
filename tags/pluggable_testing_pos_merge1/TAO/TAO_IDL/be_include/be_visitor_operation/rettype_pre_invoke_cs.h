//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_pre_invoke_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for pre processing of the return type variable before the invoke is
//    made. This one is for compiled marshaling stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_PRE_INVOKE_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_PRE_INVOKE_CS_H_

// ************************************************************
// Operation visitor for preprocessing before invoke is invoked
// ************************************************************

class be_visitor_operation_rettype_pre_invoke_cs :
  public be_visitor_operation_rettype_pre_docall_cs
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_pre_invoke_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any pre processing before the do_static_call is
  //   made
  //
public:
  be_visitor_operation_rettype_pre_invoke_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_pre_invoke_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface node

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_PRE_INVOKE_CS_H_ */
