//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_pre_docall_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for pre processing of the return type variable before the docall is
//    made.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_PRE_DOCALL_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_PRE_DOCALL_CS_H_

// ************************************************************
// Operation visitor for preprocessing before do_static_call is invoked
// ************************************************************

class be_visitor_operation_rettype_pre_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_pre_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any pre processing before the do_static_call is
  //   made
  //
public:
  be_visitor_operation_rettype_pre_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_pre_docall_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit an array node

  virtual int visit_interface (be_interface *node);
  // visit an interface node

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence node

  virtual int visit_structure (be_structure *node);
  // visit a structure node

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef node

  virtual int visit_union (be_union *node);
  // visit a union node

};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_PRE_DOCALL_CS_H_ */
