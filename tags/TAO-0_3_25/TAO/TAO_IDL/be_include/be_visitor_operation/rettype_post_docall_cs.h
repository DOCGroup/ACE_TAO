//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_post_docall_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for post processing of the return type variable after the docall is
//    made.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_POST_DOCALL_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_POST_DOCALL_CS_H_

// ************************************************************
// Operation visitor for postprocessing after do_static_call is invoked
// ************************************************************

class be_visitor_operation_rettype_post_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_psot_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any post processing after the do_static_call is
  //   made
  //
public:
  be_visitor_operation_rettype_post_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_post_docall_cs (void);
  // destructor

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node
};

// ****************************************************************

class be_visitor_operation_compiled_rettype_post_docall : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_post_docall
  //
  // = DESCRIPTION
  //   This is a visitor for post processing after the invocation in
  //   compiled marshaling
  //
  //
public:
  be_visitor_operation_compiled_rettype_post_docall (be_visitor_context *ctx);
  // constructor

  int visit_array (be_array *node);
  int visit_typedef (be_typedef *node);
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_POST_DOCALL_CS_H_ */
