//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_docall_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the code
//    for passing the return type variable to the docall
//    made.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_DOCALL_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_DOCALL_CS_H_

// ************************************************************
// Operation visitor for passing return variable to do_static_call
// ************************************************************

class be_visitor_operation_rettype_docall_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_docall_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_rettype_docall_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_docall_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  int visit_sequence (be_sequence *node);
  // visit a sequence node

  int visit_string (be_string *node);
  // visit a sequence node

  int visit_structure (be_structure *node);
  // visit a structure node

  int visit_typedef (be_typedef *node);
  // visit a typedef node

  int visit_union (be_union *node);
  // visit a union node

};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_DOCALL_CS_H_ */
