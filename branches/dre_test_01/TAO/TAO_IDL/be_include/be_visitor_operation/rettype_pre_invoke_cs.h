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

class be_visitor_operation_rettype_pre_invoke_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operation_rettype_pre_invoke_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate any pre processing before
  //   the invocation is made.
  //
public:
  be_visitor_operation_rettype_pre_invoke_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_rettype_pre_invoke_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit an array node

  virtual int visit_interface (be_interface *node);
  // visit an interface node

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit a forward decalred interface node

  virtual int visit_valuetype (be_valuetype *node);
  // visit a valuetype node

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit a forward decalred valuetype node

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
 
  virtual int visit_component (be_component *node);
  // visit component

  virtual int visit_component_fwd (be_component_fwd *node);
  // visit component forward

  virtual int visit_eventtype (be_eventtype *node);
  // visit eventtype

  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  // visit eventtype forward

  virtual int visit_home (be_home *node);
  // visit component home
};

#endif /* _BE_VISITOR_OPERATION_RETTYPE_PRE_INVOKE_CS_H_ */
