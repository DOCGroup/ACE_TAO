//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the return
//    type of the operation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_RETTYPE_H_
#define _BE_VISITOR_OPERATION_RETTYPE_H_

// ***********************************************************************
// Operation visitor for return types in the signature of the operation
// ***********************************************************************

class be_visitor_operation_rettype : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_ch
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation return types
  //
  //
public:
  be_visitor_operation_rettype (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype (void);
  // destructor

  int visit_array (be_array *node);
  // visit an array node

  int visit_enum (be_enum *node);
  // visit an enum node

  int visit_interface (be_interface *node);
  // visit an interface node

  int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface node

  int visit_native (be_native *node);
  // visit native type

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

  int visit_valuetype (be_valuetype *node);
  // visit a valuetype node

  int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit a forward declared valuetype node

  int visit_component (be_component *node);
  // visit a component node

  int visit_component_fwd (be_component_fwd *node);
  // visit a forward declared component node

  int visit_eventtype (be_eventtype *node);
  // visit an eventtype node

  int visit_eventtype_fwd (be_eventtype_fwd *node);
  // visit a forward declared eventtype node

  int visit_home (be_home *node);
  // visit a component home node

  int visit_valuebox (be_valuebox *node);
  // visit a valuebox node
  
private:
  const char *type_name (be_type *node);
  
private:
  TAO_OutStream *os;
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_H_ */
