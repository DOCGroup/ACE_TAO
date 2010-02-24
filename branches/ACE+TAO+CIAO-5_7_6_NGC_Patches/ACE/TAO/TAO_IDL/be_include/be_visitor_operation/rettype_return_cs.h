//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_return_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the return
//    statement for returning the return type variable in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_RETURN_CS_H_
#define _BE_VISITOR_OPERATION_RETTYPE_CS_H_

// ************************************************************
// Operation visitor for returning the return val
// ************************************************************

class be_visitor_operation_rettype_return_cs :public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_return_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_rettype_return_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_return_cs (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_valuebox (be_valuebox *node);
  // visit a valuebox node

  int visit_valuetype (be_valuetype *node);
  // visit valuetype

  int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype forward

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

  int visit_component (be_component *node);
  // visit component

  int visit_component_fwd (be_component_fwd *node);
  // visit component forward

  int visit_eventtype (be_eventtype *node);
  // visit eventtype

  int visit_eventtype_fwd (be_eventtype_fwd *node);
  // visit eventtype forward

  int visit_home (be_home *node);
  // visit component home
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_RETURN_CS_H_ */
